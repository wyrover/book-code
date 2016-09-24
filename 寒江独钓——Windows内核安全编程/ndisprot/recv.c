/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    recv.c

Abstract:

    NDIS protocol entry points and utility routines to handle receiving
    data.

Environment:

    Kernel mode only.

Revision History:

--*/

#include "precomp.h"

#define __FILENUMBER 'VCER'



// 分发函数之一，处理读请求。
NTSTATUS
NdisProtRead(
    IN PDEVICE_OBJECT       pDeviceObject,
    IN PIRP                 pIrp
    )
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
    PNDISPROT_OPEN_CONTEXT   pOpenContext;

    UNREFERENCED_PARAMETER(pDeviceObject);
    
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    do
    {
        // 检测打开上下文的可靠性
        if (pOpenContext == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: NULL FsContext on FileObject %p\n",
                        pIrpSp->FileObject));
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }
            
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        // Read和Write都是使用的直接IO操作，所以应该使用MdlAddress
        // 来传递缓冲。如果不是，返回非法参数错误。
        if (pIrp->MdlAddress == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: NULL MDL address on IRP %p\n", pIrp));
            NtStatus = STATUS_INVALID_PARAMETER;
            break;
        }

        // 得到缓冲的虚拟地址。
        if (MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority) == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: MmGetSystemAddr failed for IRP %p, MDL %p\n",
                    pIrp, pIrp->MdlAddress));
            NtStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }

        // 将这个请求插入处理队列里。并把打开上下文引用计数增加就1.
        // 把未处理读请求数目增加1.
        NPROT_INSERT_TAIL_LIST(&pOpenContext->PendedReads, &pIrp->Tail.Overlay.ListEntry);
        NPROT_REF_OPEN(pOpenContext);  // pended read IRP
        pOpenContext->PendedReadCount++;

        // 标记IRP未决。给IRP设置一个取消函数，使之变得可取消。
        pIrp->Tail.Overlay.DriverContext[0] = (PVOID)pOpenContext;
        IoMarkIrpPending(pIrp);
        IoSetCancelRoutine(pIrp, NdisProtCancelRead);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        NtStatus = STATUS_PENDING;

        // 调用一个处理例程来处理所有未决的读请求。
        ndisprotServiceReads(pOpenContext);

    }
    while (FALSE);

    // 读请求只返回STATUS_PENDING.如果不是，则说明出错，
    // 按错误返回。
    if (NtStatus != STATUS_PENDING)
    {
        NPROT_ASSERT(NtStatus != STATUS_SUCCESS);
        pIrp->IoStatus.Information = 0;
        pIrp->IoStatus.Status = NtStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    return (NtStatus);
}


VOID
NdisProtCancelRead(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
    )
/*++

Routine Description:

    Cancel a pending read IRP. We unlink the IRP from the open context
    queue and complete it.

Arguments:

    pDeviceObject - pointer to our device object
    pIrp - IRP to be cancelled

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PLIST_ENTRY                 pIrpEntry;
    BOOLEAN                     Found;

    UNREFERENCED_PARAMETER(pDeviceObject);
    
    IoReleaseCancelSpinLock(pIrp->CancelIrql);

    Found = FALSE;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT) pIrp->Tail.Overlay.DriverContext[0];
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    //
    //  Locate the IRP in the pended read queue and remove it if found.
    //
    for (pIrpEntry = pOpenContext->PendedReads.Flink;
         pIrpEntry != &pOpenContext->PendedReads;
         pIrpEntry = pIrpEntry->Flink)
    {
        if (pIrp == CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry))
        {
            NPROT_REMOVE_ENTRY_LIST(&pIrp->Tail.Overlay.ListEntry);
            pOpenContext->PendedReadCount--;
            Found = TRUE;
            break;
        }
    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    if (Found)
    {
        DEBUGP(DL_INFO, ("CancelRead: Open %p, IRP %p\n", pOpenContext, pIrp));
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pIrp->IoStatus.Information = 0;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);

        NPROT_DEREF_OPEN(pOpenContext); // Cancel removed pended Read
    }
}
        


VOID
ndisprotServiceReads(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext
    )
/*++

Routine Description:

    Utility routine to copy received data into user buffers and
    complete READ IRPs.

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    PIRP                pIrp;
    PLIST_ENTRY         pIrpEntry;
    PNDIS_PACKET        pRcvPacket;
    PLIST_ENTRY         pRcvPacketEntry;
    PUCHAR              pSrc, pDst;
    ULONG               BytesRemaining; // at pDst
    PNDIS_BUFFER        pNdisBuffer;
    ULONG               BytesAvailable;
    BOOLEAN             FoundPendingIrp;

    DEBUGP(DL_VERY_LOUD, ("ServiceReads: open %p/%x\n",
            pOpenContext, pOpenContext->Flags));

    NPROT_REF_OPEN(pOpenContext);  // temp ref - service reads

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    // 只要读请求队列和接收包队列同时不为空，则可以做...
    while (!NPROT_IS_LIST_EMPTY(&pOpenContext->PendedReads) &&
           !NPROT_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
    {
        FoundPendingIrp = FALSE;
  
        // 获得第一个未决读请求
        pIrpEntry = pOpenContext->PendedReads.Flink;
        while (pIrpEntry != &pOpenContext->PendedReads)
        {
            // 从链表节点得到IRP
            pIrp = CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry);

            // 检查这个请求是否正在被取消。
            if (IoSetCancelRoutine(pIrp, NULL))
            {
                // 把这个IRP出列。
                NPROT_REMOVE_ENTRY_LIST(pIrpEntry);
                FoundPendingIrp = TRUE;
                break;
            }
            else
            {
                // 如果是在去掉，则跳过这个IRP即可。使用下一个。
                DEBUGP(DL_INFO, ("ServiceReads: open %p, skipping cancelled IRP %p\n",
                        pOpenContext, pIrp));
                pIrpEntry = pIrpEntry->Flink;
            }
        }
        // 如果没有IRP,直接跳出结束。
        if (FoundPendingIrp == FALSE)
        {
            break;
        }

        // 得到第一个包（最旧的），出队列。
        pRcvPacketEntry = pOpenContext->RecvPktQueue.Flink;
        NPROT_REMOVE_ENTRY_LIST(pRcvPacketEntry);
        pOpenContext->RecvPktCount --;
        NPROT_RELEASE_LOCK(&pOpenContext->Lock);
        NPROT_DEREF_OPEN(pOpenContext);

        // 从节点获得包。
        pRcvPacket = NPROT_LIST_ENTRY_TO_RCV_PKT(pRcvPacketEntry);

        //
        //  Copy as much data as possible from the receive packet to
        //  the IRP MDL.
        //
        // 得到IRP的输出缓冲地址。然后尽量拷贝更多的数据。
        pDst = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
        NPROT_ASSERT(pDst != NULL);  // since it was already mapped
        BytesRemaining = MmGetMdlByteCount(pIrp->MdlAddress);
        pNdisBuffer = NDIS_PACKET_FIRST_NDIS_BUFFER(pRcvPacket);

        // 请注意，每个PNDIS_BUFFER都是一个PMDL，同时PNDIS_BUFFER
        // 本身都是链表。用NdisGetNextBuffer可以从一个得到它的下面一个。
        // 包的数据实际上是保存在一个缓冲描述符链表里的。
        while (BytesRemaining && (pNdisBuffer != NULL))
        {
#ifndef WIN9X
            NdisQueryBufferSafe(pNdisBuffer, &pSrc, &BytesAvailable, NormalPagePriority);

            if (pSrc == NULL) 
            {
                DEBUGP(DL_FATAL,
                    ("ServiceReads: Open %p, QueryBuffer failed for buffer %p\n",
                            pOpenContext, pNdisBuffer));
                break;
            }
#else
            NdisQueryBuffer(pNdisBuffer, &pSrc, &BytesAvailable);
#endif

            // 如果还可以继续拷贝，就继续拷贝。
            if (BytesAvailable)
            {
                ULONG       BytesToCopy = MIN(BytesAvailable, BytesRemaining);
                NPROT_COPY_MEM(pDst, pSrc, BytesToCopy);
                BytesRemaining -= BytesToCopy;
                pDst += BytesToCopy;
            }

            NdisGetNextBuffer(pNdisBuffer, &pNdisBuffer);
        }

        // 拷贝好数据之后，结束IRP即可。
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = MmGetMdlByteCount(pIrp->MdlAddress) - BytesRemaining;

        DEBUGP(DL_INFO, ("ServiceReads: Open %p, IRP %p completed with %d bytes\n",
            pOpenContext, pIrp, pIrp->IoStatus.Information));

        IoCompleteRequest(pIrp, IO_NO_INCREMENT);

        // 如果这个包描述符不是从接收包池里分配的，那么就是从
        // 网卡驱动里重用的。如果是重用的，调用NdisReturnPackets
        // 归还给网卡驱动，让它释放。
        if (NdisGetPoolFromPacket(pRcvPacket) != pOpenContext->RecvPacketPool)
        {
            NdisReturnPackets(&pRcvPacket, 1);
        }
        else
        {
            // 否则的话自己释放。
            ndisprotFreeReceivePacket(pOpenContext, pRcvPacket);
        }

        NPROT_DEREF_OPEN(pOpenContext);    // took out pended Read

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
        pOpenContext->PendedReadCount--;

    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    NPROT_DEREF_OPEN(pOpenContext);    // temp ref - service reads
}




NDIS_STATUS
NdisProtReceive(
    IN NDIS_HANDLE                              ProtocolBindingContext,
    IN NDIS_HANDLE                              MacReceiveContext,
    __in_bcount(HeaderBufferSize) IN PVOID      pHeaderBuffer,
    IN UINT                                     HeaderBufferSize,
    __in_bcount(LookaheadBufferSize) IN PVOID   pLookaheadBuffer,
    IN UINT                                     LookaheadBufferSize,
    IN UINT                                     PacketSize
    )
/*++

Routine Description:

    Our protocol receive handler called by NDIS, typically if we have
    a miniport below that doesn't indicate packets.

    We make a local packet/buffer copy of this data, queue it up, and
    kick off the read service routine.

Arguments:

    ProtocolBindingContext - pointer to open context
    MacReceiveContext - for use in NdisTransferData
    pHeaderBuffer - pointer to data header
    HeaderBufferSize - size of the above
    pLookaheadBuffer - pointer to buffer containing lookahead data
    LookaheadBufferSize - size of the above
    PacketSize - size of the entire packet, minus header size.

Return Value:

    NDIS_STATUS_NOT_ACCEPTED - if this packet is uninteresting
    NDIS_STATUS_SUCCESS - if we processed this successfully

--*/
{
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    NDIS_STATUS             Status;
    PNDIS_PACKET            pRcvPacket;
    PUCHAR                  pRcvData;
    UINT                    BytesTransferred;
    PNDIS_BUFFER            pOriginalNdisBuffer, pPartialNdisBuffer;

    // 获得绑定句柄。
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);
    pRcvPacket = NULL;
    pRcvData = NULL;
    Status = NDIS_STATUS_SUCCESS;

    do
    {
        // 如果头长度不是以太网包头的长度，则不接收这个包。
        // 本协议只接收以太网包。
        if (HeaderBufferSize != sizeof(NDISPROT_ETH_HEADER))
        {
            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }

        // 这个比较比较奇怪。难道头长度是负数?
        if ((PacketSize + HeaderBufferSize) < PacketSize)
        {
            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }
       
        // 分配一个包。包括包描述符和缓冲描述符，以及内存
        // 空间，一次性分配好。
        pRcvPacket = ndisprotAllocateReceivePacket(
                        pOpenContext,
                        PacketSize + HeaderBufferSize,
                        &pRcvData
                        );

        // 如果分配失败了，就不再接收这个包了。
        if ((pRcvPacket == NULL) || (pRcvData == NULL))
        {
            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }

        // 内存拷贝。先拷贝以太网包头。
        NdisMoveMappedMemory(pRcvData, pHeaderBuffer, HeaderBufferSize);

        // 检查前视区里是否包含了完整包的数据。
        if (PacketSize == LookaheadBufferSize)
        {
            // 如果前视区已经包括了整个数据包，那么调用NdisCopyLookaheadData
            // 就得到了完整的包，然后调用ndisprotQueueReceivePacket将这个包
            // 插入队列即可。
            NdisCopyLookaheadData(pRcvData+HeaderBufferSize,
                                  pLookaheadBuffer,
                                  LookaheadBufferSize,
                                  pOpenContext->MacOptions);
            ndisprotQueueReceivePacket(pOpenContext, pRcvPacket);
        }
        else
        {
            // 否则的话，需要分配一个新的缓冲描述符。请注意这个描述
            // 符号对应的是从包缓冲区开始之后HeaderBufferSize个字节之
            // 后处开始的空间（pRcvData + HeaderBufferSize）。
            NdisAllocateBuffer(
                &Status,
                &pPartialNdisBuffer,
                pOpenContext->RecvBufferPool,
                pRcvData + HeaderBufferSize,
                PacketSize);
            
            if (Status == NDIS_STATUS_SUCCESS)
            {
                // 如果成功了，就把包上原有的缓冲解链。使原来的缓冲描述
                // 符脱离包描述符。
                NdisUnchainBufferAtFront(pRcvPacket, &pOriginalNdisBuffer);
                // 现在把原来的包描述符保存在包描述符中（保留以备后用）
                NPROT_RCV_PKT_TO_ORIGINAL_BUFFER(pRcvPacket) = pOriginalNdisBuffer;

                // 然后把新的缓冲描述符连接到包上。
                NdisChainBufferAtBack(pRcvPacket, pPartialNdisBuffer);

                DEBUGP(DL_LOUD, ("Receive: setting up for TransferData:"
                        " Pkt %p, OriginalBuf %p, PartialBuf %p\n",
                        pRcvPacket, pOriginalNdisBuffer, pPartialNdisBuffer));

                // 然后调用NdisTransferData来传输数据包剩余的部分。这个
                // 调用完成之后，协议特征中的NdisProtTransferDataComplete
                // 会被调用。
                NdisTransferData(
                    &Status,
                    pOpenContext->BindingHandle,
                    MacReceiveContext,
                    0,  // ByteOffset
                    PacketSize,
                    pRcvPacket,
                    &BytesTransferred);
            }
            else
            {
                // 如果失败了，就不会调用NdisTransferData。但是我们还是
                // 要在NdisProtTransferDataComplete中做最后的处理。所以
                // 自己填写BytesTransferred。
                BytesTransferred = 0;
            }
    
            if (Status != NDIS_STATUS_PENDING)
            {
                // 如果前面就失败了，我们自己调用NdisProtTransferDataComplete。
                NdisProtTransferDataComplete(
                    (NDIS_HANDLE)pOpenContext,
                    pRcvPacket,
                    Status,
                    BytesTransferred);
            }
        }
    }
    while (FALSE);

    DEBUGP(DL_LOUD, ("Receive: Open %p, Pkt %p, Size %d\n",
            pOpenContext, pRcvPacket, PacketSize));

    return Status;
}



VOID
NdisProtTransferDataComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN PNDIS_PACKET                 pNdisPacket,
    IN NDIS_STATUS                  TransferStatus,
    IN UINT                         BytesTransferred
    )
/*++

Routine Description:

    NDIS entry point called to signal completion of a call to
    NdisTransferData that had pended.

Arguments:

    ProtocolBindingContext - pointer to open context
    pNdisPacket - our receive packet into which data is transferred
    TransferStatus - status of the transfer
    BytesTransferred - bytes copied into the packet.

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    PNDIS_BUFFER            pOriginalBuffer, pPartialBuffer;

    UNREFERENCED_PARAMETER(BytesTransferred);
    
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    // 得到保存过的旧的缓冲描述符。要记得在传输之前，为了让传输
    // 的内容正确的写到以太网包头后，我们分配了一个新的缓冲描述
    // 符替换了旧的缓冲描述符。现在要恢复它了。
    pOriginalBuffer = NPROT_RCV_PKT_TO_ORIGINAL_BUFFER(pNdisPacket);
    if (pOriginalBuffer != NULL)
    {

        // 和前面的替换时的操作一样，先Unchain，然后再调用Chain。
        // 调用之后已经恢复了使用旧的包描述符。
        NdisUnchainBufferAtFront(pNdisPacket, &pPartialBuffer);
        NdisChainBufferAtBack(pNdisPacket, pOriginalBuffer);

        DEBUGP(DL_LOUD, ("TransferComp: Pkt %p, OrigBuf %p, PartialBuf %p\n",
                pNdisPacket, pOriginalBuffer, pPartialBuffer));

        ASSERT(pPartialBuffer != NULL);
        
        // 那么那个新的包描述符已经没用了，调用NdisFreeBuffer释放它。
        if (pPartialBuffer != NULL)
        {
            NdisFreeBuffer(pPartialBuffer);
        }
    }

    if (TransferStatus == NDIS_STATUS_SUCCESS)
    {
        // 如果传输是成功的，将包保存到接收队列中。
        ndisprotQueueReceivePacket(pOpenContext, pNdisPacket);
    }
    else
    {
        // 如果传输失败了，直接释放这个包。
        ndisprotFreeReceivePacket(pOpenContext, pNdisPacket);
    }
}


VOID
NdisProtReceiveComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext
    )
/*++

Routine Description:

    Protocol entry point called by NDIS when the miniport
    has finished indicating up a batch of receives.

    We ignore this.

Arguments:

    ProtocolBindingContext - pointer to open context

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT   pOpenContext;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    return;
}


INT
NdisProtReceivePacket(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN PNDIS_PACKET                 pNdisPacket
    )
/*++

Routine Description:

    Protocol entry point called by NDIS if the driver below
    uses NDIS 4 style receive packet indications.

    If the miniport allows us to hold on to this packet, we
    use it as is, otherwise we make a copy.

Arguments:

    ProtocolBindingContext - pointer to open context
    pNdisPacket - the packet being indicated up.

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT   pOpenContext;
    PNDIS_BUFFER            pNdisBuffer;
    UINT                    BufferLength;
    PNDISPROT_ETH_HEADER     pEthHeader;
    PNDIS_PACKET            pCopyPacket;
    PUCHAR                  pCopyBuf;
    UINT                    TotalPacketLength;
    UINT                    BytesCopied;
    INT                     RefCount = 0;
    NDIS_STATUS             Status;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

#ifdef NDIS51
    NdisGetFirstBufferFromPacketSafe(
        pNdisPacket,
        &pNdisBuffer,
        &pEthHeader,
        &BufferLength,
        &TotalPacketLength,
        NormalPagePriority);

    if (pEthHeader == NULL)
    {
        //
        //  The system is low on resources. Set up to handle failure
        //  below.
        //
        BufferLength = 0;
    }
#else

    // 从包描述符中得到第一个缓冲描述符。
    NdisGetFirstBufferFromPacket(
        pNdisPacket,
        &pNdisBuffer,
        &pEthHeader,
        &BufferLength,
        &TotalPacketLength);
#endif

    do
    {
        // 如果这个包的长度比以太网包头还要小，丢弃之。
        if (BufferLength < sizeof(NDISPROT_ETH_HEADER))
        {
            DEBUGP(DL_WARN,
                ("ReceivePacket: Open %p, runt pkt %p, first buffer length %d\n",
                    pOpenContext, pNdisPacket, BufferLength));

            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }

        DEBUGP(DL_LOUD, ("ReceivePacket: Open %p, interesting pkt %p\n",
                    pOpenContext, pNdisPacket));

        // 如果这个包有NDIS_STATUS_RESOURCES状态，则必须拷贝
        // 而不能重用该包。当然这样就比较消耗时间和资源了。
        if ((NDIS_GET_PACKET_STATUS(pNdisPacket) == NDIS_STATUS_RESOURCES) ||
            pOpenContext->bRunningOnWin9x)
        {
            // 下面是分配一个包并拷贝其内容。读者可以参考前面讲过
            // 的内容来理解。
            pCopyPacket = ndisprotAllocateReceivePacket(
                            pOpenContext,
                            TotalPacketLength,
                            &pCopyBuf
                            );
            
            if (pCopyPacket == NULL)
            {
                DEBUGP(DL_FATAL, ("ReceivePacket: Open %p, failed to"
                    " alloc copy, %d bytes\n", pOpenContext, TotalPacketLength));
                break;
            }

            // 调用NdisCopyFromPacketToPacket来拷贝包。当然在拷贝之
            // 前调用者必须确保目标包的缓冲区长度是足够的。
            NdisCopyFromPacketToPacket(
                pCopyPacket,
                0,
                TotalPacketLength,
                pNdisPacket,
                0,
                &BytesCopied);
            
            NPROT_ASSERT(BytesCopied == TotalPacketLength);
            // 那么现在开始就用新的包了。
            pNdisPacket = pCopyPacket;
        }
        else
        {
            // 返回值。返回值表示的是我们已经一次引用了这个包。
            // 当处理完结的时候，我们就可以调用NdisReturnPackets
            // 来要求下层驱动释放这个包了。本函数把RefCount当做
            // 返回值。如果返回了0，那么下层驱动会认为我们不再
            // 需要这个数据包。
            RefCount = 1;
        }
        // 将数据包放入队列里。
        ndisprotQueueReceivePacket(pOpenContext, pNdisPacket);
    
    }
    while (FALSE);
    return (RefCount);
}


VOID
ndisprotQueueReceivePacket(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN PNDIS_PACKET                 pRcvPacket
    )
/*++

Routine Description:

    Queue up a received packet on the open context structure.
    If the queue size goes beyond a water mark, discard a packet
    at the head of the queue.

    Finally, run the queue service routine.

Arguments:
    
    pOpenContext - pointer to open context
    pRcvPacket - the received packet

Return Value:

    None

--*/
{
    PLIST_ENTRY     pEnt;
    PLIST_ENTRY     pDiscardEnt;
    PNDIS_PACKET    pDiscardPkt;

    do
    {
        pEnt = NPROT_RCV_PKT_TO_LIST_ENTRY(pRcvPacket);
        NPROT_REF_OPEN(pOpenContext);    // queued rcv packet
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        // 如果处于活动的状态，并且正确的电源状态，那么就把这个包
        // 插入接收缓冲链表中。
        if (NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE) &&
            (pOpenContext->PowerState == NetDeviceStateD0))
        {
            NPROT_INSERT_TAIL_LIST(&pOpenContext->RecvPktQueue, pEnt);
            pOpenContext->RecvPktCount++;
            DEBUGP(DL_VERY_LOUD, ("QueueReceivePacket: open %p,"
                    " queued pkt %p, queue size %d\n",
                    pOpenContext, pRcvPacket, pOpenContext->RecvPktCount));
        }
        else
        {

            // 否则的话，就直接释放掉这个包即可。
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            ndisprotFreeReceivePacket(pOpenContext, pRcvPacket);
            NPROT_DEREF_OPEN(pOpenContext);  // dropped rcv packet - bad state
            break;
        }

        // 如果输入缓冲区里包太多了，就要删除一个。
        if (pOpenContext->RecvPktCount > MAX_RECV_QUEUE_SIZE)
        {
            // 要删除的包的链节点指针
            pDiscardEnt = pOpenContext->RecvPktQueue.Flink;
            NPROT_REMOVE_ENTRY_LIST(pDiscardEnt);
            // 接收包数量减去1
            pOpenContext->RecvPktCount --;
            // 可以释放锁了。
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            // 从链节点转换为包的指针
            pDiscardPkt = NPROT_LIST_ENTRY_TO_RCV_PKT(pDiscardEnt);
            // 把包释放掉。
            ndisprotFreeReceivePacket(pOpenContext, pDiscardPkt);
            // 打开上下文解引用。这是因为每入队一个都要增加一次引用计数。
            NPROT_DEREF_OPEN(pOpenContext);  // dropped rcv packet - queue too long
            DEBUGP(DL_INFO, ("QueueReceivePacket: open %p queue"
                    " too long, discarded pkt %p\n",
                    pOpenContext, pDiscardPkt));
        }
        else
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
        }

        // 服务函数。这个函数看是否有未决的读请求。如果有，就取包
        // 来完成这个请求。
        ndisprotServiceReads(pOpenContext);
    }
    while (FALSE);
}


PNDIS_PACKET
ndisprotAllocateReceivePacket(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN UINT                         DataLength,
    OUT PUCHAR *                    ppDataBuffer
    )
/*++

Routine Description:

    Allocate resources to copy and queue a received packet.

Arguments:

    pOpenContext - pointer to open context for received packet
    DataLength - total length in bytes of the packet
    ppDataBuffer - place to return pointer to allocated buffer

Return Value:

    Pointer to NDIS packet if successful, else NULL.

--*/
{
    PNDIS_PACKET            pNdisPacket;
    PNDIS_BUFFER            pNdisBuffer;
    PUCHAR                  pDataBuffer;
    NDIS_STATUS             Status;

    pNdisPacket = NULL;
    pNdisBuffer = NULL;
    pDataBuffer = NULL;

    do
    {
        NPROT_ALLOC_MEM(pDataBuffer, DataLength);

        if (pDataBuffer == NULL)
        {
            DEBUGP(DL_FATAL, ("AllocRcvPkt: open %p, failed to alloc"
                " data buffer %d bytes\n", pOpenContext, DataLength));
            break;
        }

        //
        //  Make this an NDIS buffer.
        //
        NdisAllocateBuffer(
            &Status,
            &pNdisBuffer,
            pOpenContext->RecvBufferPool,
            pDataBuffer,
            DataLength);
        
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_FATAL, ("AllocateRcvPkt: open %p, failed to alloc"
                " NDIS buffer, %d bytes\n", pOpenContext, DataLength));
            break;
        }

        NdisAllocatePacket(&Status, &pNdisPacket, pOpenContext->RecvPacketPool);

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_FATAL, ("AllocateRcvPkt: open %p, failed to alloc"
                " NDIS packet, %d bytes\n", pOpenContext, DataLength));
            break;
        }

        NDIS_SET_PACKET_STATUS(pNdisPacket, 0);
        NPROT_RCV_PKT_TO_ORIGINAL_BUFFER(pNdisPacket) = NULL;

        NdisChainBufferAtFront(pNdisPacket, pNdisBuffer);

        *ppDataBuffer = pDataBuffer;

      
    }
    while (FALSE);

    if (pNdisPacket == NULL)
    {
        //
        //  Clean up
        //
        if (pNdisBuffer != NULL)
        {
            NdisFreeBuffer(pNdisBuffer);
        }

        if (pDataBuffer != NULL)
        {
            NPROT_FREE_MEM(pDataBuffer);
        }
    }

    return (pNdisPacket);
}



VOID
ndisprotFreeReceivePacket(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN PNDIS_PACKET                 pNdisPacket
    )
/*++

Routine Description:

    Free up all resources associated with a received packet. If this
    is a local copy, free the packet to our receive pool, else return
    this to the miniport.

Arguments:
    
    pOpenContext - pointer to open context
    pNdisPacket - pointer to packet to be freed.

Return Value:

    None

--*/
{
    PNDIS_BUFFER        pNdisBuffer;
    UINT                TotalLength;
    UINT                BufferLength;
    PUCHAR              pCopyData;

    if (NdisGetPoolFromPacket(pNdisPacket) == pOpenContext->RecvPacketPool)
    {
        //
        //  This is a local copy.
        //
#ifdef NDIS51
        NdisGetFirstBufferFromPacketSafe(
            pNdisPacket,
            &pNdisBuffer,
            (PVOID *)&pCopyData,
            &BufferLength,
            &TotalLength,
            NormalPagePriority);
#else
        NdisGetFirstBufferFromPacket(
            pNdisPacket,
            &pNdisBuffer,
            (PVOID *)&pCopyData,
            &BufferLength,
            &TotalLength);
#endif

        NPROT_ASSERT(BufferLength == TotalLength);

        NPROT_ASSERT(pNdisBuffer != NULL);

        NPROT_ASSERT(pCopyData != NULL); // we would have allocated non-paged pool

        NdisFreePacket(pNdisPacket);

        NdisFreeBuffer(pNdisBuffer);

        NPROT_FREE_MEM(pCopyData);
    }
    else
    {
        NdisReturnPackets(&pNdisPacket, 1);
    }
}
        

VOID
ndisprotCancelPendingReads(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext
    )
/*++

Routine Description:

    Cancel any pending read IRPs queued on the given open.

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    PIRP                pIrp;
    PLIST_ENTRY         pIrpEntry;

    NPROT_REF_OPEN(pOpenContext);  // temp ref - cancel reads

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    while (!NPROT_IS_LIST_EMPTY(&pOpenContext->PendedReads))
    {
        //
        //  Get the first pended Read IRP
        //
        pIrpEntry = pOpenContext->PendedReads.Flink;
        pIrp = CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry);

        //
        //  Check to see if it is being cancelled.
        //
        if (IoSetCancelRoutine(pIrp, NULL))
        {
            //
            //  It isn't being cancelled, and can't be cancelled henceforth.
            //
            NPROT_REMOVE_ENTRY_LIST(pIrpEntry);

            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

            //
            //  Complete the IRP.
            //
            pIrp->IoStatus.Status = STATUS_CANCELLED;
            pIrp->IoStatus.Information = 0;

            DEBUGP(DL_INFO, ("CancelPendingReads: Open %p, IRP %p cancelled\n",
                pOpenContext, pIrp));

            IoCompleteRequest(pIrp, IO_NO_INCREMENT);

            NPROT_DEREF_OPEN(pOpenContext);    // took out pended Read for cancelling

            NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
            pOpenContext->PendedReadCount--;
        }
        else
        {
            //
            //  It is being cancelled, let the cancel routine handle it.
            //
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

            //
            //  Give the cancel routine some breathing space, otherwise
            //  we might end up examining the same (cancelled) IRP over
            //  and over again.
            //
            NPROT_SLEEP(1);

            NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
        }
    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    NPROT_DEREF_OPEN(pOpenContext);    // temp ref - cancel reads
}


VOID
ndisprotFlushReceiveQueue(
    IN PNDISPROT_OPEN_CONTEXT            pOpenContext
    )
/*++

Routine Description:

    Free any receive packets queued up on the specified open

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    PLIST_ENTRY         pRcvPacketEntry;
    PNDIS_PACKET        pRcvPacket;

    NPROT_REF_OPEN(pOpenContext);  // temp ref - flushRcvQueue

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
    
    while (!NPROT_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
    {
        //
        //  Get the first queued receive packet
        //
        pRcvPacketEntry = pOpenContext->RecvPktQueue.Flink;
        NPROT_REMOVE_ENTRY_LIST(pRcvPacketEntry);

        pOpenContext->RecvPktCount --;

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        pRcvPacket = NPROT_LIST_ENTRY_TO_RCV_PKT(pRcvPacketEntry);

        DEBUGP(DL_LOUD, ("FlushReceiveQueue: open %p, pkt %p\n",
            pOpenContext, pRcvPacket));

        ndisprotFreeReceivePacket(pOpenContext, pRcvPacket);

        NPROT_DEREF_OPEN(pOpenContext);    // took out pended Read

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    NPROT_DEREF_OPEN(pOpenContext);    // temp ref - flushRcvQueue
}



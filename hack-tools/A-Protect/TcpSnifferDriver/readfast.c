#include <ntddk.h>
#include <ndis.h>

#include "packet.h"
#include "ndis5pkt.h"

NDIS_STATUS
NdisuioReceive(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN NDIS_HANDLE                  MacReceiveContext,
    IN PVOID                        pHeaderBuffer,
    IN UINT                         HeaderBufferSize,
    IN PVOID                        pLookaheadBuffer,
    IN UINT                         LookaheadBufferSize,
    IN UINT                         PacketSize
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
    PNDISUIO_OPEN_CONTEXT   pOpenContext;
    NDIS_STATUS             Status;
    PNDISUIO_ETH_HEADER     pEthHeader;
    PNDIS_PACKET            pRcvPacket;
    PUCHAR                  pRcvData;
    UINT                    BytesTransferred;
    PNDIS_BUFFER            pOriginalNdisBuffer, pPartialNdisBuffer;
    PIRP					pIrp;
    PLIST_ENTRY				pIrpEntry;
    ULONG					BytesRemaining; // at pDst
	PPACKET_GROUP			pGroup;

	//ULONG                   pDst;
	
    pOpenContext = (PNDISUIO_OPEN_CONTEXT)ProtocolBindingContext;
    NUIO_STRUCT_ASSERT(pOpenContext, oc);
    pRcvPacket = NULL;
    pRcvData = NULL;
    Status = NDIS_STATUS_SUCCESS;

    DEBUGP(DL_LOUD, ("Receive: Open %p, LookaheadBufferSize %d, PacketSize %d\n",
		pOpenContext, LookaheadBufferSize, PacketSize));
    
	NdisInterlockedAddLargeStatistic((PLARGE_INTEGER)&pOpenContext->ReceivedPackets, 1);

	do
    {
        if (HeaderBufferSize != sizeof(NDISUIO_ETH_HEADER))
        {
            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }

        pEthHeader = (PNDISUIO_ETH_HEADER)pHeaderBuffer;

		NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);

		//
		// Someone is reading, and this is the first packet.
		//
		if (!NUIO_IS_LIST_EMPTY(&pOpenContext->PendedReads) &&
			NUIO_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
		{
			//
			//  Get the first pended Read IRP
			//
			pIrpEntry = pOpenContext->PendedReads.Flink;
			pIrp = CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry);
			
			//
			// We don't have to worry about the situation where the IRP is cancelled
			// after we remove it from the queue and before we reset the cancel
			// routine because the cancel routine has been coded to cancel an IRP
			// only if it's in the queue.
			//
			IoSetCancelRoutine(pIrp, NULL);

			NUIO_REMOVE_ENTRY_LIST(pIrpEntry);
			
			pOpenContext->PendedReadCount--;

			NUIO_RELEASE_LOCK(&pOpenContext->Lock);

			NUIO_DEREF_OPEN(pOpenContext);  // Service: dequeue rcv packet

			//
			//  Copy as much data as possible from the receive packet to
			//  the IRP MDL.
			//
#ifndef WIN9X
			pGroup = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
			//NUIO_ASSERT(pDst != NULL);  // since it was already mapped
#else
			pGroup = MmGetSystemAddressForMdl(pIrp->MdlAddress);  // Win9x
#endif
			BytesRemaining = MmGetMdlByteCount(pIrp->MdlAddress);
			
			BytesRemaining -= sizeof(PACKET_GROUP);

			//
			//  copy the ethernet header into the actual readbuffer
			//
			NdisMoveMappedMemory(pGroup->Data, pHeaderBuffer, HeaderBufferSize);
			
			if (PacketSize == LookaheadBufferSize)
			{
				BytesTransferred = MIN(LookaheadBufferSize, BytesRemaining);

				NdisCopyLookaheadData(pGroup->Data + HeaderBufferSize,
					pLookaheadBuffer,
					BytesTransferred,
					pOpenContext->MacOptions);

				pGroup->Length = BytesTransferred + HeaderBufferSize;
				
				pIrp->IoStatus.Information = pGroup->Length + sizeof(PACKET_GROUP);
				pIrp->IoStatus.Status = STATUS_SUCCESS;				
		
				DEBUGP(DL_LOUD, ("Receive: %d bytes\n", pIrp->IoStatus.Information));
		
				IoCompleteRequest(pIrp, IO_NO_INCREMENT);
			}
			else
			{
				BytesTransferred = 0;

				NdisAllocatePacket(
					&Status,
					&pRcvPacket,
					pOpenContext->RecvBufferPool
					);
				
				if (Status != NDIS_STATUS_SUCCESS)
					goto ERROR;
				
				//
				//  Allocate an MDL to map the portion of the buffer following the
				//  header
				//
				pPartialNdisBuffer = IoAllocateMdl(pGroup->Data, BytesRemaining, FALSE, FALSE, NULL);
				
				if (pPartialNdisBuffer == NULL)
				{
					NdisFreePacket(pRcvPacket);
					Status = NDIS_STATUS_RESOURCES;
					goto ERROR;
				}
				
				//
				//  Build the mdl to point to the the portion of the buffer following
				//  the header
				//
				IoBuildPartialMdl(
					pIrp->MdlAddress,
					pPartialNdisBuffer,
					pGroup->Data + HeaderBufferSize,
					0);
				
				//
				//  Clear the next link in the new MDL
				//
				
				pPartialNdisBuffer->Next = NULL;
				
				//
				//  Get a pointer to the packet itself.
				//
				
				NUIO_IRP_FROM_RCV_PKT(pRcvPacket) = pIrp;
				NUIO_RCV_PKT_TO_ORIGINAL_BUFFER(pRcvPacket) = pPartialNdisBuffer;
				
				//
				//  Attach our partial MDL to the packet
				//
				
				NdisChainBufferAtFront(pRcvPacket, pPartialNdisBuffer);
				
				//
				//  Call the Mac to transfer the packet
				//
				
				NdisTransferData(
					&Status,
					pOpenContext->BindingHandle,
					MacReceiveContext,
					0,  // ByteOffset
					PacketSize,
					pRcvPacket,
					&BytesTransferred);

ERROR:					
				//
				//  If it didn't pend, call the completeion routine now
				//
				if (Status != NDIS_STATUS_PENDING)
				{
					NdisuioTransferDataComplete(
						(NDIS_HANDLE)pOpenContext,
						pRcvPacket,
						Status,
						BytesTransferred);
				}
			}

			break;
		}

		NUIO_RELEASE_LOCK(&pOpenContext->Lock);

		//
        //  Allocate resources for queueing this up.
        //
        pRcvPacket = ndisuioAllocateReceivePacket(
			pOpenContext,
			PacketSize + HeaderBufferSize,
			&pRcvData
			);

        if (pRcvPacket == NULL)
        {
            Status = NDIS_STATUS_NOT_ACCEPTED;
            break;
        }

        NdisMoveMappedMemory(pRcvData, pHeaderBuffer, HeaderBufferSize);

        //
        //  Check if the entire packet is within the lookahead.
        //
        if (PacketSize == LookaheadBufferSize)
        {
            NdisCopyLookaheadData(pRcvData + HeaderBufferSize,
                                  pLookaheadBuffer,
                                  LookaheadBufferSize,
                                  pOpenContext->MacOptions);
            //
            //  Queue this up for receive processing, and
            //  try to complete some read IRPs.
            //
            ndisuioQueueReceivePacket(pOpenContext, pRcvPacket);
        }
        else
        {
            //
            //  Allocate an NDIS buffer to map the receive area
            //  at an offset "HeaderBufferSize" from the current
            //  start. This is so that NdisTransferData can copy
            //  in at the right point in the destination buffer.
            //

            NdisAllocateBuffer(
                &Status,
                &pPartialNdisBuffer,
                pOpenContext->RecvBufferPool,
                pRcvData + HeaderBufferSize,
                PacketSize);
            
            if (Status == NDIS_STATUS_SUCCESS)
            {
                //
                //  Unlink and save away the original NDIS Buffer
                //  that maps the full receive buffer.
                //
                NdisUnchainBufferAtFront(pRcvPacket, &pOriginalNdisBuffer);
                NUIO_RCV_PKT_TO_ORIGINAL_BUFFER(pRcvPacket) = pOriginalNdisBuffer;
				NUIO_IRP_FROM_RCV_PKT(pRcvPacket) = NULL;

                //
                //  Link in the partial buffer for NdisTransferData to
                //  operate on.
                //
                NdisChainBufferAtBack(pRcvPacket, pPartialNdisBuffer);

                DEBUGP(DL_LOUD, ("Receive: setting up for TransferData:"
                        " Pkt %p, OriginalBuf %p, PartialBuf %p\n",
                        pRcvPacket, pOriginalNdisBuffer, pPartialNdisBuffer));

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
                //
                //  Failure handled below in TransferDataComplete.
                //
                BytesTransferred = 0;
            }
    
            if (Status != NDIS_STATUS_PENDING)
            {
                NdisuioTransferDataComplete(
                    (NDIS_HANDLE)pOpenContext,
                    pRcvPacket,
                    Status,
                    BytesTransferred);
            }
        }
    } while (FALSE);

	if (Status != NDIS_STATUS_SUCCESS && Status != NDIS_STATUS_PENDING)
		NdisInterlockedAddLargeStatistic((PLARGE_INTEGER)&pOpenContext->DroppedPackets, 1);

    return Status;
}

VOID
NdisuioTransferDataComplete(
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
    PNDISUIO_OPEN_CONTEXT   pOpenContext;
    PNDIS_BUFFER            pOriginalBuffer, pPartialBuffer;
	PIRP					pIrp;
	PPACKET_GROUP			pGroup;
	//ULONG                   pDst;

    pOpenContext = (PNDISUIO_OPEN_CONTEXT)ProtocolBindingContext;
    NUIO_STRUCT_ASSERT(pOpenContext, oc);

	pIrp = NUIO_IRP_FROM_RCV_PKT(pNdisPacket);
	if (pIrp)
	{
		PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(pIrp);

		pOriginalBuffer = NUIO_RCV_PKT_TO_ORIGINAL_BUFFER(pNdisPacket);
		
		//
		//  Free the partial MDL that we allocated
		//
		if (pOriginalBuffer)
			IoFreeMdl(pOriginalBuffer);
		
		//
		//  Put the packet on the free queue
		//
		NdisFreePacket(pNdisPacket);
		
#ifndef WIN9X
		pGroup = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
		//NUIO_ASSERT(pDst != NULL);  // since it was already mapped
#else
		pGroup = MmGetSystemAddressForMdl(pIrp->MdlAddress);  // Win9x
#endif
		pGroup->Length = BytesTransferred + sizeof(NDISUIO_ETH_HEADER);
		if (TransferStatus == NDIS_STATUS_SUCCESS)
		{
			pIrp->IoStatus.Status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = pGroup->Length + sizeof(PACKET_GROUP);
		}
		else
		{
			pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			pIrp->IoStatus.Information = 0;
		}
		
		DEBUGP(DL_LOUD, ("TransferComp: Pkt %p, OrigBuf %p, BytesTransferred %d\n",
			pNdisPacket, pOriginalBuffer, BytesTransferred));
		
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}
	else
	{
		//
		//  Check if an NDIS_BUFFER was created to map part of the receive buffer;
		//  if so, free it and link back the original NDIS_BUFFER that maps
		//  the full receive buffer to the packet.
		//
		pOriginalBuffer = NUIO_RCV_PKT_TO_ORIGINAL_BUFFER(pNdisPacket);
		if (pOriginalBuffer != NULL)
		{
			//
			//  We had stashed off the NDIS_BUFFER for the full receive
			//  buffer in the packet reserved area. Unlink the partial
			//  buffer and link in the full buffer.
			//
			NdisUnchainBufferAtFront(pNdisPacket, &pPartialBuffer);
			NdisChainBufferAtBack(pNdisPacket, pOriginalBuffer);
			
			DEBUGP(DL_LOUD, ("TransferComp: Pkt %p, OrigBuf %p, PartialBuf %p\n",
                pNdisPacket, pOriginalBuffer, pPartialBuffer));
			
			//
			//  Free up the partial buffer.
			//
			NdisFreeBuffer(pPartialBuffer);
		}
	}

    if (TransferStatus == NDIS_STATUS_SUCCESS)
    {
        //
        //  Queue this up for receive processing, and
        //  try to complete some read IRPs.
        //
        ndisuioQueueReceivePacket(pOpenContext, pNdisPacket);
    }
    else
    {
        ndisuioFreeReceivePacket(pOpenContext, pNdisPacket);

		NdisInterlockedAddLargeStatistic((PLARGE_INTEGER)&pOpenContext->DroppedPackets, 1);
    }
}

NTSTATUS
NdisuioDoFastRead(
    IN PNDISUIO_OPEN_CONTEXT        pOpenContext,
	IN PIRP							pIrp
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
    PNDIS_PACKET        pRcvPacket;
    PLIST_ENTRY         pRcvPacketEntry;
    PUCHAR              pSrc, pDst;
    ULONG               BytesRemaining; // at pDst
    PNDIS_BUFFER        pNdisBuffer;
    ULONG               BytesAvailable;
	PPACKET_GROUP		pGroup;
	CCHAR				PriorityBoost = IO_NETWORK_INCREMENT;

    DEBUGP(DL_VERY_LOUD, ("FastRead: open %p/%x\n", pOpenContext, pOpenContext->Flags));

	if (NUIO_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
		return STATUS_UNSUCCESSFUL;

	//
	//  Copy as much data as possible from the receive packet to
	//  the IRP MDL.
	//
#ifndef WIN9X
	pGroup = MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
	NUIO_ASSERT(pGroup != NULL);  // since it was already mapped
#else
	pGroup = MmGetSystemAddressForMdl(pIrp->MdlAddress);  // Win9x
#endif
	BytesRemaining = MmGetMdlByteCount(pIrp->MdlAddress);

	//
	//  Get the first queued receive packet
	//
	pRcvPacketEntry = pOpenContext->RecvPktQueue.Flink;
	pRcvPacket = NUIO_LIST_ENTRY_TO_RCV_PKT(pRcvPacketEntry);

	do
	{
        NUIO_REMOVE_ENTRY_LIST(pRcvPacketEntry);

        pOpenContext->RecvPktCount--;

        NUIO_RELEASE_LOCK(&pOpenContext->Lock);

        NUIO_DEREF_OPEN(pOpenContext);  // Service: dequeue rcv packet

        pNdisBuffer = pRcvPacket->Private.Head;

		pDst = pGroup->Data;

		BytesRemaining -= sizeof(PACKET_GROUP);
		
        while (BytesRemaining && (pNdisBuffer != NULL))
        {
#ifndef WIN9X
            NdisQueryBufferSafe(pNdisBuffer, &pSrc, &BytesAvailable, NormalPagePriority);

            if (pSrc == NULL) 
            {
                DEBUGP(DL_FATAL, ("FastRead: Open %p, QueryBuffer failed for buffer %p\n",
                            pOpenContext, pNdisBuffer));

				break;
            }
#else
            NdisQueryBuffer(pNdisBuffer, &pSrc, &BytesAvailable);
#endif

            if (BytesAvailable)
            {
                ULONG BytesToCopy = MIN(BytesAvailable, BytesRemaining);

                NUIO_COPY_MEM(pDst, pSrc, BytesToCopy);
                BytesRemaining -= BytesToCopy;
                pDst += BytesToCopy;
            }

            NdisGetNextBuffer(pNdisBuffer, &pNdisBuffer);
        }

		ndisuioFreeReceivePacket(pOpenContext, pRcvPacket);

		pGroup->Length = pDst - pGroup->Data;
		pGroup = (PPACKET_GROUP)pDst;
		
		NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);
		
		if (NUIO_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
		{
			PriorityBoost = IO_NO_INCREMENT;
			break;
		}

		pRcvPacketEntry = pOpenContext->RecvPktQueue.Flink;
		pRcvPacket = NUIO_LIST_ENTRY_TO_RCV_PKT(pRcvPacketEntry);

		NdisQueryPacketLength(pRcvPacket, &BytesAvailable);
		if (BytesRemaining < BytesAvailable + sizeof(PACKET_GROUP))
			break;
	} while (TRUE);
	
	NUIO_RELEASE_LOCK(&pOpenContext->Lock);

	//
	//  Complete the IRP.
	//
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = MmGetMdlByteCount(pIrp->MdlAddress) - BytesRemaining;
	
	DEBUGP(DL_INFO, ("FastRead: Open %p, IRP %p completed with %d bytes\n",
		pOpenContext, pIrp, pIrp->IoStatus.Information));
	
	IoCompleteRequest(pIrp, PriorityBoost);
	
	return STATUS_SUCCESS;
}

VOID
ndisuioServiceReads(
    IN PNDISUIO_OPEN_CONTEXT        pOpenContext
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

    DEBUGP(DL_VERY_LOUD, ("ServiceReads: open %p/%x\n", pOpenContext, pOpenContext->Flags));

    NUIO_REF_OPEN(pOpenContext);  // temp ref - service reads

    NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);

    while (!NUIO_IS_LIST_EMPTY(&pOpenContext->PendedReads) &&
           !NUIO_IS_LIST_EMPTY(&pOpenContext->RecvPktQueue))
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
            NUIO_REMOVE_ENTRY_LIST(pIrpEntry);

            //
            //  NOTE: we decrement PendedReadCount way below in the
            //  while loop, to avoid letting through a thread trying
            //  to unbind.
            //
        }
        else
        {
            //
            //  The IRP is being cancelled; let the cancel routine handle it.
            // __RAIN: I think this can never happened, because when cancelling
			// the irp, the pOpenContext is locked.
			//
            DEBUGP(DL_INFO, ("ServiceReads: open %p, skipping cancelled IRP %p\n",
                    pOpenContext, pIrp));

            continue;
        }

		NdisuioDoFastRead(pOpenContext, pIrp);

        NUIO_DEREF_OPEN(pOpenContext);    // took out pended Read

        NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);
        pOpenContext->PendedReadCount--;
    }

    NUIO_RELEASE_LOCK(&pOpenContext->Lock);

    NUIO_DEREF_OPEN(pOpenContext);    // temp ref - service reads
}

NTSTATUS
NdisuioRead(
    IN PDEVICE_OBJECT       pDeviceObject,
    IN PIRP                 pIrp
    )
/*++

Routine Description:

    Dispatch routine to handle IRP_MJ_READ. 

Arguments:

    pDeviceObject - pointer to our device object
    pIrp - Pointer to request packet

Return Value:

    NT status code.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
    PNDISUIO_OPEN_CONTEXT   pOpenContext;

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    do
    {
        if (pOpenContext == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: NULL FsContext on FileObject %p\n", pIrpSp->FileObject));
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }
            
        NUIO_STRUCT_ASSERT(pOpenContext, oc);

        if (pIrp->MdlAddress == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: NULL MDL address on IRP %p\n", pIrp));
            NtStatus = STATUS_INVALID_PARAMETER;
            break;
        }

        //
        // Try to get a virtual address for the MDL.
        //
#ifndef WIN9X
        if (MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority) == NULL)
        {
            DEBUGP(DL_FATAL, ("Read: MmGetSystemAddr failed for IRP %p, MDL %p\n",
                    pIrp, pIrp->MdlAddress));
            NtStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
#endif
        NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);

        if (!NUIO_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            NUIO_RELEASE_LOCK(&pOpenContext->Lock);
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }

		NtStatus = NdisuioDoFastRead(pOpenContext, pIrp);
		if (NtStatus == STATUS_SUCCESS)
		{ // if successful, lock already unlocked
			return NtStatus;
		}

        //
        //  Add this IRP to the list of pended Read IRPs
        //
        NUIO_INSERT_TAIL_LIST(&pOpenContext->PendedReads, &pIrp->Tail.Overlay.ListEntry);
        NUIO_REF_OPEN(pOpenContext);  // pended read IRP
        pOpenContext->PendedReadCount++;

        //
        //  Set up the IRP for possible cancellation.
        //
        pIrp->Tail.Overlay.DriverContext[0] = pOpenContext;
        IoMarkIrpPending(pIrp);
        IoSetCancelRoutine(pIrp, NdisuioCancelRead);

		NUIO_RELEASE_LOCK(&pOpenContext->Lock);

        NtStatus = STATUS_PENDING;
    } while (FALSE);

    if (NtStatus != STATUS_PENDING)
	{
		NUIO_ASSERT(NtStatus != STATUS_SUCCESS);
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = NtStatus;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}

    return (NtStatus);
}

VOID
ndisuioQueueReceivePacket(
    IN PNDISUIO_OPEN_CONTEXT        pOpenContext,
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

    do
    {
        pEnt = NUIO_RCV_PKT_TO_LIST_ENTRY(pRcvPacket);

        NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);

		// __RAIN: since we already received the packet, so queue it
		if (pOpenContext->RecvPktCount < MAX_RECV_QUEUE_SIZE)
		{
			NUIO_REF_OPEN(pOpenContext);    // queued rcv packet

            NUIO_INSERT_TAIL_LIST(&pOpenContext->RecvPktQueue, pEnt);
            pOpenContext->RecvPktCount++;

            DEBUGP(DL_VERY_LOUD, ("QueueReceivePacket: open %p,"
                    " queued pkt %p, queue size %d\n",
                    pOpenContext, pRcvPacket, pOpenContext->RecvPktCount));
        }
        else
        {
			static int first = 1;

			if (first)
			{
				nprintf("drop packet.\n");
				first = 0;
			}
            //
            //  No more buffer. Drop this.
            //
            NUIO_RELEASE_LOCK(&pOpenContext->Lock);

            ndisuioFreeReceivePacket(pOpenContext, pRcvPacket);

			NdisInterlockedAddLargeStatistic((PLARGE_INTEGER)&pOpenContext->DroppedPackets, 1);
            break;
        }

        NUIO_RELEASE_LOCK(&pOpenContext->Lock);

        //
        //  Run the receive queue service routine now.
        //
        ndisuioServiceReads(pOpenContext);
    } while (FALSE);
}
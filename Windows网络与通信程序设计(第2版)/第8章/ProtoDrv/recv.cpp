/////////////////////////////////////////
// recv.cpp文件
// NDIS协议入口点，处理接收数据的例程

extern "C"
{
	#include <ndis.h>
	#include <ntddk.h>
}

#include "nuiouser.h"
#include "ndisprot.h"



NTSTATUS DispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)pDevObj->DeviceExtension;
	IoIncrement(pOpen);
	do
	{
		if(!pOpen->bBound)
		{
			status = STATUS_DEVICE_NOT_READY;
			break;
		}
		PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(pIrp);
		if(irpSp->Parameters.Read.Length < ETHERNET_HEADER_LENGTH)
		{
			status = STATUS_BUFFER_TOO_SMALL;
			break;
		}
		
			// 申请封包描述表，并初始化
		PNDIS_PACKET pPacket;
		NdisAllocatePacket((PNDIS_STATUS)&status, &pPacket, pOpen->hPacketPool);
		if(status != NDIS_STATUS_SUCCESS)
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}
		RESERVED(pPacket)->pIrp = pIrp;
		RESERVED(pPacket)->pMdl = NULL;
		
			// 标识当前IRP请求未决，设置I/O请求取消例程
		IoMarkIrpPending(pIrp);
		IoSetCancelRoutine(pIrp, ReadCancelRoutine);
		
			// 添加到封包描述表列表中
		ExInterlockedInsertTailList(&pOpen->RcvList, 
			&RESERVED(pPacket)->ListElement, &pOpen->RcvSpinLock);
		
		return STATUS_PENDING;
		
	}while(FALSE);
	
	if(status != STATUS_SUCCESS)
	{
		IoDecrement(pOpen);
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}
	return status;
}

VOID
ReadCancelRoutine (
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
{
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)pDeviceObject->DeviceExtension;	
	NDIS_PACKET *pPacket = NULL;

	KIRQL oldIrql = pIrp->CancelIrql;

	KeAcquireSpinLockAtDpcLevel(&pOpen->RcvSpinLock);

	IoReleaseCancelSpinLock(KeGetCurrentIrql());

	// 遍历封包描述表列表，查找对应的封包指针。双链列表的表头没有使用，仅作为开始和结束标记
	PLIST_ENTRY pThisEntry = NULL;
	PACKET_RESERVED *pReserved;
	PLIST_ENTRY pHead = &pOpen->RcvList;
	for(pThisEntry = pHead->Flink; pThisEntry != pHead;  pThisEntry = pThisEntry->Flink)
	{
			pReserved = CONTAINING_RECORD(pThisEntry, PACKET_RESERVED, ListElement);
			if(pReserved->pIrp == pIrp)
			{
				// 从列表中移除此未决Irp的封包描述表
				RemoveEntryList(pThisEntry);
				pPacket = CONTAINING_RECORD(pReserved, NDIS_PACKET, ProtocolReserved);
				break;
			}
	}

	KeReleaseSpinLock(&pOpen->RcvSpinLock, oldIrql);

	if(pPacket != NULL)
	{
		// 释放此封包描述表占用的内存
		NdisFreePacket(pPacket);
		// 完成此未决的IRP请求
		pIrp->IoStatus.Status = STATUS_CANCELLED;
		pIrp->IoStatus.Information = 0;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		IoDecrement(pOpen);
	}
}


void CancelReadIrp(PDEVICE_OBJECT pDeviceObj)
{
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)pDeviceObj->DeviceExtension;
	PLIST_ENTRY thisEntry;
	PACKET_RESERVED *reserved;
	PNDIS_PACKET myPacket;
	PIRP pPendingIrp;
	// 移除所有未决的接收IRP请求，释放对应的封包描述表
	while(thisEntry = ExInterlockedRemoveHeadList(&pOpen->RcvList, &pOpen->RcvSpinLock))
	{
		reserved = CONTAINING_RECORD(thisEntry, PACKET_RESERVED, ListElement);
		myPacket = CONTAINING_RECORD(reserved, NDIS_PACKET, ProtocolReserved);
		pPendingIrp = RESERVED(myPacket)->pIrp;

		NdisFreePacket(myPacket);

		IoSetCancelRoutine(pPendingIrp, NULL);

		pPendingIrp->IoStatus.Information = 0;
		pPendingIrp->IoStatus.Status = STATUS_CANCELLED;

		IoCompleteRequest(pPendingIrp, IO_NO_INCREMENT);
		// 减小此适配器上的IO引用计数
		IoDecrement(pOpen);
	}
}


NDIS_STATUS
ProtocolReceive(
    IN NDIS_HANDLE ProtocolBindingContext,
    IN NDIS_HANDLE MacReceiveContext,
    IN PVOID       HeaderBuffer,
    IN UINT        HeaderBufferSize,
    IN PVOID       LookAheadBuffer,
    IN UINT        LookaheadBufferSize,
    IN UINT        PacketSize
    )
{
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)ProtocolBindingContext;

	if(HeaderBufferSize > ETHERNET_HEADER_LENGTH)
	{
		return NDIS_STATUS_SUCCESS;
	}

		// 从封包描述表列表中取出一个描述表
	PLIST_ENTRY pListEntry = ExInterlockedRemoveHeadList(&pOpen->RcvList, &pOpen->RcvSpinLock);
	if(pListEntry == NULL)	// 没有未决的读操作
	{
		return NDIS_STATUS_NOT_ACCEPTED;
	}

	PACKET_RESERVED *pReserved = CONTAINING_RECORD(pListEntry, PACKET_RESERVED, ListElement);
	NDIS_PACKET *pPacket = CONTAINING_RECORD(pReserved, NDIS_PACKET, ProtocolReserved);
	PIRP pIrp = RESERVED(pPacket)->pIrp;
	PIO_STACK_LOCATION pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

	IoSetCancelRoutine(pIrp, NULL);


		// 复制以太头到实际的读缓冲区
	NdisMoveMappedMemory(MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority), 
							HeaderBuffer, HeaderBufferSize);


		// 传输以太头后面的数据到读缓冲区

	// 读缓冲区余下部分的长度
	UINT nBufferLen = pIrpSp->Parameters.Read.Length - HeaderBufferSize; // ETHERNET_HEADER_LENGTH;
	// 计算实际要传输的字节
	UINT nSizeToTransfer = nBufferLen < LookaheadBufferSize ? nBufferLen : LookaheadBufferSize;
	
	// 申请一个MDL来映射读缓冲区中以太头以后的部分
	NDIS_STATUS status;
	PMDL pMdl = IoAllocateMdl(MmGetMdlVirtualAddress(pIrp->MdlAddress), 
									MmGetMdlByteCount(pIrp->MdlAddress), FALSE, FALSE, NULL);
	if(pMdl == NULL) 
	{
        status = NDIS_STATUS_RESOURCES;
        goto ERROR;
	}
	
	// 建立此MDL使它指向缓冲区中以太头后面的部分
    IoBuildPartialMdl(
        pIrp->MdlAddress,
        pMdl,
        ((PUCHAR)MmGetMdlVirtualAddress(pIrp->MdlAddress)) + ETHERNET_HEADER_LENGTH,
        0
        );

	// 清除新MDL中的下一个连接
    pMdl->Next=NULL;
	// 保存这个指针，以便在数据传输完毕后释放此MDL
    RESERVED(pPacket)->pMdl = pMdl;

	// 附加我们的部分MDL到封包
    NdisChainBufferAtFront(pPacket,pMdl);

    //  调用Mac来传输这个封包
	UINT nBytesTransfered;
    NdisTransferData(
        &status,
        pOpen->hAdapter,
        MacReceiveContext,
        0,
        nSizeToTransfer,
        pPacket,
        &nBytesTransfered);
    if(status == NDIS_STATUS_PENDING) 
	{
        return NDIS_STATUS_SUCCESS;
    }

ERROR:
	// 如果没有未决，现在就调用完成例程
    ProtocolTransferDataComplete(
                                pOpen,
                                pPacket,
                                status,
                                nBytesTransfered);
    return NDIS_STATUS_SUCCESS;
}

VOID
ProtocolTransferDataComplete (
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_PACKET  pPacket,
    IN NDIS_STATUS   Status,
    IN UINT          BytesTransfered
    )
{
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)ProtocolBindingContext;
	PMDL pMdl = RESERVED(pPacket)->pMdl;
	PIRP pIrp = RESERVED(pPacket)->pIrp;

		// 清除资源
	if(pMdl != NULL)
		IoFreeMdl(pMdl);
	NdisFreePacket(pPacket);

		// 完成此未决的IRP
	if(Status == NDIS_STATUS_SUCCESS)
	{
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		pIrp->IoStatus.Information = BytesTransfered + ETHERNET_HEADER_LENGTH;
	}
	else
	{
		pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
		pIrp->IoStatus.Information = 0;
	}
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoDecrement(pOpen);
}


VOID
ProtocolReceiveComplete(
    IN NDIS_HANDLE  ProtocolBindingContext
    )
{
}








////////////////////////////////////
/*

int ProtocolReceivePacket(NDIS_HANDLE ProtocolBindingContext, PNDIS_PACKET Packet)
{
		DbgPrint("ProtocolReceivePacket \n");
	UINT nBufferLen;
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)ProtocolBindingContext;

	// 取出一个未决的读请求
	PLIST_ENTRY pListEntry = ExInterlockedRemoveHeadList(&pOpen->RcvList, &pOpen->RcvSpinLock);
	if(pListEntry == NULL)
	{
		return 0;
	}

	PACKET_RESERVED *pReserved = CONTAINING_RECORD(pListEntry, PACKET_RESERVED, ListElement);
	NDIS_PACKET *pPacket = CONTAINING_RECORD(pReserved, NDIS_PACKET, ProtocolReserved);

	PIRP pIrp = RESERVED(pPacket)->pIrp;
	PIO_STACK_LOCATION pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

	IoSetCancelRoutine(pIrp, NULL);

	NTSTATUS status = STATUS_SUCCESS;

	// 下面要调用的NdisCopyFromPacketToPacket函数不检查封包内存是否可用，为了安全，我们要首先检查
	{
		PNDIS_BUFFER firstBuffer, nextBuffer;
		UINT totalLength;
		PVOID virtualAddress;
		// 得到附加到封包中的第一个缓冲区
		NdisQueryPacket(pPacket, NULL, NULL, &firstBuffer, &totalLength);
		while(firstBuffer != NULL)
		{
			// 获取缓冲区的地址
			NdisQueryBufferSafe(firstBuffer, &virtualAddress, &totalLength, NormalPagePriority);
			if(virtualAddress == NULL)
			{
				status = STATUS_INSUFFICIENT_RESOURCES;
				goto CleanExit;
			}
			NdisGetNextBuffer(firstBuffer, &nextBuffer);
			firstBuffer = nextBuffer;
		}
	}

	// 附加读缓冲区的MDL到封包描述表
	NdisChainBufferAtFront(pPacket, pIrp->MdlAddress);
	// 缓冲区大小
	nBufferLen = pIrpSp->Parameters.Read.Length;

	UINT nTransfered;
	NdisCopyFromPacketToPacket(pPacket, 0, nBufferLen,Packet,0,&nTransfered);

	// 完成请求
CleanExit:
	pIrp->IoStatus.Information = nTransfered;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoDecrement(pOpen);
	return 0;
}
*/
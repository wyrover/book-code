///////////////////////////////////////////////
// send.cpp文件
// NDIS协议入口，处理发送数据的例程


extern "C"
{
	#include <ndis.h>
	#include <ntddk.h>
}

#include "nuiouser.h"
#include "ndisprot.h"

NTSTATUS DispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status;

	// 取得描述适配器的OPEN_INSTANCE结构的指针
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)pDevObj->DeviceExtension;
	// 增加IO引用计数
	IoIncrement(pOpen);
	do
	{
		if(!pOpen->bBound)
		{
			status = STATUS_DEVICE_NOT_READY;
			break;
		}

		// 从封包池中申请一个封包
		PNDIS_PACKET pPacket;
		NdisAllocatePacket((NDIS_STATUS*)&status, &pPacket, pOpen->hPacketPool);
		if(status != NDIS_STATUS_SUCCESS)	// 封包被申请完了！
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		RESERVED(pPacket)->pIrp = pIrp; // 保存IRP指针，在完成例程中还要使用

		// 附加写缓冲区到封包
		NdisChainBufferAtFront(pPacket, pIrp->MdlAddress);

		// 注意，既然我们已经标识此IRP未决，我们必须返回STATUS_PENDING，即便是
		// 我们恰巧同步完成了这个IRP
		IoMarkIrpPending(pIrp);

		// 发送封包到下层NIC设备
		NdisSend((NDIS_STATUS*)&status, pOpen->hAdapter, pPacket);
		if(status != NDIS_STATUS_PENDING)
		{
			ProtocolSendComplete(pOpen, pPacket, status);
		}
		return STATUS_PENDING;		
	}while(FALSE);

	if(status != STATUS_SUCCESS)
	{
		IoDecrement(pOpen);
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}
	return status;
}


VOID
ProtocolSendComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_PACKET  pPacket,
    IN NDIS_STATUS   Status
    )
{
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)ProtocolBindingContext;
	PIRP pIrp = RESERVED(pPacket)->pIrp;
	PIO_STACK_LOCATION pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

	// 释放封包
	NdisFreePacket(pPacket);
	// 完成IRP请求
	if(Status == NDIS_STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = pIrpSp->Parameters.Write.Length;
		pIrp->IoStatus.Status = STATUS_SUCCESS;

		DbgPrint(" ProtoDrv: Send data success \n");
	}
	else
	{
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
	}
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoDecrement(pOpen);
}







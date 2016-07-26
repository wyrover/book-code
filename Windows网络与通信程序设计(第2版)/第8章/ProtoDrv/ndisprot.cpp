///////////////////////////////////////////////////
// ndisprot.cpp文件
// NT入口点，派遣例程

#define NDIS50 1  // 说明要使用NDIS 5.0

extern "C"
{
	#include <ndis.h>
	#include <ntddk.h>
	#include <stdio.h>
}
#include "nuiouser.h"
#include "ndisprot.h"
#pragma comment(lib, "ndis")

GLOBAL g_data;


// 初始化协议驱动
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObj = NULL;
	NDIS_STRING protoName = NDIS_STRING_CONST("Packet");
	// 给用户使用的符号连接名称
	UNICODE_STRING ustrSymbolicLink;
	BOOLEAN bSymbolicLink = FALSE;

	DbgPrint(" ProtoDrv: DriverEntry...  \n");

	// 保存驱动对象指针。这里，g_data是GLOBAL类型的全局变量
	g_data.pDriverObj = pDriverObj;

	do
	{
			// 为此驱动创建一个控制设备对象。用户程序向这个设备发送IOCTL代码，
			// 以便获取绑定的适配器信息
		UNICODE_STRING ustrDevName;
		RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
		status = IoCreateDevice(pDriverObj, 
			0,
			&ustrDevName, 
			FILE_DEVICE_UNKNOWN,
			0,
			FALSE,
			&pDeviceObj);
		if(!NT_SUCCESS(status))
		{
			DbgPrint(" ProtoDrv: CreateDevice failed \n");
			break;
		}
		// 为上面的设备创建符号连接
		RtlInitUnicodeString(&ustrSymbolicLink, LINK_NAME);
		status = IoCreateSymbolicLink(&ustrSymbolicLink, &ustrDevName);  
		if(!NT_SUCCESS(status))
		{
			DbgPrint(" ProtoDrv: CreateSymbolicLink failed \n");
			break;
		}
		bSymbolicLink = TRUE;
		// 设置为缓冲区I/O方式
		pDeviceObj->Flags |= DO_BUFFERED_IO;

			// 初始化全局变量
		g_data.pControlDevice = pDeviceObj;
		InitializeListHead(&g_data.AdapterList);
		KeInitializeSpinLock(&g_data.GlobalLock);

			// 初始化协议特征结构
		NDIS_PROTOCOL_CHARACTERISTICS protocolChar;
		NdisZeroMemory(&protocolChar, sizeof(protocolChar));
		protocolChar.Ndis40Chars.Ndis30Chars.MajorNdisVersion = 5;
		protocolChar.Ndis40Chars.Ndis30Chars.MinorNdisVersion = 0;

		protocolChar.Ndis40Chars.Ndis30Chars.Name = protoName;

		protocolChar.Ndis40Chars.BindAdapterHandler = ProtocolBindAdapter;
		protocolChar.Ndis40Chars.UnbindAdapterHandler = ProtocolUnbindAdapter;
		
		protocolChar.Ndis40Chars.Ndis30Chars.OpenAdapterCompleteHandler  = ProtocolOpenAdapterComplete;
		protocolChar.Ndis40Chars.Ndis30Chars.CloseAdapterCompleteHandler = ProtocolCloseAdapterComplete;

		protocolChar.Ndis40Chars.Ndis30Chars.ReceiveHandler              = ProtocolReceive;
//		protocolChar.Ndis40Chars.ReceivePacketHandler					= ProtocolReceivePacket;
		protocolChar.Ndis40Chars.Ndis30Chars.TransferDataCompleteHandler = ProtocolTransferDataComplete;

		protocolChar.Ndis40Chars.Ndis30Chars.SendCompleteHandler         = ProtocolSendComplete;

		
		protocolChar.Ndis40Chars.Ndis30Chars.ResetCompleteHandler        = ProtocolResetComplete;
		protocolChar.Ndis40Chars.Ndis30Chars.RequestCompleteHandler      = ProtocolRequestComplete;
		
		protocolChar.Ndis40Chars.Ndis30Chars.ReceiveCompleteHandler      = ProtocolReceiveComplete;
		
		protocolChar.Ndis40Chars.Ndis30Chars.StatusHandler               = ProtocolStatus;
		protocolChar.Ndis40Chars.Ndis30Chars.StatusCompleteHandler       = ProtocolStatusComplete;
		protocolChar.Ndis40Chars.PnPEventHandler						= ProtocolPNPHandler; 
		
			// 注册为协议驱动
		NdisRegisterProtocol((PNDIS_STATUS)&status, 
			&g_data.hNdisProtocol, &protocolChar, sizeof(protocolChar));
		if(status != NDIS_STATUS_SUCCESS)
		{
			status = STATUS_UNSUCCESSFUL;
			break;
		}
		DbgPrint(" ProtoDrv: NdisRegisterProtocol success \n");

			// 现在，设置我们要处理的派遣例程
		pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
		pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
		pDriverObj->MajorFunction[IRP_MJ_READ]  = DispatchRead;
		pDriverObj->MajorFunction[IRP_MJ_WRITE]  = DispatchWrite;
		pDriverObj->MajorFunction[IRP_MJ_CLEANUP]  = DispatchCleanup;

		pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
		pDriverObj->DriverUnload = DriverUnload;	
		status = STATUS_SUCCESS;
	}while(FALSE);
	
	if(!NT_SUCCESS(status))		// 错误处理
	{
		if(pDeviceObj != NULL)
		{
			// 删除设备对象
			IoDeleteDevice(pDeviceObj); 
			g_data.pControlDevice = NULL;
		}
		if(bSymbolicLink)
		{
			// 删除符号连接
			IoDeleteSymbolicLink(&ustrSymbolicLink);
		}
	}
	return status;
}


// 卸载
void DriverUnload(PDRIVER_OBJECT pDriverObj)
{	
	// 删除控制设备对象和对应的符号连接
	UNICODE_STRING ustrLink;
	RtlInitUnicodeString(&ustrLink, LINK_NAME);
	IoDeleteSymbolicLink(&ustrLink);
	if(g_data.pControlDevice != NULL)
		IoDeleteDevice(g_data.pControlDevice);

	// 解除所有绑定
	NDIS_STATUS status;
	while(pDriverObj->DeviceObject != NULL) // 这里除了控制设备对象之外，其它全是NIC设备对象
	{
		ProtocolUnbindAdapter(&status, pDriverObj->DeviceObject->DeviceExtension, NULL);
	}	
	// 取消协议驱动的注册
	NdisDeregisterProtocol(&status, g_data.hNdisProtocol);
}


// 处理IRP_MJ_CREATE、IRP_MJ_CLOSE功能代码
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint(" ProtoDrv: DispatchClose \n");
	NTSTATUS status = STATUS_SUCCESS;
	
	if(pDevObj == g_data.pControlDevice)
	{
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)pDevObj->DeviceExtension;

	IoIncrement(pOpen);
	
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
	IoDecrement(pOpen);


	return status;
}
// 处理IRP_MJ_CREATE、IRP_MJ_CLOSE功能代码
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint(" ProtoDrv: DispatchCreate \n");
	NTSTATUS status = STATUS_SUCCESS;
	
	if(pDevObj == g_data.pControlDevice)
	{
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)pDevObj->DeviceExtension;


	IoIncrement(pOpen);

	if(!pOpen->bBound)
	{
		status = STATUS_DEVICE_NOT_READY;
	}
	
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
	IoDecrement(pOpen);
	return status;
}


// I/O控制派遣例程
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	if(uIoControlCode == IOCTL_ENUM_ADAPTERS)
	{
		ULONG nDataLen = 0;
		if(pDevObj != g_data.pControlDevice)
			status = STATUS_INVALID_DEVICE_REQUEST;
		else
		{
			status = GetAdapterList(pIoBuffer, uOutSize, &nDataLen);
			if(status != STATUS_SUCCESS)
				DbgPrint("GetAdapterList error ");
		}
		pIrp->IoStatus.Information = nDataLen;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}

	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)pDevObj->DeviceExtension;
	if(pOpen == NULL || !pOpen->bBound)
	{
		pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return STATUS_UNSUCCESSFUL;
	}

	IoIncrement(pOpen);

	IoMarkIrpPending(pIrp);

	if(uIoControlCode == IOCTL_PROTOCOL_RESET) 
	{
		// 插入此IRP到重置IRP列表
       ExInterlockedInsertTailList(
                &pOpen->ResetIrpList,
                &pIrp->Tail.Overlay.ListEntry,
                &pOpen->ResetQueueLock);

	   // 发出重置请求
        NdisReset(
            &status,
            pOpen->hAdapter
            );
        if(status != NDIS_STATUS_PENDING) 
		{
            ProtocolResetComplete(
                pOpen,
                status);
        }
    }

	// 获取或者设置OID信息
	else if(uIoControlCode == IOCTL_PROTOCOL_SET_OID 
				|| uIoControlCode == IOCTL_PROTOCOL_QUERY_OID) // 输入参数是一个自定义的PROTOCOL_OID_DATA结构
	{
		PPROTOCOL_OID_DATA pOidData = (PPROTOCOL_OID_DATA)pIoBuffer;
		// 申请一个INTERNAL_REQUEST结构
		PINTERNAL_REQUEST pInterRequest = 
			(PINTERNAL_REQUEST)ExAllocatePool(NonPagedPool, sizeof(INTERNAL_REQUEST));
	   if(pInterRequest == NULL)
        {
            pIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
            IoCompleteRequest(pIrp, IO_NO_INCREMENT);
            IoDecrement(pOpen);
            return STATUS_PENDING;
        }         
        pInterRequest->pIrp = pIrp;

		if(uOutSize == uInSize && uOutSize >= sizeof(PROTOCOL_OID_DATA) &&
					uOutSize >= sizeof(PROTOCOL_OID_DATA) - 1 + pOidData->Length)	// 缓冲区可用？
		{
			// 初始化NDIS_REQUEST结构
			if(uIoControlCode == IOCTL_PROTOCOL_SET_OID)
			{
				pInterRequest->Request.RequestType = NdisRequestSetInformation;
				pInterRequest->Request.DATA.SET_INFORMATION.Oid = pOidData->Oid;
				pInterRequest->Request.DATA.SET_INFORMATION.InformationBuffer = pOidData->Data;
				pInterRequest->Request.DATA.SET_INFORMATION.InformationBufferLength = pOidData->Length;
			}
			else
			{
				pInterRequest->Request.RequestType = NdisRequestQueryInformation;
				pInterRequest->Request.DATA.QUERY_INFORMATION.Oid = pOidData->Oid;
				pInterRequest->Request.DATA.QUERY_INFORMATION.InformationBuffer = pOidData->Data;
				pInterRequest->Request.DATA.QUERY_INFORMATION.InformationBufferLength = pOidData->Length;
			}

			// 提交这个请求
			NdisRequest(&status, pOpen->hAdapter, &pInterRequest->Request);
		}
		else
		{
			status = NDIS_STATUS_FAILURE;
            pInterRequest->Request.DATA.SET_INFORMATION.BytesRead = 0;
            pInterRequest->Request.DATA.QUERY_INFORMATION.BytesWritten = 0;
		}
		
		if(status != NDIS_STATUS_PENDING)
		{
			ProtocolRequestComplete(pOpen, &pInterRequest->Request, status);
		}
	}

	return STATUS_PENDING;
}

VOID
ProtocolResetComplete(
    IN NDIS_HANDLE  ProtocolBindingContext,
    IN NDIS_STATUS  Status
    )

{
	OPEN_INSTANCE *pOpen;
    pOpen = (OPEN_INSTANCE*)ProtocolBindingContext;

	// 取出IRP指针
	PLIST_ENTRY pListEntry = ExInterlockedRemoveHeadList(
                       &pOpen->ResetIrpList,
                       &pOpen->ResetQueueLock
                       );
    PIRP pIrp = CONTAINING_RECORD(pListEntry,IRP,Tail.Overlay.ListEntry);

	// 完成此IRP
    if(Status == NDIS_STATUS_SUCCESS) 
	{
        pIrp->IoStatus.Status = STATUS_SUCCESS;
    } 
	else 
	{
        pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
    }
    pIrp->IoStatus.Information = 0;    
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    IoDecrement(pOpen);
}


VOID
ProtocolRequestComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_REQUEST NdisRequest,
    IN NDIS_STATUS   Status
    )
{
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)ProtocolBindingContext;
	PINTERNAL_REQUEST pInterRequest = CONTAINING_RECORD(NdisRequest, INTERNAL_REQUEST, Request);
	PIRP pIrp = pInterRequest->pIrp;

	if(Status == NDIS_STATUS_SUCCESS)
	{
		PIO_STACK_LOCATION pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
		UINT nIoControlCode = pIrpSp->Parameters.DeviceIoControl.IoControlCode;
		PPROTOCOL_OID_DATA pOidData = (PPROTOCOL_OID_DATA)pIrp->AssociatedIrp.SystemBuffer;

		//  将大小返回到用户缓冲区
        if(nIoControlCode == IOCTL_PROTOCOL_SET_OID) 
		{
            pOidData->Length = pInterRequest->Request.DATA.SET_INFORMATION.BytesRead;
        } 
		else if(nIoControlCode == IOCTL_PROTOCOL_QUERY_OID) 
		{
			pOidData->Length = pInterRequest->Request.DATA.QUERY_INFORMATION.BytesWritten;
		}	

		// 设置返回给I/O管理器的信息
		pIrp->IoStatus.Information = pIrpSp->Parameters.DeviceIoControl.InputBufferLength;
		pIrp->IoStatus.Status = STATUS_SUCCESS;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
	}
	ExFreePool(pInterRequest);
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	IoDecrement(pOpen);
}

NTSTATUS
GetAdapterList(
    IN  PVOID              Buffer,		// 缓冲区
    IN  ULONG              Length,		// 缓冲区大小
    IN  OUT PULONG         DataLength	// 返回实际需要的长度
    )
{
	KIRQL oldIrql;
	KeAcquireSpinLock(&g_data.GlobalLock, &oldIrql);

	OPEN_INSTANCE *pOpen ;

		// 遍历列表，计算所需的缓冲区大小
	ULONG nRequiredLength = 0;
	ULONG nAdapters = 0;

	PLIST_ENTRY pThisEntry;
	PLIST_ENTRY pHeader = &g_data.AdapterList;
	for(pThisEntry = pHeader->Flink ; pThisEntry != pHeader; pThisEntry = pThisEntry->Flink)
	{
		pOpen = CONTAINING_RECORD(pThisEntry, OPEN_INSTANCE, AdapterListEntry);
		nRequiredLength += pOpen->ustrAdapterName.Length + sizeof(UNICODE_NULL);
		nRequiredLength += pOpen->ustrLinkName.Length + sizeof(UNICODE_NULL);
		nAdapters++;
	}

	// 我们将要以下面的格式返回数据：
	// nAdapters + 一个或者多个（"AdapterName\0" + "SymbolicLink\0"） + UNICODE_NULL
	// 所以，下面要包含上nAapters和UNICODE_NULL的大小
	nRequiredLength += sizeof(nAdapters) + sizeof(UNICODE_NULL); 

	*DataLength = nRequiredLength;
	if(nRequiredLength > Length) 
	{
        KeReleaseSpinLock(&g_data.GlobalLock, oldIrql);
        return STATUS_BUFFER_TOO_SMALL;
    }

		// 填充缓冲区
	// 首先是适配器数量
	*(PULONG)Buffer = nAdapters;
	Buffer = (PCHAR)Buffer + sizeof(ULONG);

	// 然后复制适配器和符号连接名称
	for(pThisEntry = pHeader->Flink; 
			pThisEntry != pHeader;
			pThisEntry = pThisEntry->Flink)
    {
        pOpen = CONTAINING_RECORD(pThisEntry, OPEN_INSTANCE, AdapterListEntry);
        
        RtlCopyMemory(Buffer, pOpen->ustrAdapterName.Buffer,
                            pOpen->ustrAdapterName.Length + sizeof(WCHAR));

		Buffer = (PCHAR)Buffer + pOpen->ustrAdapterName.Length + sizeof(WCHAR);

        
        RtlCopyMemory(Buffer, pOpen->ustrLinkName.Buffer,
                            pOpen->ustrLinkName.Length + sizeof(WCHAR));

		Buffer = (PCHAR)Buffer + pOpen->ustrLinkName.Length + sizeof(WCHAR);                       
    }
	
	// 最后的结束标志
    *(PWCHAR)Buffer = UNICODE_NULL;
    
    KeReleaseSpinLock(&g_data.GlobalLock, oldIrql);

	return STATUS_SUCCESS;
}



void IoIncrement(OPEN_INSTANCE *pOpen)
{
	if(InterlockedIncrement((PLONG)&pOpen->nIrpCount) == 1)
		NdisResetEvent(&pOpen->CleanupEvent);
}

void IoDecrement(OPEN_INSTANCE *pOpen)
{
	if(InterlockedDecrement((PLONG)&pOpen->nIrpCount) == 0)
		NdisSetEvent(&pOpen->CleanupEvent);
}




NTSTATUS
DispatchCleanup(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
    POPEN_INSTANCE      pOpen = (POPEN_INSTANCE)DeviceObject->DeviceExtension;
    NTSTATUS            status = STATUS_SUCCESS;


    if(DeviceObject == g_data.pControlDevice) 
	{
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }

    IoIncrement(pOpen);
 
    CancelReadIrp(DeviceObject);

    IoDecrement(pOpen);

    NdisWaitEvent(&pOpen->CleanupEvent, 0);

    Irp->IoStatus.Information = 0;    
    Irp->IoStatus.Status = status;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    return status;

}


VOID
ProtocolStatus(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN NDIS_STATUS   Status,
    IN PVOID         StatusBuffer,
    IN UINT          StatusBufferSize
    )
{
}

VOID
ProtocolStatusComplete(
    IN NDIS_HANDLE  ProtocolBindingContext
    )
{
}



NDIS_STATUS
ProtocolPNPHandler(
    IN    NDIS_HANDLE        ProtocolBindingContext,
    IN    PNET_PNP_EVENT     NetPnPEvent
    )
{
    NDIS_STATUS                 Status  = NDIS_STATUS_SUCCESS;
    PNET_DEVICE_POWER_STATE     powerState;
    
    powerState = (PNET_DEVICE_POWER_STATE)NetPnPEvent->Buffer;
    switch(NetPnPEvent->NetEvent)
    {
         case  NetEventSetPower :
            switch (*powerState) 
			{
            
                case NetDeviceStateD0:
                    Status = NDIS_STATUS_SUCCESS;
                    break;

                default:
                    //
                    // We can't suspend, so we ask NDIS to Unbind us by
                    // returning this status:
                    //
                    Status = NDIS_STATUS_NOT_SUPPORTED;
                    break;
            }
    }
    return Status;
}





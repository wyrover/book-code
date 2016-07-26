///////////////////////////////////////////////
// ndisbind.cpp文件
// NDIS协议入口点，处理绑定和解除绑定的例程


extern "C"
{
	#include <ndis.h>
	#include <ntddk.h>
	#include <stdio.h>
}
#include "nuiouser.h"
#include "ndisprot.h"


VOID 
  ProtocolBindAdapter(
    OUT PNDIS_STATUS Status,
    IN NDIS_HANDLE  BindContext,
    IN PNDIS_STRING  DeviceName,
    IN PVOID  SystemSpecific1,
    IN PVOID  SystemSpecific2
    )
{

	DbgPrint(" ProtoDrv: ProtocolBindAdapter... \n");

	NDIS_STATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObj = NULL;
	UNICODE_STRING ustrDevName = { 0 };
	OPEN_INSTANCE *pOpen = NULL;

	do
	{
			// 为新发现的设备创建设备对象和符号连接名称
		// 构建设备对象名称。
		// 设备名称的格式是“\Device\{GUID}”，我们的设备对象名称的格式为“\Device\Packet_{GUID}”，
		// 即在设备名称前加前缀“Packet_”
		int nLen = DeviceName->Length + 7*sizeof(WCHAR) + sizeof(UNICODE_NULL);
		PWSTR strName = (PWSTR)ExAllocatePool(NonPagedPool, nLen);
		if(strName == NULL)
		{
			*Status = NDIS_STATUS_FAILURE;
			break;
		}
		swprintf(strName, L"\\Device\\Packet_%ws", &DeviceName->Buffer[8]);
		RtlInitUnicodeString(&ustrDevName, strName);
		// 创建设备对象，同时在设备对象的DeviceExtension域申请一个OPEN_INSTANCE结构
		status = IoCreateDevice(g_data.pDriverObj, 
			sizeof(OPEN_INSTANCE),	// 指定DeviceExtension域的大小
			&ustrDevName, 
			FILE_DEVICE_PROTOCOL,
			0,
			TRUE,					// 在同一时间，仅允许用户打开一个到此对象的句柄
			&pDeviceObj); 
		if(status != STATUS_SUCCESS)
		{
			DbgPrint(" ProtoDrv: CreateDevice() failed \n ");
			*Status = NDIS_STATUS_FAILURE; 
			break;
		}
		// 使用直接I/O传输数据，这种方式适合大块数据的传输
		pDeviceObj->Flags |= DO_DIRECT_IO;
		// 取得与本设备对象关联的OPEN_INSTANCE结构的指针
		pOpen = (OPEN_INSTANCE*)pDeviceObj->DeviceExtension;
		// 保存设备对象指针
		pOpen->pDeviceObj = pDeviceObj;

		// 构建符号连接名称
		// 符号连接名称格式为“\DosDevices\Packet_{GUID}”，比设备名称多4个字
		nLen = ustrDevName.Length + 4*sizeof(WCHAR) + sizeof(UNICODE_NULL);
		strName = (PWSTR)ExAllocatePool(NonPagedPool, nLen);
		if(strName == NULL)
		{
			*Status = NDIS_STATUS_FAILURE;
			break;
		}
		swprintf(strName, L"\\DosDevices\\%ws", &ustrDevName.Buffer[8]);
		RtlInitUnicodeString(&pOpen->ustrLinkName, strName);

		// 为新建设备对象创建符号连接名称
		status = IoCreateSymbolicLink(&pOpen->ustrLinkName, &ustrDevName); 
		if(status != STATUS_SUCCESS)
		{
			*Status = NDIS_STATUS_FAILURE;
			DbgPrint(" ProtoDrv: Create symbolic failed \n");
			break;
		}
		// 我们不再使用设备对象名称了，释放它占用的内存
		ExFreePool(ustrDevName.Buffer);
		ustrDevName.Buffer = NULL;

			// 初始化OPEN_INSTANCE结构. 上面已经初始化了pDeviceObj和ustrLinkName域
		// 申请封包池
		NdisAllocatePacketPool(&status, 
			&pOpen->hPacketPool, 16, sizeof(PACKET_RESERVED));
		if(status != NDIS_STATUS_SUCCESS)
		{
			*Status = NDIS_STATUS_FAILURE;
			break;
		}
		
		// 初始化用来同步打开和关闭的事件
		NdisInitializeEvent(&pOpen->BindEvent);

		// 初始化重置列表和它对应的spinlock
		InitializeListHead(&pOpen->ResetIrpList);
		KeInitializeSpinLock(&pOpen->ResetQueueLock);

		// 初始化保存未决读请求的列表和它对应的spinlock
		InitializeListHead(&pOpen->RcvList);
		KeInitializeSpinLock(&pOpen->RcvSpinLock);

			// 现在打开下面的适配器
		NDIS_MEDIUM         mediumArray = NdisMedium802_3;
		UINT mediumIndex;
		NdisOpenAdapter(Status,
				  &status,
				  &pOpen->hAdapter,
				  &mediumIndex,
				  &mediumArray,
				  sizeof(mediumArray)/sizeof(NDIS_MEDIUM),
				  g_data.hNdisProtocol,
				  pOpen,
				  DeviceName,
				  0,
				  NULL);
		if(*Status == NDIS_STATUS_PENDING)
        {
			// 打开操作完成之后，NDIS会调用我们注册的ProtocolOpenAdapterComplete函数，
			// ProtocolOpenAdapterComplete函数设置BindEvent事件，使下面的语句返回。它也设置状态代码Status
              NdisWaitEvent(&pOpen->BindEvent, 0);
              *Status = pOpen->Status;
        }
		if(*Status != NDIS_STATUS_SUCCESS)
		{
			DbgPrint(" ProtoDrv: OpenAdapter failed! \n");
			break;
		}

			// 继续初始化OPEN_INSTANCE结构
		// IRP请求数量初始值为0
		pOpen->nIrpCount = 0;

		// 已经绑定
		InterlockedExchange((PLONG)&pOpen->bBound, TRUE);

		NdisInitializeEvent(&pOpen->CleanupEvent);

		// 可以清除
		NdisSetEvent(&pOpen->CleanupEvent);

		// 保存MAC驱动的名称
		NdisQueryAdapterInstanceName(&pOpen->ustrAdapterName, pOpen->hAdapter);

		pOpen->Medium = mediumArray;

			// 连接此OPEN_INSTANCE实例到全局的适配器列表（AdapterList），准备接收用户的I/O请求
		InitializeListHead(&pOpen->AdapterListEntry);
        ExInterlockedInsertTailList(&g_data.AdapterList,
                                    &pOpen->AdapterListEntry, 
                                    &g_data.GlobalLock);
		// 清除设备对象中的DO_DEVICE_INITIALIZING标记。
		// 如果你在DriverEntry之外创建设备对象，必须要这么做。否则，应用程序不能发送I/O请求
		pDeviceObj->Flags &= ~DO_DEVICE_INITIALIZING;
	} 
	while(FALSE);


	// 出错处理
	if(*Status != NDIS_STATUS_SUCCESS)
	{
		 if(pOpen != NULL && pOpen->hPacketPool != NULL) 
		 {
             NdisFreePacketPool(pOpen->hPacketPool);
        }
		if(pDeviceObj != NULL)
			IoDeleteDevice(pDeviceObj);
		if(ustrDevName.Buffer != NULL)
			ExFreePool(ustrDevName.Buffer);
		if(pOpen->ustrLinkName.Buffer != NULL)
		{
			IoDeleteSymbolicLink(&pOpen->ustrLinkName);
			ExFreePool(pOpen->ustrLinkName.Buffer);
		}
	}
}

VOID
  ProtocolOpenAdapterComplete(
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_STATUS  Status,
      IN NDIS_STATUS  OpenErrorStatus
      )
{
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)ProtocolBindingContext;
	pOpen->Status = Status;
	// 指示绑定已经完成
    NdisSetEvent(&pOpen->BindEvent);
}


VOID
  ProtocolUnbindAdapter(
      OUT PNDIS_STATUS  Status,
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_HANDLE  UnbindContext
      )
{
	OPEN_INSTANCE *pOpen = (OPEN_INSTANCE *)ProtocolBindingContext;
	if(pOpen->hAdapter != NULL)
	{
			// 关闭下层适配器
		NdisResetEvent(&pOpen->BindEvent);

		// 说明不再有绑定了
		InterlockedExchange((PLONG)&pOpen->bBound, FALSE);

		// 取消所有未决的读IRP请求
		CancelReadIrp(pOpen->pDeviceObj);

		// 等待所有IRP完成
		NdisWaitEvent(&pOpen->CleanupEvent, 0);

		// 释放建立的绑定
		NdisCloseAdapter(Status, pOpen->hAdapter);
		// 等待这个操作完成
		if(*Status == NDIS_STATUS_PENDING)
        {
            NdisWaitEvent(&pOpen->BindEvent, 0); // ProtocolCloseAdapterComplete函数使事件受信
            *Status = pOpen->Status;
        }
        else
        {
            *Status = NDIS_STATUS_FAILURE;
        }

			// 从全局的适配器列表（AdapterList）中删除这个实例
		KIRQL oldIrql;
		KeAcquireSpinLock(&g_data.GlobalLock, &oldIrql); 
        RemoveEntryList(&pOpen->AdapterListEntry);
        KeReleaseSpinLock(&g_data.GlobalLock, oldIrql);

			// 释放绑定时申请的资源
        NdisFreePacketPool(pOpen->hPacketPool);
        NdisFreeMemory(pOpen->ustrAdapterName.Buffer, pOpen->ustrAdapterName.Length, 0);
        IoDeleteSymbolicLink(&pOpen->ustrLinkName);
        ExFreePool(pOpen->ustrLinkName.Buffer);
        IoDeleteDevice(pOpen->pDeviceObj);
	}
}


VOID 
  ProtocolCloseAdapterComplete(
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_STATUS  Status
      )
{
	POPEN_INSTANCE pOpen = (POPEN_INSTANCE)ProtocolBindingContext;
	pOpen->Status = Status;
    NdisSetEvent(&pOpen->BindEvent);
}

















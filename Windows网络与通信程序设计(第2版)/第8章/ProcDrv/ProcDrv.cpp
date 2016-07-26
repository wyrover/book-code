//////////////////////////////////////////////////
// ProcDrv.cpp文件


extern "C"
{
	#include <ntddk.h>
}
#include <devioctl.h>
#include "ProcDrv.h"

// 自定义函数的声明
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
VOID ProcessCallback(IN HANDLE  hParentId, IN HANDLE  hProcessId, IN BOOLEAN bCreate);

// 驱动内部名称、符号连接名称、事件对象名称
#define DEVICE_NAME		L"\\Device\\devNTProcDrv"
#define LINK_NAME		L"\\DosDevices\\slNTProcDrv"
#define EVENT_NAME		L"\\BaseNamedObjects\\NTProcDrvProcessEvent"


typedef struct _DEVICE_EXTENSION  // 设备对象的私有存储
{
    HANDLE  hProcessHandle;	// 事件对象句柄
    PKEVENT ProcessEvent;	// 用户和内核通信的事件对象指针

    HANDLE  hPParentId;		// 在回调函数中保存进程信息，当用户程序请求时，传递过去
    HANDLE  hPProcessId;
    BOOLEAN bPCreate;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

PDEVICE_OBJECT g_pDeviceObject;


// 驱动程序加载时调用DriverEntry例程
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;

	// 初始化各个派遣例程
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

		// 创建、初始化设备对象
	// 设备名称
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	// 创建设备对象
	PDEVICE_OBJECT pDevObj;
	status = IoCreateDevice(pDriverObj, 
				sizeof(DEVICE_EXTENSION), // 为设备扩展结构申请空间
				&ustrDevName, 
				FILE_DEVICE_UNKNOWN,
				0,
				FALSE,
				&pDevObj);
	if(!NT_SUCCESS(status))
	{
		return status;
	}
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

		// 创建符号连接名称
	// 符号连接名称
	UNICODE_STRING ustrLinkName;
	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	// 创建关联
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);  
	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);  
		return status;
	}

	// 保存到设备对象的指针，下面在进程回调函数中还要使用
	g_pDeviceObject = pDevObj;

	// 为了用户模式进程能够监视，创建事件对象
	UNICODE_STRING  uszProcessEventString;
    RtlInitUnicodeString(&uszProcessEventString, EVENT_NAME);
    pDevExt->ProcessEvent = IoCreateNotificationEvent(&uszProcessEventString, &pDevExt->hProcessHandle);

    // 设置它为非受信状态
    KeClearEvent(pDevExt->ProcessEvent);
    // 设置回调例程
    status = PsSetCreateProcessNotifyRoutine(ProcessCallback, FALSE);
	
	return status;
}


void DriverUnload(PDRIVER_OBJECT pDriverObj)
{	
	// 移除进程回调例程
    PsSetCreateProcessNotifyRoutine(ProcessCallback, TRUE);

	// 删除符号连接名称
	UNICODE_STRING strLink;
	RtlInitUnicodeString(&strLink, LINK_NAME);
	IoDeleteSymbolicLink(&strLink);

	// 删除设备对象
	IoDeleteDevice(pDriverObj->DeviceObject);
}


// 处理IRP_MJ_CREATE、IRP_MJ_CLOSE功能代码
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 完成此请求
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


// I/O控制派遣例程
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint(" ProcDrv: DispatchIoctl... \n");

	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得设备扩展结构指针
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PCALLBACK_INFO pCallbackInfo = (PCALLBACK_INFO)pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	switch(uIoControlCode)
	{
	case IOCTL_NTPROCDRV_GET_PROCINFO:  // 向用户程序返回有事件发生的进程的信息
		{
			if(uOutSize >= sizeof(CALLBACK_INFO))
			{
				pCallbackInfo->hParentId  = pDevExt->hPParentId;
                pCallbackInfo->hProcessId = pDevExt->hPProcessId;
                pCallbackInfo->bCreate    = pDevExt->bPCreate;

                status = STATUS_SUCCESS;
			}
		}
		break;
	}

	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	else
		pIrp->IoStatus.Information = 0;


	// 完成请求
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

// 进程回调函数
VOID ProcessCallback(IN HANDLE  hParentId, IN HANDLE  hProcessId, IN BOOLEAN bCreate)
{
	// 得到设备扩展结构的指针
    PDEVICE_EXTENSION pDevExt =  (PDEVICE_EXTENSION)g_pDeviceObject->DeviceExtension;

	// 安排当前值到设备扩展结构
	// 用户模式应用程序将使用DeviceIoControl调用把它取出
    pDevExt->hPParentId  = hParentId;
    pDevExt->hPProcessId = hProcessId;
    pDevExt->bPCreate    = bCreate;

	// 触发这个事件，以便任何正在监听的用户程序知道有事情发生了。
	// 用户模式下的应用程序不能重置KM事件，所以我们要在这里触发它
    KeSetEvent(pDevExt->ProcessEvent, 0, FALSE);
    KeClearEvent(pDevExt->ProcessEvent);
}
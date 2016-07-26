//////////////////////////////////////////////////
// DriverDemo.cpp文件

//---------------------------DriverDemo.cpp文件--------------------------------------//
extern "C"
{
	#include <ntddk.h>
}

// 自定义函数的声明
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);

// 驱动内部名称和符号连接名称
#define DEVICE_NAME L"\\Device\\devDriverDemo"
#define LINK_NAME L"\\??\\slDriverDemo"

// 驱动程序加载时调用DriverEntry例程
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint(" DriverDemo: DriverEntry... \n");

	// 初始化各个派遣例程
	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchCreateClose;
	pDriverObj->DriverUnload = DriverUnload;

		// 创建、初始化设备对象
	// 设备名称
	UNICODE_STRING ustrDevName;
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	// 创建设备对象
	PDEVICE_OBJECT pDevObj;
	status = IoCreateDevice(pDriverObj, 
				0,
				&ustrDevName, 
				FILE_DEVICE_UNKNOWN,
				0,
				FALSE,
				&pDevObj);
	if(!NT_SUCCESS(status))
	{
		return status;
	}

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
	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT pDriverObj)
{	
	DbgPrint(" DriverDemo: DriverUnload... \n");

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
	DbgPrint(" DriverDemo: DispatchCreateClose... \n");

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 完成此请求
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

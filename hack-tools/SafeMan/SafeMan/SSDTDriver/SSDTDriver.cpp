#include "SSDTDriver.h"


/*----------------------------------------------------------------    驱     动  -------------------------------------------------------------------------*/

NTSTATUS SSDTDriverDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	KdPrint(("Enter SSDTDriverDispatchRoutine\n"));
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;  // bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	KdPrint(("Leave SSDTDriverDispatchRoutine\n"));

	return status;
}


NTSTATUS SSDTDriverDispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	KdPrint(("Enter SSDTDriverDeviceIOControl\n"));

	NTSTATUS status = STATUS_SUCCESS;

	PVOID ioBuf = pIrp->AssociatedIrp.SystemBuffer;  // 系统提供的缓冲区

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);  // 得到当前堆栈
	ULONG inBufLength = stack->Parameters.DeviceIoControl.InputBufferLength;  // 得到输入缓冲区大小
	ULONG outBufLength = stack->Parameters.DeviceIoControl.OutputBufferLength;  // 得到输出缓冲区大小
	ULONG ioControlCode = stack->Parameters.DeviceIoControl.IoControlCode;  // 得到IOCTL码

	switch (ioControlCode)
	{
	case IOCTL_SSDTDRIVER_GETSSDT:
		{
			RtlCopyMemory(ioBuf, KeServiceDescriptorTable, sizeof(SSDT));
			pIrp->IoStatus.Information = sizeof(SSDT);
			KdPrint(("[KeServiceDescriptorTable]:%x", KeServiceDescriptorTable));
			break;
		}
	case IOCTL_SSDTDRIVER_GETFUNADDRBYINDEX:
		{
			ULONG ulIndex = *(PULONG)ioBuf;
			if (KeServiceDescriptorTable->ulNumberOfServices <= ulIndex)
			{
				status = STATUS_INVALID_PARAMETER;
				pIrp->IoStatus.Information = 0;
				break;
			}
			PULONG pBase  = (PULONG)KeServiceDescriptorTable->pvSSDTBase;
			RtlCopyMemory(ioBuf, pBase + ulIndex, sizeof(ULONG));
			pIrp->IoStatus.Information = sizeof(ULONG);
			KdPrint(("[FuntionAddress]:%x", *((PULONG)ioBuf)));
			break;
		}
	case IOCTL_SSDTDRIVER_RESTOREFUNADDR:
		{
			ULONG ulIndex = ((PRESTORE_INFO)ioBuf)->ulIndex;
			if (KeServiceDescriptorTable->ulNumberOfServices <= ulIndex)
			{
				status = STATUS_INVALID_PARAMETER;
				pIrp->IoStatus.Information = 0;
				break;
			}
			ULONG ulAddr = ((PRESTORE_INFO)ioBuf)->ulAddr;

			*((PULONG)KeServiceDescriptorTable->pvSSDTBase + ulIndex) = ulAddr;
			pIrp->IoStatus.Information = 0;

			break;
		}
	default:
		status = STATUS_INVALID_PARAMETER;
		pIrp->IoStatus.Information = 0;
		KdPrint(("[SSDTDriver] unknown IRP_MJ_DEVICE_CONTROL \n"));
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	KdPrint(("Leave SSDTDriverDeviceIOControl\n"));

	return status;
}


VOID SSDTDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	KdPrint(("Enter DriverUnload\n"));

	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName, L"\\??\\SSDTDriver");

	IoDeleteSymbolicLink(&symLinkName);  // 删除符号链接
	IoDeleteDevice(pDriverObject->DeviceObject);  // 删除设备
}


NTSTATUS CreateDevice(IN PDRIVER_OBJECT	pDriverObject) 
{
	// 创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, L"\\Device\\SSDTDriver");

	// 创建设备
	PDEVICE_OBJECT pDevObj;
	NTSTATUS status = IoCreateDevice(pDriverObject,
		sizeof(DEVICE_EXTENSION),
		&devName,
		FILE_DEVICE_UNKNOWN,
		0, 
		TRUE,
		&pDevObj);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	//创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName, L"\\??\\SSDTDriver");
	status = IoCreateSymbolicLink(&symLinkName, &devName);
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	return STATUS_SUCCESS;
}


extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{    
	KdPrint(("Enter DriverEntry\n"));

	// 注册其他驱动调用函数入口
	pDriverObject->DriverUnload = SSDTDriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = SSDTDriverDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SSDTDriverDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SSDTDriverDispatchDeviceControl;

	NTSTATUS status = CreateDevice(pDriverObject);  // 创建驱动设备对象

	KdPrint(("DriverEntry end\n"));
	return status;
}

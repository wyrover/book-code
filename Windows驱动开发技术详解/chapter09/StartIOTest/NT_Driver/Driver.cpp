/************************************************************************
* 文件名称:Driver.cpp                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/

#include "Driver.h"


#pragma LOCKEDCODE
VOID
  HelloDDKStartIO(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp 
    )
{
	KIRQL oldirql;
	KdPrint(("Enter HelloDDKStartIO\n"));

	//获取cancel自旋锁
	IoAcquireCancelSpinLock(&oldirql);
	if (Irp!=DeviceObject->CurrentIrp||Irp->Cancel)
	{
		//如果当前有正在处理的IRP，则简单的入队列，并直接返回
		//入队列的工作由系统完成，在StartIO中不用负责
		IoReleaseCancelSpinLock(oldirql);
		KdPrint(("Leave HelloDDKStartIO\n"));
		return;
	}else
	{
		//由于正在处理该IRP，所以不允许调用取消例程
		//因此将此IRP的取消例程设置为NULL
		IoSetCancelRoutine(Irp,NULL);
		IoReleaseCancelSpinLock(oldirql);
	}

	KEVENT event;
	KeInitializeEvent(&event,NotificationEvent,FALSE);

	//等3秒
	LARGE_INTEGER timeout;
	timeout.QuadPart = -3*1000*1000*10;

	//定义一个3秒的延时，主要是为了模拟该IRP操作需要大概3秒左右时间
	KeWaitForSingleObject(&event,Executive,KernelMode,FALSE,&timeout);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;	// no bytes xfered
	IoCompleteRequest(Irp,IO_NO_INCREMENT);


	//在队列中读取一个IRP，并进行StartIo
	IoStartNextPacket(DeviceObject,TRUE);

	KdPrint(("Leave HelloDDKStartIO\n"));
}

/************************************************************************
* 函数名称:DriverEntry
* 功能描述:初始化驱动程序，定位和申请硬件资源，创建内核对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
      pRegistryPath:驱动程序在注册表的中的路径
* 返回 值:返回初始化驱动状态
*************************************************************************/
#pragma INITCODE
extern "C" NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status;
	KdPrint(("Enter DriverEntry\n"));

	//设置卸载函数
	pDriverObject->DriverUnload = HelloDDKUnload;

	//设置派遣函数
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKRead;
	pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = HelloDDKDispatchRoutin;
	pDriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = HelloDDKDispatchRoutin;

	//设置StartIO例程
	pDriverObject->DriverStartIo = HelloDDKStartIO;
	
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	KdPrint(("Leave DriverEntry\n"));
	return status;
}

/************************************************************************
* 函数名称:CreateDevice
* 功能描述:初始化设备对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
* 返回 值:返回初始化状态
*************************************************************************/
#pragma INITCODE
NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;
	
	//创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\MyDDKDevice");
	
	//创建设备
	status = IoCreateDevice( pDriverObject,
						sizeof(DEVICE_EXTENSION),
						&(UNICODE_STRING)devName,
						FILE_DEVICE_UNKNOWN,
						0, TRUE,
						&pDevObj );
	if (!NT_SUCCESS(status))
		return status;

	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;

	//创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\HelloDDK");
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:HelloDDKUnload
* 功能描述:负责驱动程序的卸载操作
* 参数列表:
      pDriverObject:驱动对象
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
VOID HelloDDKUnload (IN PDRIVER_OBJECT pDriverObject) 
{
	PDEVICE_OBJECT	pNextObj;
	KdPrint(("Enter DriverUnload\n"));
	pNextObj = pDriverObject->DeviceObject;
	while (pNextObj != NULL) 
	{
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;

		//删除符号链接
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);

		pNextObj = pNextObj->NextDevice;
		IoDeleteDevice( pDevExt->pDevice );
	}
}

/************************************************************************
* 函数名称:HelloDDKDispatchRoutin
* 功能描述:对读IRP进行处理
* 参数列表:
      pDevObj:功能设备对象
      pIrp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutin(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("Enter HelloDDKDispatchRoutin\n"));

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//建立一个字符串数组与IRP类型对应起来
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};

	UCHAR type = stack->MajorFunction;
	if (type >= arraysize(irpname))
		KdPrint((" - Unknown IRP, major type %X\n", type));
	else
		KdPrint(("\t%s\n", irpname[type]));


	//对一般IRP的简单操作，后面会介绍对IRP更复杂的操作
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDispatchRoutin\n"));

	return status;
}

VOID
OnCancelIRP(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
{
	KdPrint(("Enter CancelReadIRP\n"));

	if (Irp==DeviceObject->CurrentIrp)
	{
		//表明当前正在改由StartIo处理
		//但StartIo并没有获取cancel自旋锁之前
		//这时候需要
		KIRQL oldirql = Irp->CancelIrql;

		//释放Cancel自旋锁
		IoReleaseCancelSpinLock(Irp->CancelIrql);

		IoStartNextPacket(DeviceObject,TRUE);

		KeLowerIrql(oldirql);
	}else
	{
		//从设备队列中将该IRP抽取出来
		KeRemoveEntryDeviceQueue(&DeviceObject->DeviceQueue,&Irp->Tail.Overlay.DeviceQueueEntry);
		//释放Cancel自旋锁
		IoReleaseCancelSpinLock(Irp->CancelIrql);
	}

	
	//设置完成状态为STATUS_CANCELLED
 	Irp->IoStatus.Status = STATUS_CANCELLED;
 	Irp->IoStatus.Information = 0;	// bytes xfered
 	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	KdPrint(("Leave CancelReadIRP\n"));
}


NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("Enter HelloDDKRead\n"));

	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pDevObj->DeviceExtension;

	//将IRP设置为挂起
	IoMarkIrpPending(pIrp);

	//将IRP插入系统的队列
	IoStartPacket(pDevObj,pIrp,0,OnCancelIRP);

	KdPrint(("Leave HelloDDKRead\n"));

	//返回pending状态
	return STATUS_PENDING;
}


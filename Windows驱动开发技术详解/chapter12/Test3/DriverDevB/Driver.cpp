/************************************************************************
* 文件名称:Driver.cpp                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/

#include "Driver.h"

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
	NTSTATUS ntStatus;
	KdPrint(("DriverB:Enter B DriverEntry\n"));

	//注册其他驱动调用函数入口
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKClose;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKRead;
	
	UNICODE_STRING DeviceName;
	RtlInitUnicodeString( &DeviceName, L"\\Device\\MyDDKDeviceA" );

	PDEVICE_OBJECT DeviceObject = NULL;
	PFILE_OBJECT FileObject = NULL;
	//寻找DriverA创建的设备对象
	ntStatus = IoGetDeviceObjectPointer(&DeviceName,FILE_ALL_ACCESS,&FileObject,&DeviceObject);

	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("DriverB:IoGetDeviceObjectPointer() 0x%x\n", ntStatus ));
		return ntStatus;
	}

	//创建自己的驱动设备对象
	ntStatus = CreateDevice(pDriverObject);

	if ( !NT_SUCCESS( ntStatus ) )
	{
		ObDereferenceObject( FileObject );
		DbgPrint( "IoCreateDevice() 0x%x!\n", ntStatus );
		return ntStatus;
	}

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) pDriverObject->DeviceObject->DeviceExtension;

	PDEVICE_OBJECT FilterDeviceObject = pdx->pDevice;

	//将自己的设备对象挂载在DriverA的设备对象上
	PDEVICE_OBJECT TargetDevice = IoAttachDeviceToDeviceStack( FilterDeviceObject,
										  DeviceObject );
	//将底层设备对象记录下来
	pdx->TargetDevice = TargetDevice;
	
	if ( !TargetDevice )
	{
		ObDereferenceObject( FileObject );
		IoDeleteDevice( FilterDeviceObject );
		DbgPrint( "IoAttachDeviceToDeviceStack() 0x%x!\n", ntStatus );
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	FilterDeviceObject->DeviceType = TargetDevice->DeviceType;
	FilterDeviceObject->Characteristics = TargetDevice->Characteristics;
	FilterDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	FilterDeviceObject->Flags |= ( TargetDevice->Flags & ( DO_DIRECT_IO |
														 DO_BUFFERED_IO ) );
	ObDereferenceObject( FileObject );

	KdPrint(("DriverB:B attached A successfully!\n"));
	
	KdPrint(("DriverB:Leave B DriverEntry\n"));
	return ntStatus;
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
	NTSTATUS ntStatus;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;
	
	//创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\MyDDKDeviceB");
	
	//创建设备
	ntStatus = IoCreateDevice( pDriverObject,
						sizeof(DEVICE_EXTENSION),
						&(UNICODE_STRING)devName,
						FILE_DEVICE_UNKNOWN,
						0, TRUE,
						&pDevObj );
	if (!NT_SUCCESS(ntStatus))
		return ntStatus;

	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
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
	KdPrint(("DriverB:Enter B DriverUnload\n"));
	pNextObj = pDriverObject->DeviceObject;

	while (pNextObj != NULL) 
	{
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;
		pNextObj = pNextObj->NextDevice;
		//从设备栈中弹出
		IoDetachDevice( pDevExt->TargetDevice);
		//删除该设备对象
		IoDeleteDevice( pDevExt->pDevice );
	}
	KdPrint(("DriverB:Enter B DriverUnload\n"));
}

/************************************************************************
* 函数名称:HelloDDKDispatchRoutine
* 功能描述:对读IRP进行处理
* 参数列表:
      pDevObj:功能设备对象
      pIrp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("DriverB:Enter B HelloDDKDispatchRoutine\n"));
	NTSTATUS ntStatus = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = ntStatus;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("DriverB:Leave B HelloDDKDispatchRoutine\n"));
	return ntStatus;
}

#pragma PAGEDCODE
NTSTATUS HelloDDKCreate(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("DriverB:Enter B HelloDDKCreate\n"));
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//
// 	// 完成IRP
// 	pIrp->IoStatus.Status = ntStatus;
// 	pIrp->IoStatus.Information = 0;	// bytes xfered
// 	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

    IoSkipCurrentIrpStackLocation (pIrp);

    ntStatus = IoCallDriver(pdx->TargetDevice, pIrp);

	KdPrint(("DriverB:Leave B HelloDDKCreate\n"));

	return ntStatus;
}

NTSTATUS
  MyIoCompletion(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
    )
{
	//进入此函数标志底层驱动设备将IRP完成
	KdPrint(("Enter MyIoCompletion\n"));
    if (Irp->PendingReturned) 
	{
		//传播pending位
        IoMarkIrpPending( Irp );
    }
	return STATUS_SUCCESS;//同STATUS_CONTINUE_COMPLETION
}

#pragma PAGEDCODE
NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("DriverB:Enter B HelloDDKRead\n"));
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//将自己完成IRP，改成由底层驱动负责

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	//将当前IRP堆栈拷贝底层堆栈
	IoCopyCurrentIrpStackLocationToNext(pIrp);

	//设置完成例程
	IoSetCompletionRoutine(pIrp,MyIoCompletion,NULL,TRUE,TRUE,TRUE);

	//调用底层驱动
    ntStatus = IoCallDriver(pdx->TargetDevice, pIrp);

	//当IoCallDriver后，并且完成例程返回的是STATUS_SUCCESS
	//IRP就不在属于派遣函数了，就不能对IRP进行操作了
	if (ntStatus == STATUS_PENDING)
	{
		KdPrint(("STATUS_PENDING\n"));
	}
	ntStatus = STATUS_PENDING; 

	KdPrint(("DriverB:Leave B HelloDDKRead\n"));

	return ntStatus;
}

#pragma PAGEDCODE
NTSTATUS HelloDDKClose(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("DriverB:Enter B HelloDDKClose\n"));
	NTSTATUS ntStatus = STATUS_SUCCESS;

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

    IoSkipCurrentIrpStackLocation (pIrp);

    ntStatus = IoCallDriver(pdx->TargetDevice, pIrp);	
	
	KdPrint(("DriverB:Leave B HelloDDKClose\n"));

	return ntStatus;
}
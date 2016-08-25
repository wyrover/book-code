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
	NTSTATUS status;
	KdPrint(("Enter DriverEntry\n"));

	//设置卸载函数
	pDriverObject->DriverUnload = HelloDDKUnload;

	//设置派遣函数
	for (int i = 0; i < arraysize(pDriverObject->MajorFunction); ++i)
		pDriverObject->MajorFunction[i] = HelloDDKDispatchRoutin;

	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloDDKDeviceIOControl;
	
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

	pDevObj->Flags |= DO_DIRECT_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;

	//申请模拟文件的缓冲区
	pDevExt->buffer = (PUCHAR)ExAllocatePool(PagedPool,MAX_FILE_LENGTH);
	//设置模拟文件大小
	pDevExt->file_length = 0;

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
		if (pDevExt->buffer)
		{
			ExFreePool(pDevExt->buffer);
			pDevExt->buffer = NULL;
		}

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

	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDispatchRoutin\n"));

	return status;
}

VOID
GetDeviceObjectInfo( PDEVICE_OBJECT DevObj )
{
  POBJECT_HEADER ObjectHeader;
  POBJECT_HEADER_NAME_INFO ObjectNameInfo; 

  if ( DevObj == NULL )
  {
    DbgPrint( "DevObj is NULL!\n" );
    return;
  }
  // 得到对象头
  ObjectHeader = OBJECT_TO_OBJECT_HEADER( DevObj );

  if ( ObjectHeader )
  {
    // 查询设备名称并打印
    ObjectNameInfo = OBJECT_HEADER_TO_NAME_INFO( ObjectHeader );

    if ( ObjectNameInfo && ObjectNameInfo->Name.Buffer )
    {
      DbgPrint( "Driver Name:%wZ - Device Name:%wZ - Driver Address:0x%x - Device Address:0x%x\n",
                &DevObj->DriverObject->DriverName,
                &ObjectNameInfo->Name,
                DevObj->DriverObject,
                DevObj );
    }

    // 对于没有名称的设备，则打印 NULL
    else if ( DevObj->DriverObject )
    {
      DbgPrint( "Driver Name:%wZ - Device Name:%S - Driver Address:0x%x - Device Address:0x%x\n",
                &DevObj->DriverObject->DriverName,
                L"NULL",
                DevObj->DriverObject,
                DevObj );
    }
  }
}

VOID
GetAttachedDeviceInfo( PDEVICE_OBJECT DevObj )
{
  PDEVICE_OBJECT DeviceObject;

  if ( DevObj == NULL )
  {
    DbgPrint( "DevObj is NULL!\n" );
    return;
  }

  DeviceObject = DevObj->AttachedDevice;

  while ( DeviceObject )
  {
    DbgPrint( "Attached Driver Name:%wZ,Attached Driver Address:0x%x,Attached DeviceAddress:0x%x\n",
              &DeviceObject->DriverObject->DriverName,
              DeviceObject->DriverObject,
              DeviceObject );

    DeviceObject = DeviceObject->AttachedDevice;
  }
}

PDRIVER_OBJECT
EnumDeviceStack( PWSTR pwszDeviceName )
{
  UNICODE_STRING DriverName;
  PDRIVER_OBJECT DriverObject = NULL;
  PDEVICE_OBJECT DeviceObject = NULL;


  RtlInitUnicodeString( &DriverName, pwszDeviceName );


  ObReferenceObjectByName( &DriverName,
                           OBJ_CASE_INSENSITIVE,
                           NULL,
                           0,
                           ( POBJECT_TYPE ) IoDriverObjectType,
                           KernelMode,
                           NULL,
                           (PVOID*)&DriverObject );

  if ( DriverObject == NULL )
  {
    return NULL;
  }

  DeviceObject = DriverObject->DeviceObject;

  while ( DeviceObject )
  {
    GetDeviceObjectInfo( DeviceObject );

    // 判断当前设备上是否有过滤驱动（Filter Driver）
    if ( DeviceObject->AttachedDevice )
    {
      GetAttachedDeviceInfo( DeviceObject );
    }

    // 进一步判断当前设备上 VPB 中的设备
    if ( DeviceObject->Vpb && DeviceObject->Vpb->DeviceObject )
    {
      GetDeviceObjectInfo( DeviceObject->Vpb->DeviceObject );

      if ( DeviceObject->Vpb->DeviceObject->AttachedDevice )
      {
        GetAttachedDeviceInfo( DeviceObject->Vpb->DeviceObject );
      }
    }

    // 得到建立在此驱动上的下一个设备 DEVICE_OBJECT 
    DeviceObject = DeviceObject->NextDevice;
  }

  return DriverObject;
}

#pragma PAGEDCODE
NTSTATUS HelloDDKDeviceIOControl(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	KdPrint(("Enter HelloDDKDeviceIOControl\n"));

	//得到当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	//得到输入缓冲区大小
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	//得到输出缓冲区大小
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	//得到IOCTL码
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

	ULONG info = 0;

	switch (code)
	{						// process request
		case IOCTL_DUMP_DEVICE_STACK:
		{
			KdPrint(("IOCTL_DUMP_DEVICE_STACK\n"));
			//缓冲区方式IOCTL
			//显示输入缓冲区数据
 			WCHAR* InputBuffer = (WCHAR*)pIrp->AssociatedIrp.SystemBuffer;
			KdPrint(("%ws\n",InputBuffer));

			EnumDeviceStack( InputBuffer );

			break;
		}


		default:
			status = STATUS_INVALID_VARIANT;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloDDKDeviceIOControl\n"));

	return status;
}
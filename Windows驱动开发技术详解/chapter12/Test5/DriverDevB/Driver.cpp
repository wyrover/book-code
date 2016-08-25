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

	pDevObj->Flags |= DO_DIRECT_IO;
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

const int MAX_PACKAGE_SIZE=1024;

#pragma PAGEDCODE
NTSTATUS
HelloDDKReadCompletion(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN PVOID          Context
    )
{
	KdPrint(("DriverB:Enter B HelloDDKReadCompletion\n"));

	PMYDRIVER_RW_CONTEXT rwContext = (PMYDRIVER_RW_CONTEXT) Context;
    NTSTATUS ntStatus = Irp->IoStatus.Status;

	ULONG stageLength;

	if(rwContext && NT_SUCCESS(ntStatus)) 
	{
		//已经传送了多少字节
		rwContext->Numxfer += Irp->IoStatus.Information;

       if(rwContext->Length) 
	   {
		   //设定下一阶段读取字节数
            if(rwContext->Length > MAX_PACKAGE_SIZE) 
			{
				stageLength = MAX_PACKAGE_SIZE;
            }
            else 
			{
                stageLength = rwContext->Length;
            }
			//重新利用MDL
            MmPrepareMdlForReuse(rwContext->NewMdl);

            IoBuildPartialMdl(Irp->MdlAddress,
                              rwContext->NewMdl,
                              (PVOID) rwContext->VirtualAddress,
                              stageLength);
        
            rwContext->VirtualAddress += stageLength;
            rwContext->Length -= stageLength;

			IoCopyCurrentIrpStackLocationToNext(Irp);
            PIO_STACK_LOCATION nextStack = IoGetNextIrpStackLocation(Irp);

			nextStack->Parameters.Read.Length = stageLength;

            IoSetCompletionRoutine(Irp,
                                   HelloDDKReadCompletion,
                                   rwContext,
                                   TRUE,
                                   TRUE,
                                   TRUE);

            IoCallDriver(rwContext->DeviceExtension->TargetDevice, 
                         Irp);

            return STATUS_MORE_PROCESSING_REQUIRED;
        }
        else 
		{
			//最后一次传输
            Irp->IoStatus.Information = rwContext->Numxfer;
        }
	}

	KdPrint(("DriverB:Leave B HelloDDKReadCompletion\n"));
	return STATUS_MORE_PROCESSING_REQUIRED;  
}

#pragma PAGEDCODE
NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("DriverB:Enter B HelloDDKRead\n"));
	NTSTATUS status = STATUS_SUCCESS;

	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
		pDevObj->DeviceExtension;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	
	ULONG totalLength;
	ULONG stageLength;
	PMDL mdl;
	PVOID virtualAddress;
	PMYDRIVER_RW_CONTEXT rwContext = NULL;
	PIO_STACK_LOCATION nextStack;

	if (!pIrp->MdlAddress)
	{
		status = STATUS_UNSUCCESSFUL;
		totalLength = 0;
		goto HelloDDKRead_EXIT;
	}

	//获取MDL的虚拟地址
	virtualAddress = MmGetMdlVirtualAddress(pIrp->MdlAddress);
	//获取MDL的长度
	totalLength = MmGetMdlByteCount(pIrp->MdlAddress);

	KdPrint(("DriverB:(pIrp->MdlAddress)MmGetMdlVirtualAddress:%08X\n",MmGetMdlVirtualAddress(pIrp->MdlAddress)));
	KdPrint(("DriverB:(pIrp->MdlAddress)MmGetMdlByteCount:%d\n",MmGetMdlByteCount(pIrp->MdlAddress)));

	//将总的传输，分成几个阶段，这里设定每次阶段的长度
    if(totalLength > MAX_PACKAGE_SIZE) 
	{
        stageLength = MAX_PACKAGE_SIZE;
    }else 
	{
        stageLength = totalLength;
    }

	//创建新的MDL
    mdl = IoAllocateMdl((PVOID) virtualAddress,
                        totalLength,
                        FALSE,
                        FALSE,
                        NULL);

	KdPrint(("DriverB:(new mdl)MmGetMdlVirtualAddress:%08X\n",MmGetMdlVirtualAddress(mdl)));
	KdPrint(("DriverB:(new mdl)MmGetMdlByteCount:%d\n",MmGetMdlByteCount(mdl)));

    if(mdl == NULL) 
	{
		KdPrint(("DriverB:Failed to alloc mem for mdl\n"));
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto HelloDDKRead_EXIT;
    }

	//将IRP的MDL做重新映射
    IoBuildPartialMdl(pIrp->MdlAddress,
                      mdl,
                      (PVOID) virtualAddress,
                      stageLength);
	KdPrint(("DriverB:(new mdl)MmGetMdlVirtualAddress:%08X\n",MmGetMdlVirtualAddress(mdl)));
	KdPrint(("DriverB:(new mdl)MmGetMdlByteCount:%d\n",MmGetMdlByteCount(mdl)));

	rwContext = (PMYDRIVER_RW_CONTEXT)
                ExAllocatePool(NonPagedPool,sizeof(MYDRIVER_RW_CONTEXT));

    rwContext->NewMdl			= mdl;
	rwContext->PreviousMdl		= pIrp->MdlAddress;
    rwContext->Length			= totalLength - stageLength;//还剩下多少没读取
    rwContext->Numxfer			= 0;						//读了多少字节
    rwContext->VirtualAddress	= ((ULONG_PTR)virtualAddress + stageLength);//下一阶段开始读取的地址
	rwContext->DeviceExtension	= pDevExt;

	//拷贝到底层堆栈
	IoCopyCurrentIrpStackLocationToNext(pIrp);

	nextStack = IoGetNextIrpStackLocation(pIrp);
	//根据底层驱动的实现，底层驱动有可能读取这个数值，也有可能读取mdl的length。
	nextStack->Parameters.Read.Length = stageLength;

	pIrp->MdlAddress = mdl;
	
	//设定完成例程
    IoSetCompletionRoutine(pIrp, 
                           (PIO_COMPLETION_ROUTINE)HelloDDKReadCompletion,
                           rwContext,
                           TRUE,
                           TRUE,
                           TRUE);

    IoCallDriver(pDevExt->TargetDevice,pIrp);
	
	pIrp->MdlAddress = rwContext->PreviousMdl;
	IoFreeMdl(rwContext->NewMdl);

HelloDDKRead_EXIT:
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = totalLength;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("DriverB:Leave B HelloDDKRead\n"));
	return status;
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
/************************************************************************
* 文件名称:HelloWDM.cpp                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/
#include "HelloWDM.h"

#include <initguid.h>
#include "guid.h"
#include "Ioctls.h"

/************************************************************************
* 函数名称:DriverEntry
* 功能描述:初始化驱动程序，定位和申请硬件资源，创建内核对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
      pRegistryPath:驱动程序在注册表的中的路径
* 返回 值:返回初始化驱动状态
*************************************************************************/
#pragma INITCODE 
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject,
								IN PUNICODE_STRING pRegistryPath)
{
	KdPrint(("Enter DriverEntry\n"));

	pDriverObject->DriverExtension->AddDevice = HelloWDMAddDevice;
	pDriverObject->MajorFunction[IRP_MJ_PNP] = HelloWDMPnp;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = 
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = 
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = 
	pDriverObject->MajorFunction[IRP_MJ_READ] = 
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloWDMDispatchRoutine;
	pDriverObject->DriverUnload = HelloWDMUnload;

	KdPrint(("Leave DriverEntry\n"));
	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:HelloWDMAddDevice
* 功能描述:添加新设备
* 参数列表:
      DriverObject:从I/O管理器中传进来的驱动对象
      PhysicalDeviceObject:从I/O管理器中传进来的物理设备对象
* 返回 值:返回添加新设备状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloWDMAddDevice(IN PDRIVER_OBJECT DriverObject,
                           IN PDEVICE_OBJECT PhysicalDeviceObject)
{ 
	PAGED_CODE();
	KdPrint(("Enter HelloWDMAddDevice\n"));

	NTSTATUS status;
	PDEVICE_OBJECT fdo;
	status = IoCreateDevice(
		DriverObject,
		sizeof(DEVICE_EXTENSION),
		NULL,//没有指定设备名
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&fdo);
	if( !NT_SUCCESS(status))
		return status;
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)fdo->DeviceExtension;
	pdx->fdo = fdo;
	pdx->NextStackDevice = IoAttachDeviceToDeviceStack(fdo, PhysicalDeviceObject);

	//创建设备接口
	status = IoRegisterDeviceInterface(PhysicalDeviceObject, &MY_WDM_DEVICE, NULL, &pdx->interfaceName);
	if( !NT_SUCCESS(status))
	{
		IoDeleteDevice(fdo);
		return status;
	}
	KdPrint(("%wZ\n",&pdx->interfaceName));
	IoSetDeviceInterfaceState(&pdx->interfaceName, TRUE);

	if( !NT_SUCCESS(status))
	{
		if( !NT_SUCCESS(status))
		{
			return status;
		}
	}

	fdo->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
	fdo->Flags &= ~DO_DEVICE_INITIALIZING;

	KdPrint(("Leave HelloWDMAddDevice\n"));
	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:DefaultPnpHandler
* 功能描述:对PNP IRP进行缺省处理
* 参数列表:
      pdx:设备对象的扩展
      Irp:从IO请求包
* 返回 值:返回状态
*************************************************************************/ 
#pragma PAGEDCODE
NTSTATUS DefaultPnpHandler(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter DefaultPnpHandler\n"));
	IoSkipCurrentIrpStackLocation(Irp);
	KdPrint(("Leave DefaultPnpHandler\n"));
	return IoCallDriver(pdx->NextStackDevice, Irp);
}

#pragma LOCKEDCODE
NTSTATUS OnRequestComplete(PDEVICE_OBJECT junk, PIRP Irp, PKEVENT pev)
{							// OnRequestComplete
	//在完成例程中设置等待事件
	KeSetEvent(pev, 0, FALSE);
	//标志本IRP还需要再次被完成
	return STATUS_MORE_PROCESSING_REQUIRED;
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE
NTSTATUS ForwardAndWait(PDEVICE_EXTENSION pdx, PIRP Irp)
{							// ForwardAndWait
	PAGED_CODE();
	
	KEVENT event;
	//初始化事件
	KeInitializeEvent(&event, NotificationEvent, FALSE);

	//将本层堆栈拷贝到下一层堆栈
	IoCopyCurrentIrpStackLocationToNext(Irp);
	//设置完成例程
	IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) OnRequestComplete,
		(PVOID) &event, TRUE, TRUE, TRUE);

	//调用底层驱动，即PDO
	IoCallDriver(pdx->NextStackDevice, Irp);
	//等待PDO完成
	KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
	return Irp->IoStatus.Status;
}							// ForwardAndWait


/************************************************************************
* 函数名称:HandleRemoveDevice
* 功能描述:对IRP_MN_REMOVE_DEVICE IRP进行处理
* 参数列表:
      fdo:功能设备对象
      Irp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HandleRemoveDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HandleRemoveDevice\n"));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	NTSTATUS status = DefaultPnpHandler(pdx, Irp);

	IoSetDeviceInterfaceState(&pdx->interfaceName, FALSE);
	RtlFreeUnicodeString(&pdx->interfaceName);

    //调用IoDetachDevice()把fdo从设备栈中脱开：
    if (pdx->NextStackDevice)
        IoDetachDevice(pdx->NextStackDevice);
	
    //删除fdo：
    IoDeleteDevice(pdx->fdo);

	//删除中断
	IoDisconnectInterrupt(pdx->InterruptObject);
	KdPrint(("Leave HandleRemoveDevice\n"));
	return status;
}

BOOLEAN OnInterrupt(PKINTERRUPT InterruptObject, PDEVICE_EXTENSION pdx)
{							// OnInterrupt

	//关中断
	UCHAR HSR  = READ_PORT_UCHAR(pdx->portbase);
	HSR = HSR | 0x4;
	WRITE_PORT_UCHAR(pdx->portbase,HSR);

	KdPrint(("==============interrupt!!!\n"));
	
	//恢复中断信号电平
	WRITE_REGISTER_UCHAR((PUCHAR)pdx->MemBar1+0x400000,0x10);

	IoRequestDpc(pdx->fdo, NULL, pdx);

	return TRUE;
}		

#pragma PAGEDCODE
NTSTATUS InitMyPCI(IN PDEVICE_EXTENSION pdx,IN PCM_PARTIAL_RESOURCE_LIST list)
{
	PDEVICE_OBJECT fdo = pdx->fdo;

	ULONG vector;
	KIRQL irql;
	KINTERRUPT_MODE mode;
	KAFFINITY affinity;
	BOOLEAN irqshare;
	BOOLEAN gotinterrupt = FALSE;

	PHYSICAL_ADDRESS portbase;
	BOOLEAN gotport = FALSE;
	 
	PCM_PARTIAL_RESOURCE_DESCRIPTOR resource = &list->PartialDescriptors[0];
	ULONG nres = list->Count;
	BOOLEAN IsMem0 = TRUE;
	for (ULONG i = 0; i < nres; ++i, ++resource)
		{						// for each resource
		switch (resource->Type)
			{					// switch on resource type
		case CmResourceTypePort:
			portbase = resource->u.Port.Start;
			pdx->nports = resource->u.Port.Length;
			pdx->mappedport = (resource->Flags & CM_RESOURCE_PORT_IO) == 0;
			gotport = TRUE;
			break;

		case CmResourceTypeMemory:
			if (IsMem0) 
			{
				pdx->MemBar0 = (PUCHAR)MmMapIoSpace(resource->u.Memory.Start,
					resource->u.Memory.Length,
					MmNonCached);
				pdx->nMem0 = resource->u.Memory.Length;
				IsMem0 = FALSE;
			}else
			{
				pdx->MemBar1 = (PUCHAR)MmMapIoSpace(resource->u.Memory.Start,
					resource->u.Memory.Length,
					MmNonCached);
				pdx->nMem1 = resource->u.Memory.Length;
			}

			break;

		case CmResourceTypeInterrupt:
			irql = (KIRQL) resource->u.Interrupt.Level;
			vector = resource->u.Interrupt.Vector;
			affinity = resource->u.Interrupt.Affinity;
			mode = (resource->Flags == CM_RESOURCE_INTERRUPT_LATCHED)
				? Latched : LevelSensitive;
			irqshare = resource->ShareDisposition == CmResourceShareShared;
			gotinterrupt = TRUE;
			
			break;

		default:
			KdPrint(("Unexpected I/O resource type %d\n", resource->Type));
			break;
			}					// switch on resource type
		}						// for each resource

	if (!(TRUE&& gotport&& gotinterrupt	))
		{
		KdPrint((" Didn't get expected I/O resources\n"));
		return STATUS_DEVICE_CONFIGURATION_ERROR;
		}

	if (pdx->mappedport)
		{						// map port address for RISC platform
		pdx->portbase = (PUCHAR) MmMapIoSpace(portbase, pdx->nports, MmNonCached);
		if (!pdx->mappedport)
			{
			KdPrint(("Unable to map port range %I64X, length %X\n", portbase, pdx->nports));
			return STATUS_INSUFFICIENT_RESOURCES;
			}
		}						// map port address for RISC platform
	else
		pdx->portbase = (PUCHAR) portbase.QuadPart;

	NTSTATUS status = IoConnectInterrupt(&pdx->InterruptObject, (PKSERVICE_ROUTINE) OnInterrupt,
		(PVOID) pdx, NULL, vector, irql, irql, LevelSensitive, TRUE, affinity, FALSE);
	if (!NT_SUCCESS(status))
		{
		KdPrint(("IoConnectInterrupt failed - %X\n", status));
		if (pdx->portbase && pdx->mappedport)
			MmUnmapIoSpace(pdx->portbase, pdx->nports);
		pdx->portbase = NULL;
		return status;
		}

#define IMAGE_LENGTH (640*480)
	//申请一段连续物理地址来读取图像
	PHYSICAL_ADDRESS maxAddress;
	maxAddress.u.LowPart = 0xFFFFFFFF;
	maxAddress.u.HighPart = 0;

	pdx->MemForImage = MmAllocateContiguousMemory(IMAGE_LENGTH,maxAddress);

	PHYSICAL_ADDRESS pycialAddressForImage = MmGetPhysicalAddress(pdx->MemForImage);

	WRITE_REGISTER_BUFFER_UCHAR((PUCHAR)pdx->MemBar0+0x10000,
		(PUCHAR)&pycialAddressForImage.u.LowPart,4);

	return STATUS_SUCCESS;	
}

#pragma PAGEDCODE
NTSTATUS HandleStartDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HandleStartDevice\n"));

	//转发IRP并等待返回
	NTSTATUS status = ForwardAndWait(pdx,Irp);
	if (!NT_SUCCESS(status))
	{
		Irp->IoStatus.Status = status;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return status;
	}

	//得到当前堆栈
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

	//从当前堆栈得到翻译信息
	PCM_PARTIAL_RESOURCE_LIST translated;
	if (stack->Parameters.StartDevice.AllocatedResourcesTranslated)
		translated = &stack->Parameters.StartDevice.AllocatedResourcesTranslated->List[0].PartialResourceList;
	else
		translated = NULL;

	KdPrint(("Init the PCI card!\n"));
	InitMyPCI(pdx,translated);

	//完成IRP
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	KdPrint(("Leave HandleStartDevice\n"));
	return status;
}

/************************************************************************
* 函数名称:HelloWDMPnp
* 功能描述:对即插即用IRP进行处理
* 参数列表:
      fdo:功能设备对象
      Irp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloWDMPnp(IN PDEVICE_OBJECT fdo,
                        IN PIRP Irp)
{
	PAGED_CODE();

	KdPrint(("Enter HelloWDMPnp\n"));
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	static NTSTATUS (*fcntab[])(PDEVICE_EXTENSION pdx, PIRP Irp) = 
	{
		HandleStartDevice,		// IRP_MN_START_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_REMOVE_DEVICE
		HandleRemoveDevice,		// IRP_MN_REMOVE_DEVICE
		DefaultPnpHandler,		// IRP_MN_CANCEL_REMOVE_DEVICE
		DefaultPnpHandler,		// IRP_MN_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_CANCEL_STOP_DEVICE
		DefaultPnpHandler,		// IRP_MN_QUERY_DEVICE_RELATIONS
		DefaultPnpHandler,		// IRP_MN_QUERY_INTERFACE
		DefaultPnpHandler,		// IRP_MN_QUERY_CAPABILITIES
		DefaultPnpHandler,		// IRP_MN_QUERY_RESOURCES
		DefaultPnpHandler,		// IRP_MN_QUERY_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,		// IRP_MN_QUERY_DEVICE_TEXT
		DefaultPnpHandler,		// IRP_MN_FILTER_RESOURCE_REQUIREMENTS
		DefaultPnpHandler,		// 
		DefaultPnpHandler,		// IRP_MN_READ_CONFIG
		DefaultPnpHandler,		// IRP_MN_WRITE_CONFIG
		DefaultPnpHandler,		// IRP_MN_EJECT
		DefaultPnpHandler,		// IRP_MN_SET_LOCK
		DefaultPnpHandler,		// IRP_MN_QUERY_ID
		DefaultPnpHandler,		// IRP_MN_QUERY_PNP_DEVICE_STATE
		DefaultPnpHandler,		// IRP_MN_QUERY_BUS_INFORMATION
		DefaultPnpHandler,		// IRP_MN_DEVICE_USAGE_NOTIFICATION
		DefaultPnpHandler,		// IRP_MN_SURPRISE_REMOVAL
	};

	ULONG fcn = stack->MinorFunction;
	if (fcn >= arraysize(fcntab))
	{						// 未知的子功能代码
		status = DefaultPnpHandler(pdx, Irp); // some function we don't know about
		return status;
	}						

#if DBG
	static char* fcnname[] = 
	{
		"IRP_MN_START_DEVICE",
		"IRP_MN_QUERY_REMOVE_DEVICE",
		"IRP_MN_REMOVE_DEVICE",
		"IRP_MN_CANCEL_REMOVE_DEVICE",
		"IRP_MN_STOP_DEVICE",
		"IRP_MN_QUERY_STOP_DEVICE",
		"IRP_MN_CANCEL_STOP_DEVICE",
		"IRP_MN_QUERY_DEVICE_RELATIONS",
		"IRP_MN_QUERY_INTERFACE",
		"IRP_MN_QUERY_CAPABILITIES",
		"IRP_MN_QUERY_RESOURCES",
		"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
		"IRP_MN_QUERY_DEVICE_TEXT",
		"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
		"",
		"IRP_MN_READ_CONFIG",
		"IRP_MN_WRITE_CONFIG",
		"IRP_MN_EJECT",
		"IRP_MN_SET_LOCK",
		"IRP_MN_QUERY_ID",
		"IRP_MN_QUERY_PNP_DEVICE_STATE",
		"IRP_MN_QUERY_BUS_INFORMATION",
		"IRP_MN_DEVICE_USAGE_NOTIFICATION",
		"IRP_MN_SURPRISE_REMOVAL",
	};

	KdPrint(("PNP Request (%s)\n", fcnname[fcn]));
#endif // DBG

	status = (*fcntab[fcn])(pdx, Irp);
	KdPrint(("Leave HelloWDMPnp\n"));
	return status;
}

/************************************************************************
* 函数名称:HelloWDMDispatchRoutine
* 功能描述:对缺省IRP进行处理
* 参数列表:
      fdo:功能设备对象
      Irp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloWDMDispatchRoutine(IN PDEVICE_OBJECT fdo,
								 IN PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HelloWDMDispatchRoutine\n"));
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;	// no bytes xfered
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	KdPrint(("Leave HelloWDMDispatchRoutine\n"));
	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:HelloWDMUnload
* 功能描述:负责驱动程序的卸载操作
* 参数列表:
      DriverObject:驱动对象
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
void HelloWDMUnload(IN PDRIVER_OBJECT DriverObject)
{
	PAGED_CODE();
	KdPrint(("Enter HelloWDMUnload\n"));
	KdPrint(("Leave HelloWDMUnload\n"));
}

NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info)
{							// CompleteRequest
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}
	
NTSTATUS DispatchControl(PDEVICE_OBJECT fdo, PIRP Irp)
{							// DispatchControl
	PAGED_CODE();
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

	NTSTATUS status;

	ULONG info = 0;

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG cbin = stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG cbout = stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;

	switch (code)
	{						// process request
	case IOCTL_READ_BASE_BAR0:
		{
			ULONG offset = *(ULONG*)(Irp->AssociatedIrp.SystemBuffer);
			PUCHAR buff = (PUCHAR)ExAllocatePool(NonPagedPool,cbout);
			READ_REGISTER_BUFFER_UCHAR((PUCHAR)pdx->MemBar0+offset,buff,cbout);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, buff, cbout);
			ExFreePool(buff);
			info = cbout;
			break;
		}
	case IOCTL_WRITE_BASE_BAR0:
		{
			int* tempPointer = (int*)Irp->AssociatedIrp.SystemBuffer;
			ULONG offset = *(ULONG*)(tempPointer);
			tempPointer++;
			PUCHAR buff = *(PUCHAR*)(tempPointer);
			tempPointer++;
			ULONG nInputNumber = *(ULONG*)(tempPointer);
			WRITE_REGISTER_BUFFER_UCHAR((PUCHAR)pdx->MemBar0+offset,buff,nInputNumber);
			break;
		}
	case IOCTL_READ_BASE_BAR2:
		{
			ULONG offset = *(ULONG*)(Irp->AssociatedIrp.SystemBuffer);
			PUCHAR buff = (PUCHAR)ExAllocatePool(NonPagedPool,cbout);
			READ_PORT_BUFFER_UCHAR(pdx->portbase+offset,buff,cbout);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, buff, cbout);
			ExFreePool(buff);
			info = cbout;
			break;
		}
	case IOCTL_WRITE_BASE_BAR2:
		{
			int* tempPointer = (int*)Irp->AssociatedIrp.SystemBuffer;
			ULONG offset = *(ULONG*)(tempPointer);
			tempPointer++;
			PUCHAR buff = *(PUCHAR*)(tempPointer);
			tempPointer++;
			ULONG nInputNumber = *(ULONG*)(tempPointer);
			WRITE_PORT_BUFFER_UCHAR(pdx->portbase+offset,buff,nInputNumber);
			break;
		}
	case IOCTL_READ_IMAGE:
		{
			
			PUCHAR buff = (PUCHAR)ExAllocatePool(NonPagedPool,cbout);
			READ_REGISTER_BUFFER_UCHAR((PUCHAR)pdx->MemForImage,buff,cbout);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, buff, cbout);
			ExFreePool(buff);
			info = cbout;
			break; 
		}
	case IOCTL_WRITE_IMAGE:
		{
			PUCHAR buff = (PUCHAR)ExAllocatePool(NonPagedPool,cbin);
			RtlCopyMemory(buff,Irp->AssociatedIrp.SystemBuffer,cbin);
			WRITE_REGISTER_BUFFER_UCHAR((PUCHAR)pdx->MemForImage,buff,cbin);
			ExFreePool(buff);
			info = cbin;
			break; 
		}
	case IOCTL_ENABLE_INT:
		{
			//允许中断
			UCHAR HSR = READ_PORT_UCHAR(pdx->portbase);
			HSR = HSR & 0xFB;
			WRITE_PORT_UCHAR(pdx->portbase,HSR);
			break;
		}

	case IOCTL_DISABLE_INT:
		{
			//关中断
			UCHAR HSR  = READ_PORT_UCHAR(pdx->portbase);
			HSR = HSR | 0x4;
			WRITE_PORT_UCHAR(pdx->portbase,HSR);
			break;
		}

	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;

	}						// process request

	return CompleteRequest(Irp, status, info);
}
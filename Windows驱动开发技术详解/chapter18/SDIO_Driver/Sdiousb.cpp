#include "Sdiomain.h"
#include "SdioDev.h"
#include "Ioctls.h"

SDIOGLOBALS SdioGlobals;

NTSTATUS HelloWDMRead(IN PDEVICE_OBJECT fdo,
								 IN PIRP Irp)
{
	KdPrint(("Enter HelloWDMRead\n"));
	PAGED_CODE();

	NTSTATUS status = STATUS_SUCCESS;

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

	KdPrint(("MDL:%x\n",Irp->MdlAddress));


	status = SdioReadBufferFromDevice(pdx->FunctionalDeviceObject,
		pdx->FunctionNumber,
		Irp->MdlAddress,
		IFDevice_SDIO_DATA_REG,
		pdx->Channel0_Length,
		&Irp->IoStatus.Information);

	Irp->IoStatus.Status = status;

	IoCompleteRequest( Irp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloWDMRead\n"));
	return STATUS_SUCCESS;
}

NTSTATUS HelloWDMDispatchRoutine(IN PDEVICE_OBJECT fdo,
								 IN PIRP Irp)
{
	PAGED_CODE();
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;	// no bytes xfered
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	KdPrint(("Leave HelloWDMDispatchRoutine\n"));
	return STATUS_SUCCESS;
}

NTSTATUS DefaultPnpHandler(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter DefaultPnpHandler\n"));
	IoSkipCurrentIrpStackLocation(Irp);
	KdPrint(("Leave DefaultPnpHandler\n"));
	return IoCallDriver(pdx->TopOfStackDeviceObject, Irp);
}

NTSTATUS OnRequestComplete(PDEVICE_OBJECT junk, PIRP Irp, PKEVENT pev)
{							// OnRequestComplete
	//在完成例程中设置等待事件
	KeSetEvent(pev, 0, FALSE);
	//标志本IRP还需要再次被完成
	return STATUS_MORE_PROCESSING_REQUIRED;
}

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
	IoCallDriver(pdx->TopOfStackDeviceObject, Irp);
	//等待PDO完成
	KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
	return Irp->IoStatus.Status;
}							// ForwardAndWait

NTSTATUS HandleStartDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HandleStartDevice\n"));

	pdx->Channel0_Length = 0;

	//转发IRP并等待返回
	NTSTATUS status = ForwardAndWait(pdx,Irp);
	if (!NT_SUCCESS(status))
	{
		Irp->IoStatus.Status = status;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return status;
	}
	
	UCHAR ucRegVal = 0;
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,0,&ucRegVal,REG_CCCR_IENx,FALSE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("SdioReadWriteByte fail:%x\n",status));
		return status;
	}
	KdPrint(("REG_CCCR_IENx:%x\n",ucRegVal));


	//Set Bus Width to 4 bits
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,0,&ucRegVal,REG_CCCR_BUS_INTERFACE_CTRL,FALSE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get REG_CCCR_BUS_INTERFACE_CTRL failed:%x\n",status));
		return status;
	}
	KdPrint(("Previous REG_CCCR_BUS_INTERFACE_CTRL:%x\n",ucRegVal));
	ucRegVal = ucRegVal | 0x2;
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,0,&ucRegVal,REG_CCCR_BUS_INTERFACE_CTRL,TRUE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Set Bus Width to 4 bits failed:%x\n",status));
		return status;
	}
	KdPrint(("Set Bus Width to 4 bits successfully!\n"));

	//完成IRP
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	KdPrint(("Leave HandleStartDevice\n"));
	return status;
}

NTSTATUS HandleRemoveDevice(PDEVICE_EXTENSION pdx, PIRP Irp)
{
	PAGED_CODE();
	KdPrint(("Enter HandleRemoveDevice\n"));

    if (pdx->InterfaceStandard.InterfaceDereference) 
    {
        (pdx->InterfaceStandard.InterfaceDereference)(pdx->InterfaceStandard.Context);
        pdx->InterfaceStandard.InterfaceDereference = NULL;
    }

	Irp->IoStatus.Status = STATUS_SUCCESS;
	NTSTATUS status = DefaultPnpHandler(pdx, Irp);
	IoSetDeviceInterfaceState(&pdx->InterfaceName, FALSE);

    //调用IoDetachDevice()把fdo从设备栈中脱开：
    if (pdx->TopOfStackDeviceObject)
        IoDetachDevice(pdx->TopOfStackDeviceObject);
	
    //删除fdo：
    IoDeleteDevice(pdx->FunctionalDeviceObject);
	KdPrint(("Leave HandleRemoveDevice\n"));
	return status;
}

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
NTSTATUS
DriverEntry(
IN PDRIVER_OBJECT DriverObject,
IN PUNICODE_STRING UniRegistryPath
)
{

	NTSTATUS ntStatus;
	PUNICODE_STRING pregistryPath = &SdioGlobals.Sdio_RegistryPath;
	
	KdPrint(("Enter DriverEntry\n"));
	
	//
	// Allocate pool to hold a null-terminated copy of the path.
	// Safe in paged pool since all registry routines execute at
	// PASSIVE_LEVEL.
	//
	pregistryPath->MaximumLength = UniRegistryPath->Length + sizeof(UNICODE_NULL);
	pregistryPath->Length = UniRegistryPath->Length;
	pregistryPath->Buffer = (PWSTR )ExAllocatePool(PagedPool,pregistryPath->MaximumLength);
	
	if (!(pregistryPath->Buffer)) 
	{
		KdPrint(("DriverEntry: Failed to allocate memory for registryPath\n"));
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		goto DriverEntry_Exit;
	}
	
	RtlZeroMemory (pregistryPath->Buffer,
		pregistryPath->MaximumLength);
		RtlMoveMemory (pregistryPath->Buffer,
		UniRegistryPath->Buffer,
		UniRegistryPath->Length);

	ntStatus = STATUS_SUCCESS;
	
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HelloWDMDeviceIOControl;
	DriverObject->MajorFunction[IRP_MJ_POWER] = HelloWDMDispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_PNP] = HelloWDMPnp;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = HelloWDMDispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloWDMDispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_CLEANUP] = HelloWDMDispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_READ] = HelloWDMRead;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = HelloWDMDispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = HelloWDMDispatchRoutine;

	DriverObject->DriverUnload = SdioClientDrv_DriverUnload;
	DriverObject->DriverExtension->AddDevice = (PDRIVER_ADD_DEVICE) SdioClientDrv_AddDevice;
	
	DriverEntry_Exit:
	KdPrint(("Leave DriverEntry\n"));
	return ntStatus;
}

NTSTATUS EnableInt(PDEVICE_EXTENSION pdx)
{
	NTSTATUS status;
	UCHAR ucRegVal;
	//Enable CCCR interrupt
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,0,&ucRegVal,REG_CCCR_IENx,FALSE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get REG_CCCR_IENx failed:%x\n",status));
		return status;
	}
	KdPrint(("Previous REG_CCCR_IENx:%x\n",ucRegVal));

	ucRegVal = ucRegVal | 1;
	ucRegVal = ucRegVal | (1<<pdx->FunctionNumber);

	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,0,&ucRegVal,REG_CCCR_IENx,TRUE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Enable CCCR interrupt failed:%x\n",status));
		return status;
	}
	KdPrint(("Enable CCCR interrupt successfully!\n"));


	//Enable Device interrupt
	ucRegVal = 0x07;
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,pdx->FunctionNumber,&ucRegVal,IFDevice_INT0_ENABLE,TRUE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Enable Device interrupt failed:%x\n",status));
		return status;
	}
	KdPrint(("Enable Device interrupt successfully!\n"));
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,pdx->FunctionNumber,&ucRegVal,IFDevice_INT0_ENABLE,FALSE);
	if (NT_SUCCESS(status))
	{
		KdPrint(("IFDevice_INT0_ENABLE:%x\n",ucRegVal));
	}
	return status;
}

NTSTATUS DisableInt(PDEVICE_EXTENSION pdx)
{
	NTSTATUS status;

	//Disable Device interrupt
	UCHAR ucRegVal = 0x0;
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,pdx->FunctionNumber,&ucRegVal,IFDevice_INT0_ENABLE,TRUE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Disable Device interrupt failed:%x\n",status));
		return status;
	}
	KdPrint(("Disable Device interrupt successfully!\n"));
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,pdx->FunctionNumber,&ucRegVal,IFDevice_INT0_ENABLE,FALSE);
	if (NT_SUCCESS(status))
	{
		KdPrint(("IFDevice_INT0_ENABLE:%x\n",ucRegVal));
	}
	return status;
}

NTSTATUS
SdioReadBufferFromDevice(IN PDEVICE_OBJECT deviceObject,
                   IN ULONG Function,
                   IN PMDL Mdl,
                   IN ULONG Address,
                   IN ULONG Length,
                   OUT PULONG BytesRead
                   )
{
    PDEVICE_EXTENSION pdx = 
    	(PDEVICE_EXTENSION)deviceObject->DeviceExtension; 

	//disable interrput
	DisableInt(pdx);

    SDBUS_REQUEST_PACKET sdrp;
    SD_RW_EXTENDED_ARGUMENT extendedArgument;
    NTSTATUS                 status;
    const SDCMD_DESCRIPTOR ReadIoExtendedDesc =
    {SDCMD_IO_RW_EXTENDED, SDCC_STANDARD, SDTD_READ, SDTT_SINGLE_BLOCK, SDRT_5};

    //PAGED_CODE();

    RtlZeroMemory(&sdrp, sizeof(SDBUS_REQUEST_PACKET));

    sdrp.RequestFunction = SDRF_DEVICE_COMMAND;
    sdrp.Parameters.DeviceCommand.Mdl = Mdl;

    extendedArgument.u.AsULONG = 0;
    extendedArgument.u.bits.Function = Function;
    extendedArgument.u.bits.OpCode = 0;             // non-increment address
    extendedArgument.u.bits.BlockMode = 0;
    extendedArgument.u.bits.Address = Address;

	sdrp.Parameters.DeviceCommand.CmdDesc  = ReadIoExtendedDesc;

    sdrp.Parameters.DeviceCommand.Argument = extendedArgument.u.AsULONG;
    sdrp.Parameters.DeviceCommand.Length   = Length;

    //
    // Send the IO request down to the bus driver
    //

    status = SdBusSubmitRequest(pdx->InterfaceStandard.Context, &sdrp);
    *BytesRead = (ULONG)sdrp.Information;
	//enable interrput
	EnableInt(pdx);
    return status;

}

NTSTATUS
SdioReadWriteBuffer(IN PDEVICE_OBJECT deviceObject,
                   IN ULONG Function,
                   IN PMDL Mdl,
                   IN ULONG Address,
                   IN ULONG Length,
                   IN BOOLEAN WriteToDevice,
                   OUT PULONG BytesRead
                   )
{
    PDEVICE_EXTENSION pdx = 
    	(PDEVICE_EXTENSION)deviceObject->DeviceExtension; 

    SDBUS_REQUEST_PACKET sdrp;
    SD_RW_EXTENDED_ARGUMENT extendedArgument;
    NTSTATUS                 status;
    const SDCMD_DESCRIPTOR ReadIoExtendedDesc =
    {SDCMD_IO_RW_EXTENDED, SDCC_STANDARD, SDTD_READ, SDTT_SINGLE_BLOCK, SDRT_5};

    const SDCMD_DESCRIPTOR WriteIoExtendedDesc =
    {SDCMD_IO_RW_EXTENDED, SDCC_STANDARD, SDTD_WRITE, SDTT_SINGLE_BLOCK, SDRT_5};

    //PAGED_CODE();

    RtlZeroMemory(&sdrp, sizeof(SDBUS_REQUEST_PACKET));

    sdrp.RequestFunction = SDRF_DEVICE_COMMAND;
    sdrp.Parameters.DeviceCommand.Mdl = Mdl;

    extendedArgument.u.AsULONG = 0;
    extendedArgument.u.bits.Function = Function;
    extendedArgument.u.bits.OpCode = 1;             // increment address
    extendedArgument.u.bits.BlockMode = pdx->BlockMode;
    extendedArgument.u.bits.Address = Address;

    if (WriteToDevice) {
        extendedArgument.u.bits.WriteToDevice = 1;
        sdrp.Parameters.DeviceCommand.CmdDesc  = WriteIoExtendedDesc;
    } else {
        sdrp.Parameters.DeviceCommand.CmdDesc  = ReadIoExtendedDesc;
    }

    if (pdx->BlockMode == 1) {
        sdrp.Parameters.DeviceCommand.CmdDesc.TransferType = SDTT_MULTI_BLOCK_NO_CMD12;
    }

    sdrp.Parameters.DeviceCommand.Argument = extendedArgument.u.AsULONG;
    sdrp.Parameters.DeviceCommand.Length   = Length;

    //
    // Send the IO request down to the bus driver
    //

    status = SdBusSubmitRequest(pdx->InterfaceStandard.Context, &sdrp);
    *BytesRead = (ULONG)sdrp.Information;
    return status;

}

NTSTATUS
SdioReadWriteByte(IN PDEVICE_OBJECT deviceObject,
                 IN ULONG Function,
                 IN PUCHAR Data,
                 IN ULONG Address,
                 IN BOOLEAN WriteToDevice
                 )
{
    PDEVICE_EXTENSION pdx = 
    	(PDEVICE_EXTENSION)deviceObject->DeviceExtension; 
    NTSTATUS	status;
    SDBUS_REQUEST_PACKET	sdrp;
    SD_RW_DIRECT_ARGUMENT	directArgument;

    const SDCMD_DESCRIPTOR	ReadIoDirectDesc =
    {SDCMD_IO_RW_DIRECT, SDCC_STANDARD, SDTD_READ, SDTT_CMD_ONLY, SDRT_5};

    const SDCMD_DESCRIPTOR WriteIoDirectDesc =
    {SDCMD_IO_RW_DIRECT, SDCC_STANDARD, SDTD_WRITE, SDTT_CMD_ONLY, SDRT_5};

    PAGED_CODE();
    //
    // get an SD request packet
    //

    RtlZeroMemory(&sdrp, sizeof(SDBUS_REQUEST_PACKET));

    sdrp.RequestFunction = SDRF_DEVICE_COMMAND;

    directArgument.u.AsULONG = 0;
    directArgument.u.bits.Function = Function;
    directArgument.u.bits.Address = Address;


    if (WriteToDevice) 
    {
        directArgument.u.bits.WriteToDevice = 1;
        directArgument.u.bits.Data = *Data;
        sdrp.Parameters.DeviceCommand.CmdDesc  = WriteIoDirectDesc;
    } else {
        sdrp.Parameters.DeviceCommand.CmdDesc  = ReadIoDirectDesc;
    }

    sdrp.Parameters.DeviceCommand.Argument = directArgument.u.AsULONG;

    //
    // Send the IO request down to the bus driver
    //

    status = SdBusSubmitRequest(pdx->InterfaceStandard.Context, &sdrp);

	if(!NT_SUCCESS(status))
	{
		KdPrint(("SdioReadWriteByte failed\n"));
	}

    if (NT_SUCCESS(status) && !WriteToDevice) 
    {
        *Data = sdrp.ResponseData.AsUCHAR[0];
    }
    return status;
}


NTSTATUS
SdioGetProperty(IN PDEVICE_OBJECT deviceObject,
				IN SDBUS_PROPERTY Property,
				IN PVOID Buffer,
				IN ULONG Length)
{
    PDEVICE_EXTENSION pdx;
    SDBUS_REQUEST_PACKET sdrp;

    pdx = (PDEVICE_EXTENSION)deviceObject->DeviceExtension; 
    RtlZeroMemory(&sdrp, sizeof(SDBUS_REQUEST_PACKET));

    sdrp.RequestFunction = SDRF_GET_PROPERTY;
    sdrp.Parameters.GetSetProperty.Property = Property;
    sdrp.Parameters.GetSetProperty.Buffer = Buffer;
    sdrp.Parameters.GetSetProperty.Length = Length;

    return SdBusSubmitRequest(pdx->InterfaceStandard.Context, &sdrp);
}

NTSTATUS
SdioSetProperty(IN PDEVICE_OBJECT deviceObject,
				IN SDBUS_PROPERTY Property,
				IN PVOID Buffer,
				IN ULONG Length)
{
    PDEVICE_EXTENSION pdx;
    SDBUS_REQUEST_PACKET sdrp;

    pdx = (PDEVICE_EXTENSION)deviceObject->DeviceExtension; 

    RtlZeroMemory(&sdrp, sizeof(SDBUS_REQUEST_PACKET));

    sdrp.RequestFunction = SDRF_SET_PROPERTY;
    sdrp.Parameters.GetSetProperty.Property = Property;
    sdrp.Parameters.GetSetProperty.Buffer = Buffer;
    sdrp.Parameters.GetSetProperty.Length = Length;

    return SdBusSubmitRequest(pdx->InterfaceStandard.Context, &sdrp);
}

VOID
MyDriverCallback(
  IN PVOID  CallbackRoutineContext,
  IN ULONG  InterruptType
  )
{
	static ULONG i=0;

	NTSTATUS status;

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) CallbackRoutineContext;

	KdPrint(("Enter MyDriverCallback\n"));
	
	UCHAR ucRegVal = 0;
	status = SdioReadWriteByte(pdx->FunctionalDeviceObject,pdx->FunctionNumber,&ucRegVal,IFDevice_INT0_STATUS,FALSE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("SdioReadWriteByte fail:%x\n",status));
		goto done;
	}

    if(IFDevice_INT0_STATUS_MASK&ucRegVal)
    {
		KdPrint(("Enter MyDriverCallback %d\n",i++));
    }

done:
	pdx->InterfaceStandard.AcknowledgeInterrupt(pdx->InterfaceStandard.Context);
}

NTSTATUS
SdioClientDrv_AddDevice(
IN PDRIVER_OBJECT DriverObject,
IN PDEVICE_OBJECT PhysicalDeviceObject
)
{
	NTSTATUS ntStatus;
	PDEVICE_OBJECT deviceObject = NULL;
	PDEVICE_EXTENSION pdx = NULL;
	// POWER_STATE state;
	// KIRQL oldIrql;
	
	KdPrint(("Enter SdioClientDrv_AddDevice\n"));
	
	ntStatus = IoCreateDevice(
		DriverObject, // our driver object
		sizeof(DEVICE_EXTENSION), // extension size for us
		NULL, // name for this device
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN , // device characteristics
		FALSE, // Not exclusive
		&deviceObject); // Our device object
	
	if(!NT_SUCCESS(ntStatus))
	{
		KdPrint(("AddDevice:device obj not created\n"));
		return ntStatus;
	}
	KdPrint(("AddDevice: device obj created\n"));
	
	//
	// Initialize the device extension
	//
	
	ntStatus = STATUS_SUCCESS;
	
	// Initialize the device extension
	//
	pdx = (PDEVICE_EXTENSION) deviceObject->DeviceExtension;
	// Set all Device Extension pointers to NULL and all variable to zero
	//
	RtlZeroMemory(pdx, sizeof(DEVICE_EXTENSION));
	pdx->FunctionalDeviceObject = deviceObject;
	pdx->PhysicalDeviceObject = PhysicalDeviceObject;
	deviceObject->Flags |= DO_DIRECT_IO;
	//
	//initialize OpenHandleCount
	//
	pdx->OpenHandleCount = 0;
	
	//
	// Initialize the selective suspend variables
	//
	KeInitializeSpinLock(&pdx->IdleReqStateLock);
	pdx->IdleReqPend = 0;
	pdx->PendingIdleIrp = NULL;
	
	//
	// set the flags as underlying PDO
	//
	
	if(PhysicalDeviceObject->Flags & DO_POWER_PAGABLE) 
	{
		deviceObject->Flags |= DO_POWER_PAGABLE;
	}
	
	//
	// attach our driver to device stack
	// The return value of IoAttachDeviceToDeviceStack is the top of the
	// attachment chain. This is where all the IRPs should be routed.
	//
	
	pdx->TopOfStackDeviceObject = 
		IoAttachDeviceToDeviceStack(deviceObject,PhysicalDeviceObject);
	
	if(NULL == pdx->TopOfStackDeviceObject) 
	{
		KdPrint(("AddDevice: IoAttachDeviceToDeviceStack failed"));
		IoDeleteDevice(deviceObject);
		return STATUS_NO_SUCH_DEVICE;
	}
	
	ntStatus = IoRegisterDeviceInterface(
		PhysicalDeviceObject,
		(LPGUID) &GUID_DEVINTERFACE_SDIO_DEVICE,
		NULL,
		&pdx->InterfaceName);

	if (!NT_SUCCESS (ntStatus)) 
	{
		KdPrint(("AddDevice: IoRegisterDeviceInterface failed %x", ntStatus));
		IoDetachDevice(pdx->TopOfStackDeviceObject);
		IoDeleteDevice(deviceObject);
		return ntStatus;
	}

	KdPrint(("%wZ\n",&pdx->InterfaceName));
	IoSetDeviceInterfaceState(&pdx->InterfaceName, TRUE);
	
	//Open the SD Bus Interface
	//Lib routine returns ptr to the
	ntStatus = SdBusOpenInterface (
		pdx->PhysicalDeviceObject,
		&pdx->InterfaceStandard,
		sizeof(SDBUS_INTERFACE_STANDARD),
		SDBUS_INTERFACE_VERSION);
	
	KdPrint(("AddDevive: SDBusOpenInterface-ntStatus 0x%x",ntStatus));
	
	if (NT_SUCCESS(ntStatus)) 
	{
		SDBUS_INTERFACE_PARAMETERS interfaceParameters = {0};
		KdPrint(("Sd Bus initialization success\n"));
		interfaceParameters.Size = sizeof(SDBUS_INTERFACE_PARAMETERS);
		interfaceParameters.TargetObject = pdx->TopOfStackDeviceObject;
		interfaceParameters.DeviceGeneratesInterrupts = TRUE;
		interfaceParameters.CallbackAtDpcLevel = FALSE;
		interfaceParameters.CallbackRoutine  = MyDriverCallback;
		interfaceParameters.CallbackRoutineContext = pdx;

		ntStatus = STATUS_UNSUCCESSFUL;
		if (pdx->InterfaceStandard.InitializeInterface) 
		{
			ntStatus = (pdx->InterfaceStandard.InitializeInterface)
				(pdx->InterfaceStandard.Context, &interfaceParameters);

			if (NT_SUCCESS(ntStatus)) 
			{
				KdPrint(("Execute InterfaceStandard.InitializeInterface\n"));
			}
		}
	}
	
 	ntStatus = SdioGetProperty(deviceObject,
		SDP_FUNCTION_NUMBER,
		&pdx->FunctionNumber,
		sizeof(pdx->FunctionNumber));
	if (!NT_SUCCESS(ntStatus)) 
	{
		return ntStatus;
	}
	KdPrint(("FunctionNumber:%d\n",pdx->FunctionNumber));
	
    pdx->DriverVersion = SDBUS_DRIVER_VERSION_1;

    SdioGetProperty(deviceObject,
                    SDP_BUS_DRIVER_VERSION,
                    &pdx->DriverVersion,
                    sizeof(pdx->DriverVersion));

    pdx->BlockMode = 0;
	
	KdPrint(("DriverVersion:%x\n",pdx->DriverVersion));
	
	if (!NT_SUCCESS(ntStatus)) 
	{
		return ntStatus;
	}

	// Clear the DO_DEVICE_INITIALIZING flag.
	deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	
	ntStatus = STATUS_SUCCESS;
	KdPrint(("Leave SdioClientDrv_AddDevice\n"));
	return ntStatus;
}

void
SdioClientDrv_DriverUnload(
IN PDRIVER_OBJECT DriverObject
)
{
	KdPrint(("SdioClientDrv_DriverUnload\n"));
}

NTSTATUS HelloWDMDeviceIOControl(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	KdPrint(("Enter HelloWDMDeviceIOControl\n"));

	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) pDevObj->DeviceExtension;

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
		case IOCTL_ENABLE_INT:
		{
			KdPrint(("IOCTL_ENABLE_INT\n"));
			status = EnableInt(pdx);
			break;
		}
		case IOCTL_DISABLE_INT:
		{
			KdPrint(("IOCTL_DISABLE_INT\n"));
			status = DisableInt(pdx);
			break;
		}
		default:
			status = STATUS_INVALID_VARIANT;
	}

	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = info;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );

	KdPrint(("Leave HelloWDMDeviceIOControl\n"));

	return status;
}
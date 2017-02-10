#include "stdafx.h"
#include "main.h"

__drv_dispatchType(IRP_MJ_CREATE)
__drv_dispatchType(IRP_MJ_CLOSE)
__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
__drv_dispatchType(IRP_MJ_INTERNAL_DEVICE_CONTROL)
__drv_functionClass(DRIVER_DISPATCH)
__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
NTSTATUS 
DrvIoDispatch(
    __in PDEVICE_OBJECT DeviceObject,
    __inout PIRP Irp
    );

#ifdef ALLOC_PRAGMA
    #pragma alloc_text (INIT, DriverEntry)
    #pragma alloc_text (PAGE, LeakDriverUnload)
    #pragma alloc_text (PAGE, LeakRoutine)
#endif

//
// Global driver and control device objects
//
PDRIVER_OBJECT LeakDrvDriverObject = NULL;
PDEVICE_OBJECT LeakDrvDeviceObject = NULL;

//
// Device names
//
UNICODE_STRING DrvNtDeviceName = RTL_CONSTANT_STRING(L"\\Device\\" DRV_CONTROL_DEVICE_NAME);
UNICODE_STRING DrvWin32DeviceName = RTL_CONSTANT_STRING(L"\\??\\" DRV_CONTROL_DEVICE_NAME);

//
// DRIVER ENTRY POINT
//
__drv_functionClass(DRIVER_INITIALIZE)
__drv_sameIRQL
NTSTATUS
DriverEntry(
    __inout PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
{
    PDEVICE_OBJECT DeviceObject;
    NTSTATUS Status;

    PAGED_CODE();

    DRVPRINT("Starting up...\n");

    Status = STATUS_SUCCESS;

    DriverObject->DriverUnload = LeakDriverUnload;
    LeakDrvDriverObject = DriverObject;

    //
    // IO routines
    //
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DrvIoDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DrvIoDispatch;

    //
    // User/kernel mode IO requests
    //
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DrvIoDispatch;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = DrvIoDispatch;

    //
    // Create Device Object
    //
    Status = IoCreateDevice(DriverObject,
        0,
        &DrvNtDeviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &DeviceObject);

    if (NT_SUCCESS(Status)) {
        Status = IoCreateSymbolicLink(
            &DrvWin32DeviceName, &DrvNtDeviceName);

        if (!NT_SUCCESS(Status)) {
            IoDeleteDevice(DeviceObject);
        } else {
            LeakDrvDeviceObject = DeviceObject;
        }
    }

    DRVPRINT("Started.\n");

    return Status;
    UNREFERENCED_PARAMETER(RegistryPath);
}

__drv_functionClass(DRIVER_UNLOAD)
__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
VOID
LeakDriverUnload(
   __in PDRIVER_OBJECT DriverObject
   )
{
    PAGED_CODE();

    NT_ASSERT(LeakDrvDriverObject == DriverObject);

    DRVPRINT("Unloading...\n");

    if (LeakDrvDeviceObject != NULL) {
        IoDeleteSymbolicLink(&DrvWin32DeviceName);
        IoDeleteDevice(LeakDrvDeviceObject);
    }

    LeakDrvDriverObject = NULL;
    LeakDrvDeviceObject = NULL;

    DRVPRINT("Unloaded.\n");

    return;
    UNREFERENCED_PARAMETER(DriverObject);
}

__drv_dispatchType(IRP_MJ_CREATE)
__drv_dispatchType(IRP_MJ_CLOSE)
__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
__drv_dispatchType(IRP_MJ_INTERNAL_DEVICE_CONTROL)
__drv_functionClass(DRIVER_DISPATCH)
__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
NTSTATUS 
DrvIoDispatch(
    __in PDEVICE_OBJECT DeviceObject,
    __inout PIRP Irp
    )
{
    PIO_STACK_LOCATION IrpSp;

    ULONG InputLength;
    ULONG OutputLength;
    PVOID SystemBuffer;

    ULONG ControlCode;
    NTSTATUS Status;

    PAGED_CODE();

    NT_ASSERT(DeviceObject == LeakDrvDeviceObject);

    IrpSp = IoGetCurrentIrpStackLocation(Irp);

    //
    // No return data by default
    //
    Irp->IoStatus.Information = 0;
    Status = STATUS_INVALID_DEVICE_REQUEST;

    switch (IrpSp->MajorFunction) {

        case IRP_MJ_CLOSE:
        case IRP_MJ_CREATE:
            Status = STATUS_SUCCESS;
            break;

        case IRP_MJ_DEVICE_CONTROL:
        case IRP_MJ_INTERNAL_DEVICE_CONTROL:

            ControlCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;
            InputLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
            OutputLength = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;
            SystemBuffer = Irp->AssociatedIrp.SystemBuffer;

            switch (ControlCode) {
                case IOCTL_DRV_TEST_API:
                    //
                    // No output returned. Simply leak a memory buffer.
                    //
                    Status = LeakRoutine(DRV_LEAK_ALLOCATION_SIZE);
                    Irp->IoStatus.Information = 0;
                    break;
            }
            break;
    }

    Irp->IoStatus.Status = Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Status;
    UNREFERENCED_PARAMETER(DeviceObject);
}

NTSTATUS
LeakRoutine(
    __in ULONG Length
    )
{
    PVOID Buffer;

    PAGED_CODE();

    Buffer = ExAllocatePoolWithTag(PagedPool, Length, DRV_POOL_TAG);
    if (Buffer == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    DRVPRINT("Allocated %d bytes at 0x%x.\n", Length, Buffer);
    return STATUS_SUCCESS;
    //
    // BUG! Memory is Leaked here
    // ExFreePoolWithTag(Buffer, DRV_POOL_TAG);
    //
}

VOID
__cdecl
DrvDbgPrint(
    __in_z __drv_formatString(printf) PCSTR pszFormat, 
    ...
    )
{
    va_list argList;

    va_start(argList, pszFormat);

    vDbgPrintExWithPrefix(
        "Leak Test Driver: ",
        DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL,
        pszFormat, argList
        );
}

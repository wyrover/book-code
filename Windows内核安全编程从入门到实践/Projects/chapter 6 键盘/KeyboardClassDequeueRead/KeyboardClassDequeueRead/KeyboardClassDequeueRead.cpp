#include "stdafx.h"

void KeyboardClassDequeueReadUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS KeyboardClassDequeueReadCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS KeyboardClassDequeueReadDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    RtlInitUnicodeString(&DeviceName, L"\\Device\\KeyboardClassDequeueRead0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\KeyboardClassDequeueRead0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = KeyboardClassDequeueReadDefaultHandler;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = KeyboardClassDequeueReadCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = KeyboardClassDequeueReadCreateClose;
    DriverObject->DriverUnload = KeyboardClassDequeueReadUnload;
    status = IoCreateDevice(DriverObject,
                            0,
                            &DeviceName,
                            FILE_DEVICE_UNKNOWN,
                            0,
                            FALSE,
                            &DeviceObject);

    if (!NT_SUCCESS(status))
        return status;

    if (!DeviceObject)
        return STATUS_UNEXPECTED_IO_ERROR;

    DeviceObject->Flags |= DO_DIRECT_IO;
    DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
    status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    HookDequeueRead();
    return STATUS_SUCCESS;
}

void KeyboardClassDequeueReadUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    __asm {
        cli
        push   eax
        mov    eax, cr0
        and    eax, not 10000h
        mov    cr0, eax
        pop    eax
    }
    InterlockedExchange((LONG*)(uHookedPoint+1), (LONG)OffsetOrgDequeueRead);
    __asm {
        push   eax
        mov    eax, cr0
        or     eax, 10000h
        mov    cr0, eax
        pop    eax
        sti
    }
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\KeyboardClassDequeueRead0");
    IoDeleteSymbolicLink(&Win32Device);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS KeyboardClassDequeueReadCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS KeyboardClassDequeueReadDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

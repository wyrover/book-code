#include "ntddk.h"
#include <windef.h>
#include <stdlib.h>
#include "dbghelp.h"
#include "Win7x64Drv.h"

typedef UINT64(__fastcall *SCFN)(UINT64, UINT64, UINT64, UINT64);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString);
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
VOID DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

VOID test()
{
    SCFN scfn;
    UINT64 ret;
    UCHAR strShellCode[14] = "\x48\x03\xCA\x49\x03\xC8\x49\x03\xC9\x48\x8B\xC1\xC3";
    /*
    add rcx,rdx
    add rcx,r8
    add rcx,r9
    mov rax,rcx
    ret
    */
    scfn = ExAllocatePool(NonPagedPool, 14);
    memcpy(scfn, strShellCode, 14);
    ret = scfn(11, 22, 33, 44);
    DbgPrint("[x64Drv] Inline ASM return: %lld", ret);
    ExFreePool(scfn);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING ustrLinkName;
    UNICODE_STRING ustrDevName;
    PDEVICE_OBJECT pDevObj;
    dprintf("[x64Drv] DriverEntry: %S\n", pRegistryString->Buffer);
    //Create dispatch points for device control, create, close.
    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    pDriverObj->DriverUnload = DriverUnload;
    RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
    status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
    dprintf("[x64Drv] Device Name %S", ustrDevName.Buffer);

    if (!NT_SUCCESS(status)) {
        dprintf("[x64Drv] IoCreateDevice = 0x%x\n", status);
        return status;
    }

    RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

    if (!NT_SUCCESS(status)) {
        dprintf("[x64Drv] IoCreateSymbolicLink = 0x%x\n", status);
        IoDeleteDevice(pDevObj);
        return status;
    }

    dprintf("[x64Drv] SymbolicLink:%S", ustrLinkName.Buffer);
    //test Begin
    test();
    //test End
    return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
    RtlInitUnicodeString(&strLink, LINK_NAME);
    IoDeleteSymbolicLink(&strLink);
    IoDeleteDevice(pDriverObj->DeviceObject);
    dprintf("[x64Drv] Unloaded\n");
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    dprintf("[x64Drv] IRP_MJ_CREATE\n");
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    dprintf("[x64Drv] IRP_MJ_CLOSE\n");
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION pIrpStack;
    ULONG uIoControlCode;
    PVOID pIoBuffer;
    ULONG uInSize;
    ULONG uOutSize;
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
    pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    switch (uIoControlCode) {
    }

    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}
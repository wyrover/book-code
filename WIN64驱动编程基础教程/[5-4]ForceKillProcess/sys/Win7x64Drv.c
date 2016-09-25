#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>
#include "Win7x64Drv.h"

#define DIRECTORY_TABLE_BASE            0x028

typedef NTSTATUS(__fastcall *PSPTERMINATETHREADBYPOINTER)
(
    IN PETHREAD Thread,
    IN NTSTATUS ExitStatus,
    IN BOOLEAN DirectTerminate
);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString);
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
VOID DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(IN HANDLE ProcessId, OUT PEPROCESS *Process);
NTKERNELAPI NTSTATUS PsLookupThreadByThreadId(IN HANDLE ThreadId, OUT PETHREAD *Thread);
NTKERNELAPI NTSTATUS MmUnmapViewOfSection(IN PEPROCESS Process, IN ULONG BaseAddress);
NTKERNELAPI PEPROCESS IoThreadToProcess(IN PETHREAD Thread);

ULONG32 idTarget = 0;
PEPROCESS epTarget = NULL;
PSPTERMINATETHREADBYPOINTER PspTerminateThreadByPointer = NULL;

ULONG64 Get64bitValue(PVOID p)
{
    if (MmIsAddressValid(p) == FALSE)
        return 0;

    return *(PULONG64)p;
}

ULONG32 Get32bitValue(PVOID p)
{
    if (MmIsAddressValid(p) == FALSE)
        return 0;

    return *(PULONG32)p;
}

VOID Set64bitValue(PVOID p, ULONG64 v)
{
    if (MmIsAddressValid(p) == FALSE)
        return ;

    *(PULONG64)p = v;
}

VOID Set32bitValue(PVOID p, ULONG32 v)
{
    if (MmIsAddressValid(p) == FALSE)
        return ;

    *(PULONG32)p = v;
}

PVOID GetFunctionAddr(PCWSTR FunctionName)
{
    UNICODE_STRING UniCodeFunctionName;
    RtlInitUnicodeString(&UniCodeFunctionName, FunctionName);
    return MmGetSystemRoutineAddress(&UniCodeFunctionName);
}

NTSTATUS HwlTerminateProcess64(PEPROCESS Process)
{
    //get pspterminatethreadbypointer
    ULONG32 callcode = 0;
    ULONG64 AddressOfPspTTBP = 0, AddressOfPsTST = 0, i = 0;
    PETHREAD Thread = NULL;
    PEPROCESS tProcess = NULL;
    NTSTATUS status = 0;

    if (PspTerminateThreadByPointer == NULL) {
        AddressOfPsTST = (ULONG64)GetFunctionAddr(L"PsTerminateSystemThread");

        if (AddressOfPsTST == 0)
            return STATUS_UNSUCCESSFUL;

        for (i = 1; i < 0xff; i++) {
            if (MmIsAddressValid((PVOID)(AddressOfPsTST + i)) != FALSE) {
                if (*(BYTE *)(AddressOfPsTST + i) == 0x01 && *(BYTE *)(AddressOfPsTST + i + 1) == 0xe8) { //目标地址-原始地址-5=机器码 ==> 目标地址=机器码+5+原始地址
                    RtlMoveMemory(&callcode, (PVOID)(AddressOfPsTST + i + 2), 4);
                    AddressOfPspTTBP = (ULONG64)callcode + 5 + AddressOfPsTST + i + 1;
                }
            }
        }

        PspTerminateThreadByPointer = (PSPTERMINATETHREADBYPOINTER)AddressOfPspTTBP;
    }

    //loop call pspterminatethreadbypointer
    for (i = 4; i < 0x40000; i += 4) {
        status = PsLookupThreadByThreadId((HANDLE)i, &Thread);

        if (NT_SUCCESS(status)) {
            tProcess = IoThreadToProcess(Thread);

            if (tProcess == Process)
                PspTerminateThreadByPointer(Thread, 0, 1);

            ObDereferenceObject(Thread);
        }
    }

    //return status
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING ustrLinkName;
    UNICODE_STRING ustrDevName;
    PDEVICE_OBJECT pDevObj;
    DbgPrint("[x64Drv] DriverEntry: %S\n", pRegistryString->Buffer);
    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    pDriverObj->DriverUnload = DriverUnload;
    RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
    status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
    DbgPrint("[x64Drv] Device Name %S", ustrDevName.Buffer);

    if (!NT_SUCCESS(status)) {
        DbgPrint("[x64Drv] IoCreateDevice = 0x%x\n", status);
        return status;
    }

    RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

    if (!NT_SUCCESS(status)) {
        DbgPrint("[x64Drv] IoCreateSymbolicLink = 0x%x\n", status);
        IoDeleteDevice(pDevObj);
        return status;
    }

    DbgPrint("[x64Drv] SymbolicLink:%S", ustrLinkName.Buffer);
    return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
    RtlInitUnicodeString(&strLink, LINK_NAME);
    IoDeleteSymbolicLink(&strLink);
    IoDeleteDevice(pDriverObj->DeviceObject);
    DbgPrint("[x64Drv] Unloaded\n");
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    DbgPrint("[x64Drv] IRP_MJ_CREATE\n");
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    DbgPrint("[x64Drv] IRP_MJ_CLOSE\n");
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
    case IOCTL_PsKillProcess64: {
        __try {
            memcpy(&idTarget, pIoBuffer, sizeof(idTarget));
            DbgPrint("[x64Drv] PID: %ld", idTarget);
            status = PsLookupProcessByProcessId((HANDLE)idTarget, &epTarget);

            if (!NT_SUCCESS(status)) {
                DbgPrint("[x64Drv] Cannot get target! Status: %x; EPROCESS: %llx", status, (ULONG64)epTarget);
                break;
            } else {
                DbgPrint("[x64Drv] Get target OK! EPROCESS: %llx", (ULONG64)epTarget);
                HwlTerminateProcess64(epTarget);
                ObDereferenceObject(epTarget);
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            ;
        }

        break;
    }
    }

    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}
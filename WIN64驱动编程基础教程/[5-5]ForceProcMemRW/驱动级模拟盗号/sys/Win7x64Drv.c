#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>
#include "Win7x64Drv.h"

#define DIRECTORY_TABLE_BASE            0x028

typedef NTSTATUS(__fastcall *PSPTERMINATETHREADBYPOINTER)(IN PETHREAD Thread, IN NTSTATUS ExitStatus, IN BOOLEAN DirectTerminate);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString);
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
VOID DriverUnload(PDRIVER_OBJECT pDriverObj);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(IN HANDLE ProcessId, OUT PEPROCESS *Process);

UINT32 idTarget = 0;
PEPROCESS epTarget = NULL;
UINT32 idGame = 0;
PEPROCESS epGame = NULL;
UINT32 rw_len = 0;
UINT64 base_addr = 0;

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

NTSTATUS HwlPVASE64(PEPROCESS Process)
{
    ULONG64 pDTB = 0, OldCr3 = 0, vAddr = 0;
    //Get DTB
    pDTB = Get64bitValue((UCHAR*)Process + DIRECTORY_TABLE_BASE);

    if (pDTB == 0) {
        DbgPrint("[x64Drv] Can not get PDT");
        return STATUS_UNSUCCESSFUL;
    }

    //Record old cr3 and set new cr3
    _disable();
    OldCr3 = __readcr3();
    __writecr3(pDTB);
    _enable();

    //P.V.A.S.E
    for (vAddr = 0; vAddr <= 0x7fffffff; vAddr += 0x1000) {
        if (MmIsAddressValid((PVOID)vAddr)) {
            _try {
                ProbeForWrite((PVOID)vAddr, PAGE_SIZE, PAGE_SIZE);
                memset((PVOID)vAddr, 0x0, PAGE_SIZE);
            }
            _except(1) {
                continue;
            }
        }
    }

    //Restore old cr3
    _disable();
    __writecr3(OldCr3);
    _enable();
    //return status
    return STATUS_SUCCESS;
}

void KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, OUT PVOID Buffer)
{
    ULONG64 pDTB = 0, OldCr3 = 0, vAddr = 0;
    //Get DTB
    pDTB = Get64bitValue((UCHAR*)Process + DIRECTORY_TABLE_BASE);

    if (pDTB == 0) {
        DbgPrint("[x64Drv] Can not get PDT");
        return;
    }

    //Record old cr3 and set new cr3
    _disable();
    OldCr3 = __readcr3();
    __writecr3(pDTB);
    _enable();

    //Read process memory
    if (MmIsAddressValid(Address)) {
        RtlCopyMemory(Buffer, Address, Length);
        DbgPrint("[x64Drv] Date read: %ld", *(PDWORD)Buffer);
    }

    //Restore old cr3
    _disable();
    __writecr3(OldCr3);
    _enable();
}

void KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
{
    ULONG64 pDTB = 0, OldCr3 = 0, vAddr = 0;
    //Get DTB
    pDTB = Get64bitValue((UCHAR*)Process + DIRECTORY_TABLE_BASE);

    if (pDTB == 0) {
        DbgPrint("[x64Drv] Can not get PDT");
        return;
    }

    //Record old cr3 and set new cr3
    _disable();
    OldCr3 = __readcr3();
    __writecr3(pDTB);
    _enable();

    //Read process memory
    if (MmIsAddressValid(Address)) {
        RtlCopyMemory(Address, Buffer, Length);
        DbgPrint("[x64Drv] Date wrote.");
    }

    //Restore old cr3
    _disable();
    __writecr3(OldCr3);
    _enable();
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
    case IOCTL_MmKillProcess64: {
        __try {
            memcpy(&idTarget, pIoBuffer, sizeof(idTarget));
            DbgPrint("[x64Drv] PID: %ld", idTarget);
            status = PsLookupProcessByProcessId((HANDLE)idTarget, &epTarget);

            if (!NT_SUCCESS(status)) {
                DbgPrint("[x64Drv] Cannot get target! Status: %x; EPROCESS: %llx", status, (ULONG64)epTarget);
                break;
            } else {
                DbgPrint("[x64Drv] Get target OK! EPROCESS: %llx", (ULONG64)epTarget);
                HwlPVASE64(epTarget);
                ObDereferenceObject(epTarget);
            }
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            ;
        }

        break;
    }

    case IOCTL_InputProcessId: {
        memcpy(&idGame, pIoBuffer, sizeof(idGame));
        DbgPrint("[x64Drv] PID: %ld", idGame);
        status = PsLookupProcessByProcessId((HANDLE)idGame, &epGame);

        if (!NT_SUCCESS(status))
            DbgPrint("[x64Drv] Cannot get target! Status: %x; EPROCESS: %llx", status, (ULONG64)epGame);
        else
            DbgPrint("[x64Drv] Get target OK! EPROCESS: %llx", (ULONG64)epGame);

        break;
    }

    case IOCTL_InputBaseAddress: {
        memcpy(&base_addr, pIoBuffer, 8);
        DbgPrint("[x64Drv] Base address: %lld", base_addr);
        break;
    }

    case IOCTL_InputReadWriteLen: {
        memcpy(&rw_len, pIoBuffer, 4);
        DbgPrint("[x64Drv] Read/Write length: %ld", rw_len);
        break;
    }

    case IOCTL_KReadProcessMemory: { //OutputBuffer
        KReadProcessMemory(epGame, (PVOID)base_addr, rw_len, pIoBuffer);

        if (epGame != NULL)
            ObDereferenceObject(epGame);

        break;
    }

    case IOCTL_KWriteProcessMemory: { //InputBuffer
        KWriteProcessMemory(epGame, (PVOID)base_addr, rw_len, pIoBuffer);

        if (epGame != NULL)
            ObDereferenceObject(epGame);

        break;
    }
    }

    status = STATUS_SUCCESS;

    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}
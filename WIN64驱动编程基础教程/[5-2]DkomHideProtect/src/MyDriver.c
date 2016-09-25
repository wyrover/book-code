//驱动开发模板_Win64
//作者：Tesla.Angela(GDUT.HWL)

#include <ntddk.h>
#include "MyDriver.h"

NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);
NTKERNELAPI CHAR* PsGetProcessImageFileName(PEPROCESS Process);

//目标进程
PEPROCESS audiodg = NULL, dwm = NULL;
ULONG op_dat;

//偏移定义
#define PROCESS_ACTIVE_PROCESS_LINKS_OFFSET 0x188
#define PROCESS_FLAG_OFFSET                 0x440

//获得EPROCESS
PEPROCESS GetProcessObjectByName(char *name)
{
    SIZE_T i;

    for (i = 100; i < 20000; i += 4) {
        NTSTATUS st;
        PEPROCESS ep;
        st = PsLookupProcessByProcessId((HANDLE)i, &ep);

        if (NT_SUCCESS(st)) {
            char *pn = PsGetProcessImageFileName(ep);

            if (_stricmp(pn, name) == 0)
                return ep;
        }
    }

    return NULL;
}

//摘除双向链表的指定项
VOID RemoveListEntry(PLIST_ENTRY ListEntry)
{
    KIRQL OldIrql;
    OldIrql = KeRaiseIrqlToDpcLevel();

    if (ListEntry->Flink != ListEntry &&
        ListEntry->Blink != ListEntry &&
        ListEntry->Blink->Flink == ListEntry &&
        ListEntry->Flink->Blink == ListEntry) {
        ListEntry->Flink->Blink = ListEntry->Blink;
        ListEntry->Blink->Flink = ListEntry->Flink;
        ListEntry->Flink = ListEntry;
        ListEntry->Blink = ListEntry;
    }

    KeLowerIrql(OldIrql);
}
//隐藏进程
VOID HideProcess(PEPROCESS Process)
{
    RemoveListEntry((PLIST_ENTRY)((ULONG64)Process + PROCESS_ACTIVE_PROCESS_LINKS_OFFSET));
}

//保护进程
ULONG ProtectProcess(PEPROCESS Process, BOOLEAN bIsProtect, ULONG v)
{
    ULONG op;

    if (bIsProtect) {
        op = *(PULONG)((ULONG64)Process + PROCESS_FLAG_OFFSET);
        *(PULONG)((ULONG64)Process + PROCESS_FLAG_OFFSET) = 0;
        return op;
    } else {
        *(PULONG)((ULONG64)Process + PROCESS_FLAG_OFFSET) = v;
        return 0;
    }
}

VOID test()
{
    audiodg = GetProcessObjectByName("audiodg.exe");
    DbgPrint("audiodg: %p\n", audiodg);

    if (audiodg) {
        op_dat = ProtectProcess(audiodg, 1, 0);
        ObDereferenceObject(audiodg);
    }

    dwm = GetProcessObjectByName("dwm.exe");
    DbgPrint("dwm: %p\n", dwm);

    if (dwm) {
        HideProcess(dwm);
        ObDereferenceObject(dwm);
    }
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;

    //unprotect or BSOD
    if (audiodg && MmIsAddressValid(audiodg)) {
        ProtectProcess(audiodg, 0, op_dat);
    }

    RtlInitUnicodeString(&strLink, LINK_NAME);
    IoDeleteSymbolicLink(&strLink);
    IoDeleteDevice(pDriverObj->DeviceObject);
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
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
        ;
    }

    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING ustrLinkName;
    UNICODE_STRING ustrDevName;
    PDEVICE_OBJECT pDevObj;
    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    pDriverObj->DriverUnload = DriverUnload;
    RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
    status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);

    if (!NT_SUCCESS(status)) return status;

    if (IoIsWdmVersionAvailable(1, 0x10))
        RtlInitUnicodeString(&ustrLinkName, LINK_GLOBAL_NAME);
    else
        RtlInitUnicodeString(&ustrLinkName, LINK_NAME);

    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(pDevObj);
        return status;
    }

    //test hide protect process
    test();
    return STATUS_SUCCESS;
}
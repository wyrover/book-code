//驱动开发模板_Win64
//作者：Tesla.Angela(GDUT.HWL)

#include <ntddk.h>
#include "MyDriver.h"
#include "rwkm.h"

NTKERNELAPI PUCHAR PsGetProcessImageFileName(IN PEPROCESS Process);
NTKERNELAPI PEPROCESS IoThreadToProcess(IN PETHREAD Thread);

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
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
    case IOCTL_SET_RWKM_ADR: {
        rwkm_adr = *(UINT64 *)pIoBuffer;
        DbgPrint("rwkm_adr: %lld", rwkm_adr);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_SET_RWKM_LEN: {
        rwkm_len = *(UINT64 *)pIoBuffer;
        DbgPrint("rwkm_len: %lld", rwkm_len);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_READ_KRNL_MM: {
        VxkCopyMemory(pIoBuffer, (PVOID)rwkm_adr, (SIZE_T)rwkm_len);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_MODIFY_KN_MM: {
        VxkCopyMemory((PVOID)rwkm_adr, pIoBuffer, (SIZE_T)rwkm_len);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GET_PN_BY_ET: {
        PETHREAD et;
        PEPROCESS ep;
        PUCHAR pn;
        et = *(PETHREAD *)pIoBuffer;
        ep = IoThreadToProcess(et);
        pn = PsGetProcessImageFileName(ep);
        memcpy(pIoBuffer, pn, 16);
        status = STATUS_SUCCESS;
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

    return STATUS_SUCCESS;
}
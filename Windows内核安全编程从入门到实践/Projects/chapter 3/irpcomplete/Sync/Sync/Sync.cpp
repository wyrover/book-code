#include "stdafx.h"

void SyncUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS SyncCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS SyncDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS SyncRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    RtlInitUnicodeString(&DeviceName, L"\\Device\\Sync0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Sync0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = SyncDefaultHandler;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = SyncCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = SyncCreateClose;
    DriverObject->MajorFunction[IRP_MJ_READ] = SyncRead;
    DriverObject->DriverUnload = SyncUnload;
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
    return STATUS_SUCCESS;
}

void SyncUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Sync0");
    IoDeleteSymbolicLink(&Win32Device);
    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS SyncCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS SyncDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

NTSTATUS SyncRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    PVOID pBuf = NULL;
    ULONG uBuf = 0;
    KdPrint(("[SyncRead]\n"));

    do {
        //读写请求使用的是直接I/O方式
        pBuf = MmGetSystemAddressForMdl(Irp->MdlAddress);

        if (NULL == pBuf) {
            status = STATUS_UNSUCCESSFUL;
            break;
        }

        uBuf = pSP->Parameters.Read.Length;
        KdPrint(("Read Len: %d\n", uBuf));
        //最大支持5字节读请求
        uBuf = uBuf >= 5 ? 5 : uBuf;
        //简单地向读请求缓冲区中写入"hello"
        RtlCopyMemory(pBuf, "hello", uBuf);
    } while (FALSE);

    //填写返回状态以及返回大小
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = uBuf;
    //完成该IRP
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

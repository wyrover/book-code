//
//Description:
//  本程序用于展示回调对象的简单使用方法。在XP SP3平台上测试通过。
//  使用vs2008+visualDDK+WDK7100编译
//
//Writen By: 宁妖 2011/4/17
//
//Last Write Time: 2011/4/18
//
#include "stdafx.h"

PCALLBACK_OBJECT pCallback = NULL;

void CallbackUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS CallbackCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS CallbackDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    UNICODE_STRING DeviceName, Win32Device;
    PDEVICE_OBJECT DeviceObject = NULL;
    NTSTATUS status;
    unsigned i;
    UNICODE_STRING DemoCallback;
    OBJECT_ATTRIBUTES  oa;
    RtlInitUnicodeString(&DeviceName, L"\\Device\\Callback0");
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Callback0");

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = CallbackDefaultHandler;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = CallbackCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CallbackCreateClose;
    DriverObject->DriverUnload = CallbackUnload;
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
    status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    //
    //创建一个回调对象
    //  注意回调对象都是放在\Callback目录中
    //
    RtlInitUnicodeString(&DemoCallback, L"\\Callback\\DemoCallback");
    //注意一定要指定OBJ_PERMANENT，否则ExCreateCallback无法成功创建一个带有名字的回调函数
    InitializeObjectAttributes(&oa, &DemoCallback, OBJ_CASE_INSENSITIVE | OBJ_PERMANENT , NULL, NULL);
    status = ExCreateCallback(&pCallback, &oa, TRUE, TRUE);

    if (!NT_SUCCESS(status))
        KdPrint(("Callback ExCreateCallback Failure!status: 0x%08x\n", status));

    KdPrint(("Callback ——pCallback: 0x%08x\n", pCallback));
    return STATUS_SUCCESS;
}

void CallbackUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING Win32Device;
    RtlInitUnicodeString(&Win32Device, L"\\DosDevices\\Callback0");
    IoDeleteSymbolicLink(&Win32Device);
    IoDeleteDevice(DriverObject->DeviceObject);

    //创建的回调对象需要调用ObDereferenceObject减少一次引用次数（引用次数为0则删除）。
    if (pCallback)
        ObDereferenceObject(pCallback);
}

//
//当有程序打开或者关闭本驱动的时候，将通知所有向回调对象注册的函数。
//
NTSTATUS CallbackCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
    char* pImageName = NULL;

    if (pCallback) {
        //获取进程名,使用硬编码：0x174
        pImageName = (char*)((ULONG)PsGetCurrentProcess() + 0x174);
        //
        //通知注册函数
        //arg1:主功能码（IRP_MJ_READ或者IRP_MJ_WRITE）
        //arg2:当前进程的进程名
        //
        ExNotifyCallback(pCallback, (PVOID)pSP->MajorFunction, pImageName);
    }

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS CallbackDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

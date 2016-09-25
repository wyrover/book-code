/*
    WIN64驱动开发模板
    作者：Tesla.Angela
*/

//【0】包含的头文件，可以加入系统或自己定义的头文件
#include <ntddk.h>
#include <windef.h>
#include <stdlib.h>

//【1】定义符号链接，一般来说修改为驱动的名字即可
#define DEVICE_NAME         L"\\Device\\KrnlHW64"
#define LINK_NAME           L"\\DosDevices\\KrnlHW64"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\KrnlHW64"

//【2】定义驱动功能号和名字，提供接口给应用程序调用
#define IOCTL_IO_TEST       CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SAY_HELLO     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

//【3】驱动卸载的处理例程
VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
    DbgPrint("[KrnlHW64]DriverUnload\n");
    //删除符号连接和设备
    RtlInitUnicodeString(&strLink, LINK_NAME);
    IoDeleteSymbolicLink(&strLink);
    IoDeleteDevice(pDriverObj->DeviceObject);
}

//【4】IRP_MJ_CREATE对应的处理例程，一般不用管它
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    DbgPrint("[KrnlHW64]DispatchCreate\n");
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

//【5】IRP_MJ_CLOSE对应的处理例程，一般不用管它
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    DbgPrint("[KrnlHW64]DispatchClose\n");
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

//【6】IRP_MJ_DEVICE_CONTROL对应的处理例程，驱动最重要的函数之一，一般走正常途径调用驱动功能的程序，都会经过这个函数
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION pIrpStack;
    ULONG uIoControlCode;
    PVOID pIoBuffer;
    ULONG uInSize;
    ULONG uOutSize;
    DbgPrint("[KrnlHW64]DispatchIoctl\n");
    //获得IRP里的关键数据
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    //这个就是传说中的控制码
    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
    //输入和输出的缓冲区（DeviceIoControl的InBuffer和OutBuffer都是它）
    pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
    //EXE发送传入数据的BUFFER长度（DeviceIoControl的nInBufferSize）
    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
    //EXE接收传出数据的BUFFER长度（DeviceIoControl的nOutBufferSize）
    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    switch (uIoControlCode) {
    //在这里加入接口
    case IOCTL_IO_TEST: {
        DWORD dw = 0;
        //输入
        memcpy(&dw, pIoBuffer, sizeof(DWORD));
        //使用
        dw++;
        //输出
        memcpy(pIoBuffer, &dw, sizeof(DWORD));
        //返回通信状态
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_SAY_HELLO: {
        DbgPrint("[KrnlHW64]IOCTL_SAY_HELLO\n");
        status = STATUS_SUCCESS;
        break;
    }
    }

    //这里设定DeviceIoControl的*lpBytesReturned的值（如果通信失败则返回0长度）
    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    //这里设定DeviceIoControl的返回值是成功还是失败
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}

//【7】驱动加载的处理例程，里面进行了驱动的初始化工作
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING ustrLinkName;
    UNICODE_STRING ustrDevName;
    PDEVICE_OBJECT pDevObj;
    //设置分发函数和卸载例程
    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    pDriverObj->DriverUnload = DriverUnload;
    //创建一个设备
    RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
    status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);

    if (!NT_SUCCESS(status)) return status;

    //判断支持的WDM版本，其实这个已经不需要了，纯属WIN9X和WINNT并存时代的残留物
    if (IoIsWdmVersionAvailable(1, 0x10))
        RtlInitUnicodeString(&ustrLinkName, LINK_GLOBAL_NAME);
    else
        RtlInitUnicodeString(&ustrLinkName, LINK_NAME);

    //创建符号连接
    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(pDevObj);
        return status;
    }

    DbgPrint("[KrnlHW64]DriverEntry\n");
    //返回加载驱动的状态（如果返回失败，驱动讲被清除出内核空间）
    return STATUS_SUCCESS;
}
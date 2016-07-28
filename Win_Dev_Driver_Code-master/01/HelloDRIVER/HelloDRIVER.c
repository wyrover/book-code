/* 
******************************************************************************* 
*= = 文件名称：HelloDRIVER.c 
*= = 文件描述：驱动程序HelloDRIVER例子 
*= = 作    者：竹林蹊径 
*= = 编写时间：2009-04-23 21:16:00 
******************************************************************************* 
*/ 

#include "HelloDRIVER.h" 

//*============================================================================ 
//*= = 预处理定义 
//*============================================================================ 

#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DefaultDispatch)
#pragma alloc_text(PAGE, DriverUnload)

//*============================================================================
//*= = 函数名称：DriverEntry
//*= = 功能描述：驱动程序入口函数 
//*= = 入口参数：PDRIVER_OBJECT, PUNICODE_STRING 
//*= = 出口参数：NTSTATUS
//*============================================================================

NTSTATUS
DriverEntry (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
{
    NTSTATUS status;
    PDEVICE_OBJECT deviceObject;
    PDEVICE_EXTENSION deviceExtension;
    UNICODE_STRING symbolicLink;
    UNICODE_STRING deviceName;
    ULONG i;
    KdPrint(("Enter HelloDRIVER DriverEntry!\n"));
    
    UNREFERENCED_PARAMETER(RegistryPath); 
    
    RtlInitUnicodeString(&deviceName, L"\\Device\\HelloDRIVER");

    // 处理派遣例程 
    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
        DriverObject->MajorFunction[i] = DefaultDispatch; 
    }
    
    DriverObject->DriverUnload = DriverUnload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DefaultDispatch; 
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DefaultDispatch; 
    DriverObject->MajorFunction[IRP_MJ_READ] = DefaultDispatch; 
    DriverObject->MajorFunction[IRP_MJ_WRITE] = DefaultDispatch; 
    
    // 创建设备 
    status = IoCreateDevice( DriverObject,
                             sizeof(DEVICE_EXTENSION),
                             &deviceName,
                             FILE_DEVICE_UNKNOWN,
                             0,
                             TRUE,
                             &deviceObject);
    if(!NT_SUCCESS(status))
    {
        return status;
    }
    
    deviceObject->Flags = DO_BUFFERED_IO;
    deviceExtension = (PDEVICE_EXTENSION)deviceObject->DeviceExtension;
    deviceExtension->DeviceObject = deviceObject;
    deviceExtension->DeviceName = deviceName;
    
    RtlInitUnicodeString(&symbolicLink, L"\\??\\HelloDRIVER");
    deviceExtension->SymbolicLink = symbolicLink;
    
    // 创建符号链接 
    status = IoCreateSymbolicLink(&symbolicLink, &deviceName);
    
    if(!NT_SUCCESS(status))
    {
        IoDeleteDevice(deviceObject);
        return status;
    }
    
    KdPrint(("End of HelloDRIVER DriverEntry!\n")); 
    return status;
}

//*============================================================================
//*= = 函数名称：DriverUnload 
//*= = 功能描述：驱动程序卸载函数 
//*= = 入口参数：PDRIVER_OBJECT 
//*= = 出口参数：VOID 
//*============================================================================

VOID
DriverUnload (
    __in PDRIVER_OBJECT DriverObject
    )
{
    PDEVICE_OBJECT deviceObject;
    UNICODE_STRING linkName;
    KdPrint(("Enter HelloDRIVER DriverUnload!\n"));
    
    deviceObject = DriverObject->DeviceObject;
    
    while(NULL != deviceObject)
    {
        PDEVICE_EXTENSION deviceExtesion = 
                   (PDEVICE_EXTENSION)deviceObject->DeviceExtension;
        
        // 删除符号链接与设备
        linkName = deviceExtesion->SymbolicLink;
        IoDeleteSymbolicLink(&linkName);
        deviceObject = deviceObject->NextDevice;
        IoDeleteDevice(deviceExtesion->DeviceObject);
    }
    
    KdPrint(("End of HelloDRIVER DriverUnload!\n"));
}

//*============================================================================
//*= = 函数名称：DefaultDispatch 
//*= = 功能描述：驱动程序默认派遣例程 
//*= = 入口参数：PDEVICE_OBJECT, PIRP 
//*= = 出口参数：NTSTATUS
//*============================================================================

NTSTATUS
DefaultDispatch (
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
    )
{
    NTSTATUS status;
    KdPrint(("Enter HelloDRIVER DefaultDispatch!\n"));
    
    UNREFERENCED_PARAMETER(DeviceObject);
    status = STATUS_SUCCESS;
    
    // 完成IRP请求 
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
    KdPrint(("End of HelloDRIVER DefaultDispatch!\n"));
    return status;
}

//*============================================================================
//*= = 文件结束 
//*============================================================================

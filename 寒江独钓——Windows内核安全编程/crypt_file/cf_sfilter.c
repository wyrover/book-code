///
/// @file         cf_sfilter.c
/// @author    crazy_chu
/// @date       2009-1-29
/// @brief      实现sfilter的回调函数。 
/// 
/// 免责声明
/// 本代码为示例代码。未经详尽测试，不保证可靠性。作者对
/// 任何人使用此代码导致的直接和间接损失不负责任。
/// 
/// 授权协议
/// 本代码从属于工程crypt_file.是楚狂人与wowocock为《寒江独
/// 钓——Windows内核编程与信息安全》所编写的文件透明加密
/// 示例。本工程仅仅支持WindowsXP下，FastFat文件系统下记事
/// 本的加密。未测试与杀毒软件或者其他文件过滤驱动并存的
/// 情况。本代码全部权利为作者保留，仅供读者学习和阅读使
/// 用。未经两位作者书面授权，不得直接复制、或者基于此代
/// 码进行修改、利用此代码提供的全部或者部分技术用于商业
/// 的软件开发、或者其他的获利行为。如有违反，作者保留起
/// 诉和获取赔偿之权力。阅读此代码，则自动视为接受以上授
/// 权协议。如不接受此协议者，请不要阅读此代码。
///

#include <ntifs.h>
#include "..\inc\sfilter\sfilter.h"
#include "cf_proc.h"
#include "cf_list.h"
#include "cf_modify_irp.h"
#include "cf_create.h"
#include "fat_headers/fat.h"
#include "fat_headers/nodetype.h"
#include "fat_headers/fatstruc.h"

#define CF_FILE_HEADER_SIZE (1024*4)

SF_RET OnSfilterIrpPre(
		IN PDEVICE_OBJECT dev,
		IN PDEVICE_OBJECT next_dev,
		IN PVOID extension,
		IN PIRP irp,
		OUT NTSTATUS *status,
		PVOID *context)
{
    // 获得当前调用栈
		PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);
    PFILE_OBJECT file = irpsp->FileObject;
    // 看当前进程是否是加密进程
    BOOLEAN proc_sec = cfIsCurProcSec();
    BOOLEAN file_sec;

	// 我仅仅过滤文件请求。 FileObject不存在的情况一律passthru.
	if(file == NULL)
		return SF_IRP_PASS;


	// 首要决定哪些请求是我们必须过滤的。多余的提前passthru掉。
	if( irpsp->MajorFunction != IRP_MJ_CREATE &&
		irpsp->MajorFunction != IRP_MJ_CLOSE &&
		irpsp->MajorFunction != IRP_MJ_READ &&
		irpsp->MajorFunction != IRP_MJ_WRITE &&
		irpsp->MajorFunction != IRP_MJ_CLOSE  &&
		irpsp->MajorFunction != IRP_MJ_CLEANUP &&
		irpsp->MajorFunction != IRP_MJ_SET_INFORMATION &&
		irpsp->MajorFunction != IRP_MJ_DIRECTORY_CONTROL &&
		irpsp->MajorFunction != IRP_MJ_QUERY_INFORMATION)
		return SF_IRP_PASS;

    if(!cfListInited())
        return SF_IRP_PASS;


    // 对于文件打开，用cfIrpCreatePre统一处理。
    if(irpsp->MajorFunction == IRP_MJ_CREATE)
    {
        if(proc_sec)
            return cfIrpCreatePre(irp,irpsp,file,next_dev);
        else
        {
            // 其他的情况，作为普通进程，不允许打开一个正在加
            // 密的文件。但是在这里无法判断这个文件是否正在加
            // 密，所以返回GO_ON来判断。
            return SF_IRP_GO_ON;
        }
    }

    cfListLock();
    file_sec = cfIsFileCrypting(file);
    cfListUnlock();

    // 如果不是加密的文件的话，就可以直接passthru了，没有别
    // 的事情了。
    if(!file_sec)
        return SF_IRP_PASS;

    // 如果是close就可以删除节点了 
    if(irpsp->MajorFunction == IRP_MJ_CLOSE)
        return SF_IRP_GO_ON;

 	// 操作上有偏移。以下三种请求必须特殊处理。进行GO_ON
    // 处理。其他的set information操作不需要处理。
	// 1.SET FILE_ALLOCATION_INFORMATION
	// 2.SET FILE_END_OF_FILE_INFORMATION
	// 3.SET FILE_VALID_DATA_LENGTH_INFORMATION
    if(irpsp->MajorFunction == IRP_MJ_SET_INFORMATION &&
		(irpsp->Parameters.SetFile.FileInformationClass == FileAllocationInformation ||
		 irpsp->Parameters.SetFile.FileInformationClass == FileEndOfFileInformation ||
		 irpsp->Parameters.SetFile.FileInformationClass == FileValidDataLengthInformation ||
		 irpsp->Parameters.SetFile.FileInformationClass == FileStandardInformation ||
		 irpsp->Parameters.SetFile.FileInformationClass == FileAllInformation ||
		 irpsp->Parameters.SetFile.FileInformationClass == FilePositionInformation))
    {
        // 对这些set information给予修改，使之隐去前面的4k文件头。
        cfIrpSetInforPre(irp,irpsp/*,next_dev,file*/);
		return SF_IRP_PASS;
    }

    if(irpsp->MajorFunction == IRP_MJ_QUERY_INFORMATION)
    {
        // 要对这些read information的结果给予修改。所以返回go on.
        // 结束后会调用cfIrpQueryInforPost(irp,irpsp);
        if(irpsp->Parameters.QueryFile.FileInformationClass == FileAllInformation ||
         irpsp->Parameters.QueryFile.FileInformationClass == FileAllocationInformation ||
		 irpsp->Parameters.QueryFile.FileInformationClass == FileEndOfFileInformation ||
         irpsp->Parameters.QueryFile.FileInformationClass == FileStandardInformation ||
		 irpsp->Parameters.QueryFile.FileInformationClass == FilePositionInformation ||
         irpsp->Parameters.QueryFile.FileInformationClass == FileValidDataLengthInformation)
            return SF_IRP_GO_ON;
        else
        {
            // KdPrint(("OnSfilterIrpPre: %x\r\n",irpsp->Parameters.QueryFile.FileInformationClass));
            return SF_IRP_PASS;
        }
    }

	// 暂时不处理。
	//if(irpsp->MajorFunction == IRP_MJ_DIRECTORY_CONTROL)
	//{
	//	// 要对这些read information的结果给予修改。所以返回go on.
	//	// 结束后会调用cfIrpQueryInforPost(irp,irpsp);
	//	if(irpsp->Parameters.QueryDirectory.FileInformationClass == FileDirectoryInformation ||
	//		irpsp->Parameters.QueryDirectory.FileInformationClass == FileFullDirectoryInformation ||
	//		irpsp->Parameters.QueryDirectory.FileInformationClass == FileBothDirectoryInformation)
	//		return SF_IRP_GO_ON;
	//	else
	//	{
    //           KdPrint(("OnSfilterIrpPre: Query information: %x passthru.\r\n",
    //               irpsp->Parameters.QueryDirectory.FileInformationClass));
	//		return SF_IRP_PASS;
	//	}
	//}

    // 最后两种是read和write，这两种都要修改请求后再下传。同时，read要有完成
    // 处理。请注意：只处理直接读硬盘的请求。对缓冲文件请求不处理。
    if(irpsp->MajorFunction == IRP_MJ_READ &&
       (irp->Flags & (IRP_PAGING_IO|IRP_SYNCHRONOUS_PAGING_IO|IRP_NOCACHE)))
    {
        cfIrpReadPre(irp,irpsp);
        return SF_IRP_GO_ON;
    }
    if(irpsp->MajorFunction == IRP_MJ_WRITE &&
       (irp->Flags & (IRP_PAGING_IO|IRP_SYNCHRONOUS_PAGING_IO|IRP_NOCACHE)))
    {
        if(cfIrpWritePre(irp,irpsp,context))
            return SF_IRP_GO_ON;
        else
        {
            IoCompleteRequest(irp, IO_NO_INCREMENT);
            return SF_IRP_COMPLETED;
        }
    }

    // 不加任何处理，直接返回。
    return SF_IRP_PASS;
}

VOID OnSfilterIrpPost(
		IN PDEVICE_OBJECT dev,
		IN PDEVICE_OBJECT next_dev,
		IN PVOID extension,
		IN PIRP irp,
		IN NTSTATUS status,
		PVOID context)
{
    // 获得当前调用栈
		PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);
    BOOLEAN crypting,sec_proc,need_crypt,need_write_header;
    PFILE_OBJECT file = irpsp->FileObject;
    ULONG desired_access;
    BOOLEAN proc_sec = cfIsCurProcSec();

    // 当前进程是否是加密进程
    sec_proc = cfIsCurProcSec();

    // 如果操作不成功，就没有必要处理。
    if( !NT_SUCCESS(status) &&
        !(irpsp->MajorFunction == IRP_MJ_QUERY_INFORMATION &&
            irpsp->Parameters.QueryFile.FileInformationClass == FileAllInformation &&
            irp->IoStatus.Information > 0) &&
        irpsp->MajorFunction != IRP_MJ_WRITE)
    {
        if(irpsp->MajorFunction == IRP_MJ_READ)
        {
            KdPrint(("OnSfilterIrpPost: IRP_MJ_READ failed. status = %x information = %x\r\n",
                status,irp->IoStatus.Information));
        }
        else if(irpsp->MajorFunction == IRP_MJ_WRITE)
        {
            KdPrint(("OnSfilterIrpPost: IRP_MJ_WRITE failed. status = %x information = %x\r\n",
                status,irp->IoStatus.Information));
        }
        return;
    }

   // 是否是一个已经被加密进程打开的文件
    cfListLock();
    // 如果是create,不需要恢复文件长度。如果是其他请求，在pre的
    // 时候就应该已经恢复了。
    crypting = cfIsFileCrypting(file);
    cfListUnlock();

    // 对所有的文件打开，都用如下的过程操作：
    if(irpsp->MajorFunction == IRP_MJ_CREATE)
    {
        if(proc_sec)
        {
            ASSERT(crypting == FALSE);
            // 如果是加密进程，则追加进去即可。
            if(!cfFileCryptAppendLk(file))
            {
                IoCancelFileOpen(next_dev,file);
			    irp->IoStatus.Status = STATUS_ACCESS_DENIED;
			    irp->IoStatus.Information = 0;
                KdPrint(("OnSfilterIrpPost: file %wZ failed to call cfFileCryptAppendLk!!!\r\n",&file->FileName));            
            }
            else
            {
                KdPrint(("OnSfilterIrpPost: file %wZ begin to crypting.\r\n",&file->FileName));
            }
        }
        else
        {
            // 是普通进程。根据是否是加密文件。如果是加密文件，
            // 否决这个操作。
            if(crypting)
            {
                IoCancelFileOpen(next_dev,file);
			    irp->IoStatus.Status = STATUS_ACCESS_DENIED;
			    irp->IoStatus.Information = 0;
            }
        }
    }
    else if(irpsp->MajorFunction == IRP_MJ_CLOSE)
    {
        // clean up结束了。这里删除加密节点，删除缓冲。
        ASSERT(crypting);
        cfCryptFileCleanupComplete(file);
    }
    else if(irpsp->MajorFunction == IRP_MJ_QUERY_INFORMATION)
    {
        ASSERT(crypting);
        cfIrpQueryInforPost(irp,irpsp);
    }
    else if(irpsp->MajorFunction == IRP_MJ_READ)
    {
        ASSERT(crypting);
        cfIrpReadPost(irp,irpsp);
    }
    else if(irpsp->MajorFunction == IRP_MJ_WRITE)
    {
        ASSERT(crypting);
        cfIrpWritePost(irp,irpsp,context);
    }
    else
    {
        ASSERT(FALSE);
    }
}

NTSTATUS OnSfilterDriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath,
	OUT PUNICODE_STRING userNameString,
	OUT PUNICODE_STRING syblnkString,
	OUT PULONG extensionSize)
{
 	UNICODE_STRING user_name,syb_name;
	NTSTATUS status = STATUS_SUCCESS;

#if DBG
//    _asm int 3
#endif

    // 初始化加密链表
    cfListInit();

  	// 确定控制设备的名字和符号链接。
	RtlInitUnicodeString(&user_name,L"crypt_file_cdo");
	RtlInitUnicodeString(&syb_name,L"crypt_file_cdo_syb");
	RtlCopyUnicodeString(userNameString,&user_name);
	RtlCopyUnicodeString(syblnkString,&syb_name);

	// 设置控制设备为所有用户可用
	sfilterSetCdoAccessForAll();

    // 初始化进程名字查找
    cfCurProcNameInit();


    return STATUS_SUCCESS;
}

VOID OnSfilterDriverUnload()
{
    // 没什么要做的...;
}

NTSTATUS OnSfilterCDODispatch(
		IN PDEVICE_OBJECT DeviceObject,
		IN PIRP Irp)
{
    return STATUS_UNSUCCESSFUL;
}

BOOLEAN OnSfilterAttachPre(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PUNICODE_STRING DeviceName,
		IN PVOID extension)
{
    // 直接返回TRUE，所有设备都绑定
    return TRUE;
}

VOID OnSfilterAttachPost(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PDEVICE_OBJECT theDeviceToAttached,
		IN PVOID extension,
		IN NTSTATUS status)
{
    // 不需要做什么。
}

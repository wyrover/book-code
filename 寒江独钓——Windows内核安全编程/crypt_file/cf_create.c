///
/// @file         cf_create.c
/// @author    crazy_chu
/// @date       2009-2-4
/// @brief      实现对create irp的处理。 
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
#include "cf_list.h"
#include "cf_file_irp.h"

#define CF_FILE_HEADER_SIZE (1024*4)
#define CF_MEM_TAG 'cfct'

// 在create之前的时候，获得完整的路径。
ULONG
cfFileFullPathPreCreate(
						PFILE_OBJECT file,
                        PUNICODE_STRING path
						)
{
	NTSTATUS status;
	POBJECT_NAME_INFORMATION  obj_name_info = NULL;
	WCHAR buf[64] = { 0 };
	void *obj_ptr;
	ULONG length = 0;
	BOOLEAN need_split = FALSE;

	ASSERT( file != NULL );
	if(file == NULL)
		return 0;
	if(file->FileName.Buffer == NULL)
		return 0;

	obj_name_info = (POBJECT_NAME_INFORMATION)buf;
	do {

		// 获取FileName前面的部分（设备路径或者根目录路径）
		if(file->RelatedFileObject != NULL)
			obj_ptr = (void *)file->RelatedFileObject;
		else
			obj_ptr= (void *)file->DeviceObject;
		status = ObQueryNameString(obj_ptr,obj_name_info,64*sizeof(WCHAR),&length);
		if(status == STATUS_INFO_LENGTH_MISMATCH)
		{
			obj_name_info = ExAllocatePoolWithTag(NonPagedPool,length,CF_MEM_TAG);
			if(obj_name_info == NULL)
				return STATUS_INSUFFICIENT_RESOURCES;
			RtlZeroMemory(obj_name_info,length);
			status = ObQueryNameString(obj_ptr,obj_name_info,length,&length);            
		}
		// 失败了就直接跳出即可
		if(!NT_SUCCESS(status))
			break;

		// 判断二者之间是否需要多一个斜杠。这需要两个条件:
		// FileName第一个字符不是斜杠。obj_name_info最后一个
		// 字符不是斜杠。
		if( file->FileName.Length > 2 &&
			file->FileName.Buffer[ 0 ] != L'\\' &&
			obj_name_info->Name.Buffer[ obj_name_info->Name.Length / sizeof(WCHAR) - 1 ] != L'\\' )
			need_split = TRUE;

		// 获总体名字的长度。如果长度不足，也直接返回。
		length = obj_name_info->Name.Length + file->FileName.Length;
		if(need_split)
			length += sizeof(WCHAR);
		if(path->MaximumLength < length)
			break;

		// 先把设备名拷贝进去。
		RtlCopyUnicodeString(path,&obj_name_info->Name);
		if(need_split)
			// 追加一个斜杠
			RtlAppendUnicodeToString(path,L"\\");

		// 然后追加FileName
		RtlAppendUnicodeStringToString(path,&file->FileName);
	} while(0);

	// 如果分配过空间就释放掉。
	if((void *)obj_name_info != (void *)buf)
		ExFreePool(obj_name_info);
	return length;
}

// 用IoCreateFileSpecifyDeviceObjectHint来打开文件。
// 这个文件打开之后不进入加密链表，所以可以直接
// Read和Write,不会被加密。
HANDLE cfCreateFileAccordingIrp(
   IN PDEVICE_OBJECT dev,
   IN PUNICODE_STRING file_full_path,
   IN PIO_STACK_LOCATION irpsp,
   OUT NTSTATUS *status,
   OUT PFILE_OBJECT *file,
   OUT PULONG information)
{
	HANDLE file_h = NULL;
	IO_STATUS_BLOCK io_status;
	ULONG desired_access;
	ULONG disposition;
	ULONG create_options;
	ULONG share_access;
	ULONG file_attri;
    OBJECT_ATTRIBUTES obj_attri;

    ASSERT(irpsp->MajorFunction == IRP_MJ_CREATE);

    *information = 0;

    // 填写object attribute
    InitializeObjectAttributes(
        &obj_attri,
        file_full_path,
        OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,
        NULL,
        NULL);

    // 获得IRP中的参数。
	desired_access = irpsp->Parameters.Create.SecurityContext->DesiredAccess;
	disposition = (irpsp->Parameters.Create.Options>>24);
	create_options = (irpsp->Parameters.Create.Options & 0x00ffffff);
	share_access = irpsp->Parameters.Create.ShareAccess;
	file_attri = irpsp->Parameters.Create.FileAttributes;

    // 调用IoCreateFileSpecifyDeviceObjectHint打开文件。
    *status = IoCreateFileSpecifyDeviceObjectHint(
        &file_h,
        desired_access,
        &obj_attri,
        &io_status,
        NULL,
        file_attri,
        share_access,
        disposition,
        create_options,
        NULL,
        0,
        CreateFileTypeNone,
        NULL,
        0,
        dev);

    if(!NT_SUCCESS(*status))
        return file_h;

    // 记住information,便于外面使用。
    *information = io_status.Information;

    // 从句柄得到一个fileobject便于后面的操作。记得一定要解除
    // 引用。
    *status = ObReferenceObjectByHandle(
        file_h,
        0,
        *IoFileObjectType,
        KernelMode,
        file,
        NULL);

    // 如果失败了就关闭，假设没打开文件。但是这个实际上是不
    // 应该出现的。
    if(!NT_SUCCESS(*status))
    {
        ASSERT(FALSE);
        ZwClose(file_h);
    }
    return file_h;
}

// 写入一个文件头。
NTSTATUS cfWriteAHeader(PFILE_OBJECT file,PDEVICE_OBJECT next_dev)
{
    static WCHAR header_flags[CF_FILE_HEADER_SIZE/sizeof(WCHAR)] = {L'C',L'F',L'H',L'D'};
    LARGE_INTEGER file_size,offset;
    ULONG length = CF_FILE_HEADER_SIZE;
    NTSTATUS status;

    offset.QuadPart = 0;
    file_size.QuadPart = CF_FILE_HEADER_SIZE;
    // 首先设置文件的大小为4k。
    status = cfFileSetFileSize(next_dev,file,&file_size);
    if(status != STATUS_SUCCESS)
        return status;

    // 然后写入8个字节的头。
   return cfFileReadWrite(next_dev,file,&offset,&length,header_flags,FALSE);
}


// 打开预处理。
ULONG cfIrpCreatePre(
    PIRP irp,
    PIO_STACK_LOCATION irpsp,
    PFILE_OBJECT file,
    PDEVICE_OBJECT next_dev)
{
    UNICODE_STRING path = { 0 };
    // 首先获得要打开文件的路径。
    ULONG length = cfFileFullPathPreCreate(file,&path);
    NTSTATUS status;
    ULONG ret = SF_IRP_PASS;
    PFILE_OBJECT my_file = NULL;
    HANDLE file_h;
    ULONG information = 0;
    LARGE_INTEGER file_size,offset = { 0 };
    BOOLEAN dir,sec_file;
    // 获得打开访问期望。
	ULONG desired_access = irpsp->Parameters.Create.SecurityContext->DesiredAccess;
    WCHAR header_flags[4] = {L'C',L'F',L'H',L'D'};
    WCHAR header_buf[4] = { 0 };
    ULONG disp;

    // 无法得到路径，直接放过即可。
    if(length == 0)
        return SF_IRP_PASS;

    // 如果只是想打开目录的话，直接放过
    if(irpsp->Parameters.Create.Options & FILE_DIRECTORY_FILE)
        return SF_IRP_PASS;

    do {

        // 给path分配缓冲区
        path.Buffer = ExAllocatePoolWithTag(NonPagedPool,length+4,CF_MEM_TAG);
        path.Length = 0;
        path.MaximumLength = (USHORT)length + 4;
        if(path.Buffer == NULL)
        {
            // 内存不够，这个请求直接挂掉
            status = STATUS_INSUFFICIENT_RESOURCES;
            ret = SF_IRP_COMPLETED;
            break;
        }
        length = cfFileFullPathPreCreate(file,&path);

        // 得到了路径，打开这个文件。
        file_h = cfCreateFileAccordingIrp(
            next_dev,
            &path,
            irpsp,
            &status,
            &my_file,
            &information);

        // 如果没有成功的打开，那么说明这个请求可以结束了
        if(!NT_SUCCESS(status))
        {
            ret = SF_IRP_COMPLETED;
            break;
        }

        // 得到了my_file之后，首先判断这个文件是不是已经在
        // 加密的文件之中。如果在，直接返回passthru即可
        cfListLock();
        sec_file = cfIsFileCrypting(my_file);
        cfListUnlock();
        if(sec_file)
        {
            ret = SF_IRP_PASS;
            break;
        }

        // 现在虽然打开，但是这依然可能是一个目录。在这里
        // 判断一下。同时也可以得到文件的大小。
        status = cfFileGetStandInfo(
	        next_dev,
	        my_file,
	        NULL,
	        &file_size,
	        &dir);

        // 查询失败。禁止打开。
        if(!NT_SUCCESS(status))
        {
            ret = SF_IRP_COMPLETED;
            break;
        }

        // 如果这是一个目录，那么不管它了。
        if(dir)
        {
            ret = SF_IRP_PASS;
            break;
        }

        // 如果文件大小为0，且有写入或者追加数据的意图，
        // 就应该加密文件。应该在这里写入文件头。这也是唯
        // 一需要写入文件头的地方。
        if(file_size.QuadPart == 0 && 
            (desired_access & 
                (FILE_WRITE_DATA| 
		        FILE_APPEND_DATA)))
        {
            // 不管是否成功。一定要写入头。
            cfWriteAHeader(my_file,next_dev);
            // 写入头之后，这个文件属于必须加密的文件
            ret = SF_IRP_GO_ON;
            break;
        }

        // 这个文件有大小，而且大小小于头长度。不需要加密。
        if(file_size.QuadPart < CF_FILE_HEADER_SIZE)
        {
            ret = SF_IRP_PASS;
            break;
        }

        // 现在读取文件。比较来看是否需要加密，直接读个8字
        // 节就足够了。这个文件有大小，而且比CF_FILE_HEADER_SIZE
        // 长。此时读出前8个字节，判断是否要加密。
        length = 8;
        status = cfFileReadWrite(next_dev,my_file,&offset,&length,header_buf,TRUE);
        if(status != STATUS_SUCCESS)
        {
            // 如果失败了就不加密了。
            ASSERT(FALSE);
            ret = SF_IRP_PASS;
            break;
        }
        // 读取到内容，比较和加密标志是一致的，加密。
        if(RtlCompareMemory(header_flags,header_buf,8) == 8)
        {
            // 到这里认为是必须加密的。这种情况下，必须返回GO_ON.
            ret = SF_IRP_GO_ON;
            break;
        }

        // 其他的情况都是不需要加密的。
        ret = SF_IRP_PASS;
    } while(0);

    if(path.Buffer != NULL)
        ExFreePool(path.Buffer);    
    if(file_h != NULL)
        ZwClose(file_h);
    if(ret == SF_IRP_GO_ON)
    {
        // 要加密的，这里清一下缓冲。避免文件头出现在缓冲里。
        cfFileCacheClear(my_file);
    }
    if(my_file != NULL)
        ObDereferenceObject(my_file);

    // 如果要返回完成，则必须把这个请求完成。这一般都是
    // 以错误作为结局的。
    if(ret == SF_IRP_COMPLETED)
    {
		irp->IoStatus.Status = status;
		irp->IoStatus.Information = information;
        IoCompleteRequest(irp, IO_NO_INCREMENT);
    }

    // 要注意:
    // 1.文件的CREATE改为OPEN.
    // 2.文件的OVERWRITE去掉。不管是不是要加密的文件，
    // 都必须这样做。否则的话，本来是试图生成文件的，
    // 结果发现文件已经存在了。本来试图覆盖文件的，再
    // 覆盖一次会去掉加密头。
    disp = FILE_OPEN;
    irpsp->Parameters.Create.Options &= 0x00ffffff;
    irpsp->Parameters.Create.Options |= (disp << 24);
    return ret;
}

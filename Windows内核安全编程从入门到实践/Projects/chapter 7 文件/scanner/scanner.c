/*++

Copyright (c) 1999-2002  Microsoft Corporation

Module Name:

    scanner.c

Abstract:

    This is the main module of the scanner filter.

    This filter scans the data in a file before allowing an open to proceed.  This is similar
    to what virus checkers do.

Environment:

    Kernel mode

--*/

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include "scanuk.h"
#include "scanner.h"

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

//
//  Structure that contains all the global data structures
//  used throughout the scanner.
//

SCANNER_DATA ScannerData;

//
//  This is a static list of file name extensions files we are interested in scanning
//

const UNICODE_STRING ScannerExtensionsToScan[] = {
    RTL_CONSTANT_STRING(L"doc"),
    RTL_CONSTANT_STRING(L"txt"),
    RTL_CONSTANT_STRING(L"bat"),
    RTL_CONSTANT_STRING(L"cmd"),
    RTL_CONSTANT_STRING(L"inf"),
    /*RTL_CONSTANT_STRING( L"ini"),   Removed, to much usage*/
    {0, 0, NULL}
};


//
//  Function prototypes
//

NTSTATUS
ScannerPortConnect(
    __in PFLT_PORT ClientPort,
    __in_opt PVOID ServerPortCookie,
    __in_bcount_opt(SizeOfContext) PVOID ConnectionContext,
    __in ULONG SizeOfContext,
    __deref_out_opt PVOID *ConnectionCookie
);

VOID
ScannerPortDisconnect(
    __in_opt PVOID ConnectionCookie
);

NTSTATUS
ScannerpScanFileInUserMode(
    __in PFLT_INSTANCE Instance,
    __in PFILE_OBJECT FileObject,
    __out PBOOLEAN SafeToOpen
);

BOOLEAN
ScannerpCheckExtension(
    __in PUNICODE_STRING Extension
);

//
//  Assign text sections for each routine.
//

#ifdef ALLOC_PRAGMA
    #pragma alloc_text(INIT, DriverEntry)
    #pragma alloc_text(PAGE, ScannerInstanceSetup)
    #pragma alloc_text(PAGE, ScannerPreCreate)
    #pragma alloc_text(PAGE, ScannerPortConnect)
    #pragma alloc_text(PAGE, ScannerPortDisconnect)
#endif




//
//  I/O操作的回调函数
//
const FLT_OPERATION_REGISTRATION Callbacks[] = {

    {
        IRP_MJ_CREATE,
        0,
        ScannerPreCreate,
        ScannerPostCreate
    },

    {
        IRP_MJ_CLEANUP,
        0,
        ScannerPreCleanup,
        NULL
    },

    {
        IRP_MJ_WRITE,
        0,
        ScannerPreWrite,
        NULL
    },

    { IRP_MJ_OPERATION_END}
};

//
//  上下文结构，FltAllocateContext中使用的自定义上下文结构大小需要在这里登记
//
const FLT_CONTEXT_REGISTRATION ContextRegistration[] = {

    {
        FLT_STREAMHANDLE_CONTEXT,
        0,
        NULL,
        sizeof(SCANNER_STREAM_HANDLE_CONTEXT),
        'chBS'
    },

    { FLT_CONTEXT_END }
};

const FLT_REGISTRATION FilterRegistration = {

    sizeof(FLT_REGISTRATION),
    FLT_REGISTRATION_VERSION,
    0,
    ContextRegistration,                //  上下文结构
    Callbacks,                          //  I/O操作的回调函数
    ScannerUnload,
    ScannerInstanceSetup,               //  实例创建处理
    ScannerQueryTeardown,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

////////////////////////////////////////////////////////////////////////////
//
//    Filter initialization and unload routines.
//
////////////////////////////////////////////////////////////////////////////

NTSTATUS
DriverEntry(
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
)
{
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING uniString;
    PSECURITY_DESCRIPTOR sd;
    NTSTATUS status;
    UNREFERENCED_PARAMETER(RegistryPath);
    KdPrint(("[DriverEntry]\n"));
    //
    //  向过滤管理器注册.
    //
    status = FltRegisterFilter(DriverObject,
                               &FilterRegistration,
                               &ScannerData.Filter);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    //  创建一个通信服务端口
    //
    RtlInitUnicodeString(&uniString, ScannerPortName);
    //
    //  建立安全描述符，保证只有管理员权限才可以访问
    //
    status = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);

    if (NT_SUCCESS(status)) {
        InitializeObjectAttributes(&oa,
                                   &uniString,
                                   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                   NULL,
                                   sd);
        status = FltCreateCommunicationPort(ScannerData.Filter,
                                            &ScannerData.ServerPort,
                                            &oa,
                                            NULL,
                                            ScannerPortConnect,
                                            ScannerPortDisconnect,
                                            NULL,
                                            1);
        FltFreeSecurityDescriptor(sd);

        if (NT_SUCCESS(status)) {
            //
            //  开始过滤I/O请求
            //
            status = FltStartFiltering(ScannerData.Filter);

            if (NT_SUCCESS(status)) {
                KdPrint(("[DriverEntry] FltStartFiltering OK...\n"));
                return STATUS_SUCCESS;
            }

            FltCloseCommunicationPort(ScannerData.ServerPort);
        }
    }

    FltUnregisterFilter(ScannerData.Filter);
    return status;
}


NTSTATUS
ScannerPortConnect(
    __in PFLT_PORT ClientPort,
    __in_opt PVOID ServerPortCookie,
    __in_bcount_opt(SizeOfContext) PVOID ConnectionContext,
    __in ULONG SizeOfContext,
    __deref_out_opt PVOID *ConnectionCookie
)
/*++

Routine Description

    当用户模式应用程序调用FilterConnectCommunicationPort时，本函数将会被调用。


Arguments

    ClientPort - This is the client connection port that will be used to
        send messages from the filter

    ServerPortCookie - The context associated with this port when the
        minifilter created this port.

    ConnectionContext - Context from entity connecting to this port (most likely
        your user mode service)

    SizeofContext - Size of ConnectionContext in bytes

    ConnectionCookie - Context to be passed to the port disconnect routine.

Return Value

    STATUS_SUCCESS - to accept the connection

--*/
{
    PAGED_CODE();
    UNREFERENCED_PARAMETER(ServerPortCookie);
    UNREFERENCED_PARAMETER(ConnectionContext);
    UNREFERENCED_PARAMETER(SizeOfContext);
    UNREFERENCED_PARAMETER(ConnectionCookie);
    ASSERT(ScannerData.ClientPort == NULL);
    ASSERT(ScannerData.UserProcess == NULL);
    //
    //  保存进程和通信客户端
    //
    ScannerData.UserProcess = PsGetCurrentProcess();
    ScannerData.ClientPort = ClientPort;
    DbgPrint("!!! scanner.sys --- connected, port=0x%p\n", ClientPort);
    return STATUS_SUCCESS;
}


VOID
ScannerPortDisconnect(
    __in_opt PVOID ConnectionCookie
)
/*++

Routine Description

    This is called when the connection is torn-down. We use it to close our
    handle to the connection

Arguments

    ConnectionCookie - Context from the port connect routine

Return value

    None

--*/
{
    UNREFERENCED_PARAMETER(ConnectionCookie);
    PAGED_CODE();
    DbgPrint("!!! scanner.sys --- disconnected, port=0x%p\n", ScannerData.ClientPort);
    //
    //  关闭通信客户端
    //
    FltCloseClientPort(ScannerData.Filter, &ScannerData.ClientPort);
    ScannerData.UserProcess = NULL;
}


NTSTATUS
ScannerUnload(
    __in FLT_FILTER_UNLOAD_FLAGS Flags
)
/*++

Routine Description:

    This is the unload routine for the Filter driver.  This unregisters the
    Filter with the filter manager and frees any allocated global data
    structures.

Arguments:

    None.

Return Value:

    Returns the final status of the deallocation routines.

--*/
{
    UNREFERENCED_PARAMETER(Flags);
    //
    //  关闭通信服务端
    //
    FltCloseCommunicationPort(ScannerData.ServerPort);
    //
    //  Unregister the filter
    //
    FltUnregisterFilter(ScannerData.Filter);
    return STATUS_SUCCESS;
}

NTSTATUS
ScannerInstanceSetup(
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
)
/*++

Routine Description:

    一个新的微过滤实例被创建时，将会调用本函数。

Arguments:

    FltObjects - Describes the instance and volume which we are being asked to
        setup.

    Flags - Flags describing the type of attachment this is.

    VolumeDeviceType - The DEVICE_TYPE for the volume to which this instance
        will attach.

    VolumeFileSystemType - The file system formatted on this volume.

Return Value:

  FLT_NOTIFY_STATUS_ATTACH              - we wish to attach to the volume
  FLT_NOTIFY_STATUS_DO_NOT_ATTACH       - no, thank you

--*/
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(VolumeFilesystemType);
    PAGED_CODE();
    ASSERT(FltObjects->Filter == ScannerData.Filter);
    ShowInstanceInfo(FltObjects, Flags, VolumeDeviceType, VolumeFilesystemType);

    //
    // 不要绑定到网络文件系统设备上
    //

    if (VolumeDeviceType == FILE_DEVICE_NETWORK_FILE_SYSTEM) {
        return STATUS_FLT_DO_NOT_ATTACH;
    }

    return STATUS_SUCCESS;
}

NTSTATUS
ScannerQueryTeardown(
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
)
/*++

Routine Description:

    This is the instance detach routine for the filter. This
    routine is called by filter manager when a user initiates a manual instance
    detach. This is a 'query' routine: if the filter does not want to support
    manual detach, it can return a failure status

Arguments:

    FltObjects - Describes the instance and volume for which we are receiving
        this query teardown request.

    Flags - Unused

Return Value:

    STATUS_SUCCESS - we allow instance detach to happen

--*/
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
    return STATUS_SUCCESS;
}


FLT_PREOP_CALLBACK_STATUS
ScannerPreCreate(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
/*++

Routine Description:

    Pre create callback.  We need to remember whether this file has been
    opened for write access.  If it has, we'll want to rescan it in cleanup.
    This scheme results in extra scans in at least two cases:
    -- if the create fails (perhaps for access denied)
    -- the file is opened for write access but never actually written to
    The assumption is that writes are more common than creates, and checking
    or setting the context in the write path would be less efficient than
    taking a good guess before the create.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - Output parameter which can be used to pass a context
        from this pre-create callback to the post-create callback.

Return Value:

   FLT_PREOP_SUCCESS_WITH_CALLBACK - If this is not our user-mode process.
   FLT_PREOP_SUCCESS_NO_CALLBACK - All other threads.

--*/
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PAGED_CODE();

    //
    //  如果本次打开请求是由微过滤的应用程序发出的，则不执行过滤操作。
    //

    if (IoThreadToProcess(Data->Thread) == ScannerData.UserProcess) {
        DbgPrint("!!! scanner.sys -- allowing create for trusted process \n");
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

BOOLEAN
ScannerpCheckExtension(
    __in PUNICODE_STRING Extension
)
/*++

Routine Description:

    Checks if this file name extension is something we are interested in

Arguments

    Extension - Pointer to the file name extension

Return Value

    TRUE - Yes we are interested
    FALSE - No
--*/
{
    const UNICODE_STRING *ext;

    if (Extension->Length == 0) {
        return FALSE;
    }

    //
    //  检查后缀名是否匹配已定义的感兴趣的后缀名
    //
    ext = ScannerExtensionsToScan;

    while (ext->Buffer != NULL) {
        if (RtlCompareUnicodeString(Extension, ext, TRUE) == 0) {
            return TRUE;
        }

        ext++;
    }

    return FALSE;
}


FLT_POSTOP_CALLBACK_STATUS
ScannerPostCreate(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
)
/*++

Routine Description:

    Post create callback.  We can't scan the file until after the create has
    gone to the filesystem, since otherwise the filesystem wouldn't be ready
    to read the file for us.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - The operation context passed fron the pre-create
        callback.

    Flags - Flags to say why we are getting this post-operation callback.

Return Value:

    FLT_POSTOP_FINISHED_PROCESSING - ok to open the file or we wish to deny
                                     access to this file, hence undo the open

--*/
{
    PSCANNER_STREAM_HANDLE_CONTEXT scannerContext;
    FLT_POSTOP_CALLBACK_STATUS returnStatus = FLT_POSTOP_FINISHED_PROCESSING;
    PFLT_FILE_NAME_INFORMATION nameInfo;
    NTSTATUS status;
    BOOLEAN safeToOpen, scanFile;
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(Flags);

    //
    //  请求已经失败了，就直接跳过了
    //

    if (!NT_SUCCESS(Data->IoStatus.Status) ||
        (STATUS_REPARSE == Data->IoStatus.Status)) {
        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    //
    //  检查该文件是否是感兴趣的对象
    //
    status = FltGetFileNameInformation(Data,
                                       FLT_FILE_NAME_NORMALIZED |
                                       FLT_FILE_NAME_QUERY_DEFAULT,
                                       &nameInfo);

    if (!NT_SUCCESS(status)) {
        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    FltParseFileNameInformation(nameInfo);
    //
    //  检查后缀名
    //
    scanFile = ScannerpCheckExtension(&nameInfo->Extension);
    FltReleaseFileNameInformation(nameInfo);

    if (!scanFile) {
        //
        //  Not an extension we are interested in
        //
        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    //该文件是感兴趣的对象，将交由应用程序做进一步检测
    (VOID) ScannerpScanFileInUserMode(FltObjects->Instance,
                                      FltObjects->FileObject,
                                      &safeToOpen);

    if (!safeToOpen) {
        //
        //  检测到恶意特征值，拒绝打开
        //
        DbgPrint("!!! scanner.sys -- foul language detected in postcreate !!!\n");
        DbgPrint("!!! scanner.sys -- undoing create \n");
        FltCancelFileOpen(FltObjects->Instance, FltObjects->FileObject);
        Data->IoStatus.Status = STATUS_ACCESS_DENIED;
        Data->IoStatus.Information = 0;
        returnStatus = FLT_POSTOP_FINISHED_PROCESSING;
    } else if (FltObjects->FileObject->WriteAccess) {
        //
        //
        //  如果打开请求中包含了写属性，则记录下来，将会在文件关闭时重新检测一次。
        //  SCANNER_STREAM_HANDLE_CONTEXT结构大小已经在DriverEntry中注册过了
        //
        status = FltAllocateContext(ScannerData.Filter,
                                    FLT_STREAMHANDLE_CONTEXT,
                                    sizeof(SCANNER_STREAM_HANDLE_CONTEXT),
                                    PagedPool,
                                    &scannerContext);

        if (NT_SUCCESS(status)) {
            scannerContext->RescanRequired = TRUE;
            (VOID) FltSetStreamHandleContext(FltObjects->Instance,
                                             FltObjects->FileObject,
                                             FLT_SET_CONTEXT_REPLACE_IF_EXISTS,
                                             scannerContext,
                                             NULL);
            FltReleaseContext(scannerContext);
        }
    }

    return returnStatus;
}


FLT_PREOP_CALLBACK_STATUS
ScannerPreCleanup(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
/*++

Routine Description:

    Pre cleanup callback.  If this file was opened for write access, we want
    to rescan it now.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - Output parameter which can be used to pass a context
        from this pre-cleanup callback to the post-cleanup callback.

Return Value:

    Always FLT_PREOP_SUCCESS_NO_CALLBACK.

--*/
{
    NTSTATUS status;
    PSCANNER_STREAM_HANDLE_CONTEXT context;
    BOOLEAN safe;
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(CompletionContext);
    status = FltGetStreamHandleContext(FltObjects->Instance,
                                       FltObjects->FileObject,
                                       &context);

    if (NT_SUCCESS(status)) {
        if (context->RescanRequired) {
            (VOID) ScannerpScanFileInUserMode(FltObjects->Instance,
                                              FltObjects->FileObject,
                                              &safe);

            if (!safe) {
                DbgPrint("!!! scanner.sys -- foul language detected in precleanup !!!\n");
            }
        }

        FltReleaseContext(context);
    }

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK_STATUS
ScannerPreWrite(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
/*++

Routine Description:

    Pre write callback.  We want to scan what's being written now.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - Output parameter which can be used to pass a context
        from this pre-write callback to the post-write callback.

Return Value:

    Always FLT_PREOP_SUCCESS_NO_CALLBACK.

--*/
{
    FLT_PREOP_CALLBACK_STATUS returnStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;
    NTSTATUS status;
    PSCANNER_NOTIFICATION notification = NULL;
    PSCANNER_STREAM_HANDLE_CONTEXT context = NULL;
    ULONG replyLength;
    BOOLEAN safe = TRUE;
    PUCHAR buffer;
    UNREFERENCED_PARAMETER(CompletionContext);

    if (ScannerData.ClientPort == NULL) {
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    status = FltGetStreamHandleContext(FltObjects->Instance,
                                       FltObjects->FileObject,
                                       &context);

    if (!NT_SUCCESS(status)) {
        //
        //  返回失败说明该文件不是我们感兴趣的文件，因为对感兴趣的文件都设置了context
        //
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    try {
        //
        //  将待写入的缓冲区传递到应用程序做进一步检测
        //
        if (Data->Iopb->Parameters.Write.Length != 0) {
            //
            //  获取用户缓冲区。如果定义了MDL，则使用MDL，否则直接使用给的用户缓冲区地址
            //
            if (Data->Iopb->Parameters.Write.MdlAddress != NULL) {
                buffer = MmGetSystemAddressForMdlSafe(Data->Iopb->Parameters.Write.MdlAddress,
                                                      NormalPagePriority);

                if (buffer == NULL) {
                    Data->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                    Data->IoStatus.Information = 0;
                    returnStatus = FLT_PREOP_COMPLETE;
                    leave;
                }
            } else {
                buffer  = Data->Iopb->Parameters.Write.WriteBuffer;
            }

            //
            //将待扫描的内容传递给应用程序做进一步扫描
            //
            notification = ExAllocatePoolWithTag(NonPagedPool,
                                                 sizeof(SCANNER_NOTIFICATION),
                                                 'nacS');

            if (notification == NULL) {
                Data->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
                leave;
            }

            notification->BytesToScan = min(Data->Iopb->Parameters.Write.Length, SCANNER_READ_BUFFER_SIZE);

            try  {
                RtlCopyMemory(&notification->Contents,
                              buffer,
                              notification->BytesToScan);
            }

            except(EXCEPTION_EXECUTE_HANDLER) {
                Data->IoStatus.Status = GetExceptionCode() ;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
                leave;
            }
            //
            //发送给应用程序
            //
            replyLength = sizeof(SCANNER_REPLY);
            status = FltSendMessage(ScannerData.Filter,
                                    &ScannerData.ClientPort,
                                    notification,
                                    sizeof(SCANNER_NOTIFICATION),
                                    notification,
                                    &replyLength,
                                    NULL);

            if (STATUS_SUCCESS == status) {
                safe = ((PSCANNER_REPLY) notification)->SafeToOpen;
            } else {
                DbgPrint("!!! scanner.sys --- couldn't send message to user-mode to scan file, status 0x%X\n", status);
            }
        }

        if (!safe) {
            //
            //  只要不是换页操作，就阻止此次操作——换页操作是一种比较特殊的请求，应当放过。
            //  正是因为放过了换页操作，scanner无法阻止使用文件映射方式向目标文件中写入具有恶意特征值的内容，因为这种方式是直接操作虚拟内存，
            //  不会产生任何读写IRP，scanner只能在该文件关闭时检测到具有恶意特征的内容已经写入了目标文件。
            //
            //
            DbgPrint("!!! scanner.sys -- foul language detected in write !!!\n");

            if (!FlagOn(Data->Iopb->IrpFlags, IRP_PAGING_IO)) {
                DbgPrint("!!! scanner.sys -- blocking the write !!!\n");
                Data->IoStatus.Status = STATUS_ACCESS_DENIED;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
            }
        }
    }

    finally {

        if (notification != NULL)
        {
            ExFreePoolWithTag(notification, 'nacS');
        }

        if (context)
        {
            FltReleaseContext(context);
        }
    }
    return returnStatus;
}

//////////////////////////////////////////////////////////////////////////
//  Local support routines.
//
/////////////////////////////////////////////////////////////////////////

NTSTATUS
ScannerpScanFileInUserMode(
    __in PFLT_INSTANCE Instance,
    __in PFILE_OBJECT FileObject,
    __out PBOOLEAN SafeToOpen
)
/*++

Routine Description:

    This routine is called to send a request up to user mode to scan a given
    file and tell our caller whether it's safe to open this file.

    Note that if the scan fails, we set SafeToOpen to TRUE.  The scan may fail
    because the service hasn't started, or perhaps because this create/cleanup
    is for a directory, and there's no data to read & scan.

    If we failed creates when the service isn't running, there'd be a
    bootstrapping problem -- how would we ever load the .exe for the service?

Arguments:

    Instance - Handle to the filter instance for the scanner on this volume.

    FileObject - File to be scanned.

    SafeToOpen - Set to FALSE if the file is scanned successfully and it contains
                 foul language.

Return Value:

    The status of the operation, hopefully STATUS_SUCCESS.  The common failure
    status will probably be STATUS_INSUFFICIENT_RESOURCES.

--*/

{
    NTSTATUS status = STATUS_SUCCESS;
    PVOID buffer = NULL;
    ULONG bytesRead;
    PSCANNER_NOTIFICATION notification = NULL;
    FLT_VOLUME_PROPERTIES volumeProps;
    LARGE_INTEGER offset;
    ULONG replyLength, length;
    PFLT_VOLUME volume = NULL;
    *SafeToOpen = TRUE;

    if (ScannerData.ClientPort == NULL) {
        return STATUS_SUCCESS;
    }

    try {
        //
        //  获取卷对象
        //
        status = FltGetVolumeFromInstance(Instance, &volume);

        if (!NT_SUCCESS(status)) {
            leave;
        }

        //
        //  获取卷扇区大小
        //
        status = FltGetVolumeProperties(volume,
                                        &volumeProps,
                                        sizeof(volumeProps),
                                        &length);

        if (NT_ERROR(status)) {
            leave;
        }

        length = max(SCANNER_READ_BUFFER_SIZE, volumeProps.SectorSize);
        buffer = FltAllocatePoolAlignedWithTag(Instance,
                                               NonPagedPool,
                                               length,
                                               'nacS');

        if (NULL == buffer) {
            status = STATUS_INSUFFICIENT_RESOURCES;
            leave;
        }

        notification = ExAllocatePoolWithTag(NonPagedPool,
                                             sizeof(SCANNER_NOTIFICATION),
                                             'nacS');

        if (NULL == notification) {
            status = STATUS_INSUFFICIENT_RESOURCES;
            leave;
        }

        //
        //  读取目标文件开始一个扇区大小的内容
        //
        offset.QuadPart = bytesRead = 0;
        status = FltReadFile(Instance,
                             FileObject,
                             &offset,
                             length,
                             buffer,
                             FLTFL_IO_OPERATION_NON_CACHED |
                             FLTFL_IO_OPERATION_DO_NOT_UPDATE_BYTE_OFFSET,
                             &bytesRead,
                             NULL,
                             NULL);

        if (NT_SUCCESS(status) && (0 != bytesRead)) {
            notification->BytesToScan = (ULONG) bytesRead;
            //
            //  将获取的文件内容拷贝到目标缓冲区中，并发送到应用程序中
            //
            RtlCopyMemory(&notification->Contents,
                          buffer,
                          min(notification->BytesToScan, SCANNER_READ_BUFFER_SIZE));
            replyLength = sizeof(SCANNER_REPLY);
            status = FltSendMessage(ScannerData.Filter,
                                    &ScannerData.ClientPort,
                                    notification,
                                    sizeof(SCANNER_NOTIFICATION),
                                    notification,
                                    &replyLength,
                                    NULL);

            if (STATUS_SUCCESS == status) {
                *SafeToOpen = ((PSCANNER_REPLY) notification)->SafeToOpen;
            } else {
                DbgPrint("!!! scanner.sys --- couldn't send message to user-mode to scan file, status 0x%X\n", status);
            }
        }
    }

    finally {

        if (NULL != buffer)
        {
            FltFreePoolAlignedWithTag(Instance, buffer, 'nacS');
        }

        if (NULL != notification)
        {
            ExFreePoolWithTag(notification, 'nacS');
        }

        if (NULL != volume)
        {
            FltObjectDereference(volume);
        }
    }
    return status;
}



NTSTATUS ShowInstanceInfo(
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG uBufferSize = 0;
    PVOID pBuffer = NULL;
    CHAR pName[1024] = {0};
    KdPrint(("\n[ScannerInstanceSetup]\n"));
    KdPrint(("Filter:0x%08x Volume:0x%08x Instance:0x%08x\n",
             FltObjects->Filter, FltObjects->Volume, FltObjects->Instance));
    status = FltGetInstanceInformation(
                 FltObjects->Instance,
                 InstanceFullInformation,
                 NULL,
                 0,
                 &uBufferSize
             );

    if (STATUS_BUFFER_TOO_SMALL == status) {
        pBuffer = ExAllocatePoolWithTag(NonPagedPool, uBufferSize, 'wjl');

        if (NULL == pBuffer) {
            return STATUS_UNSUCCESSFUL;
        }

        status = FltGetInstanceInformation(
                     FltObjects->Instance,
                     InstanceFullInformation,
                     pBuffer,
                     uBufferSize,
                     &uBufferSize
                 );

        if (!NT_SUCCESS(status)) {
            ExFreePoolWithTag(pBuffer, 'wjl');
            return STATUS_UNSUCCESSFUL;
        }

        KdPrint(("NextEntryOffset: 0x%x\n", ((PINSTANCE_FULL_INFORMATION)pBuffer)->NextEntryOffset));
        RtlZeroMemory(pName, 1024);
        RtlCopyMemory(
            pName,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->InstanceNameBufferOffset + (ULONG)pBuffer,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->InstanceNameLength
        );
        KdPrint(("InstanceName: %ws\n", pName));
        RtlZeroMemory(pName, 1024);
        RtlCopyMemory(
            pName,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->AltitudeBufferOffset + (ULONG)pBuffer,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->AltitudeLength
        );
        KdPrint(("Altitude: %ws\n", pName));
        RtlZeroMemory(pName, 1024);
        RtlCopyMemory(
            pName,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->FilterNameBufferOffset + (ULONG)pBuffer,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->FilterNameLength
        );
        KdPrint(("FilterName: %ws\n", pName));
        RtlZeroMemory(pName, 1024);
        RtlCopyMemory(
            pName,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->VolumeNameBufferOffset + (ULONG)pBuffer,
            ((PINSTANCE_FULL_INFORMATION)pBuffer)->VolumeNameLength
        );
        KdPrint(("VolumeName: %ws\n", pName));
        ExFreePoolWithTag(pBuffer, 'wjl');
    }

    switch (Flags) {
    case FLTFL_INSTANCE_SETUP_AUTOMATIC_ATTACHMENT:
        KdPrint(("Flags: FLTFL_INSTANCE_SETUP_AUTOMATIC_ATTACHMENT\n"));
        break;

    case FLTFL_INSTANCE_SETUP_MANUAL_ATTACHMENT:
        KdPrint(("Flags: FLTFL_INSTANCE_SETUP_MANUAL_ATTACHMENT\n"));
        break;

    case FLTFL_INSTANCE_SETUP_NEWLY_MOUNTED_VOLUME:
        KdPrint(("Flags: FLTFL_INSTANCE_SETUP_NEWLY_MOUNTED_VOLUME\n"));
        break;

    //  case FLTFL_INSTANCE_SETUP_DETACHED_VOLUME:
    //      KdPrint(("Flags: FLTFL_INSTANCE_SETUP_DETACHED_VOLUME\n"));
    //      break;

    default:
        KdPrint(("Flags: Unknown\n"));
        break;
    }

    switch (VolumeDeviceType) {
    case FILE_DEVICE_CD_ROM_FILE_SYSTEM:
        KdPrint(("VolumeDeviceType: FILE_DEVICE_CD_ROM_FILE_SYSTEM\n"));
        break;

    case FILE_DEVICE_DISK_FILE_SYSTEM:
        KdPrint(("VolumeDeviceType: FILE_DEVICE_DISK_FILE_SYSTEM\n"));
        break;

    case FILE_DEVICE_NETWORK_FILE_SYSTEM:
        KdPrint(("VolumeDeviceType: FILE_DEVICE_NETWORK_FILE_SYSTEM\n"));
        break;

    default:
        KdPrint(("VolumeDeviceType: Unknown\n"));
        break;
    }

    switch (VolumeFilesystemType) {
    case FLT_FSTYPE_UNKNOWN:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_UNKNOWN\n"));
        break;

    case FLT_FSTYPE_RAW:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_RAW\n"));
        break;

    case FLT_FSTYPE_NTFS:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_NTFS\n"));
        break;

    case FLT_FSTYPE_FAT:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_FAT\n"));
        break;

    case FLT_FSTYPE_CDFS:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_CDFS\n"));
        break;

    case FLT_FSTYPE_UDFS:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_UDFS\n"));
        break;

    case FLT_FSTYPE_LANMAN:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_LANMAN\n"));
        break;

    case FLT_FSTYPE_WEBDAV:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_WEBDAV\n"));
        break;

    case FLT_FSTYPE_RDPDR:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_RDPDR\n"));
        break;

    case FLT_FSTYPE_NFS:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_NFS\n"));
        break;

    case FLT_FSTYPE_MS_NETWARE:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_MS_NETWARE\n"));
        break;

    case FLT_FSTYPE_NETWARE:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_NETWARE\n"));
        break;

    case FLT_FSTYPE_BSUDF:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_BSUDF\n"));
        break;

    case FLT_FSTYPE_MUP:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_MUP\n"));
        break;

    case FLT_FSTYPE_RSFX:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_RSFX\n"));
        break;

    case FLT_FSTYPE_ROXIO_UDF1:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_ROXIO_UDF1\n"));
        break;

    case FLT_FSTYPE_ROXIO_UDF2:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_ROXIO_UDF2\n"));
        break;

    case FLT_FSTYPE_ROXIO_UDF3:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_ROXIO_UDF3\n"));
        break;

    case FLT_FSTYPE_TACIT:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_TACIT\n"));
        break;

    case FLT_FSTYPE_FS_REC:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_FS_REC\n"));
        break;

    case FLT_FSTYPE_INCD:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_INCD\n"));
        break;

    case FLT_FSTYPE_INCD_FAT:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_INCD_FAT\n"));
        break;

    case FLT_FSTYPE_EXFAT:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_EXFAT\n"));
        break;

    case FLT_FSTYPE_PSFS:
        KdPrint(("VolumeFilesystemType: FLT_FSTYPE_PSFS\n"));
        break;

    default:
        KdPrint(("VolumeFilesystemType: default\n"));
        break;
    }

    return STATUS_SUCCESS;
}

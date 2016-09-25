#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <ntddscsi.h>
#include <ntddk.h>

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

NTSTATUS
DriverEntry
(
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
);

NTSTATUS
NPUnload
(
    __in FLT_FILTER_UNLOAD_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreCreate
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostCreate
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreSetInformation
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostSetInformation
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreRead
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostRead
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
NPPreWrite
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS
NPPostWrite
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
);

//Assign text sections for each routine.
#ifdef ALLOC_PRAGMA
    #pragma alloc_text(INIT, DriverEntry)
    #pragma alloc_text(PAGE, NPUnload)
    #pragma alloc_text(PAGE, NPPostCreate)
    #pragma alloc_text(PAGE, NPPostSetInformation)
    #pragma alloc_text(PAGE, NPPostRead)
    #pragma alloc_text(PAGE, NPPostWrite)
#endif

//operation registration
const FLT_OPERATION_REGISTRATION Callbacks[] = {
    {
        IRP_MJ_CREATE,
        0,
        NPPreCreate,
        NPPostCreate
    },
    {
        IRP_MJ_SET_INFORMATION,
        0,
        NPPreSetInformation,
        NPPostSetInformation
    },
    {
        IRP_MJ_READ,
        0,
        NPPreRead,
        NPPostRead
    },
    {
        IRP_MJ_WRITE,
        0,
        NPPreWrite,
        NPPostWrite
    },
    {
        IRP_MJ_OPERATION_END
    }
};

//This defines what we want to filter with FltMgr
const FLT_REGISTRATION FilterRegistration = {
    sizeof(FLT_REGISTRATION),           //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags
    NULL,                               //  Context
    Callbacks,                          //  Operation callbacks
    NPUnload,                           //  MiniFilterUnload
    NULL,                               //  InstanceSetup
    NULL,                               //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete
    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent
};

PFLT_FILTER g_pFilterHandle = NULL;
PFLT_PORT   g_pServerPort = NULL;
PFLT_PORT   g_pClientPort = NULL;

NTSTATUS DriverEntry
(
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    PSECURITY_DESCRIPTOR sd;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING uniString;       //for communication port name
    UNREFERENCED_PARAMETER(RegistryPath);
    //注册过滤器
    status = FltRegisterFilter(DriverObject,
                               &FilterRegistration,
                               &g_pFilterHandle);
    ASSERT(NT_SUCCESS(status));

    if (NT_SUCCESS(status)) {
        status = FltStartFiltering(g_pFilterHandle);

        if (!NT_SUCCESS(status)) {
            FltUnregisterFilter(g_pFilterHandle);
        }
    }

_Exit_DriverEntry:

    if (!NT_SUCCESS(status)) {
        if (NULL != g_pFilterHandle) {
            FltUnregisterFilter(g_pFilterHandle);
            g_pFilterHandle = NULL;
        }
    }

    DbgPrint("[MiniFilter][DriverEntry]status:%x\n", status);
    return status;
}

NTSTATUS NPUnload
(
    __in FLT_FILTER_UNLOAD_FLAGS Flags
)
{
    UNREFERENCED_PARAMETER(Flags);
    PAGED_CODE();
    FltUnregisterFilter(g_pFilterHandle);
    DbgPrint("[MiniFilter][DriverUnload]\n");
    return STATUS_SUCCESS;
}

FLT_POSTOP_CALLBACK_STATUS NPPostCreate
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
)
{
    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreCreate
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PAGED_CODE();
    {
        UCHAR MajorFunction = 0;
        ULONG Options = 0;
        PFLT_FILE_NAME_INFORMATION nameInfo;
        MajorFunction = Data->Iopb->MajorFunction;
        Options = Data->Iopb->Parameters.Create.Options;

        //如果是IRP_MJ_CREATE，且选项是FILE_DELETE_ON_CLOSE，并且能成功获得文件名信息
        if (IRP_MJ_CREATE == MajorFunction && FILE_DELETE_ON_CLOSE == Options &&
            NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo))) {
            //如果解析文件信息成功
            if (NT_SUCCESS(FltParseFileNameInformation(nameInfo))) {
                WCHAR pTempBuf[ 512 ] = { 0 };
                WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;

                if (nameInfo->Name.MaximumLength > 512) {
                    pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
                    pTemp = pNonPageBuf;
                }

                RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
                DbgPrint("[MiniFilter][IRP_MJ_CREATE]%wZ", &nameInfo->Name);
                _wcsupr(pTemp);

                if (NULL != wcsstr(pTemp, L"README.TXT")) {   // 检查是不是要保护的文件
                    //DbgPrint( "\r\nIn NPPreCreate(), FilePath{%wZ} is forbided.", &nameInfo->Name );
                    if (NULL != pNonPageBuf)
                        ExFreePool(pNonPageBuf);

                    FltReleaseFileNameInformation(nameInfo);
                    return FLT_PREOP_COMPLETE;
                }

                if (NULL != pNonPageBuf)
                    ExFreePool(pNonPageBuf);
            }

            FltReleaseFileNameInformation(nameInfo);
        }
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostSetInformation
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
)
{
    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreSetInformation
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PAGED_CODE();
    {
        UCHAR MajorFunction = 0;
        PFLT_FILE_NAME_INFORMATION nameInfo;
        MajorFunction = Data->Iopb->MajorFunction;

        //如果操作是IRP_MJ_SET_INFORMATION且成功获得文件名信息
        if (IRP_MJ_SET_INFORMATION == MajorFunction &&
            NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo))) {
            if (NT_SUCCESS(FltParseFileNameInformation(nameInfo))) {
                WCHAR pTempBuf[ 512 ] = { 0 };
                WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;

                if (nameInfo->Name.MaximumLength > 512) {
                    pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
                    pTemp = pNonPageBuf;
                }

                RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
                DbgPrint("[MiniFilter][IRP_MJ_SET_INFORMATION]%wZ", &nameInfo->Name);
                _wcsupr(pTemp);

                if (NULL != wcsstr(pTemp, L"README.TXT")) {   // 检查是不是要保护的文件
                    //DbgPrint( "\r\nIn NPPreSetInformation(), FilePath{%wZ} is forbided.", &nameInfo->Name );
                    if (NULL != pNonPageBuf)
                        ExFreePool(pNonPageBuf);

                    FltReleaseFileNameInformation(nameInfo);
                    return FLT_PREOP_DISALLOW_FASTIO;
                }

                if (NULL != pNonPageBuf)
                    ExFreePool(pNonPageBuf);
            }

            FltReleaseFileNameInformation(nameInfo);
        }
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostRead
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
)
{
    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreRead
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PAGED_CODE();
    {
        PFLT_FILE_NAME_INFORMATION nameInfo;

        //直接获得文件名并检查
        if (NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo))) {
            if (NT_SUCCESS(FltParseFileNameInformation(nameInfo))) {
                WCHAR pTempBuf[ 512 ] = { 0 };
                WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;

                if (nameInfo->Name.MaximumLength > 512) {
                    pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
                    pTemp = pNonPageBuf;
                }

                RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
                DbgPrint("[MiniFilter][IRP_MJ_READ]%wZ", &nameInfo->Name);

                /*_wcsupr( pTemp );
                if( NULL != wcsstr( pTemp, L"README.TXT" ) )  // 检查是不是要保护的文件
                {
                    //DbgPrint( "\r\nIn NPPreWrite(), FilePath{%wZ} is forbided.", &nameInfo->Name );
                    if( NULL != pNonPageBuf )
                        ExFreePool( pNonPageBuf );
                    FltReleaseFileNameInformation( nameInfo );
                    return FLT_PREOP_DISALLOW_FASTIO;
                }*/
                if (NULL != pNonPageBuf)
                    ExFreePool(pNonPageBuf);
            }

            FltReleaseFileNameInformation(nameInfo);
        }
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS NPPostWrite
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
)
{
    return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS NPPreWrite
(
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PAGED_CODE();
    {
        PFLT_FILE_NAME_INFORMATION nameInfo;

        //直接获得文件名并检查
        if (NT_SUCCESS(FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &nameInfo))) {
            if (NT_SUCCESS(FltParseFileNameInformation(nameInfo))) {
                WCHAR pTempBuf[ 512 ] = { 0 };
                WCHAR *pNonPageBuf = NULL, *pTemp = pTempBuf;

                if (nameInfo->Name.MaximumLength > 512) {
                    pNonPageBuf = ExAllocatePool(NonPagedPool, nameInfo->Name.MaximumLength);
                    pTemp = pNonPageBuf;
                }

                RtlCopyMemory(pTemp, nameInfo->Name.Buffer, nameInfo->Name.MaximumLength);
                DbgPrint("[MiniFilter][IRP_MJ_WRITE]%wZ", &nameInfo->Name);
                _wcsupr(pTemp);

                if (NULL != wcsstr(pTemp, L"README.TXT")) {   // 检查是不是要保护的文件
                    //DbgPrint( "\r\nIn NPPreWrite(), FilePath{%wZ} is forbided.", &nameInfo->Name );
                    if (NULL != pNonPageBuf)
                        ExFreePool(pNonPageBuf);

                    FltReleaseFileNameInformation(nameInfo);
                    return FLT_PREOP_DISALLOW_FASTIO;
                }

                if (NULL != pNonPageBuf)
                    ExFreePool(pNonPageBuf);
            }

            FltReleaseFileNameInformation(nameInfo);
        }
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}
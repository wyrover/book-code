/*
    在WIN64上驱动级解锁文件（作者：Tesla.Angela）
*/

#include <ntddk.h>

#define kprintf     DbgPrint
#define kmalloc(_s) ExAllocatePoolWithTag(NonPagedPool, _s, 'SYSQ')
#define kfree(_p)   ExFreePool(_p)

typedef struct _HANDLE_INFO {
    UCHAR   ObjectTypeIndex;
    UCHAR   HandleAttributes;
    USHORT  HandleValue;
    ULONG   GrantedAccess;
    ULONG64 Object;
    UCHAR   Name[256];
} HANDLE_INFO, *PHANDLE_INFO;

HANDLE_INFO HandleInfo[1024];

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
    USHORT  UniqueProcessId;
    USHORT  CreatorBackTraceIndex;
    UCHAR   ObjectTypeIndex;
    UCHAR   HandleAttributes;
    USHORT  HandleValue;
    PVOID   Object;
    ULONG   GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG64 NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryObject
(
    HANDLE  Handle,
    ULONG   ObjectInformationClass,
    PVOID   ObjectInformation,
    ULONG   ObjectInformationLength,
    PULONG  ReturnLength OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation
(
    ULONG   SystemInformationClass,
    PVOID   SystemInformation,
    ULONG   SystemInformationLength,
    PULONG  ReturnLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateObject
(
    HANDLE      SourceProcessHandle,
    HANDLE      SourceHandle,
    HANDLE      TargetProcessHandle OPTIONAL,
    PHANDLE     TargetHandle OPTIONAL,
    ACCESS_MASK DesiredAccess,
    ULONG       HandleAttributes,
    ULONG       Options
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcess
(
    PHANDLE             ProcessHandle,
    ACCESS_MASK         AccessMask,
    POBJECT_ATTRIBUTES  ObjectAttributes,
    PCLIENT_ID          ClientId
);

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectAllInformation,
    ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_BASIC_INFORMATION {
    ULONG                   Attributes;
    ACCESS_MASK             DesiredAccess;
    ULONG                   HandleCount;
    ULONG                   ReferenceCount;
    ULONG                   PagedPoolUsage;
    ULONG                   NonPagedPoolUsage;
    ULONG                   Reserved[3];
    ULONG                   NameInformationLength;
    ULONG                   TypeInformationLength;
    ULONG                   SecurityDescriptorLength;
    LARGE_INTEGER           CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

/*typedef struct _OBJECT_NAME_INFORMATION {
  UNICODE_STRING          Name;
  WCHAR                   NameBuffer[0];
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;*/

typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING          TypeName;
    ULONG                   TotalNumberOfHandles;
    ULONG                   TotalNumberOfObjects;
    WCHAR                   Unused1[8];
    ULONG                   HighWaterNumberOfHandles;
    ULONG                   HighWaterNumberOfObjects;
    WCHAR                   Unused2[8];
    ACCESS_MASK             InvalidAttributes;
    GENERIC_MAPPING         GenericMapping;
    ACCESS_MASK             ValidAttributes;
    BOOLEAN                 SecurityRequired;
    BOOLEAN                 MaintainHandleCount;
    USHORT                  MaintainTypeList;
    POOL_TYPE               PoolType;
    ULONG                   DefaultPagedPoolCharge;
    ULONG                   DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _KAPC_STATE {
    LIST_ENTRY ApcListHead[2];
    PVOID Process;
    BOOLEAN KernelApcInProgress;
    BOOLEAN KernelApcPending;
    BOOLEAN UserApcPending;
} KAPC_STATE, *PKAPC_STATE;

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION {
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

NTKERNELAPI
NTSTATUS
ObSetHandleAttributes(HANDLE Handle, POBJECT_HANDLE_FLAG_INFORMATION HandleFlags, KPROCESSOR_MODE PreviousMode);

NTKERNELAPI
VOID
KeStackAttachProcess(PEPROCESS PROCESS, PKAPC_STATE ApcState);

NTKERNELAPI
VOID
KeUnstackDetachProcess(PKAPC_STATE ApcState);

NTKERNELAPI
NTSTATUS
PsLookupProcessByProcessId(IN HANDLE ProcessId, OUT PEPROCESS *Process);

PEPROCESS LookupProcess(HANDLE Pid)
{
    PEPROCESS eprocess = NULL;

    if (NT_SUCCESS(PsLookupProcessByProcessId(Pid, &eprocess)))
        return eprocess;
    else
        return NULL;
}

VOID UnicodeStringToCharArray(PUNICODE_STRING dst, char *src)
{
    ANSI_STRING string;

    if (dst->Length > 260) return;

    RtlUnicodeStringToAnsiString(&string, dst, TRUE);
    strcpy(src, string.Buffer);
    RtlFreeAnsiString(&string);
}

VOID ForceCloseHandle(PEPROCESS Process, ULONG64 HandleValue)
{
    HANDLE h;
    KAPC_STATE ks;
    OBJECT_HANDLE_FLAG_INFORMATION ohfi;

    if (Process == NULL)
        return;

    if (!MmIsAddressValid(Process))
        return;

    KeStackAttachProcess(Process, &ks);
    h = (HANDLE)HandleValue;
    ohfi.Inherit = 0;
    ohfi.ProtectFromClose = 0;
    ObSetHandleAttributes(h, &ohfi, KernelMode);
    ZwClose(h);
    KeUnstackDetachProcess(&ks);
}

VOID CloseFileHandle(char *szFileName)
{
    PVOID Buffer;
    ULONG BufferSize = 0x20000, rtl = 0;
    NTSTATUS Status, qost = 0;
    NTSTATUS ns = STATUS_SUCCESS;
    ULONG64 i = 0;
    ULONG64 qwHandleCount;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO *p;
    OBJECT_BASIC_INFORMATION BasicInfo;
    POBJECT_NAME_INFORMATION pNameInfo;
    ULONG ulProcessID;
    HANDLE hProcess;
    HANDLE hHandle;
    HANDLE hDupObj;
    CLIENT_ID cid;
    OBJECT_ATTRIBUTES oa;
    CHAR szFile[260] = {0};
    Buffer = kmalloc(BufferSize);
    memset(Buffer, 0, BufferSize);
    Status = ZwQuerySystemInformation(16, Buffer, BufferSize, 0);   //SystemHandleInformation

    while (Status == 0xC0000004) { //STATUS_INFO_LENGTH_MISMATCH
        kfree(Buffer);
        BufferSize = BufferSize * 2;
        Buffer = kmalloc(BufferSize);
        memset(Buffer, 0, BufferSize);
        Status = ZwQuerySystemInformation(16, Buffer, BufferSize, 0);
    }

    if (!NT_SUCCESS(Status)) return;

    qwHandleCount = ((SYSTEM_HANDLE_INFORMATION *)Buffer)->NumberOfHandles;
    p = (SYSTEM_HANDLE_TABLE_ENTRY_INFO *)((SYSTEM_HANDLE_INFORMATION *)Buffer)->Handles;
    //clear array
    memset(HandleInfo, 0, 1024 * sizeof(HANDLE_INFO));

    //ENUM HANDLE PROC
    for (i = 0; i < qwHandleCount; i++) {
        ulProcessID = (ULONG)p[i].UniqueProcessId;
        cid.UniqueProcess = (HANDLE)ulProcessID;
        cid.UniqueThread = (HANDLE)0;
        hHandle = (HANDLE)p[i].HandleValue;
        InitializeObjectAttributes(&oa , NULL , 0 , NULL , NULL);
        ns = ZwOpenProcess(&hProcess , PROCESS_DUP_HANDLE , &oa , &cid);

        if (!NT_SUCCESS(ns)) {
            KdPrint(("ZwOpenProcess : Fail "));
            continue;
        }

        ns = ZwDuplicateObject(hProcess , hHandle , NtCurrentProcess() , &hDupObj , PROCESS_ALL_ACCESS , 0 , DUPLICATE_SAME_ACCESS);

        if (!NT_SUCCESS(ns)) {
            KdPrint(("ZwDuplicateObject : Fail "));
            continue;
        }

        //get basic information
        ZwQueryObject(hDupObj , ObjectBasicInformation , &BasicInfo , sizeof(OBJECT_BASIC_INFORMATION) , NULL);
        //get name information
        pNameInfo = ExAllocatePoolWithTag(PagedPool , 1024 , 'ONON');
        RtlZeroMemory(pNameInfo , 1024);
        qost = ZwQueryObject(hDupObj, ObjectNameInformation, pNameInfo, 1024, &rtl);
        //get information and close handle
        UnicodeStringToCharArray(&(pNameInfo->Name), szFile);
        ExFreePool(pNameInfo);
        ZwClose(hDupObj);
        ZwClose(hProcess);

        //if(!_stricmp(szFile,szFileName))
        if (strstr(_strlwr(szFile), szFileName)) {
            PEPROCESS ep = LookupProcess((HANDLE)(p[i].UniqueProcessId));
            ForceCloseHandle(ep, p[i].HandleValue);
            ObDereferenceObject(ep);
        }
    }
}

NTSTATUS UnloadDriver(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("[UnlockFile]Unloaded!\n");
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    DriverObject->DriverUnload = UnloadDriver;
    CloseFileHandle("lockfile.xxx");
    DbgPrint("[UnlockFile]Loaded!\n");
    return STATUS_SUCCESS;
}
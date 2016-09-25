
NTKERNELAPI PCHAR PsGetProcessImageFileName(PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process);

/*typedef struct _PS_CREATE_NOTIFY_INFO {
  SIZE_T              Size;
  union {
    ULONG  Flags;
    struct {
      ULONG FileOpenNameAvailable  :1;
      ULONG Reserved  :31;
    };
  };
  HANDLE              ParentProcessId;
  CLIENT_ID           CreatingThreadId;
  struct _FILE_OBJECT  *FileObject;
  PCUNICODE_STRING    ImageFileName;
  PCUNICODE_STRING    CommandLine;
  NTSTATUS            CreationStatus;
} PS_CREATE_NOTIFY_INFO, *PPS_CREATE_NOTIFY_INFO;*/

/*VOID MyCreateProcessNotifyEx  //仅监视
(
    __inout   PEPROCESS Process,
    __in      HANDLE ProcessId,
    __in_opt  PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    if(CreateInfo==NULL)
        DbgPrint("[monitor_create_process_x64] Process exit: %s",PsGetProcessImageFileName(Process));
    else
        DbgPrint("[monitor_create_process_x64] Process create: %wZ",CreateInfo->CommandLine);
}*/

PCHAR GetProcessNameByProcessId(HANDLE ProcessId)
{
    NTSTATUS st = STATUS_UNSUCCESSFUL;
    PEPROCESS ProcessObj = NULL;
    PCHAR string = NULL;
    st = PsLookupProcessByProcessId(ProcessId, &ProcessObj);

    if (NT_SUCCESS(st)) {
        string = PsGetProcessImageFileName(ProcessObj);
        ObfDereferenceObject(ProcessObj);
    }

    return string;
}

VOID MyCreateProcessNotifyEx
(
    __inout   PEPROCESS Process,
    __in      HANDLE ProcessId,
    __in_opt  PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
    NTSTATUS st = 0;
    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES oa = {0};
    CLIENT_ID ClientId = {0};
    char xxx[16] = {0};

    if (CreateInfo != NULL) { //进程创建事件
        DbgPrint("[monitor_create_process_x64][%ld]%s创建进程: %wZ",
                 CreateInfo->ParentProcessId,
                 GetProcessNameByProcessId(CreateInfo->ParentProcessId),
                 CreateInfo->ImageFileName);
        strcpy(xxx, PsGetProcessImageFileName(Process));

        if (!_stricmp(xxx, "calc.exe")) {
            DbgPrint("禁止创建计算器进程！");
            CreateInfo->CreationStatus = STATUS_UNSUCCESSFUL; //禁止创建进程
        }
    } else {
        DbgPrint("[monitor_create_process_x64]进程退出: %s", PsGetProcessImageFileName(Process));
    }
}

VOID MyCreateThreadNotify
(
    IN HANDLE  ProcessId,
    IN HANDLE  ThreadId,
    IN BOOLEAN  Create
)
{
    if (Create)
        DbgPrint("[monitor_create_process_x64]线程创建! PID=%ld;TID=%ld", ProcessId, ThreadId);
    else
        DbgPrint("[monitor_create_process_x64]线程退出! PID=%ld;TID=%ld", ProcessId, ThreadId);
}
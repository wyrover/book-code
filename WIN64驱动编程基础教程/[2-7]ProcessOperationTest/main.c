#include <ntddk.h>
#include <windef.h>

//声明结构体
typedef struct _KAPC_STATE {
    LIST_ENTRY ApcListHead[2];
    PKPROCESS Process;
    UCHAR KernelApcInProgress;
    UCHAR KernelApcPending;
    UCHAR UserApcPending;
} KAPC_STATE, *PKAPC_STATE;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY64    InLoadOrderLinks;
    LIST_ENTRY64    InMemoryOrderLinks;
    LIST_ENTRY64    InInitializationOrderLinks;
    PVOID           DllBase;
    PVOID           EntryPoint;
    ULONG           SizeOfImage;
    UNICODE_STRING  FullDllName;
    UNICODE_STRING  BaseDllName;
    ULONG           Flags;
    USHORT          LoadCount;
    USHORT          TlsIndex;
    PVOID           SectionPointer;
    ULONG           CheckSum;
    PVOID           LoadedImports;
    PVOID           EntryPointActivationContext;
    PVOID           PatchInformation;
    LIST_ENTRY64    ForwarderLinks;
    LIST_ENTRY64    ServiceTagLinks;
    LIST_ENTRY64    StaticLinks;
    PVOID           ContextInformation;
    ULONG64         OriginalBase;
    LARGE_INTEGER   LoadTime;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

//声明API
NTKERNELAPI UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);
NTKERNELAPI HANDLE PsGetProcessInheritedFromUniqueProcessId(IN PEPROCESS Process);
NTKERNELAPI PPEB PsGetProcessPeb(PEPROCESS Process);
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(HANDLE Id, PEPROCESS *Process);
NTKERNELAPI NTSTATUS PsLookupThreadByThreadId(HANDLE Id, PETHREAD *Thread);
NTKERNELAPI PEPROCESS IoThreadToProcess(PETHREAD Thread);
NTKERNELAPI VOID NTAPI KeAttachProcess(PEPROCESS Process);
NTKERNELAPI VOID NTAPI KeDetachProcess();
NTKERNELAPI VOID NTAPI KeStackAttachProcess(PEPROCESS Process, PKAPC_STATE ApcState);
NTKERNELAPI VOID NTAPI KeUnstackDetachProcess(PKAPC_STATE ApcState);

//声明偏移
ULONG64 LdrInPebOffset = 0x018;     //peb.ldr
ULONG64 ModListInPebOffset = 0x010; //peb.ldr.InLoadOrderModuleList


//根据进程ID返回进程EPROCESS，失败返回NULL
PEPROCESS LookupProcess(HANDLE Pid)
{
    PEPROCESS eprocess = NULL;

    if (NT_SUCCESS(PsLookupProcessByProcessId(Pid, &eprocess)))
        return eprocess;
    else
        return NULL;
}

//根据线程ID返回线程ETHREAD，失败返回NULL
PETHREAD LookupThread(HANDLE Tid)
{
    PETHREAD ethread;

    if (NT_SUCCESS(PsLookupThreadByThreadId(Tid, &ethread)))
        return ethread;
    else
        return NULL;
}


//枚举指定进程的线程
VOID EnumThread(PEPROCESS Process)
{
    ULONG i = 0, c = 0;
    PETHREAD ethrd = NULL;
    PEPROCESS eproc = NULL;

    for (i = 4; i < 262144; i = i + 4) { //262144是一个随便的数字，一般来说没有超过100000的PID和TID
        ethrd = LookupThread((HANDLE)i);

        if (ethrd != NULL) {
            //获得线程所属进程
            eproc = IoThreadToProcess(ethrd);

            if (eproc == Process) {
                //打印出ETHREAD和TID
                DbgPrint("[THREAD]ETHREAD=%p TID=%ld\n",
                         ethrd,
                         (ULONG)PsGetThreadId(ethrd));
            }

            ObDereferenceObject(ethrd);
        }
    }
}

//枚举指定进程的模块
VOID EnumModule(PEPROCESS Process)
{
    SIZE_T Peb = 0;
    SIZE_T Ldr = 0;
    PLIST_ENTRY ModListHead = 0;
    PLIST_ENTRY Module = 0;
    ANSI_STRING AnsiString;
    KAPC_STATE ks;

    //EPROCESS地址无效则退出
    if (!MmIsAddressValid(Process))
        return;

    //获取PEB地址
    Peb = (SIZE_T)PsGetProcessPeb(Process);

    //PEB地址无效则退出
    if (!Peb)
        return;

    //依附进程
    KeStackAttachProcess(Process, &ks);

    __try {
        //获得LDR地址
        Ldr = Peb + (SIZE_T)LdrInPebOffset;
        //测试是否可读，不可读则抛出异常退出
        ProbeForRead((CONST PVOID)Ldr, 8, 8);
        //获得链表头
        ModListHead = (PLIST_ENTRY)(*(PULONG64)Ldr + ModListInPebOffset);
        //再次测试可读性
        ProbeForRead((CONST PVOID)ModListHead, 8, 8);
        //获得第一个模块的信息
        Module = ModListHead->Flink;

        while (ModListHead != Module) {
            //打印信息：基址、大小、DLL路径
            DbgPrint("[MODULE]Base=%p Size=%ld Path=%wZ\n",
                     (PVOID)(((PLDR_DATA_TABLE_ENTRY)Module)->DllBase),
                     (ULONG)(((PLDR_DATA_TABLE_ENTRY)Module)->SizeOfImage),
                     &(((PLDR_DATA_TABLE_ENTRY)Module)->FullDllName));
            Module = Module->Flink;
            //测试下一个模块信息的可读性
            ProbeForRead((CONST PVOID)Module, 80, 8);
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        DbgPrint("[EnumModule]__except (EXCEPTION_EXECUTE_HANDLER)");
    }

    //取消依附进程
    KeUnstackDetachProcess(&ks);
}

//枚举进程
VOID EnumProcess()
{
    ULONG i = 0;
    PEPROCESS eproc = NULL;

    for (i = 4; i < 262144; i = i + 4) { //262144是一个随便的数字，一般来说没有超过100000的PID和TID
        eproc = LookupProcess((HANDLE)i);

        if (eproc != NULL) {
            ObDereferenceObject(eproc);
            DbgPrint("EPROCESS=%p PID=%ld PPID=%ld Name=%s\n",
                     eproc,
                     (DWORD)PsGetProcessId(eproc),
                     (DWORD)PsGetProcessInheritedFromUniqueProcessId(eproc),
                     PsGetProcessImageFileName(eproc));
            EnumThread(eproc);//枚举线程
            EnumModule(eproc);//枚举模块
            DbgPrint("\n");
        }
    }
}


//正规方法结束进程
void ZwKillProcess(HANDLE Pid)
{
    HANDLE hProcess = NULL;
    CLIENT_ID ClientId;
    OBJECT_ATTRIBUTES oa;
    //填充CID
    ClientId.UniqueProcess = Pid; //这里修改为你要的PID
    ClientId.UniqueThread = 0;
    //填充OA
    oa.Length = sizeof(oa);
    oa.RootDirectory = 0;
    oa.ObjectName = 0;
    oa.Attributes = 0;
    oa.SecurityDescriptor = 0;
    oa.SecurityQualityOfService = 0;
    //打开进程，如果句柄有效，则结束进程
    ZwOpenProcess(&hProcess, 1, &oa, &ClientId);

    if (hProcess) {
        ZwTerminateProcess(hProcess, 0);
        ZwClose(hProcess);
    };
}

//正规方法结束线程
void ZwKillThread(HANDLE Tid)
{
    HANDLE hThread = NULL;
    CLIENT_ID ClientId;
    OBJECT_ATTRIBUTES oa;
    //填充CID
    ClientId.UniqueProcess = 0;
    ClientId.UniqueThread = Tid; //这里修改为你要的TID
    //填充OA
    oa.Length = sizeof(oa);
    oa.RootDirectory = 0;
    oa.ObjectName = 0;
    oa.Attributes = 0;
    oa.SecurityDescriptor = 0;
    oa.SecurityQualityOfService = 0;

    //打开线程，如果句柄有效，则结束进程
    //ZwOpenThread需要自行定位（相关知识在SSDT相关章节有讲）
    //ZwOpenThread(&hThread,1,&oa,&ClientId);
    if (hThread) {
        //ZwTerminateThread需要自行定位（相关知识在SSDT相关章节有讲）
        //ZwTerminateThread(hThread,0);
        ZwClose(hThread);
    };
}


VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("[MyDriver]Unloaded...\n");
    return;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;
    DbgPrint("[MyDriver]Loaded...\n");
    //枚举进程，内含枚举线程和模块
    EnumProcess();
    return STATUS_SUCCESS;
}
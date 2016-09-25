
#define kprintf     DbgPrint
#define kmalloc(_s) ExAllocatePoolWithTag(NonPagedPool, _s, 'SYSQ')
#define kfree(_p)   ExFreePool(_p)

#pragma intrinsic(__readmsr)

typedef struct _SYSTEM_SERVICE_TABLE {
    PVOID       ServiceTableBase;
    PVOID       ServiceCounterTableBase;
    ULONGLONG   NumberOfServices;
    PVOID       ParamTableBase;
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef ULONG64(*NTUSERQUERYWINDOW)
(
    IN HWND     WindowHandle,
    IN ULONG64  TypeInformation
);

typedef ULONG64(*NTUSERPOSTMESSAGE)
(
    HWND    hWnd,
    UINT    Msg,
    WPARAM  wParam,
    LPARAM  lParam
);

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTableShadow = NULL;
ULONG64 ul64W32pServiceTable = 0;
ULONG64 IndexOfNtUserPostMessage = 0x100f;
ULONG64 IndexOfNtUserQueryWindow = 0x1010;
NTUSERQUERYWINDOW NtUserQueryWindow = NULL;
NTUSERPOSTMESSAGE NtUserPostMessage = NULL;
ULONG64 MyProcessId = 0;
//ULONG64   Win32kBase = 0;
ULONG64 IndexOfNtUserWindowFromPhysicalPoint = 0x1337;
ULONG64 AddressNtUserWindowFromPhysicalPoint = 0;

NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation
(
    IN ULONG    SystemInformationClass,
    OUT PVOID   SystemInformation,
    IN ULONG    Length,
    OUT PULONG  ReturnLength
);

KIRQL WPOFFx64()
{
    KIRQL irql = KeRaiseIrqlToDpcLevel();
    UINT64 cr0 = __readcr0();
    cr0 &= 0xfffffffffffeffff;
    __writecr0(cr0);
    _disable();
    return irql;
}

void WPONx64(KIRQL irql)
{
    UINT64 cr0 = __readcr0();
    cr0 |= 0x10000;
    _enable();
    __writecr0(cr0);
    KeLowerIrql(irql);
}

ULONGLONG GetKeServiceDescriptorTableShadow64()
{
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    UCHAR b1 = 0, b2 = 0, b3 = 0;
    ULONG templong = 0;
    ULONGLONG addr = 0;

    for (i = StartSearchAddress; i < EndSearchAddress; i++) {
        if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2)) {
            b1 = *i;
            b2 = *(i + 1);
            b3 = *(i + 2);

            if (b1 == 0x4c && b2 == 0x8d && b3 == 0x1d) { //4c8d1d
                memcpy(&templong, i + 3, 4);
                addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
                return addr;
            }
        }
    }

    return 0;
}

ULONGLONG GetSSSDTFuncCurAddr64(ULONG64 Index)
{
    ULONGLONG               W32pServiceTable = 0, qwTemp = 0;
    LONG                    dwTemp = 0;
    PSYSTEM_SERVICE_TABLE   pWin32k;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));
    W32pServiceTable = (ULONGLONG)(pWin32k->ServiceTableBase);
    ul64W32pServiceTable = W32pServiceTable;
    qwTemp = W32pServiceTable + 4 * (Index - 0x1000); //这里是获得偏移地址的位置，要HOOK的话修改这里即可
    dwTemp = *(PLONG)qwTemp;
    dwTemp = dwTemp >> 4;
    qwTemp = W32pServiceTable + (LONG64)dwTemp;
    return qwTemp;
}

VOID ModifySSSDT(ULONG64 Index, ULONG64 Address)
{
    ULONGLONG               W32pServiceTable = 0, qwTemp = 0;
    LONG                    dwTemp = 0;
    PSYSTEM_SERVICE_TABLE   pWin32k;
    KIRQL                   irql;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));  //4*8
    W32pServiceTable = (ULONGLONG)(pWin32k->ServiceTableBase);
    qwTemp = W32pServiceTable + 4 * (Index - 0x1000);
    dwTemp = (LONG)(Address - W32pServiceTable);
    dwTemp = dwTemp << 4;   //DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);
    irql = WPOFFx64();
    *(PLONG)qwTemp = dwTemp;
    WPONx64(irql);
}

ULONG64 ProxyNtUserPostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (NtUserQueryWindow(hWnd, 0) == MyProcessId && PsGetCurrentProcessId() != (HANDLE)MyProcessId) {
        DbgPrint("Do not fuck with me!");
        return 0;
    } else {
        DbgPrint("OriNtUserPostMessage called!");
        return NtUserPostMessage(hWnd, Msg, wParam, lParam);
    }
}

/*[23 bytes]
lkd> u win32k!NtUserWindowFromPhysicalPoint
win32k!NtUserWindowFromPhysicalPoint:
fffff960`0014cef0 48894c2408      mov     qword ptr [rsp+8],rcx
fffff960`0014cef5 53              push    rbx
fffff960`0014cef6 4883ec60        sub     rsp,60h
fffff960`0014cefa 488b0df7072000  mov     rcx,qword ptr [win32k!gpresUser (fffff960`0034d6f8)]
fffff960`0014cf01 ff1569351c00    call    qword ptr [win32k!_imp_ExEnterPriorityRegionAndAcquireResourceExclusive (fffff960`00310470)]
*/
VOID FuckFunc()
{
    KIRQL irql;
    UCHAR fuckcode[] = "\x48\x33\xC0\xC3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    irql = WPOFFx64();
    memcpy((PVOID)AddressNtUserWindowFromPhysicalPoint, fuckcode, 23);  //覆盖23个字节【保持指令完整性】
    WPONx64(irql);
}

VOID HOOK_SSSDT()
{
    KIRQL irql;
    ULONG64 myfun;
    UCHAR jmp_code[] = "\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";  //需要14字节+4字节（xor rax,rax + ret）
    //代理函数地址
    myfun = (ULONGLONG)ProxyNtUserPostMessage;
    //填充shellcode
    memcpy(jmp_code + 6, &myfun, 8);
    //写入shellcode
    FuckFunc();
    irql = WPOFFx64();
    memcpy((PVOID)(AddressNtUserWindowFromPhysicalPoint + 4), jmp_code, 14);
    WPONx64(irql);
    //修改记录原始地址的地方，[AddressNtUserWindowFromPhysicalPoint+4]开始是一条long jmp，跳转到ProxyNtUserPostMessage
    ModifySSSDT(IndexOfNtUserPostMessage, AddressNtUserWindowFromPhysicalPoint + 4);
    DbgPrint("HOOK_SSSDT OK!");
}

VOID UNHOOK_SSSDT()
{
    ModifySSSDT(IndexOfNtUserPostMessage, (ULONG64)NtUserPostMessage);
    DbgPrint("UNHOOK_SSSDT OK!");
}

/*
ULONG64 GetKernelModuleBase(char* lpModuleName)
{
    typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
    {
        ULONG Unknow1;
        ULONG Unknow2;
        ULONG Unknow3;
        ULONG Unknow4;
        PVOID Base;
        ULONG Size;
        ULONG Flags;
        USHORT Index;
        USHORT NameLength;
        USHORT LoadCount;
        USHORT ModuleNameOffset;
        char ImageName[256];
    } SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;
    typedef struct _SYSTEM_MODULE_INFORMATION
    {
        ULONG Count;//内核中以加载的模块的个数
        SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
    } SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;
    typedef struct _KLDR_DATA_TABLE_ENTRY
    {
        LIST_ENTRY64 InLoadOrderLinks;
        ULONG64 __Undefined1;
        ULONG64 __Undefined2;
        ULONG64 __Undefined3;
        ULONG64 NonPagedDebugInfo;
        ULONG64 DllBase;
        ULONG64 EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG   Flags;
        USHORT  LoadCount;
        USHORT  __Undefined5;
        ULONG64 __Undefined6;
        ULONG   CheckSum;
        ULONG   __padding1;
        ULONG   TimeDateStamp;
        ULONG   __padding2;
    }KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;
    ULONG NeedSize, i, ModuleCount, BufferSize = 0x5000;
    PVOID pBuffer = NULL;
    PCHAR pDrvName = NULL;
    NTSTATUS Result;
    PSYSTEM_MODULE_INFORMATION pSystemModuleInformation;
    do
    {
        //分配内存
        pBuffer = kmalloc( BufferSize );
        if( pBuffer == NULL )
            return 0;
        //查询模块信息
        Result = ZwQuerySystemInformation( 11, pBuffer, BufferSize, &NeedSize );
        if( Result == STATUS_INFO_LENGTH_MISMATCH )
        {
            kfree( pBuffer );
            BufferSize *= 2;
        }
        else if( !NT_SUCCESS(Result) )
        {
            //查询失败则退出
            kfree( pBuffer );
            return 0;
        }
    }
    while( Result == STATUS_INFO_LENGTH_MISMATCH );
    pSystemModuleInformation = (PSYSTEM_MODULE_INFORMATION)pBuffer;
    //获得模块的总数量
    ModuleCount = pSystemModuleInformation->Count;
    //遍历所有的模块
    for( i = 0; i < ModuleCount; i++ )
    {
        if((ULONG64)(pSystemModuleInformation->Module[i].Base) > (ULONG64)0x8000000000000000)
        {
            pDrvName = pSystemModuleInformation->Module[i].ImageName+pSystemModuleInformation->Module[i].ModuleNameOffset;
            if( _stricmp(pDrvName,lpModuleName)==0 )
                return (ULONG64)pSystemModuleInformation->Module[i].Base;
        }
    }
    kfree(pBuffer);
    return 0;
}

PVOID KernelVirtualProtect(PVOID Address, ULONG Size, ULONG NewProtect) //return new virtual address of physical address
{
    NTSTATUS st;
    PVOID p=NULL, pSafeAddress=NULL;
    PMDL pMdl=NULL;
    pMdl = IoAllocateMdl(Address, Size, FALSE, FALSE, NULL);
    __try
    {
        MmProbeAndLockPages( pMdl, KernelMode, IoModifyAccess );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        IoFreeMdl( pMdl );
        return NULL;
    }
    p=MmMapLockedPages(pMdl, KernelMode);
    st=MmProtectMdlSystemAddress(pMdl, NewProtect); //PAGE_EXECUTE_READWRITE
    DbgPrint("MmProtectMdlSystemAddress status: %x",st);
    pSafeAddress = MmGetSystemAddressForMdlSafe( pMdl, NormalPagePriority );
    DbgPrint("Orig Address: %llx",Address);
    DbgPrint("Safe Address: %llx",pSafeAddress);
    return pSafeAddress;
}

VOID FuckFunc()
{
    KIRQL irql;
    Win32kBase=GetKernelModuleBase("win32k.sys");
    DbgPrint("Win32kBase: %llx", Win32kBase);
    irql=WPOFFx64();
    memset((PVOID)Win32kBase,0x90,56);
    WPONx64(irql);
}

VOID HOOK_SSSDT()
{
    KIRQL irql;
    ULONG64 myfun;
    UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";
    //代理函数地址
    myfun=(ULONGLONG)ProxyNtUserPostMessage;
    //填充shellcode
    memcpy(jmp_code+6,&myfun,8);
    //写入shellcode
    FuckFunc();
    irql=WPOFFx64();
    memcpy((PVOID)Win32kBase,jmp_code,14);
    WPONx64(irql);
    //修改内存属性并获得新的虚拟地址
    Win32kBase=(ULONG64)KernelVirtualProtect((PVOID)Win32kBase,56,PAGE_EXECUTE_READWRITE);
    //修改记录原始地址的地方，[AddressNtUserWindowFromPhysicalPoint+4]开始是一条long jmp，跳转到ProxyNtUserPostMessage
    ModifySSSDT(IndexOfNtUserPostMessage, Win32kBase);
    DbgPrint("HOOK_SSSDT OK!");
}
*/
//Tesla.Angela's Win64 Kernel API Hook Engine && Example && Explication

#include <ntddk.h>

#define kmalloc(_s) ExAllocatePoolWithTag(NonPagedPool, _s, 'SYSQ')
#define kfree(_p) ExFreePool(_p)

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

void *GetFunctionAddr(PCWSTR FunctionName)
{
    UNICODE_STRING UniCodeFunctionName;
    RtlInitUnicodeString(&UniCodeFunctionName, FunctionName);
    return MmGetSystemRoutineAddress(&UniCodeFunctionName);
}

ULONG GetPatchSize(PUCHAR Address)
{
    ULONG LenCount = 0, Len = 0;

    while (LenCount <= 14) { //至少需要14字节
        Len = LDE(Address, 64);
        Address = Address + Len;
        LenCount = LenCount + Len;
    }

    return LenCount;
}

//传入：待HOOK函数地址，代理函数地址，接收原始函数地址的指针，接收补丁长度的指针；返回：原来头N字节的数据
PVOID HookKernelApi(IN PVOID ApiAddress, IN PVOID Proxy_ApiAddress, OUT PVOID *Original_ApiAddress, OUT ULONG *PatchSize)
{
    KIRQL irql;
    UINT64 tmpv;
    PVOID head_n_byte, ori_func;
    UCHAR jmp_code[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    UCHAR jmp_code_orifunc[] = "\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    //How many bytes shoule be patch
    *PatchSize = GetPatchSize((PUCHAR)ApiAddress);
    //step 1: Read current data
    head_n_byte = kmalloc(*PatchSize);
    irql = WPOFFx64();
    memcpy(head_n_byte, ApiAddress, *PatchSize);
    WPONx64(irql);
    //step 2: Create ori function
    ori_func = kmalloc(*PatchSize + 14); //原始机器码+跳转机器码
    RtlFillMemory(ori_func, *PatchSize + 14, 0x90);
    tmpv = (ULONG64)ApiAddress + *PatchSize; //跳转到没被打补丁的那个字节
    memcpy(jmp_code_orifunc + 6, &tmpv, 8);
    memcpy((PUCHAR)ori_func, head_n_byte, *PatchSize);
    memcpy((PUCHAR)ori_func + *PatchSize, jmp_code_orifunc, 14);
    *Original_ApiAddress = ori_func;
    //step 3: fill jmp code
    tmpv = (UINT64)Proxy_ApiAddress;
    memcpy(jmp_code + 6, &tmpv, 8);
    //step 4: Fill NOP and hook
    irql = WPOFFx64();
    RtlFillMemory(ApiAddress, *PatchSize, 0x90);
    memcpy(ApiAddress, jmp_code, 14);
    WPONx64(irql);
    //return ori code
    return head_n_byte;
}

//传入：被HOOK函数地址，原始数据，补丁长度
VOID UnhookKernelApi(IN PVOID ApiAddress, IN PVOID OriCode, IN ULONG PatchSize)
{
    KIRQL irql;
    irql = WPOFFx64();
    memcpy(ApiAddress, OriCode, PatchSize);
    WPONx64(irql);
}

typedef NTSTATUS(__fastcall *PSLOOKUPPROCESSBYPROCESSID)(HANDLE ProcessId, PEPROCESS *Process);
ULONG64 my_eprocess = 0;        //待保护进程的eprocess
ULONG pslp_patch_size = 0;      //PsLookupProcessByProcessId被修改了N字节
PUCHAR pslp_head_n_byte = NULL; //PsLookupProcessByProcessId的前N字节数组
PVOID ori_pslp = NULL;          //PsLookupProcessByProcessId的原函数

NTSTATUS Proxy_PsLookupProcessByProcessId(HANDLE ProcessId, PEPROCESS *Process)
{
    NTSTATUS st;
    st = ((PSLOOKUPPROCESSBYPROCESSID)ori_pslp)(ProcessId, Process);

    if (NT_SUCCESS(st)) {
        if (*Process == (PEPROCESS)my_eprocess) {
            *Process = 0;
            st = STATUS_ACCESS_DENIED;
        }
    }

    return st;
}

VOID HookPsLookupProcessByProcessId()
{
    pslp_head_n_byte = HookKernelApi(GetFunctionAddr(L"PsLookupProcessByProcessId"),
                                     (PVOID)Proxy_PsLookupProcessByProcessId,
                                     &ori_pslp,
                                     &pslp_patch_size);
}

VOID UnhookPsLookupProcessByProcessId()
{
    UnhookKernelApi(GetFunctionAddr(L"PsLookupProcessByProcessId"),
                    pslp_head_n_byte,
                    pslp_patch_size);
}

/*
【实例讲解】
HOOK前：
lkd> u pslookupprocessbyprocessid
nt!PsLookupProcessByProcessId:
fffff800`0194c750 48895c2408      mov     qword ptr [rsp+8],rbx
fffff800`0194c755 48896c2410      mov     qword ptr [rsp+10h],rbp
fffff800`0194c75a 4889742418      mov     qword ptr [rsp+18h],rsi
fffff800`0194c75f 57              push    rdi
fffff800`0194c760 4154            push    r12
fffff800`0194c762 4155            push    r13
fffff800`0194c764 4883ec20        sub     rsp,20h
fffff800`0194c768 65488b3c2588010000 mov   rdi,qword ptr gs:[188h]

HOOK后：
lkd> u pslookupprocessbyprocessid
nt!PsLookupProcessByProcessId:
fffff800`0194c750 ff2500000000    jmp     qword ptr [nt!PsLookupProcessByProcessId+0x6 (fffff800`0194c756)]
fffff800`0194c756 dc50e9          fcom    qword ptr [rax-17h]
fffff800`0194c759 06              ???
fffff800`0194c75a 80f8ff          cmp     al,0FFh
fffff800`0194c75d ff9057415441    call    qword ptr [rax+41544157h]
fffff800`0194c763 55              push    rbp
fffff800`0194c764 4883ec20        sub     rsp,20h
fffff800`0194c768 65488b3c2588010000 mov   rdi,qword ptr gs:[188h]

机器码看似乱了，为什么呢？
因为从我们需要patch 15字节。
除了ff2500000000是指令外，fffff800`0194c756 ～ fffff800`0194c75e之间的机器码都不是指令，而是地址（前8字节）以及填充码（最后1字节）。
所以，从fffff800`0194c75f开始看就好了：
lkd> u fffff800`0194c75f
nt!PsLookupProcessByProcessId+0xf:
fffff800`0194c75f 57              push    rdi
fffff800`0194c760 4154            push    r12
fffff800`0194c762 4155            push    r13
fffff800`0194c764 4883ec20        sub     rsp,20h
fffff800`0194c768 65488b3c2588010000 mov   rdi,qword ptr gs:[188h]
fffff800`0194c771 4533e4          xor     r12d,r12d
fffff800`0194c774 488bea          mov     rbp,rdx
fffff800`0194c777 66ff8fc4010000  dec     word ptr [rdi+1C4h]
*/
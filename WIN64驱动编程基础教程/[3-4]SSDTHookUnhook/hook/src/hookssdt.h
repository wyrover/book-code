//作者：Tesla.Angela [www.vb-asm.com]

#include <ntddk.h>

typedef struct _SYSTEM_SERVICE_TABLE {
    PVOID       ServiceTableBase;
    PVOID       ServiceCounterTableBase;
    ULONGLONG   NumberOfServices;
    PVOID       ParamTableBase;
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
    SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
    SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
    SYSTEM_SERVICE_TABLE Table3;    // not used
    SYSTEM_SERVICE_TABLE Table4;    // not used
} SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;

typedef NTSTATUS(__fastcall *NTTERMINATEPROCESS)(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus);

NTKERNELAPI
UCHAR *
PsGetProcessImageFileName(PEPROCESS Process);

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
NTTERMINATEPROCESS NtTerminateProcess = NULL;
ULONG OldTpVal;

NTSTATUS __fastcall Fake_NtTerminateProcess(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus)
{
    PEPROCESS Process;
    NTSTATUS st = ObReferenceObjectByHandle(ProcessHandle, 0, *PsProcessType, KernelMode, &Process, NULL);
    DbgPrint("Fake_NtTerminateProcess called!");

    if (NT_SUCCESS(st)) {
        if (!_stricmp(PsGetProcessImageFileName(Process), "loaddrv.exe") || !_stricmp(PsGetProcessImageFileName(Process), "calc.exe"))
            return STATUS_ACCESS_DENIED;
        else
            return NtTerminateProcess(ProcessHandle, ExitStatus);
    } else
        return STATUS_ACCESS_DENIED;
}

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

ULONGLONG GetKeServiceDescriptorTable64() //鬼佬的方法
{
    char KiSystemServiceStart_pattern[13] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00"; //睇唔明系么春特征码
    ULONGLONG CodeScanStart = (ULONGLONG)&_strnicmp;
    ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;
    UNICODE_STRING Symbol;
    ULONGLONG i, tbl_address, b;

    for (i = 0; i < CodeScanEnd - CodeScanStart; i++) {
        if (!memcmp((char*)(ULONGLONG)CodeScanStart + i, (char*)KiSystemServiceStart_pattern, 13)) {
            for (b = 0; b < 50; b++) {
                tbl_address = ((ULONGLONG)CodeScanStart + i + b);

                if (*(USHORT*)((ULONGLONG)tbl_address) == (USHORT)0x8d4c)
                    return ((LONGLONG)tbl_address + 7) + *(LONG*)(tbl_address + 3);
            }
        }
    }

    return 0;
}

ULONGLONG GetSSDTFuncCurAddr(ULONG id)
{
    LONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    dwtmp = ServiceTableBase[id];
    dwtmp = dwtmp >> 4;
    return (LONGLONG)dwtmp + (ULONGLONG)ServiceTableBase;
}

ULONG GetOffsetAddress(ULONGLONG FuncAddr)
{
    ULONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    dwtmp = (ULONG)(FuncAddr - (ULONGLONG)ServiceTableBase);
    return dwtmp << 4;
}

VOID FuckKeBugCheckEx()
{
    KIRQL irql;
    ULONGLONG myfun;
    UCHAR jmp_code[] = "\x48\xB8\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\xFF\xE0";
    myfun = (ULONGLONG)Fake_NtTerminateProcess;
    memcpy(jmp_code + 2, &myfun, 8);
    irql = WPOFFx64();
    memset(KeBugCheckEx, 0x90, 15);
    memcpy(KeBugCheckEx, jmp_code, 12);
    WPONx64(irql);
}

/*
填写KeBugCheckEx的地址
在KeBugCheckEx填写jmp，跳到Fake_NtTerminateProcess
不能直接填写Fake_NtTerminateProcess的地址，因为它们不再同一个4GB
*/
VOID HookSSDT()
{
    KIRQL irql;
    ULONGLONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;
    //get old address
    NtTerminateProcess = (NTTERMINATEPROCESS)GetSSDTFuncCurAddr(41);
    dprintf("Old_NtTerminateProcess: %llx", (ULONGLONG)NtTerminateProcess);
    //set kebugcheckex
    FuckKeBugCheckEx();
    //show new address
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    OldTpVal = ServiceTableBase[41]; //record old offset value
    irql = WPOFFx64();
    ServiceTableBase[41] = GetOffsetAddress((ULONGLONG)KeBugCheckEx);
    WPONx64(irql);
    dprintf("KeBugCheckEx: %llx", (ULONGLONG)KeBugCheckEx);
    dprintf("New_NtTerminateProcess: %llx", GetSSDTFuncCurAddr(41));
}

VOID UnhookSSDT()
{
    KIRQL irql;
    PULONG ServiceTableBase = NULL;
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    //set value
    irql = WPOFFx64();
    ServiceTableBase[41] = GetOffsetAddress((ULONGLONG)NtTerminateProcess); //OldTpVal; //直接填写这个旧值也行
    WPONx64(irql);
    //没必要恢复KeBugCheckEx的内容了，反正执行到KeBugCheckEx时已经完蛋了。
    dprintf("NtTerminateProcess: %llx", GetSSDTFuncCurAddr(41));
}
#include "stdafx.h"

NTSTATUS InitVerion()
{
    RTL_OSVERSIONINFOW osVer;
    RtlGetVersion(&osVer);
    KdPrintEx((xx, "MajorOversion:%d MinorVersion:%d\n",
               osVer.dwMajorVersion, osVer.dwMinorVersion));

    if (osVer.dwMajorVersion != 6) {
        KdPrintEx((xx, "[InitVerion] 不支持此版本的windows\n"));
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}

NTSTATUS Init_HCode()
{
    ULONG sysProc;
    UCHAR *cTemp = NULL;
    UNICODE_STRING uDllName;
    int i = 0;
    //获取ImageFileNameOffset
    sysProc = (ULONG)PsGetCurrentProcess();
    cTemp = (UCHAR*)sysProc;

    for (i = 0; i < 0x200; ++i) {
        if (*cTemp == 'S') {
            if (!_strnicmp(cTemp, "System", strlen(cTemp))) {
                ImageFileNameOffset = (ULONG)cTemp - sysProc;
                KdPrintEx((xx, "[Init_HCode] ImageFileNameOffset:0x%0x\n",
                           ImageFileNameOffset));
                break;
            }
        }

        ++cTemp;
    }

    //获取NtCreateUserProcessID
    RtlInitUnicodeString(&uDllName, L"\\Device\\HarddiskVolume1\\Windows\\System32\\ntdll.dll");
    NtCreateUserProcessID = GetSysServiceId(&uDllName, "ZwCreateUserProcess");

    if (!NtCreateUserProcessID) {
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}

ULONG Hook_SSDT(ULONG fnID, PVOID fnName)
{
    ULONG            Address;
    ULONG            RealAddress;
    Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + fnID * 4;
    RealAddress = *(ULONG*)Address;
    __asm {
        cli
        push   eax
        mov    eax, cr0
        and    eax, not 10000h
        mov    cr0, eax
        pop    eax
    }
    InterlockedExchange((LONG*)Address, (LONG)fnName);
    __asm {
        push   eax
        mov    eax, cr0
        or     eax, 10000h
        mov    cr0, eax
        pop    eax
        sti
    }
    return RealAddress;
}


VOID Unhook(ULONG fnID, ULONG RealAddr)
{
    ULONG   Address;
    KIRQL   oldIrql;
    Address = (ULONG)KeServiceDescriptorTable->ServiceTableBase + fnID * 4;
    __asm {
        cli
        mov    eax, cr0
        and    eax, not 10000h
        mov    cr0, eax
    }
    oldIrql=KeRaiseIrqlToDpcLevel();
    InterlockedExchange((LONG *)Address, RealAddr);
    KeLowerIrql(oldIrql);
    __asm {
        mov    eax, cr0
        or    eax, 10000h
        mov    cr0, eax
        sti
    }
}



NTSTATUS myNtCreateUserProcess(
    OUT PHANDLE ProcessHandle,
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK ProcessDesiredAccess,
    IN ACCESS_MASK ThreadDesiredAccess,
    IN POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL,
    IN POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL,
    IN ULONG CreateProcessFlags,
    IN ULONG CreateThreadFlags,
    IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    IN PVOID Parameter9,
    IN PVOID AttributeList
)
{
    NTSTATUS                    status = STATUS_SUCCESS;
    PCWSTR                      ProcessName = NULL;
    UNICODE_STRING              FilePath;
    pNotifyBlock                pNB = NULL;
    UNICODE_STRING              uUpperFilePath;
    WCHAR                       wFilePath[260] = {0};
    BOOLEAN                     bUACstart = FALSE;

    do {
        //
        //全局标记，用于判断进程监控功能是否打开
        //或者应用程序是否运行
        //
        if (!GFlag.bPProtect || !GFlag.bAppRun) {
            break;
        }

        _try {
            if (NULL == ProcessParameters->ImagePathName.Buffer)
            {
                break;
            }

            FilePath.Buffer = (PWSTR)ExAllocatePool(PagedPool, 512);

            if (NULL == FilePath.Buffer)
            {
                KdPrintEx((xx, "[myNtCreateUserProcess] FilePath.Buffer is NULL\n"));
                break;
            }

            FilePath.MaximumLength = 512;

            RtlCopyUnicodeString(&FilePath, &ProcessParameters->ImagePathName);

            RtlUpcaseUnicodeString(&uUpperFilePath, &FilePath, TRUE);

            RtlCopyMemory(wFilePath, uUpperFilePath.Buffer, uUpperFilePath.Length);

            RtlFreeUnicodeString(&uUpperFilePath);

            KdPrintEx((xx, "[myNtCreateUserProcess]  %ws\n", wFilePath));

            //放过PRESENTATIONFONTCACHE进程，这是.Net启动程序，
            //放过才能顺利启动应用程序
            if (wcsstr(wFilePath, L"PRESENTATIONFONTCACHE"))
            {
                ExFreePool(FilePath.Buffer);
                break;
            }


        }
        _except(EXCEPTION_EXECUTE_HANDLER) {
            KdPrintEx((xx, "[myNtCreateUserProcess] Exception\n"));
            break;
        }

        if (
            pShareMM_SYS
            && gEvent_A
            && gEvent_B
            && !KeReadStateEvent(gEvent_A)
            && !KeReadStateEvent(gEvent_B)
        ) {
            bUACstart = IsUACRunning();

            if (FALSE == bUACstart) {
                ProcessName = GetFullProcessName();
                //使用互斥体保证执行的同步性
                KeWaitForMutexObject(gMutex, Executive, KernelMode, FALSE, NULL);
                //填充自定义通知块，通过此数据结构将信息反馈给用户
                RtlZeroMemory(pShareMM_SYS, 1024);
                pNB = (pNotifyBlock)pShareMM_SYS;
                pNB->NotifyType = ProcessMonitor;

                if (ProcessName) {
                    RtlCopyMemory(pNB->CurPro, ProcessName, wcslen(ProcessName) * 2 + 2);
                }

                RtlCopyMemory(pNB->u.ProMon.R_Pro, FilePath.Buffer, FilePath.Length);
                ExFreePool(FilePath.Buffer);
                KdPrintEx((xx, "[myNtCreateUserProcess] pNB->CurPro:%ws pNB->u.ProMon.R_Pro:%ws\n",
                           pNB->CurPro, pNB->u.ProMon.R_Pro));
                //设置事件A，该事件用于通知应用程序有新的进程创建
                KeSetEvent(gEvent_A, 0, FALSE);
                KdPrintEx((xx, "[myNtCreateUserProcess] 等待用户判断…\n"));
                //等待判断结果，判断结果来自用户显示判断和智能判断
                KeWaitForSingleObject(gEvent_B, Executive, KernelMode, FALSE, NULL);
                KeClearEvent(gEvent_B);
                KdPrintEx((xx, "[myNtCreateUserProcess] 用户已判断…\n"));

                if (iChoose == 0) {
                    KeReleaseMutex(gMutex, FALSE);
                    return 0x80070000;
                }

                KeReleaseMutex(gMutex, FALSE);
            } else {
                KdPrintEx((xx, "[myNtCreateUserProcess] UAC is starting!\n"));
                ExFreePool(FilePath.Buffer);
            }
        }
    } while (FALSE);

    //调用原始服务函数
    RealNtCreateUserProcess = (fnNtCreateUserProcess)RealAddr.NtCreateUserProcess;
    status = RealNtCreateUserProcess(
                 ProcessHandle,
                 ThreadHandle,
                 ProcessDesiredAccess,
                 ThreadDesiredAccess,
                 ProcessObjectAttributes,
                 ThreadObjectAttributes,
                 CreateProcessFlags,
                 CreateThreadFlags,
                 ProcessParameters,
                 Parameter9,
                 AttributeList
             );
    return status;
}


PCWSTR GetFullProcessName()
{
    ULONG uAddr;
    PCWSTR ProcessName = NULL;
    RTL_OSVERSIONINFOW osVer;

    if (KeGetCurrentIrql() != PASSIVE_LEVEL) {
        return NULL;
    }

    _try {
        uAddr = (ULONG)PsGetCurrentProcess();

        //得到PEB
        uAddr += 0x1a8;

        uAddr = *(ULONG*)uAddr;

        // 通过peb取得RTL_USER_PROCESS_PARAMETERS
        uAddr += 0x010;

        uAddr = *(ULONG*)uAddr;

        // 在RTL_USER_PROCESS_PARAMETERS->ImagePathName保存了路径，偏移为38
        uAddr += 0x038;

        ProcessName = ((UNICODE_STRING*)uAddr)->Buffer;

    }
    _except(EXCEPTION_EXECUTE_HANDLER) {
        return NULL;
    }
    return ProcessName;
}

ULONG GetSysServiceId(IN PUNICODE_STRING uDllName, IN char* cSearchFnName)
{
    ULONG uFnAddr = 0;
    ULONG uID = 0;
    int i = 0;
    NTSTATUS ntStatus;
    HANDLE hFile;
    OBJECT_ATTRIBUTES oattr;
    IO_STATUS_BLOCK iosb;
    HANDLE hSection;
    PVOID pBaseAddr = NULL;
    SIZE_T viewSize = 0;
    ULONG uModBase = 0;
    IMAGE_DOS_HEADER* doshdr;
    IMAGE_OPTIONAL_HEADER* opthdr;
    IMAGE_EXPORT_DIRECTORY* pExportTable;
    ULONG *dwAddrFns, *dwAddrNames;
    USHORT *dwAddrNameOrdinals;
    char* cFunName;
    ULONG dwFnOrdinal;
    //
    //首先将目标dll映射到内存中
    //
    InitializeObjectAttributes(&oattr, uDllName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE ,
                               NULL, NULL);
    ntStatus = ZwOpenFile(&hFile, GENERIC_READ, &oattr,
                          &iosb, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SYNCHRONOUS_IO_ALERT);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetSysServiceId] ZwOpenFile Failure,err:0x%08x\n",
                   ntStatus));
        return 0;
    }

    ntStatus = ZwCreateSection(&hSection, SECTION_MAP_READ | SECTION_MAP_WRITE, NULL, 0,
                               PAGE_READWRITE, 0x1000000, hFile);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetSysServiceId] ZwCreateSection Failure,err:0x%08x\n",
                   ntStatus));
        ZwClose(hFile);
        return 0;
    }

    ntStatus = ZwMapViewOfSection(hSection, NtCurrentProcess(), &pBaseAddr,
                                  0, 1024, 0, &viewSize, ViewShare, MEM_TOP_DOWN, PAGE_READWRITE);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetSysServiceId] ZwMapViewOfSection Failure,err:0x%08x\n",
                   ntStatus));
        ZwClose(hFile);
        ZwClose(hSection);
        return 0;
    }

    uModBase = (ULONG)pBaseAddr;

    //
    //从目标文件的导出表中找到待查询函数的地址，
    //服务号即此地址加一处的四个字节
    //

    do {
        doshdr = (IMAGE_DOS_HEADER*)uModBase;

        if (NULL == doshdr) {
            break;
        }

        opthdr = (IMAGE_OPTIONAL_HEADER*)(uModBase + doshdr->e_lfanew + 24);

        if (NULL == opthdr) {
            break;
        }

        pExportTable = (IMAGE_EXPORT_DIRECTORY*)(uModBase +
                       opthdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

        if (NULL == pExportTable) {
            break;
        }

        dwAddrFns = (ULONG*)(uModBase + pExportTable->AddressOfFunctions);
        dwAddrNames = (ULONG*)(uModBase + pExportTable->AddressOfNames);
        dwAddrNameOrdinals = (USHORT*)(uModBase + pExportTable->AddressOfNameOrdinals);

        for (i = 0; i < pExportTable->NumberOfNames; ++i) {
            cFunName = (char*)(uModBase + dwAddrNames[i]);

            if (!_strnicmp(cSearchFnName, cFunName, strlen(cSearchFnName))) {
                dwFnOrdinal = pExportTable->Base + dwAddrNameOrdinals[i] - 1;
                uFnAddr = uModBase + dwAddrFns[dwFnOrdinal];
                uID = *(ULONG*)((ULONG)uFnAddr + 1);
                break;
            }
        }
    } while (FALSE);

    ZwUnmapViewOfSection(NtCurrentProcess(), pBaseAddr);
    ZwClose(hFile);
    ZwClose(hSection);
    return uID;
}

BOOLEAN IsUACRunning()
{
#define ActiveProcessLinksOffset 0x0b8
    PVOID eProcess = NULL, eCurProcess = NULL;
    char *cImageName;
    eCurProcess = eProcess = PsGetCurrentProcess();

    do {
        cImageName = (char*)((ULONG)eProcess + ImageFileNameOffset);

        if (!_strnicmp(cImageName, "consent.exe", strlen(cImageName))) {
            return TRUE;
        }

        eProcess = (PVOID)(*(ULONG*)((ULONG)eProcess + ActiveProcessLinksOffset) - ActiveProcessLinksOffset);
    } while (eProcess != eCurProcess);

    return FALSE;
}
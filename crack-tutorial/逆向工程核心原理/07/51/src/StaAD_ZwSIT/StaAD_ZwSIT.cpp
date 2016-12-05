#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void DetachDebugger()
{
    typedef enum _THREAD_INFORMATION_CLASS {
        ThreadBasicInformation,
        ThreadTimes,
        ThreadPriority,
        ThreadBasePriority,
        ThreadAffinityMask,
        ThreadImpersonationToken,
        ThreadDescriptorTableEntry,
        ThreadEnableAlignmentFaultFixup,
        ThreadEventPair,
        ThreadQuerySetWin32StartAddress,
        ThreadZeroTlsCell,
        ThreadPerformanceCount,
        ThreadAmILastThread,
        ThreadIdealProcessor,
        ThreadPriorityBoost,
        ThreadSetTlsArrayAddress,
        ThreadIsIoPending,
        ThreadHideFromDebugger           // 17 (0x11)
    } THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

    typedef NTSTATUS (WINAPI* ZWSETINFORMATIONTHREAD)(
        HANDLE ThreadHandle,
        THREAD_INFORMATION_CLASS ThreadInformationClass,
        PVOID ThreadInformation,
        ULONG ThreadInformationLength
    );

    ZWSETINFORMATIONTHREAD pZwSetInformationThread = NULL;
    pZwSetInformationThread = (ZWSETINFORMATIONTHREAD)
                              GetProcAddress(GetModuleHandle(L"ntdll.dll"), 
                                             "ZwSetInformationThread");

    pZwSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, 0);

    printf("ZwSetInformationThread() -> Debugger detached!!!\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    DetachDebugger();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}
#include "stdio.h"
#include "windows.h"
#include "tchar.h"

enum PROCESSINFOCLASS
{
    ProcessBasicInformation = 0,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort = 7,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    MaxProcessInfoClass,
    ProcessWow64Information = 26,
    ProcessImageFileName = 27,
    ProcessDebugObjectHandle = 30,
    ProcessDebugFlags = 31,
    SystemKernelDebuggerInformation = 35
};

void MyNtQueryInformationProcess()
{
    typedef NTSTATUS (WINAPI *NTQUERYINFORMATIONPROCESS)(
        HANDLE ProcessHandle,
        PROCESSINFOCLASS ProcessInformationClass,
        PVOID ProcessInformation,
        ULONG ProcessInformationLength,
        PULONG ReturnLength
    );

    NTQUERYINFORMATIONPROCESS pNtQueryInformationProcess = NULL;

    pNtQueryInformationProcess = (NTQUERYINFORMATIONPROCESS)
                                 GetProcAddress(GetModuleHandle(L"ntdll.dll"), 
                                                "NtQueryInformationProcess");

    // ProcessDebugPort (0x7)
    DWORD dwDebugPort = 0;
    pNtQueryInformationProcess(GetCurrentProcess(),
                               ProcessDebugPort,
                               &dwDebugPort,
                               sizeof(dwDebugPort),
                               NULL);
    printf("NtQueryInformationProcess(ProcessDebugPort) = 0x%X\n", dwDebugPort);
    if( dwDebugPort != 0x0  )  printf("  => Debugging!!!\n\n");
    else                       printf("  => Not debugging...\n\n");

    // ProcessDebugObjectHandle (0x1E)
    HANDLE hDebugObject = NULL;
    pNtQueryInformationProcess(GetCurrentProcess(),
                               ProcessDebugObjectHandle,
                               &hDebugObject,
                               sizeof(hDebugObject),
                               NULL);
    printf("NtQueryInformationProcess(ProcessDebugObjectHandle) = 0x%X\n", hDebugObject);
    if( hDebugObject != 0x0  )  printf("  => Debugging!!!\n\n");
    else                        printf("  => Not debugging...\n\n");

    // ProcessDebugFlags (0x1F)
    BOOL bDebugFlag = TRUE;
    pNtQueryInformationProcess(GetCurrentProcess(),
                               ProcessDebugFlags,
                               &bDebugFlag,
                               sizeof(bDebugFlag),
                               NULL);
    printf("NtQueryInformationProcess(ProcessDebugFlags) = 0x%X\n", bDebugFlag);
    if( bDebugFlag == 0x0  )  printf("  => Debugging!!!\n\n");
    else                      printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    MyNtQueryInformationProcess();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}
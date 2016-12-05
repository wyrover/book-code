#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void MyNtQuerySystemInformation()
{
    typedef NTSTATUS (WINAPI *NTQUERYSYSTEMINFORMATION)(
        ULONG SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength
    );

    typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION 
    {
        BOOLEAN DebuggerEnabled;
        BOOLEAN DebuggerNotPresent;
    } SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

    NTQUERYSYSTEMINFORMATION NtQuerySystemInformation;
  
    NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)  
                                GetProcAddress(GetModuleHandle(L"ntdll"), 
                                               "NtQuerySystemInformation");

    ULONG SystemKernelDebuggerInformation = 0x23;
    ULONG ulReturnedLength = 0;
    SYSTEM_KERNEL_DEBUGGER_INFORMATION DebuggerInfo = {0,};

    NtQuerySystemInformation(SystemKernelDebuggerInformation, 
                             (PVOID) &DebuggerInfo, 
                             sizeof(DebuggerInfo),      // 2 bytes
                             &ulReturnedLength);

    printf("NtQuerySystemInformation(SystemKernelDebuggerInformation) = 0x%X 0x%X\n", 
           DebuggerInfo.DebuggerEnabled, DebuggerInfo.DebuggerNotPresent);
    if( DebuggerInfo.DebuggerEnabled )  printf("  => Debugging!!!\n\n");
    else                                printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    // boot.ini 의 /debug 옵션을 체크
    MyNtQuerySystemInformation();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}
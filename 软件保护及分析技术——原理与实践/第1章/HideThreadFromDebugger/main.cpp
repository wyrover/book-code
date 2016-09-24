#include <windows.h>
#include <iostream>

typedef NTSTATUS (NTAPI* LPNtSetInformationThread)(
        IN HANDLE hThread,
        IN INT/*THREADINFOCLASS*/ ThreadInfoClass,
        IN PVOID ThreadInfoBuffer,
        IN ULONG ThreadInfoBufferLength
        );
LPNtSetInformationThread glpNtSetInformationThread;

int main()
{
    LPNtSetInformationThread lpNtSetInformationThread =
            (LPNtSetInformationThread)GetProcAddress(
                GetModuleHandleA("NTDLL"),"NtSetInformationThread");

    lpNtSetInformationThread(GetCurrentThread(),17/*ThreadHideFromDebugger*/,
                0,0);
    DebugBreak();
    return 0;
}


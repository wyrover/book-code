#include <windows.h>

#pragma comment(linker, "/INCLUDE:__tls_used")

void print_console(char* szMsg)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteConsoleA(hStdout, szMsg, strlen(szMsg), NULL, NULL);
}

void NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    char szMsg[80] = {0,};
    wsprintfA(szMsg, "TLS_CALLBACK1() : DllHandle = %X, Reason = %d\n", DllHandle, Reason);
    print_console(szMsg);
}

void NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
    char szMsg[80] = {0,};
    wsprintfA(szMsg, "TLS_CALLBACK2() : DllHandle = %X, Reason = %d\n", DllHandle, Reason);
    print_console(szMsg);
}

#pragma data_seg(".CRT$XLX")
    PIMAGE_TLS_CALLBACK pTLS_CALLBACKs[] = { TLS_CALLBACK1, TLS_CALLBACK2, 0 };
#pragma data_seg()

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    print_console("ThreadProc() start\n");

    print_console("ThreadProc() end\n");

    return 0;
}

int main(void)
{
    HANDLE hThread = NULL;

    print_console("main() start\n");

    hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    WaitForSingleObject(hThread, 60*1000);
    CloseHandle(hThread);

    print_console("main() end\n");

    return 0;
}

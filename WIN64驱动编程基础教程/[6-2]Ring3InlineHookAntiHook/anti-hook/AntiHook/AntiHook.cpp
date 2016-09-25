// AntiHook.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef HANDLE(__stdcall *MYOPENPROCESS)(DWORD, BOOL, DWORD);

int _tmain(int argc, _TCHAR* argv[])
{
    DWORD dwpid = 0;
    DWORD mypid = GetCurrentProcessId();
    printf("My PID is %ld\n", mypid);
    printf("Inject [hookdll.dll] to this process, then input PID of [calc.exe]: ");
    scanf("%ld", &dwpid);
    HANDLE h = OpenProcess(1, 0, dwpid);
    printf("Process Handle=%ld\n", h);
    TerminateProcess(h, 0);
    getchar();
    printf("Reload kernelbase.dll now!");
    getchar();
    CopyFileW(L"c:\\windows\\system32\\kernelbase.dll", L"c:\\basekernel.dll", 0);
    HMODULE hlib = LoadLibraryW(L"c:\\basekernel.dll");

    if (hlib == 0) {
        printf("Cannot load [c:\\basekernel.dll]!");
        getchar();
        return 0;
    }

    MYOPENPROCESS MyOpenProcess = (MYOPENPROCESS)GetProcAddress(hlib, "OpenProcess");
    h = MyOpenProcess(1, 0, dwpid);
    printf("Process Handle=%ld\n", h);
    TerminateProcess(h, 0);
    CloseHandle(h);
    FreeLibrary(hlib);
    DeleteFileW(L"c:\\basekernel.dll");
    getchar();
    return 0;
}
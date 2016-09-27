// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL (__cdecl *HookFunction)(ULONG_PTR OriginalFunction, ULONG_PTR NewFunction);
VOID (__cdecl *UnhookFunction)(ULONG_PTR Function);
ULONG_PTR(__cdecl *GetOriginalFunction)(ULONG_PTR Hook);
typedef LONG(__stdcall *NTRESUMEPROCESS)(HANDLE ProcessHandle);
NTRESUMEPROCESS NtResumeProcess;
typedef HWND (WINAPI *FINDWINDOWW)
(
    __in_opt  LPCTSTR lpClassName,
    __in_opt  LPCTSTR lpWindowName
);
typedef DWORD (WINAPI *GETWINDOWTHREADPROCESSID)
(
    __in       HWND hWnd,
    __out_opt  LPDWORD lpdwProcessId
);
FINDWINDOWW pfnFindWindowW;
GETWINDOWTHREADPROCESSID pfnGetWindowThreadProcessId;

BOOL WINAPI InjectProxyW(DWORD dwPID, PCWSTR pwszProxyFile)
{
    BOOL ret = FALSE;
    HANDLE hToken = NULL;
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    FARPROC pfnThreadRtn = NULL;
    PWSTR pwszPara = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    pfnThreadRtn = GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
    size_t iProxyFileLen = wcslen(pwszProxyFile) * sizeof(WCHAR); //May be in your case iProxyFileLen containes invalid value.
    pwszPara = (PWSTR)VirtualAllocEx(hProcess, NULL, iProxyFileLen, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pwszPara, (PVOID)pwszProxyFile, iProxyFileLen, NULL);
    hThread = CreateRemoteThread(hProcess, NULL, 1024, (LPTHREAD_START_ROUTINE)pfnThreadRtn, pwszPara, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pwszPara, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return (TRUE);
}

HANDLE Fake_OpenProcess(DWORD da, BOOL ih, DWORD PId)
{
    HWND myhwnd = 0;
    DWORD mypid = 0;
    HANDLE(WINAPI * pOpenProcess)(DWORD da, BOOL ih, DWORD PId);
    pOpenProcess = (HANDLE(WINAPI *)(DWORD, BOOL, DWORD))GetOriginalFunction((ULONG_PTR)Fake_OpenProcess);
    myhwnd = pfnFindWindowW(L"CalcFrame", L"Calculator");

    if (myhwnd == 0)
        myhwnd = pfnFindWindowW(L"CalcFrame", L"计算器");

    if (myhwnd == 0) {
        return pOpenProcess(da, ih, PId);
    } else {
        pfnGetWindowThreadProcessId(myhwnd, &mypid);

        if (PId == mypid)
            return pOpenProcess(da, ih, 0); //set pid as 0
        else
            return pOpenProcess(da, ih, PId);
    }
}

HANDLE Fake_OpenThread(DWORD da, BOOL ih, DWORD TId)
{
    HWND myhwnd = 0;
    DWORD mypid = 0, mytid = 0;
    HANDLE(WINAPI * pOpenThread)(DWORD da, BOOL ih, DWORD TId);
    pOpenThread = (HANDLE(WINAPI *)(DWORD, BOOL, DWORD))GetOriginalFunction((ULONG_PTR)Fake_OpenThread);
    myhwnd = pfnFindWindowW(L"CalcFrame", L"Calculator");

    if (myhwnd == 0)
        myhwnd = pfnFindWindowW(L"CalcFrame", L"计算器");

    if (myhwnd == 0) {
        return pOpenThread(da, ih, TId);
    } else {
        mytid = pfnGetWindowThreadProcessId(myhwnd, &mypid);

        if (TId == mytid)
            return pOpenThread(da, ih, 0); //set tid as 0
        else
            return pOpenThread(da, ih, TId);
    }
}

BOOL WINAPI Fake_CreateProcessW
(
    __in_opt     LPCWSTR lpApplicationName,
    __inout_opt  LPWSTR lpCommandLine,
    __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
    __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
    __in         BOOL bInheritHandles,
    __in         DWORD dwCreationFlags,
    __in_opt     LPVOID lpEnvironment,
    __in_opt     LPCWSTR lpCurrentDirectory,
    __in         LPSTARTUPINFO lpStartupInfo,
    __out        LPPROCESS_INFORMATION lpProcessInformation
)
{
    BOOL cpwret;//char szpid[5]={0,0,0,0,0};
    BOOL (WINAPI * pCreateProcessW)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
    pCreateProcessW = (BOOL (WINAPI *)(LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION))GetOriginalFunction((ULONG_PTR)Fake_CreateProcessW);
    cpwret = pCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
    //itoa(lpProcessInformation->dwProcessId,szpid,10);
    //MessageBoxA(0,szpid,"New PID",0);
    InjectProxyW(lpProcessInformation->dwProcessId, L"C:\\HOOKDLL.DLL");
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, lpProcessInformation->dwProcessId);
    NtResumeProcess(hp);
    CloseHandle(hp);
    return cpwret;
}

void InitHook()
{
    HMODULE hHookEngineDll = LoadLibraryW(L"C:\\NtHookEngine.dll");
    HookFunction = (BOOL (__cdecl *)(ULONG_PTR, ULONG_PTR))GetProcAddress(hHookEngineDll, "HookFunction");
    UnhookFunction = (VOID (__cdecl *)(ULONG_PTR))GetProcAddress(hHookEngineDll, "UnhookFunction");
    GetOriginalFunction = (ULONG_PTR(__cdecl *)(ULONG_PTR))GetProcAddress(hHookEngineDll, "GetOriginalFunction");

    if (HookFunction == NULL || UnhookFunction == NULL || GetOriginalFunction == NULL)
        return;

    HookFunction((ULONG_PTR) GetProcAddress(GetModuleHandleW(L"kernelbase.dll"), "OpenProcess"), (ULONG_PTR) &Fake_OpenProcess);
    //HookFunction((ULONG_PTR) GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"OpenThread"), (ULONG_PTR) &Fake_OpenThread);
    HookFunction((ULONG_PTR) GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CreateProcessW"), (ULONG_PTR) &Fake_CreateProcessW);
    NtResumeProcess = (NTRESUMEPROCESS)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtResumeProcess");
    pfnFindWindowW = (FINDWINDOWW)GetProcAddress(LoadLibraryW(L"user32.dll"), "FindWindowW");
    pfnGetWindowThreadProcessId = (GETWINDOWTHREADPROCESSID)GetProcAddress(LoadLibraryW(L"user32.dll"), "GetWindowThreadProcessId");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        InitHook();
        //MessageBoxW(0,L"Init Over!",L"MiniHookEngine",0);
        break;
    }

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH: {
        UnhookFunction((ULONG_PTR)GetProcAddress(GetModuleHandleW(L"kernelbase.dll"), "OpenProcess"));
        //UnhookFunction( (ULONG_PTR)GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"OpenThread") );
        UnhookFunction((ULONG_PTR)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CreateProcessW"));
        break;
    }
    }

    return TRUE;
}
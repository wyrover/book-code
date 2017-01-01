// HookSvr.h

#pragma once
#include <Windows.h>

#include <stdio.h>


#include<Tlhelp32.h>


HANDLE WINAPI MyCreateFileW(
    __in     LPCWSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
);

typedef HANDLE(WINAPI *ptrCreateFileW)(
    __in     LPCWSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
);

extern ptrCreateFileW realCreateFileW;

HANDLE WINAPI MyCreateFileA(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
);

typedef HANDLE(WINAPI *ptrCreateFileA)(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
);

extern ptrCreateFileA realCreateFileA;


BOOL RtlFileExists(WCHAR* InPath)
{
    HANDLE          hFile;

    if ((hFile = CreateFileW(InPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
        return FALSE;

    CloseHandle(hFile);
    return TRUE;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                          &hToken)) {
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL,             // lookup privilege on local system
                              lpszPrivilege,    // privilege to lookup
                              &luid)) {         // receives LUID of privilege
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;

    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.
    if (!AdjustTokenPrivileges(hToken,
                               FALSE,
                               &tp,
                               sizeof(TOKEN_PRIVILEGES),
                               (PTOKEN_PRIVILEGES) NULL,
                               (PDWORD) NULL)) {
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        //The token does not have the specified privilege.
        return FALSE;
    }

    return TRUE;
}

typedef DWORD (WINAPI *PFNTCREATETHREADEX)
(
    PHANDLE                 ThreadHandle,
    ACCESS_MASK             DesiredAccess,
    LPVOID                  ObjectAttributes,
    HANDLE                  ProcessHandle,
    LPTHREAD_START_ROUTINE  lpStartAddress,
    LPVOID                  lpParameter,
    BOOL                   CreateSuspended,
    DWORD                   dwStackSize,
    DWORD                   dw1,
    DWORD                   dw2,
    LPVOID                  Unknown
);

BOOL MyCreateRemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pRemoteBuf)
{
    HANDLE      hThread = NULL;
    FARPROC     pFunc = NULL;
    BOOL bHook;
    // 判断系统版本
    OSVERSIONINFO osvi;
    //BOOL bIsWindowsXPorLater;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    if (osvi.dwMajorVersion == 6) {
        bHook = TRUE;
    } else {
        bHook = FALSE;
    }

    if (bHook) { // Vista, 7, Server2008
        pFunc = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");

        if (pFunc == NULL) {
            //GetLastError());
            return FALSE;
        }

        OutputDebugString(L"MyCreateRemoteThread");
        ((PFNTCREATETHREADEX)pFunc)(&hThread,
                                    0x1FFFFF,
                                    NULL,
                                    hProcess,
                                    pThreadProc,
                                    pRemoteBuf,
                                    FALSE,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);

        if (hThread == NULL) {
            return FALSE;
        }
    } else {                // 2000, XP, Server2003
        hThread = CreateRemoteThread(hProcess,
                                     NULL,
                                     0,
                                     pThreadProc,
                                     pRemoteBuf,
                                     0,
                                     NULL);

        if (hThread == NULL) {
            return FALSE;
        }
    }

    if (WAIT_FAILED == WaitForSingleObject(hThread, INFINITE)) {
        return FALSE;
    }

    return TRUE;
}

BOOL InjectDll(DWORD dwPID, const wchar_t *szDllName)
{
    HANDLE hProcess = NULL;
    LPVOID pRemoteBuf = NULL;
    FARPROC pThreadProc = NULL;
    DWORD dwBufSize = wcslen(szDllName) * sizeof(wchar_t) + 2;

    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) {
        return FALSE;
    }

    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize,
                                MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName,
                       dwBufSize, NULL);
    pThreadProc = GetProcAddress(GetModuleHandle(L"kernel32.dll"),
                                 "LoadLibraryW");

    if (!MyCreateRemoteThread(hProcess, (LPTHREAD_START_ROUTINE)pThreadProc, pRemoteBuf)) {
        return FALSE;
    }

    VirtualFreeEx(hProcess, pRemoteBuf, dwBufSize, MEM_RELEASE);
    CloseHandle(hProcess);
    return TRUE;
}

int DoInject(DWORD aPid, const WCHAR *aFullpath)
{
    if (wcslen(aFullpath) <= 0) {
        return -1;
    }

    //判断dll是否存在
    HANDLE hFile = CreateFile(aFullpath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwsize = GetFileSize(hFile, NULL);
        CloseHandle(hFile);

        if (dwsize < 10) {
            return -2;
        }
    } else {
        return -3;
    }

    BOOL bSuc = SetPrivilege(SE_DEBUG_NAME, TRUE);
    bSuc = InjectDll((DWORD)aPid, aFullpath);

    if (bSuc) {
        return -4;
    }

    return 0;
}

DWORD GetProcessID(WCHAR *FileName)
{
    HANDLE myhProcess;
    PROCESSENTRY32 mype;
    mype.dwSize = sizeof(PROCESSENTRY32);
    BOOL mybRet;
    mype.dwSize = sizeof(PROCESSENTRY32);
    //进行进程快照
    myhProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //TH32CS_SNAPPROCESS快照所有进程
    //开始进程查找
    mybRet = Process32First(myhProcess, &mype);

    //循环比较，得出ProcessID
    while (mybRet) {
        if (wcscmp(FileName, mype.szExeFile) == 0)
            return mype.th32ProcessID;
        else
            mybRet = Process32Next(myhProcess, &mype);
    }

    return 0;
}



int __cdecl main(int argc, TCHAR * argv[])
{
    DWORD Pid;
    Pid = GetProcessID(L"wtl_dialog_basic.exe");
    printf("Pid: %d.\n", Pid);

    //if(argc<3)
    //{
    //  printf("Usage is : %s <pid> <dll name>\n");
    //  return 0;
    //}

    //Pid=wcstoul(argv[1], NULL, 10);
    if (Pid == 0 || Pid == 4) {
        printf("inject error!\n", argv[1]);
        getchar();
        return -1;
    }

    //InjectDll(Pid, argv[2]);
    //int result = DoInject(Pid, argv[2]/*L"E:\\src\\easyhook\\trunk\\Debug\\x86\\HookSvr.dll"*/);
    int result = DoInject(Pid, L"hookdll.dll");
    //int result = DoInject(Pid, L"D:\\Visual Studio 2005\\Projects\\hookdll\\Debug\\hookdll.dll");

    if (result == -4)
        printf("[+] hookdll.dll successfully injected !\n");
    else
        printf("inject error??\n");

    getchar();
    return 0;
}
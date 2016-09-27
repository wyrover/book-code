// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL (__cdecl *HookFunction)(ULONG_PTR OriginalFunction, ULONG_PTR NewFunction);
VOID (__cdecl *UnhookFunction)(ULONG_PTR Function);
ULONG_PTR(__cdecl *GetOriginalFunction)(ULONG_PTR Hook);
typedef LONG(__stdcall *NTRESUMEPROCESS)(HANDLE ProcessHandle);
typedef struct _UNICODE_STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
typedef struct _OBJECT_ATTRIBUTES {
    ULONG  Length;
    HANDLE  RootDirectory;
    PUNICODE_STRING  ObjectName;
    ULONG  Attributes;
    PVOID  SecurityDescriptor;
    PVOID  SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef CONST OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;
typedef struct _IO_STATUS_BLOCK {
    union xxx {
        long  Status;
        PVOID  Pointer;
    };
    ULONG_PTR  Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
typedef long (__stdcall *NTCREATEFILE)(
    PHANDLE  FileHandle,
    ACCESS_MASK  DesiredAccess,
    POBJECT_ATTRIBUTES  ObjectAttributes,
    PIO_STATUS_BLOCK  IoStatusBlock,
    PLARGE_INTEGER  AllocationSize,
    ULONG  FileAttributes,
    ULONG  ShareAccess,
    ULONG  CreateDisposition,
    ULONG  CreateOptions,
    PVOID  EaBuffer,
    ULONG  EaLength);
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
NTRESUMEPROCESS NtResumeProcess;
NTCREATEFILE NtCreateFile;

#define BUFSIZE 512

LPWSTR GetFileNameFromHandleW3(HANDLE hFile, LPWSTR lpFilePath)
{
    const int FileNameInformation = 9;  // enum FILE_INFORMATION_CLASS;
    typedef struct _IO_STATUS_BLOCK {
        union {
            ULONG  Status;   // NTSTATUS
            PVOID  Pointer;
        };
        ULONG_PTR  Information;
    } IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;  // Defined in Wdm.h
    typedef struct _FILE_NAME_INFORMATION {
        ULONG  FileNameLength;
        WCHAR  FileName[MAX_PATH];
    } FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;
    typedef LONG(CALLBACK * ZWQUERYINFORMATIONFILE)(
        HANDLE  FileHandle,
        IO_STATUS_BLOCK * IoStatusBlock,
        PVOID  FileInformation,
        ULONG  Length,
        ULONG  FileInformationClass
    );
    lpFilePath[0] = 0x00;
    HMODULE hNtDLL = LoadLibraryW(L"ntdll.dll");

    if (hNtDLL == 0x00)                 {
        return 0x00;
    }

    ZWQUERYINFORMATIONFILE ZwQueryInformationFile = (ZWQUERYINFORMATIONFILE)GetProcAddress(hNtDLL, "ZwQueryInformationFile");

    if (ZwQueryInformationFile == 0x00) {
        return 0x00;
    }

    FILE_NAME_INFORMATION fni;
    IO_STATUS_BLOCK isb;

    if (ZwQueryInformationFile(hFile, &isb, &fni, sizeof(fni), FileNameInformation) != 0)            {
        return 0x00;
    }

    fni.FileName[fni.FileNameLength / sizeof(WCHAR)] = 0x00;
    BY_HANDLE_FILE_INFORMATION fi;

    if (!GetFileInformationByHandle(hFile, &fi) || (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  {
        return 0x00;
    }

    WCHAR  szDrive   [MAX_PATH];
    WCHAR *lpDrive = szDrive;
    int    iPathLen;

    if (GetLogicalDriveStringsW(MAX_PATH - 1, szDrive) >= MAX_PATH)                                  {
        return 0x00;
    }

    while ((iPathLen = lstrlenW(lpDrive)) != 0) {
        DWORD dwVolumeSerialNumber;

        if (!GetVolumeInformation(lpDrive, NULL, NULL, &dwVolumeSerialNumber, NULL, NULL, NULL, NULL)) {
            return 0x00;
        }

        if (dwVolumeSerialNumber == fi.dwVolumeSerialNumber) {
            lstrcpynW(lpFilePath, lpDrive, lstrlen(lpDrive));
            lstrcatW(lpFilePath, fni.FileName);
            break;
        }

        lpDrive += iPathLen + 1;
    }

    return lpFilePath;
}

void PWSTR2PCHAR(LPCWSTR tc, PCHAR c)
{
    WideCharToMultiByte(CP_ACP, 0, tc, -1, c, wcslen(tc), 0, 0);
    c[wcslen(tc)] = 0;
}

BOOL __stdcall Fake_ZwReadFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event  OPTIONAL,
    IN PVOID  ApcRoutine  OPTIONAL,
    IN PVOID  ApcContext  OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    OUT PVOID  Buffer,
    IN ULONG  Length,
    IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
    IN PULONG  Key  OPTIONAL)
{
    long (WINAPI * pZwReadFile)(
        IN HANDLE  FileHandle,
        IN HANDLE  Event  OPTIONAL,
        IN PVOID  ApcRoutine  OPTIONAL,
        IN PVOID  ApcContext  OPTIONAL,
        OUT PIO_STATUS_BLOCK  IoStatusBlock,
        OUT PVOID  Buffer,
        IN ULONG  Length,
        IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
        IN PULONG  Key  OPTIONAL);
    pZwReadFile = (long (WINAPI *)(HANDLE, HANDLE, PVOID, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG))GetOriginalFunction((ULONG_PTR)Fake_ZwReadFile);
    char lszsrc[522];
    char dllname[] = "kernelbase.dll";
    char dllname2[] = "ntdll.dll";
    char *ptrx, *ptry;
    WCHAR wszsrc[MAX_PATH + 1];
    GetFileNameFromHandleW3(FileHandle, wszsrc);
    PWSTR2PCHAR(wszsrc, lszsrc);
    ptrx = strstr(strlwr(lszsrc), dllname);
    ptry = strstr(strlwr(lszsrc), dllname2);

    if (ptrx != NULL || ptry != NULL)
        return 0x80070000;
    else
        return pZwReadFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);
}

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

/*HANDLE Fake_OpenThread(DWORD da, BOOL ih, DWORD TId)
{
    HWND myhwnd=0;
    DWORD mypid=0,mytid=0;
    HANDLE (WINAPI *pOpenThread)(DWORD da, BOOL ih, DWORD TId);
    pOpenThread=(HANDLE (WINAPI *)(DWORD,BOOL,DWORD))GetOriginalFunction( (ULONG_PTR)Fake_OpenThread );
    myhwnd=FindWindowW(L"CalcFrame",L"Calculator");
    if (myhwnd==0)
        myhwnd=FindWindowW(L"CalcFrame",L"计算器");
    if (myhwnd==0)
    {
        return pOpenThread(da,ih,TId);
    }
    else
    {
        mytid=GetWindowThreadProcessId(myhwnd,&mypid);
        if (TId==mytid)
            return pOpenThread(da,ih,0); //set tid as 0
        else
            return pOpenThread(da,ih,TId);
    }
}*/

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
    HookFunction((ULONG_PTR) GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwReadFile"), (ULONG_PTR) &Fake_ZwReadFile);
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
        UnhookFunction((ULONG_PTR)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwReadFile"));
        break;
    }
    }

    return TRUE;
}
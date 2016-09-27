#include <stdio.h>
#include <windows.h>

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

int main()
{
    WCHAR dllname[MAX_PATH];
    DWORD dwPID = 0;
    printf("input pid: ");
    scanf("%ld", &dwPID);
    printf("input dll full path: ");
    scanf("%ws", dllname);
    //InjectProxyW(dwPID,L"C:\\DLL_TEST.DLL");
    InjectProxyW(dwPID, dllname);
    getchar();
    printf("inject over!");
    getchar();
    return 0;
}
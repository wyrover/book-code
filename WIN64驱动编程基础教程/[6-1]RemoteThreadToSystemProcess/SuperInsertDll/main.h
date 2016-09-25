
#ifndef NT_SUCCESS
    #define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif
#ifndef STATUS_SUCCESS
    #define STATUS_SUCCESS       ((NTSTATUS)0x00000000L)
#endif
#ifndef STATUS_UNSUCCESSFUL
    #define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef void* (__fastcall *LPFN_KernelBaseGetGlobalData)(void);

BOOL WINAPI InjectDllExW(DWORD dwPID, PCWSTR pwszProxyFile)
{
    BOOL ret = FALSE;
    HANDLE hToken = NULL;
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    FARPROC pfnThreadRtn = NULL;
    PWSTR pwszPara = NULL;
    PVOID pRemoteShellcode = NULL;
    CLIENT_ID Cid = {0};
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if (!hProcess)
        return FALSE;

    //Get Function Address
    pfnThreadRtn = GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "LoadLibraryW");
    //Set String to remote process
    size_t iProxyFileLen = wcslen(pwszProxyFile) * sizeof(WCHAR);
    pwszPara = (PWSTR)VirtualAllocEx(hProcess, NULL, iProxyFileLen, MEM_COMMIT, PAGE_READWRITE);

    if (!pwszPara)
        return FALSE;

    WriteProcessMemory(hProcess, pwszPara, (PVOID)pwszProxyFile, iProxyFileLen, NULL);
    //Start patch
    LPFN_KernelBaseGetGlobalData pKernelBaseGetGlobalData = NULL;
    UCHAR* pGlobalData = NULL;
    UCHAR* pMisc = NULL;
    ULONG PatchOffset = 0;
    pKernelBaseGetGlobalData = (LPFN_KernelBaseGetGlobalData)GetProcAddress(LoadLibraryW(L"KernelBase.dll"), "KernelBaseGetGlobalData");
    pGlobalData = (UCHAR*)pKernelBaseGetGlobalData();
    OSVERSIONINFOA osi = {0};
    osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    GetVersionEx(&osi);

    //Get patch position by build number
    switch (osi.dwBuildNumber) {
    /*
    KERNELBASE!CreateRemoteThreadEx+0x224:
    000007fe`fdb1b184 803db156050000  cmp     byte ptr [KERNELBASE!KernelBaseGlobalData+0x5c (000007fe`fdb7083c)],0
    */
    case 7600:
    case 7601: {
        PatchOffset = 0x5C;
        break;
    }

    /*
    KERNELBASE!CreateRemoteThreadEx+0x1a8:
    000007fa`7859ef28 44380d35470b00  cmp     byte ptr [KERNELBASE!KernelBaseGlobalData+0x4 (000007fa`78653664)],r9b
    */
    case 9200: {
        PatchOffset = 0x4;
        break;
    }

    default:
        break;
    }

    printf("PatchOffset: %x\n", PatchOffset);
    pMisc =  pGlobalData + PatchOffset;
    *pMisc = 1;
    //Create remote thread
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnThreadRtn, pwszPara, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pwszPara, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    return TRUE;
}
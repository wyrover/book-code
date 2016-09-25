#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define KEY_WOW64_64KEY 0x0100

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

typedef VOID (WINAPI *GETNATIVESYSTEMINFO)(LPSYSTEM_INFO);
GETNATIVESYSTEMINFO GetNativeSystemInfo;

typedef BOOLEAN(WINAPI *WOW64ENABLEWOW64FSREDIRECTION)(BOOLEAN);
WOW64ENABLEWOW64FSREDIRECTION Wow64EnableWow64FsRedirection;

BOOL IsWow64()
{
    //From: http://msdn.microsoft.com/en-us/library/ms684139(VS.85).aspx
    BOOL bIsWow64 = FALSE;
    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.
    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
                           GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
            //handle error
        }
    }

    if (bIsWow64)
        _tprintf(TEXT("The process is running under WOW64.\n"));
    else
        _tprintf(TEXT("The process is not running under WOW64.\n"));

    return bIsWow64;
}

BOOL IsWow64_TA()
{
    SYSTEM_INFO si1;
    SYSTEM_INFO si2;
    GetNativeSystemInfo = (GETNATIVESYSTEMINFO)GetProcAddress(LoadLibraryA("kernel32.dll"), "GetNativeSystemInfo");
    memset(&si1, 0, sizeof(SYSTEM_INFO));
    memset(&si2, 0, sizeof(SYSTEM_INFO));
    GetSystemInfo(&si1);
    GetNativeSystemInfo(&si2);

    if (si1.wProcessorArchitecture != si2.wProcessorArchitecture) {
        printf("The process is running under WOW64.\n");
        return TRUE;
    } else {
        printf("The process is not running under WOW64.\n");
        return FALSE;
    }
}

VOID RegRedirectionTest()
{
    HKEY hkey, us;
    printf("Create key [__test__] in [HKEY_LOCAL_MACHINE\\Software\\Wow6432Node]!\n");
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software", 0, KEY_ALL_ACCESS, &hkey);
    RegCreateKeyA(hkey, "__test__", &us);
    getchar();
    printf("Create key [__test__] in [HKEY_LOCAL_MACHINE\\Software]!\n");
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software", 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkey);
    RegCreateKeyA(hkey, "__test__", &us);
    getchar();
}

VOID FsRedirectionTest()
{
    Wow64EnableWow64FsRedirection = (WOW64ENABLEWOW64FSREDIRECTION)GetProcAddress(LoadLibraryA("kernel32.dll"), "Wow64EnableWow64FsRedirection");
    printf("Create folder [__test__] in [C:\\WINDOWS\\SYSWOW64]!\n");
    _mkdir("c:\\windows\\system32\\__test__");
    getchar();
    Wow64EnableWow64FsRedirection(FALSE); //Close Redirection
    printf("Create folder [__test__] in [C:\\WINDOWS\\SYSTEM32]!\n");
    _mkdir("c:\\windows\\system32\\__test__");
    getchar();
}

int main(void)
{
    IsWow64();
    getchar();
    IsWow64_TA();
    getchar();
    RegRedirectionTest();
    FsRedirectionTest();
    return 0;
}

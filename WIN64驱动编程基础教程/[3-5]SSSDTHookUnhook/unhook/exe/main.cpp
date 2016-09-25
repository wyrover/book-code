#include <stdio.h>
#include <direct.h>
#include <Windows.h>
//#include "ntimage.h"
#include "EnumDrv.h"
#include "DrvCtrl.h"

HANDLE hMyDrv;

void PrintAddressByIndex()
{
    LONG64 id = 0;
    ULONG64 addr = 0;
st:
    printf("Input index (HEX without \"0x\" like: 1000, input -1 to exit): ");
    scanf("%llx", &id);

    if (id < 0) return;

    IoControl(hMyDrv , CTL_CODE_GEN(0x807), &id, 8, &addr, 8);
    printf("%llx\n", addr);
    getchar();
    goto st;
}

DWORD FileLen(char *filename)
{
    WIN32_FIND_DATAA fileInfo = {0};
    DWORD fileSize = 0;
    HANDLE hFind;
    hFind = FindFirstFileA(filename , &fileInfo);

    if (hFind != INVALID_HANDLE_VALUE) {
        fileSize = fileInfo.nFileSizeLow;
        FindClose(hFind);
    }

    return fileSize;
}

CHAR *LoadDllContext(char *filename)
{
    DWORD dwReadWrite, LenOfFile = FileLen(filename);
    HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

    if (hFile != INVALID_HANDLE_VALUE) {
        PCHAR buffer = (PCHAR)malloc(LenOfFile);
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        ReadFile(hFile, buffer, LenOfFile, &dwReadWrite, 0);
        CloseHandle(hFile);
        return buffer;
    }

    return NULL;
}

ULONG64 GetWin32kImageBase()
{
    PIMAGE_NT_HEADERS64 pinths64;
    PIMAGE_DOS_HEADER pdih;
    char *NtosFileData = NULL;
    NtosFileData = LoadDllContext("c:\\win32k.dll");
    pdih = (PIMAGE_DOS_HEADER)NtosFileData;
    pinths64 = (PIMAGE_NT_HEADERS64)(NtosFileData + pdih->e_lfanew);
    return pinths64->OptionalHeader.ImageBase;
}

void GetOriAddress()
{
    ULONG64 W32pServiceTable, Win32kBase, Win32kImageBase, Win32kInProcess = 0, retv;
    IoControl(hMyDrv , CTL_CODE_GEN(0x806), NULL, 0, &W32pServiceTable, 8);
    Win32kBase = GetWin32kBase();
    CopyFileA("c:\\windows\\system32\\win32k.sys", "c:\\win32k.dll", 0);
    Win32kImageBase = GetWin32kImageBase();
    printf("W32pServiceTable:      %llx\n", W32pServiceTable);
    printf("WIN32K.SYS base:       %llx\n", Win32kBase);
    printf("WIN32K.SYS image base: %llx\n\n\n", Win32kImageBase);
    ULONG index = 0;

    if (Win32kInProcess == 0)
        Win32kInProcess = (ULONGLONG)LoadLibraryExA("c:\\win32k.dll", 0, DONT_RESOLVE_DLL_REFERENCES);

    for (index = 0; index < 825; index++) { //825是WIN7X64上SSSDT的函数个数
        ULONGLONG RVA = W32pServiceTable - Win32kBase;
        ULONGLONG temp = *(PULONGLONG)(Win32kInProcess + RVA + 8 * (ULONGLONG)index);
        ULONGLONG RVA_index = temp - Win32kImageBase;
        retv = RVA_index + Win32kBase;
        printf("Shadow SSDT Function[%ld]: %llx\n", index, retv);

        if (index % 100 == 0) {
            printf("Press any key to continue......\n");
            getchar();
        }
    }
}

int main()
{
    hMyDrv = openDriver();
    GetOriAddress();
    uninstallDriver();
    return 0;
}
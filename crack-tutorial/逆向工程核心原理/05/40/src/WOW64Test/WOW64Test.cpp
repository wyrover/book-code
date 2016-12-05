#include "stdio.h"
#include "windows.h"
#include "Shlobj.h"
#include "tchar.h"
#pragma comment(lib, "Shell32.lib")

int _tmain(int argc, TCHAR* argv[])
{
    HKEY    hKey                = NULL;
    HANDLE  hFile               = INVALID_HANDLE_VALUE;
    TCHAR   szPath[MAX_PATH]    = {0,};

    ////////////////
    // system32 folder
    if( GetSystemDirectory(szPath, MAX_PATH) )
    {
        _tprintf(L"1) system32 path = %s\n", szPath);
    }

    ////////////////
    // File size
    _tcscat_s(szPath, L"\\kernel32.dll");
    hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hFile != INVALID_HANDLE_VALUE )
    {
        _tprintf(L"2) File size of \"%s\" = %d\n", 
        szPath, GetFileSize(hFile, NULL));
        CloseHandle(hFile);
    }

    ////////////////
    // Program Files
    if( SHGetSpecialFolderPath(NULL, szPath, 
                               CSIDL_PROGRAM_FILES, FALSE) )
    {
        _tprintf(L"3) Program Files path = %s\n", szPath);
    }

    ////////////////
    // Registry
    if( ERROR_SUCCESS == RegCreateKey(HKEY_LOCAL_MACHINE, 
                                      L"SOFTWARE\\ReverseCore", &hKey) )
    {
        RegCloseKey(hKey);
        _tprintf(L"4) Create Registry Key : HKLM\\SOFTWARE\\ReverseCore\n");
    }

    return 0;
}

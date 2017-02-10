/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>

VOID SimulateMemoryCorruption ( ) ;

class CAppInfo
{
public: 
    CAppInfo(LPWSTR wszAppName, LPWSTR wszVersion)
    {
        m_wszAppName=wszAppName;
        m_wszVersion=wszVersion;
    }

    VOID PrintAppInfo()
    {
        wprintf(L"\nFull application Name: %s\n", m_wszAppName);
        wprintf(L"Version: %s\n", m_wszVersion);
    }

private:
    LPWSTR m_wszAppName ;
    LPWSTR m_wszVersion ;
} ;

CAppInfo* g_AppInfo ;

int __cdecl wmain (int argc, WCHAR* args[])
{
    wint_t iChar = 0 ;
    g_AppInfo = new CAppInfo(L"Memory Corruption Sample", L"1.0" );
    if(!g_AppInfo)
    {
        return 1;
    }

    wprintf(L"Press: \n");
    wprintf(L"    1    To display application information\n");
    wprintf(L"    2    To simulated memory corruption\n");
    wprintf(L"    3    To exit\n\n\n>");

    while((iChar=_getwche())!='3')
    {
        switch(iChar)
        {
            case '1':
               g_AppInfo->PrintAppInfo();
               break;

            case '2':
              SimulateMemoryCorruption();
              wprintf(L"\nMemory Corruption completed\n");
              break;

            default:
              wprintf(L"\nInvalid option\n");
        }
        wprintf(L"\n\n> ");
    }
    return 0;
}


VOID SimulateMemoryCorruption ( )
{
    char* pszWrite="Corrupt";
    BYTE* p=(BYTE*) g_AppInfo;
    CopyMemory(p, pszWrite, strlen(pszWrite));
}

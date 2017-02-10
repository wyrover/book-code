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

#define SZ_MAX_LEN  10

WCHAR* pszCopy = NULL ;

BOOL DupString(WCHAR* psz);

void __cdecl wmain (int argc, WCHAR* args[])
{
    if(argc==2)
    {
        wprintf(L"Press any key to start\n");
        _getch();
        DupString(args[1]);
    }
    else
    {
        wprintf(L"Please enter a string");
    }
}


BOOL DupString(WCHAR* psz)
{
    BOOL bRet=FALSE;
    
    if(psz!=NULL)
    {
        pszCopy=(WCHAR*) HeapAlloc(GetProcessHeap(), 0, SZ_MAX_LEN*sizeof(WCHAR));
        if(pszCopy)
        {
            wcscpy(pszCopy, psz);
            wprintf(L"Copy of string: %s", pszCopy);
            HeapFree(GetProcessHeap(), 0, pszCopy);
            bRet=TRUE;
        }
    }
    return bRet;
}
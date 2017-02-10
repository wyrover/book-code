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

typedef int (__cdecl *MYPROC)(DWORD dwOne, DWORD dwTwo); 
VOID CallProc(MYPROC pProc);

int __cdecl wmain ()
{
    HMODULE hMod = LoadLibrary (L"05mod.dll");
    if(hMod)
    {
        MYPROC pProc = (MYPROC) GetProcAddress(hMod, "InitModule");
        if(pProc)
        {
            CallProc(pProc);
        }
        else
        {
            wprintf(L"Failed to get proc address of InitModule");
        }
        
        FreeLibrary(hMod);
    }
    else
    {
        wprintf(L"Failed to load 05mod.dll.");
    }
    return 0;
}


VOID CallProc(MYPROC pProc)
{
    pProc(1,2);
}

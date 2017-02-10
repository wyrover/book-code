/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>

void __cdecl CDeclFunc(DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
void __stdcall StdcallFunc(DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);

int __cdecl wmain ()
{
    wprintf(L"Calling CDeclFunc\n");   
    CDeclFunc(1,2,3);

    wprintf(L"Calling StdcallFunc\n");   
    StdcallFunc(1,2,3);

    return 0;
}

void __cdecl CDeclFunc(DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
    wprintf(L"Inside CDeclFunc\n");
}

void StdcallFunc(DWORD dwParam1, DWORD dwParam2, DWORD dwParam3)
{
    wprintf(L"Inside StdcallFunc\n");
}

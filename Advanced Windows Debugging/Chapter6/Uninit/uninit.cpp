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

#define ARRAY_SIZE 10

BOOL InitArray(int** pPtrArray);

int __cdecl wmain (int argc, WCHAR* args[])
{
    int iRes=1;

    wprintf(L"Press any key to start...");
    _getch();

    int** pPtrArray=(int**) HeapAlloc(GetProcessHeap(),
                                      0,
                                      sizeof(int*[ARRAY_SIZE]));
    if(pPtrArray!=NULL)
    {
        InitArray(pPtrArray);
        *(pPtrArray[0])=10;
        iRes=0;
        HeapFree(GetProcessHeap(), 0, pPtrArray);
    }
    return iRes;
}

BOOL InitArray(int** pPtrArray)
{
    return FALSE ;
}



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

DWORD WINAPI ThreadProcedure(LPVOID lpParameter);
VOID ProcA();
VOID Sum(int* numArray, int iCount, int* sum);

void __cdecl wmain ()
{
    HANDLE hThread = NULL ;

    wprintf(L"Starting new thread...");

    hThread = CreateThread(NULL, 0, ThreadProcedure, NULL, 0, NULL);
    if(hThread!=NULL)
    {
        wprintf(L"Successfully created thread\n");
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}

DWORD WINAPI ThreadProcedure(LPVOID lpParameter)
{
    ProcA();    
    wprintf(L"Press any key to exit thread\n");
    _getch();
    return 0;
}

VOID ProcA()
{
    int iCount = 3;
    int iNums[] = {1,2,3};
    int iSum = 0;

    Sum(iNums, iCount, &iSum);
    wprintf(L"Sum is: %d\n", iSum);
}

VOID Sum(int* numArray, int iCount, int* sum)
{
    for(int i=0; i<iCount;i++)
    {
        *sum+=numArray[i];
    }
}
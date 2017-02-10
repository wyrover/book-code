#include <windows.h>
#include <stdio.h>
#include <conio.h>

CRITICAL_SECTION cs_DB1;
CRITICAL_SECTION cs_DB2;

DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{ 
    EnterCriticalSection(&cs_DB1);
    
    //
    // Do work on stack
    //
    wprintf(L"Updating database 1\n");
    Sleep(3000);
    EnterCriticalSection(&cs_DB2);

    wprintf(L"Updating database 2\n");

    LeaveCriticalSection(&cs_DB2);
    LeaveCriticalSection(&cs_DB1);

    return 1;
}

void __cdecl main ( )
{
    HANDLE hThread=NULL;
    DWORD dwId=0;

    if(!InitializeCriticalSectionAndSpinCount(&cs_DB1, 4000))
    {
        return;
    }
    if(!InitializeCriticalSectionAndSpinCount(&cs_DB2, 4000))
    {
        DeleteCriticalSection(&cs_DB1);
        return;
    }
    
    hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwId);
    if(hThread)
    {
        Sleep(2000);
        EnterCriticalSection(&cs_DB2);
        
        //
        // Do some work with queue
        // 
        wprintf(L"Updating database 2\n");
        Sleep(2000);
        EnterCriticalSection(&cs_DB1);
        
        //
        // Do work on stack
        //
        wprintf(L"Updating database 1\n");

        LeaveCriticalSection(&cs_DB1);
        LeaveCriticalSection(&cs_DB2);

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    DeleteCriticalSection(&cs_DB1);
    DeleteCriticalSection(&cs_DB2);
}

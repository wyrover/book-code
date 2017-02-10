#include <windows.h>
#include <stdio.h>
#include <conio.h>

CRITICAL_SECTION g_cs;

class InvalidParameterException{};

VOID Call3rdPartyCode(LPVOID lpParam)
{
    if(lpParam==NULL)
    {
        throw InvalidParameterException();
    }

    //
    // Do some work
    //
}

DWORD WINAPI ThreadProc(LPVOID lpParam) 
{ 
    DWORD dwRet=1;

    try
    {
        EnterCriticalSection(&g_cs);
        Call3rdPartyCode(lpParam);
        LeaveCriticalSection(&g_cs);
    }
    catch(...)
    {
        // Error occured
        dwRet=0;
    }

    return dwRet;
}

void __cdecl main ( )
{
    DWORD dwId=0;
    HANDLE hThread=NULL;

    if(!InitializeCriticalSectionAndSpinCount(&g_cs, 4000))
    {
        return;
    }

    hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwId);
    if(hThread)
    {
        Sleep(500);

        wprintf(L"Acquiring critical section\n");

        EnterCriticalSection(&g_cs);

        //
        // Do some work
        // 
        
        wprintf(L"Leaving critical section\n");
        LeaveCriticalSection(&g_cs);

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    DeleteCriticalSection(&g_cs);
}

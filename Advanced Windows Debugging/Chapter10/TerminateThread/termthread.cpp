#include <windows.h>
#include <stdio.h>
#include <conio.h>


DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{ 
    BYTE* pData=NULL;
    for(;;)
    {
        pData=new BYTE[10000];
        //
        // Use memory
        //
        delete[] pData;
    }

    wprintf(L"Exiting thread\n");
    return 1;
}

void __cdecl wmain ( )
{
    DWORD dwId=0;
    HANDLE hThread=NULL;

    hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwId);
    if(hThread)
    {
        BYTE* pData=NULL;
        Sleep(500);

        wprintf(L"Terminating worker thread...\n");
        TerminateThread(hThread, 0);

        pData=new BYTE[10000];
        if(pData)
        {
            //
            // Use memory
            //
            delete[] pData;
        }
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}

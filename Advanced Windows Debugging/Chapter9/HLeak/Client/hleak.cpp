#include <windows.h>
#include "srv.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define NUM_ARGS    4
#define NUM_OPS     3
#define GETSID      1
#define GETPC       2
#define GETGC       3

DWORD dwIterations=10;
DWORD dwSleepTime=0;

CServer serverInst;

VOID DisplayUsage()
{
    printf ("stressclient.exe /t:<num_threads> /i:<iterations_per_thread> /s:<sleep_time_per_iteration>\n");
}

DWORD WINAPI ThreadWorker(LPVOID lpParameter)
{
    srand ( (unsigned int)time ( NULL ) ) ;

    for(DWORD i=0;i<dwIterations;i++)
    {
        //
        // Randomly pick server operation to run against...
        //
        DWORD dwOperation = static_cast<DWORD> ( NUM_OPS * rand() / (RAND_MAX + 1.0 ) ) ;
        if(dwOperation==GETSID)
        {
            PSID pSid=serverInst.GetSID();
            if(pSid==NULL)
            {
                wprintf(L"Failed to get SID!\n");
            }
            else
            {
                wprintf(L".");
                delete[] pSid;
            }
        }
        else if(dwOperation==GETPC)
        {
            DWORD dwPrivCount=serverInst.GetPrivilegeCount();
            if(dwPrivCount==0)
            {
                wprintf(L"Failed to get privilege count!\n");
            }
            else
            {
                wprintf(L".");
            }
        }
        else 
        {
            DWORD dwGroupCount=serverInst.GetGroupCount();
            if(dwGroupCount==0)
            {
                wprintf(L"Failed to get group count!\n");
            }
            else
            {
                wprintf(L".");
            }
        }
        Sleep(dwSleepTime*1000);
    }
    return 0;
}

void __cdecl wmain (int argc, wchar_t* pArgs[])
{
    if(argc!=NUM_ARGS)
    {
        DisplayUsage();
    }

    wprintf(L"Press any key to start stress application...\n");
    _getch();

    DWORD dwNumThreads=10;
    
    BOOL bFailed=FALSE;
    for(int i=1;i<NUM_ARGS && bFailed==FALSE;i++)
    {
        if(wcslen(pArgs[i])<4)
        {
            wprintf(L"Invalid switch: %s\n", pArgs[i]);
            bFailed=TRUE;
            break;
        }
        if(pArgs[i][0]!='/')
        {
            wprintf(L"Invalid switch: %s\n", pArgs[i]);
            bFailed=TRUE;
            break;
        }
        switch(pArgs[i][1])
        {
            case 't':
            case 'T':
                dwNumThreads=_wtoi(pArgs[i]+3);
                break;
            
            case 'i':
            case 'I':
                dwIterations=_wtoi(pArgs[i]+3);
                break;

            case 's':
            case 'S':
                dwSleepTime=_wtoi(pArgs[i]+3);
                break;

            default:
                wprintf(L"Invalid switch: /%c\n", pArgs[i][1]);
                bFailed=TRUE;
                break;
       }
    }
    if(bFailed==FALSE)
    {
        DWORD dwActualThreadCount=0;
        HANDLE* pHandles=new HANDLE[dwNumThreads];
        if(pHandles!=NULL)
        {
            wprintf(L"Running...");
            for(DWORD i=0;i<dwNumThreads;i++,dwActualThreadCount++)
            {

                pHandles[i]=CreateThread(NULL,
                                         0,
                                         ThreadWorker,
                                         NULL,
                                         0,
                                         NULL
                                         );
                if(pHandles[i]==NULL)
                {
                    wprintf(L"Failed to create thread: %d\n", GetLastError());
                    break;
                }
            }
            if(dwActualThreadCount>0)
            {
                DWORD dwRes=WaitForMultipleObjects(dwActualThreadCount, pHandles, TRUE, INFINITE);
                if(dwRes==WAIT_OBJECT_0)
                {
                    for(DWORD i=0;i<dwActualThreadCount;i++)
                    {
                        CloseHandle(pHandles[i]);
                        pHandles[i]=NULL;
                    }
                }
                else
                {
                    wprintf(L"Failed to wait for thread completion: %d\n", GetLastError());
                }
            }
            else
            {
                wprintf(L"Unable to start any threads\n");                
            }
            delete[] pHandles;
        }

    }
    wprintf(L"\nPress any key to exit stress application...\n");
    _getch();
}

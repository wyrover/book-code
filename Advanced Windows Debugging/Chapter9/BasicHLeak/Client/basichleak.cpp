#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include "srv.h"

#define MAX_LEN 255

void __cdecl wmain()
{
    wprintf(L"Client application console menu\n");
    wprintf(L"================================\n");

    HANDLE hCompletionEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
    if(hCompletionEvent==NULL)
    {
        wprintf(L"\nFailed to create a new event\n");
        return;
    }
    
    BOOL bContinue=TRUE;
    while(bContinue)
    {
        wprintf(L"Enter filename to read > ");

        WCHAR pFileName[MAX_LEN];
        StringCchGets(pFileName, MAX_LEN);
        
        if(wcslen(pFileName))
        {
            CHAR szFiledata[1024];
            BOOL bRet=CServer::GetTextFileContents(hCompletionEvent, 
                                                   pFileName, 
                                                   szFiledata, 1024 ) ;
            if(bRet==FALSE)
            {
                wprintf(L"\nFailed to read file\n");
            }
            else
            {
                wprintf(L"\nScheduled request successfully\n");
                if(WaitForSingleObject(hCompletionEvent, INFINITE)!=WAIT_FAILED)
                {
                    wprintf(L"\nData read:\n%s", szFiledata);
                }
            }
        }
        else
        {
            wprintf(L"Exiting application...\n");
            bContinue=FALSE;
        }
    }

    CloseHandle(hCompletionEvent);
}

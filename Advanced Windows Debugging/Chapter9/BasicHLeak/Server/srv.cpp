#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "srv.h"

BOOL CServer::GetTextFileContents(HANDLE hEvent, 
                                  PWSTR pszFileName, 
                                  PSTR pBuffer, 
                                  DWORD dwBufferLen)
{
    BOOL bRet=FALSE;

    if(hEvent!=NULL && pszFileName!=NULL && pBuffer!=NULL && dwBufferLen!=0)
    {
        WorkerData* pWorkerData=new WorkerData;
        if(pWorkerData!= NULL)
        {
            pWorkerData->dwBufferLen=dwBufferLen;
            pWorkerData->pBuffer=pBuffer;
            pWorkerData->pszFileName=pszFileName;
            pWorkerData->hCompletionHandle=hEvent;
            
            bRet=QueueUserWorkItem(RequestWorker, 
                                   (LPVOID) pWorkerData, 
                                   WT_EXECUTELONGFUNCTION);
            if(!bRet)
            {
                delete pWorkerData;
            }
        }
    }

    return bRet;
}


DWORD WINAPI CServer::RequestWorker(LPVOID lpParameter)
{
    DWORD dwRet=0;

    WorkerData* pWorkerData=reinterpret_cast<WorkerData*>(lpParameter);

    HANDLE hFile=CreateFile(pWorkerData->pszFileName, 
                            FILE_READ_DATA, 
                            FILE_SHARE_READ, 
                            NULL, 
                            OPEN_EXISTING, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL); 
    
    if(hFile!=INVALID_HANDLE_VALUE)
    {
        DWORD dwBytesRead=0;
        BOOL bRet=ReadFile(hFile, 
                           (LPVOID) pWorkerData->pBuffer, 
                           (pWorkerData->dwBufferLen-1), 
                           &dwBytesRead, 
                           NULL);
        if(bRet==TRUE)
        {
            dwRet=1;
        }
    }

    SetEvent(pWorkerData->hCompletionHandle);
    delete pWorkerData;

    return dwRet;
}

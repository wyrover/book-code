#include <windows.h>
#include <stdio.h>
#include "srv.h"

CIdentity* CServer::GetSID()
{
    PSID pSid = NULL;
    HANDLE hToken = INVALID_HANDLE_VALUE;
    hToken = GetToken();
    if(hToken!=INVALID_HANDLE_VALUE)
    {
        DWORD dwNeeded=0;
        BOOL bRes=GetTokenInformation(hToken,
                                      TokenUser,
                                      NULL,
                                      0,
                                      &dwNeeded
                                      );
        if(bRes==FALSE && GetLastError()==ERROR_INSUFFICIENT_BUFFER)
        {
            TOKEN_USER* pBuffer=(TOKEN_USER*) new BYTE[dwNeeded];
            if(pBuffer!=NULL)
            {
                BOOL bRes=GetTokenInformation(hToken,
                                              TokenUser,
                                              (LPVOID)pBuffer,
                                              dwNeeded,
                                              &dwNeeded
                                              );
                if(bRes==TRUE)
                {
                    DWORD dwSidLen=GetLengthSid(pBuffer->User.Sid);
                    pSid=(PSID) new BYTE[dwSidLen];
                    if(pSid!=NULL)
                    {
                        if(CopySid(dwSidLen, pSid, pBuffer->User.Sid)==FALSE)
                        {
                            delete[] pSid;
                            pSid=NULL;
                        }
                    }
                }
                delete[] pBuffer;
            }
        }
        CloseHandle(hToken);
    }
    
    CSID* pIdentity=NULL ;
    if(pSid!=NULL)
    {
        pIdentity=new CSID(pSid);
        if(pIdentity==NULL)
        {
            delete[] pSid;
        }
    }
    return (CIdentity*) pIdentity;
}

DWORD CServer::GetPrivilegeCount()
{
    DWORD dwPrivCount=0;
    HANDLE hToken = INVALID_HANDLE_VALUE;
    hToken = GetToken();
    if(hToken!=INVALID_HANDLE_VALUE)
    {
        DWORD dwNeeded=0;
        BOOL bRes=GetTokenInformation(hToken,
                                      TokenStatistics,
                                      NULL,
                                      0,
                                      &dwNeeded
                                      );
        if(bRes==FALSE && GetLastError()==ERROR_INSUFFICIENT_BUFFER)
        {
            TOKEN_STATISTICS* pBuffer=reinterpret_cast<TOKEN_STATISTICS*>( new BYTE[dwNeeded]);
            if(pBuffer!=NULL)
            {
                BOOL bRes=GetTokenInformation(hToken,
                                              TokenStatistics,
                                              (LPVOID)pBuffer,
                                              dwNeeded,
                                              &dwNeeded
                                              );
                if(bRes==TRUE)
                {
                    dwPrivCount=pBuffer->PrivilegeCount;
                }
            }
            delete[] pBuffer;
        }
        CloseHandle(hToken);
    }
    return dwPrivCount;
}

DWORD CServer::GetGroupCount()
{
    DWORD dwGroupCount=0;
    HANDLE hToken = INVALID_HANDLE_VALUE;
    hToken = GetToken();
    if(hToken!=INVALID_HANDLE_VALUE)
    {
        DWORD dwNeeded=0;
        BOOL bRes=GetTokenInformation(hToken,
                                      TokenStatistics,
                                      NULL,
                                      0,
                                      &dwNeeded
                                      );
        if(bRes==FALSE && GetLastError()==ERROR_INSUFFICIENT_BUFFER)
        {
            TOKEN_STATISTICS* pBuffer=reinterpret_cast<TOKEN_STATISTICS*>( new BYTE[dwNeeded]);
            if(pBuffer!=NULL)
            {
                BOOL bRes=GetTokenInformation(hToken,
                                              TokenStatistics,
                                              (LPVOID)pBuffer,
                                              dwNeeded,
                                              &dwNeeded
                                              );
                if(bRes==TRUE)
                {
                    dwGroupCount=pBuffer->GroupCount;
                }
            }
            delete[] pBuffer;
        }
        CloseHandle(hToken);
    }
    return dwGroupCount;
}


HANDLE CServer::GetToken()
{
    HANDLE hHandle = GetCurrentThread();
    HANDLE hToken = INVALID_HANDLE_VALUE;
    BOOL bRet = OpenThreadToken ( hHandle,
                                  TOKEN_READ,
                                  TRUE,
                                  &hToken
                                  );
    if(bRet==FALSE)
    {
        hHandle = GetCurrentProcess();
        bRet=OpenProcessToken(hHandle,
                              TOKEN_READ,
                              &hToken
                              );
    }
    return hToken;
}

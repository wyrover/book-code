// NamePipeClientDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\NamePipeServerDemo\Core\NamedPipeClient.h"
#include "..\NamePipeServerDemo\Core\IIPCInterface.h"
#include <conio.h>
#include <locale.h>

class CNamedPipeEvent : public IIPCEvent
{
public:
    CNamedPipeEvent()
    {

    }

    virtual ~CNamedPipeEvent()
    {

    }

    virtual void OnRequest(IIPCObject* pServer, IIPCConnector* pClient, LPCVOID lpBuf, DWORD dwBufSize)
    {
        if(NULL == lpBuf || dwBufSize == 0)
            return ;

        LPUSER_DATA_PACKAGE userRequest = (LPUSER_DATA_PACKAGE)lpBuf;

        if(userRequest->dwPackageType == 1)
        {
            _tsetlocale(LC_ALL, _T("chs"));
            _tprintf_s(_T("%s"), userRequest->lpBuf);
        }
    }
};

DWORD __stdcall SendThread(LPVOID lpParam)
{
    IIPCObject* pServer = (IIPCObject*)lpParam;

    IIPCConnectorIterator* pClientIterator = pServer->GetClients();

    int x = 10;

    while(x > 0)
    {
        x--;

        for(pClientIterator->Begin(); !pClientIterator->End(); pClientIterator->Next())
        {
            IIPCConnector* aClient = pClientIterator->GetCurrent();

            if(NULL == aClient)
                continue;

            TCHAR* sRequest = _T("异步消息：你好，服务端\r\n");

            _USER_DATA_PACKAGE userRequest = {0};
            userRequest.dwPackageType = 1;
            _tcscpy_s(userRequest.lpBuf, MAX_PATH, sRequest);

            aClient->PostMessage(&userRequest, sizeof(_USER_DATA_PACKAGE));
        }
    }

    return 0;
}

void TestRequestAndReply(IIPCObject* pNamedPipeClient)
{
    IIPCConnectorIterator* pClientIterator = pNamedPipeClient->GetClients();

    int x = 10;

    while(x > 0)
    {
        x--;

        for(pClientIterator->Begin(); !pClientIterator->End(); pClientIterator->Next())
        {
            IIPCConnector* aClient = pClientIterator->GetCurrent();

            if(NULL == aClient)
                continue;


            DWORD dwTransSize = 0;

            TCHAR* sRequest = _T("同步消息：1+1=?\r\n");
            _USER_DATA_PACKAGE userRequest = {0};
            userRequest.dwPackageType = 100;
            _tcscpy_s(userRequest.lpBuf, MAX_PATH, sRequest);

            _USER_DATA_PACKAGE userReply = {0};

            if(aClient->RequestAndReply(&userRequest, sizeof(_USER_DATA_PACKAGE), &userReply, sizeof(_USER_DATA_PACKAGE), &dwTransSize))
            {
                _tsetlocale(LC_ALL, _T("chs"));
                _tprintf_s(_T("%s"), userReply.lpBuf);
            }
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    IIPCEvent* pEvent = new CNamedPipeEvent;

    IIPCObject* pNamedPipeClient = new CNamedPipeClient(pEvent);

    if(NULL == pNamedPipeClient)
        return -1;

    if(!pNamedPipeClient->Create(_T("NamedPipeServer")))
        return -1;

    SendThread(pNamedPipeClient);

    TestRequestAndReply(pNamedPipeClient);

    _getch();

    pNamedPipeClient->Close();
    delete pNamedPipeClient;
    pNamedPipeClient = NULL;

    delete pEvent;
    pEvent = NULL;
    return 0;
}


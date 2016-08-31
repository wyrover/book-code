// NamePipeServerDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stddef.h>
#include <conio.h>
#include "Core\NamedPipeServer.h"
#include <locale.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <locale.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

BOOL g_bExit = FALSE;
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
        _tsetlocale(LC_ALL, _T("chs"));
        _tprintf_s(_T("%s"), userRequest->lpBuf);


        if(userRequest->dwPackageType == 100)
        {
            TCHAR sReply[MAX_PATH] = {0};
            _stprintf_s(sReply, _T("同步应答：1+1=2 \r\n"));

            _USER_DATA_PACKAGE userRequest = {0};
            userRequest.dwPackageType = 100;
            _tcscpy_s(userRequest.lpBuf, MAX_PATH, sReply);

            pClient->PostMessage(&userRequest, sizeof(_USER_DATA_PACKAGE));
        }
        else
        {
            TCHAR sReply[MAX_PATH] = {0};
            _stprintf_s(sReply, _T("异步应答：你好，客户端 \r\n"));

            _USER_DATA_PACKAGE userRequest = {0};
            userRequest.dwPackageType = 1;
            _tcscpy_s(userRequest.lpBuf, MAX_PATH, sReply);

            pClient->PostMessage(&userRequest, sizeof(_USER_DATA_PACKAGE));
        }

    }
};

DWORD __stdcall PostThread(LPVOID lpParam)
{
    IIPCObject* pServer = (IIPCObject*)lpParam;

    IIPCConnectorIterator* pClientIterator = pServer->GetClients();

    while(FALSE == g_bExit)
    {
        for(pClientIterator->Begin(); !pClientIterator->End(); pClientIterator->Next())
        {
            IIPCConnector* aClient = pClientIterator->GetCurrent();

            if(NULL == aClient)
                continue;

            TCHAR* sRequest = _T("异步消息：你好，客户端端\r\n");

            _USER_DATA_PACKAGE userRequest = {0};
            userRequest.dwPackageType = 1;
            _tcscpy_s(userRequest.lpBuf, MAX_PATH, sRequest);

            aClient->PostMessage(&userRequest, sizeof(_USER_DATA_PACKAGE));
        }

        Sleep(10);
    }

    return 0;
}


void TestRequestAndReply(IIPCObject* pNamedPipeClient)
{
    IIPCConnectorIterator* pClientIterator = pNamedPipeClient->GetClients();

    while(FALSE == g_bExit)
    {
        for(pClientIterator->Begin(); !pClientIterator->End(); pClientIterator->Next())
        {
            IIPCConnector* aClient = pClientIterator->GetCurrent();

            if(NULL == aClient)
                continue;

            TCHAR* sRequest = _T("1+1=2\r\n");
            DWORD dwRequestSize = _tcslen(sRequest) * sizeof(TCHAR);
            DWORD dwTransSize = 0;

            TCHAR sReply[MAX_PATH] = {0};

            if(aClient->RequestAndReply(sRequest, dwRequestSize, sReply, MAX_PATH, &dwTransSize))
            {
                _tsetlocale(LC_ALL, _T("chs"));
                _tprintf_s(_T("%s"), sReply);
            }
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    IIPCEvent* pEvent = new CNamedPipeEvent;
    IIPCObject* pNamedPipeServer = new CNamedPipeServer(pEvent);

    if(NULL == pNamedPipeServer)
        return -1;

    if(!pNamedPipeServer->Create(_T("NamedPipeServer")))
        return -1;

//    PostThread(pNamedPipeServer);

    _getch();

    pNamedPipeServer->Close();
    delete pNamedPipeServer;
    pNamedPipeServer = NULL;
    delete pEvent;
    pEvent = NULL;
    return 0;
}


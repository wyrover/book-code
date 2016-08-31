#include "stdafx.h"
#include "NamedPipeServer.h"
#include <tchar.h>
#include <locale.h>


CNamedPipeServer::CNamedPipeServer(IIPCEvent* pEvent): m_pEvent(pEvent), m_hThreadIOCP(NULL)
{
    InitializeCriticalSection(&m_csClientMap);
}

CNamedPipeServer::~CNamedPipeServer()
{
    Close();
    DeleteCriticalSection(&m_csClientMap);
}

BOOL CNamedPipeServer::Create(LPCTSTR lpPipeName)
{
    if(NULL == lpPipeName)
        return FALSE;

    _tcscpy_s(m_sPipeName, MAX_PATH, _T("\\\\.\\pipe\\"));
    _tcscat_s(m_sPipeName, lpPipeName);

    const DWORD dwThreadCount = GetCpuNum();

    if(!m_iocp.Create(dwThreadCount))
        return FALSE;

    m_hThreadIOCP = new HANDLE[dwThreadCount];

    for(DWORD i = 0; i < dwThreadCount; ++i)
        m_hThreadIOCP[i] = CreateThread(0, 0, IOCompletionThread, this, 0, NULL);

    WaitClientConnect();
    return TRUE;
}

void CNamedPipeServer::Close()
{
    m_iocp.Close();
    const DWORD dwThreadCount = GetCpuNum();

    if(NULL != m_hThreadIOCP)
        WaitForMultipleObjects(dwThreadCount, m_hThreadIOCP, TRUE, INFINITE);

    for(DWORD i = 0; i < dwThreadCount; i++)
    {
        if(NULL != m_hThreadIOCP && NULL != m_hThreadIOCP[i])
        {
            CloseHandle(m_hThreadIOCP[i]);
            m_hThreadIOCP[i] = NULL;
        }
    }

    delete m_hThreadIOCP;
    m_hThreadIOCP = NULL;

    for(ConnectorMap::const_iterator cit = m_connectorMap.begin(); cit != m_connectorMap.end(); cit++)
    {
        CNamedPipeConnector* pClient = dynamic_cast<CNamedPipeConnector*>(*cit);

        if(NULL != pClient)
        {
            pClient->Close();
            delete pClient;
            pClient = NULL;
        }
    }

    m_connectorMap.clear();
}

IIPCConnectorIterator* CNamedPipeServer::GetClients()
{
    return this;
}

ConnectorMap::const_iterator CNamedPipeServer::FindClient(CNamedPipeConnector* pClient)
{
    EnterCriticalSection(&m_csClientMap);
    ConnectorMap::const_iterator citFinded = m_connectorMap.end();

    for(ConnectorMap::const_iterator cit = m_connectorMap.begin(); cit != m_connectorMap.end(); cit++)
    {
        CNamedPipeConnector* pConnector = dynamic_cast<CNamedPipeConnector*>(*cit);

        if(NULL != pConnector && pConnector->GetHandle() == pClient->GetHandle())
        {
            citFinded = cit;
            break;
        }
    }

    LeaveCriticalSection(&m_csClientMap);
    return citFinded;
}

void CNamedPipeServer::Begin()
{
    EnterCriticalSection(&m_csClientMap);
    m_citCurrent = m_connectorMap.begin();
    LeaveCriticalSection(&m_csClientMap);
}

BOOL CNamedPipeServer::End()
{
    BOOL bEnd = FALSE;
    EnterCriticalSection(&m_csClientMap);
    bEnd = m_citCurrent == m_connectorMap.end();
    LeaveCriticalSection(&m_csClientMap);
    return bEnd;
}

void CNamedPipeServer::Next()
{
    EnterCriticalSection(&m_csClientMap);
    m_citCurrent++;
    LeaveCriticalSection(&m_csClientMap);
}

IIPCConnector* CNamedPipeServer::GetCurrent()
{
    IIPCConnector* pConnector = NULL;
    EnterCriticalSection(&m_csClientMap);
    pConnector = *m_citCurrent;
    LeaveCriticalSection(&m_csClientMap);
    return pConnector;
}

DWORD WINAPI CNamedPipeServer::IOCompletionThread(LPVOID lpParam)
{
    CNamedPipeServer* pThis = (CNamedPipeServer*)lpParam;

    if(NULL == lpParam)
        return -1;

    CIOCompletionPort* iocp = &pThis->m_iocp;

    CNamedPipeConnector* pClient = NULL;
    DWORD dwBytesTransferred = 0;
    IPC_DATA_OVERLAPPEDEX* po = NULL;
    BOOL bSucess = FALSE;

    while(TRUE)
    {
        bSucess = iocp->DequeuePacket((ULONG_PTR*)&pClient, &dwBytesTransferred, (OVERLAPPED **)&po, INFINITE);

        if(!bSucess && NULL == po)
        {
            break;
        }
        else if(!bSucess && GetLastError() == ERROR_BROKEN_PIPE)
        {
            EnterCriticalSection(&pThis->m_csClientMap);
            ConnectorMap::const_iterator cit = pThis->FindClient(pClient);

            if(cit != pThis->m_connectorMap.end())
                pThis->m_connectorMap.erase(cit);

            LeaveCriticalSection(&pThis->m_csClientMap);
            pClient->ClearOverlapped(po);
            pClient->Close();
            delete pClient;
            pClient = NULL;
            continue;
        }

        IPC_MESSAGE_TYPE type = IPC_MESSAGE_UKNOWN;

        if(NULL != po)
            type = po->emMessageType;

        switch(type)
        {
            case IPC_MESSAGE_CLIENTCONNECT:
                pClient->DoRead();
                pThis->WaitClientConnect();
                break;

            case IPC_MESSAGE_READ:
                pClient->m_dwProcessID = po->ipcDataPackage.dwProcessID;
                pThis->m_pEvent->OnRequest(pThis, pClient, &po->ipcDataPackage.lpData, dwBytesTransferred);
                pClient->ClearOverlapped(po);
                break;

            case IPC_MESSAGE_WRITE:
                pClient->DoRead();
                pClient->ClearOverlapped(po);
                break;

			default:
				pClient->ClearOverlapped(po);
				break;
        }
    }

    _tprintf(_T("DequeuePacket failed w/err 0x%08lx\n"), GetLastError());

    return 0;
}

BOOL CNamedPipeServer::WaitClientConnect()
{
    CNamedPipeConnector* pClient = new CNamedPipeConnector();

    if(!pClient->Create(m_sPipeName))
    {
        delete pClient;
        pClient = NULL;
        return FALSE;
    }

    m_connectorMap.push_back(pClient);

    if(!m_iocp.AssociateDevice(pClient->GetHandle(), (ULONG_PTR)pClient))
        return FALSE;

    if(!pClient->WaitConnect())
        return FALSE;

    return TRUE;
}

DWORD CNamedPipeServer::GetCpuNum()
{
    SYSTEM_INFO sysInfo = { 0 };
    GetNativeSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

CNamedPipeConnector::CNamedPipeConnector(): m_bExit(FALSE), m_conn(NULL)
{

}

CNamedPipeConnector::~CNamedPipeConnector()
{
    ClearOverlapped(m_conn);
}

DWORD CNamedPipeConnector::GetSID()
{
    return m_dwProcessID;
}

LPCTSTR CNamedPipeConnector::GetName()
{
    return NULL;
}

BOOL CNamedPipeConnector::PostMessage(LPCVOID lpBuf, DWORD dwBufSize)
{
    LPIPC_DATA_OVERLAPPEDEX package = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(package, sizeof(IPC_DATA_OVERLAPPEDEX));
    package->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    package->emMessageType = IPC_MESSAGE_WRITE;
    package->ipcDataPackage.dwDataSize = dwBufSize;
    package->ipcDataPackage.dwProcessID = GetCurrentProcessId();
    package->ipcDataPackage.dwTotalSize = sizeof(IPC_DATA_PACKAGE) - SYELOG_MAXIMUM_MESSAGE * sizeof(TCHAR) + dwBufSize;
    GetSystemTimeAsFileTime(&package->ipcDataPackage.ftOccurance);
    memcpy_s(package->ipcDataPackage.lpData, SYELOG_MAXIMUM_MESSAGE, lpBuf, dwBufSize);

    DWORD dwWrited = 0;
    BOOL bSucess = m_pipe.WriteFile(&package->ipcDataPackage, package->ipcDataPackage.dwTotalSize, &dwWrited, package);
    return (bSucess || GetLastError() == ERROR_IO_PENDING);
}

BOOL CNamedPipeConnector::RequestAndReply(LPVOID lpSendBuf, DWORD dwSendBufSize, LPVOID lpReplyBuf, DWORD dwReplyBufSize, LPDWORD dwTransactSize)
{
    LPIPC_DATA_OVERLAPPEDEX sendPackage = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(sendPackage, sizeof(IPC_DATA_OVERLAPPEDEX));
    sendPackage->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    sendPackage->ipcDataPackage.dwDataSize = dwSendBufSize;
    sendPackage->ipcDataPackage.dwProcessID = GetCurrentProcessId();
    sendPackage->ipcDataPackage.dwTotalSize = sizeof(IPC_DATA_PACKAGE) - SYELOG_MAXIMUM_MESSAGE * sizeof(TCHAR) + dwSendBufSize;
    GetSystemTimeAsFileTime(&sendPackage->ipcDataPackage.ftOccurance);
    memcpy_s(sendPackage->ipcDataPackage.lpData, SYELOG_MAXIMUM_MESSAGE, lpSendBuf, dwSendBufSize);

    IPC_DATA_OVERLAPPEDEX recePackage;
    ZeroMemory(&recePackage, sizeof(IPC_DATA_PACKAGE));

    DWORD dwWrited = 0;
    BOOL bSucess = m_pipe.WriteFile(&sendPackage->ipcDataPackage, sendPackage->ipcDataPackage.dwTotalSize, &dwWrited, sendPackage);

    if(!bSucess && GetLastError() == ERROR_IO_PENDING)
    {
        if(GetOverlappedResult(m_pipe.GetHandle(), sendPackage, dwTransactSize, TRUE))
            bSucess = TRUE;
    }

    LPIPC_DATA_OVERLAPPEDEX recvPackage = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(recvPackage, sizeof(IPC_DATA_OVERLAPPEDEX));
    recvPackage->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    DWORD dwReaded = 0;
    bSucess = m_pipe.ReadFile(&recePackage.ipcDataPackage, sizeof(IPC_DATA_PACKAGE), &dwReaded, recvPackage);

    if(!bSucess && GetLastError() == ERROR_IO_PENDING)
    {
        if(GetOverlappedResult(m_pipe.GetHandle(), recvPackage, dwTransactSize, TRUE))
            bSucess = TRUE;
    }

    if(bSucess)
    {
        memcpy_s(lpReplyBuf, dwReplyBufSize, recePackage.ipcDataPackage.lpData, recePackage.ipcDataPackage.dwDataSize);
        *dwTransactSize = recePackage.ipcDataPackage.dwDataSize;
    }

    return bSucess;
}

BOOL CNamedPipeConnector::Create(LPCTSTR lpPipeName)
{
    return m_pipe.CreateNamedPipe(lpPipeName, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                                  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                  PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, NULL);
}

void CNamedPipeConnector::Close()
{
    m_pipe.FlushFileBuffers();
    m_pipe.DisconnectNamedPipe();
    m_pipe.Close();
}

HANDLE CNamedPipeConnector::GetHandle()
{
    return m_pipe.GetHandle();
}

BOOL CNamedPipeConnector::DoRead()
{
    LPIPC_DATA_OVERLAPPEDEX package = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(package, sizeof(IPC_DATA_OVERLAPPEDEX));
    package->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    package->emMessageType = IPC_MESSAGE_READ;
    BOOL bSucess = m_pipe.ReadFile(&package->ipcDataPackage, sizeof(IPC_DATA_PACKAGE), NULL, package);
    return (bSucess || ERROR_IO_PENDING == GetLastError());
}

BOOL CNamedPipeConnector::WaitConnect()
{
    m_conn = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(m_conn, sizeof(IPC_DATA_OVERLAPPEDEX));
    m_conn->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_conn->emMessageType = IPC_MESSAGE_CLIENTCONNECT;
    BOOL bSucess = m_pipe.ConnectNamedPipe(m_conn);
    return (bSucess || GetLastError() == ERROR_IO_PENDING);
}

void CNamedPipeConnector::ClearOverlapped(LPIPC_DATA_OVERLAPPEDEX dataOverlapped)
{
    if(NULL != dataOverlapped)
    {
        CloseHandle(dataOverlapped->hEvent);
        dataOverlapped->hEvent = NULL;
        delete dataOverlapped;
        dataOverlapped = NULL;
    }
}

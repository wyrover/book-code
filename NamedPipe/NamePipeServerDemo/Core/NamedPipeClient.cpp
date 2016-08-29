#include "stdafx.h"
#include "NamedPipeClient.h"


CNamedPipeClient::CNamedPipeClient(IIPCEvent* pEvent): m_pEvent(pEvent), m_hThreadIOCP(NULL), m_dwProcessID(0)
{

}


CNamedPipeClient::~CNamedPipeClient(void)
{
    Close();
}

BOOL CNamedPipeClient::Create(LPCTSTR lpPipeName)
{
    TCHAR sPipeName[MAX_PATH] = {0};
    _tcscpy_s(sPipeName, MAX_PATH, _T("\\\\.\\pipe\\"));
    _tcscat_s(sPipeName, lpPipeName);

    while(TRUE)
    {
        if(m_pipe.CreateFile(sPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL))
            break;

        DWORD dwError = GetLastError();

        if(ERROR_PIPE_BUSY != dwError)
            return FALSE;

        if(!m_pipe.WaitNamedPipe(sPipeName, 5000))
            return FALSE;
    }

    const DWORD dwThreadCount = GetCpuNum();

    if(!m_iocp.Create(dwThreadCount))
        return FALSE;

    m_hThreadIOCP = new HANDLE[dwThreadCount];

    for(DWORD i = 0; i < dwThreadCount; ++i)
        m_hThreadIOCP[i] = CreateThread(0, 0, IOCompletionThread, this, 0, NULL);

    DWORD dwMode = PIPE_READMODE_MESSAGE ;

    if(!m_pipe.SetNamedPipeHandleState(&dwMode, NULL, NULL))
        return FALSE;

    if(!m_iocp.AssociateDevice(m_pipe.GetHandle(), (ULONG_PTR)this))
        return FALSE;

    return TRUE;
}

void CNamedPipeClient::Close()
{
    m_pipe.Close();
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
}

HANDLE CNamedPipeClient::GetHandle()
{
    return m_pipe.GetHandle();
}

BOOL CNamedPipeClient::PostMessage(LPCVOID lpBuf, DWORD dwBufSize)
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

IIPCConnectorIterator* CNamedPipeClient::GetClients()
{
    return this;
}

void CNamedPipeClient::Begin()
{
    m_iIterator = 0;
}

BOOL CNamedPipeClient::End()
{
    return (1 == m_iIterator);
}

void CNamedPipeClient::Next()
{
    m_iIterator++;
}

IIPCConnector* CNamedPipeClient::GetCurrent()
{
    return this;
}

BOOL CNamedPipeClient::RequestAndReply(LPVOID lpSendBuf, DWORD dwSendBufSize, LPVOID lpReplyBuf, DWORD dwReplyBufSize, LPDWORD dwTransactSize)
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


DWORD CNamedPipeClient::GetSID()
{
    return m_dwProcessID;
}

LPCTSTR CNamedPipeClient::GetName()
{
    return m_sName;
}

DWORD CNamedPipeClient::IOCompletionThread(LPVOID lpParam)
{
    CNamedPipeClient* pThis = (CNamedPipeClient*)lpParam;

    if(NULL == lpParam)
        return -1;

    CIOCompletionPort* iocp = &pThis->m_iocp;

    CNamedPipeClient* pClient = NULL;
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
            pClient->ClearOverlapped(po);
            continue;
        }

        IPC_MESSAGE_TYPE type = IPC_MESSAGE_UKNOWN;

        if(NULL != po)
            type = po->emMessageType;

        switch(type)
        {
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

void CNamedPipeClient::ClearOverlapped(LPOVERLAPPED lpo)
{
    if(NULL != lpo)
    {
        CloseHandle(lpo->hEvent);
        lpo->hEvent = NULL;
        delete lpo;
        lpo = NULL;
    }
}

BOOL CNamedPipeClient::CloseConnection(CNamedPipeClient* pConnector)
{
    m_pipe.FlushFileBuffers();
    m_pipe.DisconnectNamedPipe();
    m_pipe.Close();
    return TRUE;
}

BOOL CNamedPipeClient::DoRead()
{
    LPIPC_DATA_OVERLAPPEDEX package = new IPC_DATA_OVERLAPPEDEX;
    ZeroMemory(package, sizeof(IPC_DATA_OVERLAPPEDEX));
    package->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    package->emMessageType = IPC_MESSAGE_READ;
    BOOL bSucess = m_pipe.ReadFile(&package->ipcDataPackage, sizeof(IPC_DATA_PACKAGE), NULL, package);
    return (bSucess || ERROR_IO_PENDING == GetLastError());
}

DWORD CNamedPipeClient::GetCpuNum()
{
    SYSTEM_INFO sysInfo = { 0 };
    GetNativeSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

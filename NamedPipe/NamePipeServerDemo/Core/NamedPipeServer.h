#pragma once
#include "IIPCInterface.h"
#include "IoCompletePort.h"
#include "NamedPipeWrapper.h"

class CNamedPipeConnector;
class CNamedPipeServer: public IIPCObject, public IIPCConnectorIterator
{
public:
    CNamedPipeServer(IIPCEvent* pEvent);

    virtual ~CNamedPipeServer();

    virtual BOOL Create(LPCTSTR lpPipeName);

    virtual void Close();

    virtual IIPCConnectorIterator* GetClients();

    virtual void Begin();

    virtual BOOL End();

    virtual void Next();

    virtual IIPCConnector* GetCurrent();

protected:

    static DWORD WINAPI IOCompletionThread(LPVOID lpParam);

    BOOL WaitClientConnect();

    DWORD GetCpuNum();

    ConnectorMap::const_iterator FindClient(CNamedPipeConnector* pClient);

private:

    CRITICAL_SECTION m_csClientMap;

    CIOCompletionPort m_iocp;

    ConnectorMap m_connectorMap;

    ConnectorMap::const_iterator m_citCurrent;

    TCHAR m_sPipeName[MAX_PATH];

    HANDLE* m_hThreadIOCP;

    IIPCEvent* m_pEvent;
};

class CNamedPipeConnector : public IIPCConnector
{
public:
    CNamedPipeConnector();

    virtual ~CNamedPipeConnector();

    BOOL Create(LPCTSTR lpPipeName);

    BOOL WaitConnect();

    void Close();

    BOOL DoRead();

    BOOL DoReadWrite(LPVOID lpRequest, DWORD dwRequestSize);

    HANDLE GetHandle();

    virtual DWORD GetSID();

    virtual LPCTSTR GetName();

    virtual BOOL PostMessage(LPCVOID lpBuf, DWORD dwBufSize);

    virtual BOOL RequestAndReply(LPVOID lpSendBuf, DWORD dwSendBufSize, LPVOID lpReplyBuf, DWORD dwReplyBufSize, LPDWORD dwTransactSize);

    friend class CNamedPipeServer;

protected:

    void ClearOverlapped(LPIPC_DATA_OVERLAPPEDEX dataOverlapped);

private:
    CNamedPipeWrapper m_pipe;

    BOOL m_bExit;

    DWORD m_dwProcessID;

    LPIPC_DATA_OVERLAPPEDEX m_conn;
};
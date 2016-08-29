#pragma once
#include "IIPCInterface.h"
#include "IoCompletePort.h"
#include "NamedPipeWrapper.h"

class CNamedPipeClient : public IIPCObject , public IIPCConnector, public IIPCConnectorIterator
{
public:

    CNamedPipeClient(IIPCEvent* pEvent);

    virtual ~CNamedPipeClient(void);

    virtual BOOL Create(LPCTSTR lpPipeName);

    virtual void Close();

    virtual IIPCConnectorIterator* GetClients();

    virtual HANDLE GetHandle();

    virtual DWORD GetSID();

    virtual LPCTSTR GetName();

    virtual BOOL PostMessage(LPCVOID lpBuf, DWORD dwBufSize);

    virtual BOOL RequestAndReply(LPVOID lpSendBuf, DWORD dwSendBufSize, LPVOID lpReplyBuf, DWORD dwReplyBufSize, LPDWORD dwTransactSize);

    virtual void Begin();

    virtual BOOL End();

    virtual void Next();

    virtual IIPCConnector* GetCurrent();

protected:

    static DWORD WINAPI IOCompletionThread(LPVOID lpParam);

    BOOL CloseConnection(CNamedPipeClient* pConnector);

    BOOL DoRead();

    void ClearOverlapped(LPOVERLAPPED lpo);

    DWORD GetCpuNum();

private:

    IIPCEvent* m_pEvent;

    int m_iIterator;

    TCHAR m_sName[MAX_PATH];

    DWORD m_dwProcessID;

    CIOCompletionPort m_iocp;

    CNamedPipeWrapper m_pipe;

    HANDLE* m_hThreadIOCP;

};


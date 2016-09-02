// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)
#define AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "SocketComm.h"

#define WM_UPDATE_CONNECTION    WM_APP+0x1234

class CSocketManager : public CSocketComm
{
public:
    CSocketManager();
    virtual ~CSocketManager();

    void SetMessageWindow(CEdit* pMsgCtrl);
    void AppendMessage(LPCTSTR strText);
public:

    virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
    virtual void OnEvent(UINT uEvent, LPVOID lpvData);

protected:
    void DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom);
    CEdit* m_pMsgCtrl;
};

#endif // !defined(AFX_SOCKETMANAGER_H__7403BD71_338A_4531_BD91_3D7E5B505793__INCLUDED_)

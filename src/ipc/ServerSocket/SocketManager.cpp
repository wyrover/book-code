// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlconv.h>
#include "ServerSocket.h"
#include "SocketManager.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #define new DEBUG_NEW
#endif

/*
const UINT EVT_CONSUCCESS = 0x0000; // Connection established
const UINT EVT_CONFAILURE = 0x0001; // General failure - Wait Connection failed
const UINT EVT_CONDROP    = 0x0002; // Connection dropped
const UINT EVT_ZEROLENGTH = 0x0003; // Zero length message
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager()
    : m_pMsgCtrl(NULL)
{
}

CSocketManager::~CSocketManager()
{
}


void CSocketManager::DisplayData(const LPBYTE lpData, DWORD dwCount, const SockAddrIn& sfrom)
{
    CString strData;
#ifndef UNICODE
    USES_CONVERSION;
    memcpy(strData.GetBuffer(dwCount), A2CT((LPSTR)lpData), dwCount);
    strData.ReleaseBuffer(dwCount);
#else
    MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<LPCSTR>(lpData), dwCount, strData.GetBuffer(dwCount + 1), dwCount + 1);
    strData.ReleaseBuffer(dwCount);
#endif

    if (!sfrom.IsNull()) {
        LONG  uAddr = sfrom.GetIPAddr();
        BYTE* sAddr = (BYTE*) &uAddr;
        int nPort = ntohs(sfrom.GetPort());   // show port in host format...
        CString strAddr;
        // Address is stored in network format...
        strAddr.Format(_T("%u.%u.%u.%u (%d)>"),
                       (UINT)(sAddr[0]), (UINT)(sAddr[1]),
                       (UINT)(sAddr[2]), (UINT)(sAddr[3]), nPort);
        strData = strAddr + strData;
    }

    AppendMessage(strData);
}


void CSocketManager::AppendMessage(LPCTSTR strText)
{
    if (NULL == m_pMsgCtrl)
        return;

    /*
        if (::IsWindow( m_pMsgCtrl->GetSafeHwnd() ))
        {
            int nLen = m_pMsgCtrl->GetWindowTextLength();
            m_pMsgCtrl->SetSel(nLen, nLen);
            m_pMsgCtrl->ReplaceSel( strText );
        }
    */
    HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
    DWORD dwResult = 0;

    if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0) {
        int nLen = (int) dwResult;

        if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0) {
            if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)strText, SMTO_NORMAL, 1000L, &dwResult) != 0) {
            }
        }
    }
}


void CSocketManager::SetMessageWindow(CEdit* pMsgCtrl)
{
    m_pMsgCtrl = pMsgCtrl;
}


void CSocketManager::OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount)
{
    LPBYTE lpData = lpBuffer;
    SockAddrIn origAddr;
    stMessageProxy msgProxy;

    if (IsSmartAddressing()) {
        dwCount = __min(sizeof(msgProxy), dwCount);
        memcpy(&msgProxy, lpBuffer, dwCount);
        origAddr = msgProxy.address;

        if (IsServer()) {
            // broadcast message to all
            msgProxy.address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            WriteComm((const LPBYTE)&msgProxy, dwCount, 0L);
        }

        dwCount -= sizeof(msgProxy.address);
        lpData = msgProxy.byData;
    }

    // Display data to message list
    DisplayData(lpData, dwCount, origAddr);
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
// Send message to parent window to indicate connection status
void CSocketManager::OnEvent(UINT uEvent, LPVOID lpvData)
{
    if (NULL == m_pMsgCtrl)
        return;

    CWnd* pParent = m_pMsgCtrl->GetParent();

    if (!::IsWindow(pParent->GetSafeHwnd()))
        return;

    switch (uEvent) {
    case EVT_CONSUCCESS:
        AppendMessage(_T("Connection Established\r\n"));
        break;

    case EVT_CONFAILURE:
        AppendMessage(_T("Connection Failed\r\n"));
        break;

    case EVT_CONDROP:
        AppendMessage(_T("Connection Abandonned\r\n"));
        break;

    case EVT_ZEROLENGTH:
        AppendMessage(_T("Zero Length Message\r\n"));
        break;

    default:
        TRACE("Unknown Socket event\n");
        break;
    }

    pParent->PostMessage(WM_UPDATE_CONNECTION, uEvent, (LPARAM) this);
}

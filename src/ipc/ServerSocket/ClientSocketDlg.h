#if !defined(AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_)
#define AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000
// ClientSocketDlg.h : header file
//
#include "SocketManager.h"

/////////////////////////////////////////////////////////////////////////////
// CClientSocketDlg dialog

class CClientSocketDlg : public CDialog
{
// Construction
public:
    CClientSocketDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CClientSocketDlg)
    enum { IDD = IDD_CLIENTSOCKET_DIALOG };
    CEdit   m_ctlMessage;
    CSpinButtonCtrl m_ctlPortInc;
    CEdit   m_ctlMsgList;
    CIPAddressCtrl  m_ctlIPAddress;
    CString m_strPort;
    int     m_nSockType;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CClientSocketDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    CSocketManager m_SocketManager;
    SockAddrIn      m_SockPeer;
    // Generated message map functions
    //{{AFX_MSG(CClientSocketDlg)
    afx_msg void OnBtnConnect();
    afx_msg void OnBtnDisconnect();
    afx_msg void OnBtnSend();
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKETDLG_H__014D90D9_161C_4561_A8DC_982D723B8F65__INCLUDED_)

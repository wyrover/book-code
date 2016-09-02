// ServerSocketDlg.h : header file
//

#if !defined(AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_)
#define AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "SocketManager.h"

#define MAX_CONNECTION      10

/////////////////////////////////////////////////////////////////////////////
// CServerSocketDlg dialog

class CServerSocketDlg : public CDialog
{
// Construction
public:
    CServerSocketDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CServerSocketDlg)
    enum { IDD = IDD_SERVERSOCKET_DIALOG };
    CEdit   m_ctlMessage;
    CEdit   m_ctlMsgList;
    CSpinButtonCtrl m_ctlPortInc;
    CString m_strPort;
    int     m_nSockType;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CServerSocketDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    CSocketManager m_SocketManager[MAX_CONNECTION];
    CSocketManager* m_pCurServer;

    void PickNextAvailable();
    bool StartServer();

    // Generated message map functions
    //{{AFX_MSG(CServerSocketDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBtnStart();
    afx_msg void OnBtnStop();
    afx_msg void OnDestroy();
    afx_msg void OnBtnSend();
    //}}AFX_MSG
    afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKETDLG_H__BF055235_5494_4FFC_8289_20DBFD9503A8__INCLUDED_)

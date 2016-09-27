// MFCSampleClientDlg.h : header file
//
#import "..\MFCSample\debug\MFCSample.tlb" rename_namespace("AccountDriver")
using namespace AccountDriver;

#if !defined(AFX_MFCSAMPLECLIENTDLG_H__3B4F8EAC_80C1_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_MFCSAMPLECLIENTDLG_H__3B4F8EAC_80C1_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CMFCSampleClientDlg dialog

class CMFCSampleClientDlg : public CDialog
{
// Construction
public:
    CMFCSampleClientDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CMFCSampleClientDlg)
    enum { IDD = IDD_MFCSAMPLECLIENT_DIALOG };
    long    m_amount;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCSampleClientDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    IAccountPtr m_account;

    // Generated message map functions
    //{{AFX_MSG(CMFCSampleClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    afx_msg void OnPost();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCSAMPLECLIENTDLG_H__3B4F8EAC_80C1_11D4_A23D_02608CACEDBB__INCLUDED_)

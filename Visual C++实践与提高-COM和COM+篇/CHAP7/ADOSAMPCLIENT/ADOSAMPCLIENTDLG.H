// ADOSampClientDlg.h : header file
//

#if !defined(AFX_ADOSAMPCLIENTDLG_H__504685A9_825B_11D4_8C0E_446260000000__INCLUDED_)
#define AFX_ADOSAMPCLIENTDLG_H__504685A9_825B_11D4_8C0E_446260000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CADOSampClientDlg dialog

class CADOSampClientDlg : public CDialog
{
// Construction
public:
    CADOSampClientDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CADOSampClientDlg)
    enum { IDD = IDD_ADOSAMPCLIENT_DIALOG };
    long    m_lAccount;
    long    m_lAmount;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CADOSampClientDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    IAccountPtr m_account;

    // Generated message map functions
    //{{AFX_MSG(CADOSampClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPost();
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADOSAMPCLIENTDLG_H__504685A9_825B_11D4_8C0E_446260000000__INCLUDED_)

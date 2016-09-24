// NewPingDlg.h : header file
//

#if !defined(AFX_NEWPINGDLG_H__BD1F1015_91F7_4A90_8D90_DC7CE5B14227__INCLUDED_)
#define AFX_NEWPINGDLG_H__BD1F1015_91F7_4A90_8D90_DC7CE5B14227__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNewPingDlg dialog

class CNewPingDlg : public CDialog
{
// Construction
public:
    CNewPingDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CNewPingDlg)
    enum { IDD = IDD_NEWPING_DIALOG };
    CListCtrl   m_ctlListEcho;
    CString m_strDestIP;
    CString m_strComment;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewPingDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CNewPingDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONIcmpDll();
    afx_msg void OnBUTTONClear();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPINGDLG_H__BD1F1015_91F7_4A90_8D90_DC7CE5B14227__INCLUDED_)

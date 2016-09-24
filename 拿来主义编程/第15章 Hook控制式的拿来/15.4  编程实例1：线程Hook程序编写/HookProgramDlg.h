// HookProgramDlg.h : header file
//

#if !defined(AFX_HOOKPROGRAMDLG_H__6F731002_D652_4C6C_B816_6E1C77EE82EF__INCLUDED_)
#define AFX_HOOKPROGRAMDLG_H__6F731002_D652_4C6C_B816_6E1C77EE82EF__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHookProgramDlg dialog

class CHookProgramDlg : public CDialog
{
// Construction
public:
    CHookProgramDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CHookProgramDlg)
    enum { IDD = IDD_HOOKPROGRAM_DIALOG };
    CListBox    m_ctlListSave;
    CString m_strComment;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHookProgramDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CHookProgramDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONInstallThreadHook();
    afx_msg void OnBUTTONUninstallThreadHook();
    afx_msg void OnBUTTONInstallSystemHook();
    afx_msg void OnBUTTONUninstallSystemHook();
    afx_msg void OnBUTTONClear();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKPROGRAMDLG_H__6F731002_D652_4C6C_B816_6E1C77EE82EF__INCLUDED_)

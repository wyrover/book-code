// ProcessMonitorDlg.h : header file
//

#if !defined(AFX_PROCESSMONITORDLG_H__04244D22_D782_4328_8964_47B6C95D4A5B__INCLUDED_)
#define AFX_PROCESSMONITORDLG_H__04244D22_D782_4328_8964_47B6C95D4A5B__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProcessMonitorDlg dialog

class CProcessMonitorDlg : public CDialog
{
// Construction
public:
    void TerminateProcessByID(CString strID);
    CString m_strFullProcessName;//所有进程的名称，为了查找方便。
    CString m_strProgramDir;
    CProcessMonitorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CProcessMonitorDlg)
    enum { IDD = IDD_PROCESSMONITOR_DIALOG };
    CListCtrl   m_ctlListMonitor;
    CTreeCtrl   m_ctlTreeProcess;
    UINT    m_iFrequence;
    CString m_strProgram;
    BOOL    m_bRunOrNot;
    CString m_strPathname;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProcessMonitorDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CProcessMonitorDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONEnum();
    afx_msg void OnBUTTONTerminateProcess();
    afx_msg void OnBUTTONEnumWindow();
    afx_msg void OnBUTTONBrowser();
    afx_msg void OnBUTTONAdd();
    afx_msg void OnBUTTONDelete();
    afx_msg void OnBUTTONMonitor();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMONITORDLG_H__04244D22_D782_4328_8964_47B6C95D4A5B__INCLUDED_)

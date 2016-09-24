// CWebDeviceMonitorDlg.h : header file
//

#if !defined(AFX_CWEBDEVICEMONITORDLG_H__76AC1404_61B4_4D8A_ADA2_B7AF86781DDD__INCLUDED_)
#define AFX_CWEBDEVICEMONITORDLG_H__76AC1404_61B4_4D8A_ADA2_B7AF86781DDD__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCWebDeviceMonitorDlg dialog

class CCWebDeviceMonitorDlg : public CDialog
{
// Construction
public:
    CString GetValue(CString strWhole, CString strLabel, CString strHead, CString strTail);
    void ReadHTML();
    CCWebDeviceMonitorDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CCWebDeviceMonitorDlg)
    enum { IDD = IDD_CWEBDEVICEMONITOR_DIALOG };
    CString m_strHTMLWhole;
    CString m_strHTMLMonitor1;
    CString m_strHTMLMonitor2;
    CString m_strHTMLMonitor3;
    CString m_strComment;
    CString m_strURL;
    int     m_iReadFreq;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCWebDeviceMonitorDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CCWebDeviceMonitorDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONStart();
    afx_msg void OnBUTTONStop();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWEBDEVICEMONITORDLG_H__76AC1404_61B4_4D8A_ADA2_B7AF86781DDD__INCLUDED_)

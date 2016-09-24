// EmbedScreenDlg.h : header file
//

#if !defined(AFX_EMBEDSCREENDLG_H__D909089F_5376_44A9_BCC5_C43D0A8F9DF6__INCLUDED_)
#define AFX_EMBEDSCREENDLG_H__D909089F_5376_44A9_BCC5_C43D0A8F9DF6__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEmbedScreenDlg dialog

class CEmbedScreenDlg : public CDialog
{
// Construction
public:
    void CopyScreen();
    CRect m_rectEmbed;
    CEmbedScreenDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CEmbedScreenDlg)
    enum { IDD = IDD_EMBEDSCREEN_DIALOG };
    CString m_strProgramName;
    int     m_iOffsetLeft;
    int     m_iOffsetTop;
    int     m_iRatio;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEmbedScreenDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CEmbedScreenDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONEmbedStart();
    afx_msg void OnBUTTONEmbedStop();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMBEDSCREENDLG_H__D909089F_5376_44A9_BCC5_C43D0A8F9DF6__INCLUDED_)

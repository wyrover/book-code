// PipeDlg.h : header file
//

#if !defined(AFX_PIPEDLG_H__74325A11_74C9_4154_8BCB_E1AF1F3209AB__INCLUDED_)
#define AFX_PIPEDLG_H__74325A11_74C9_4154_8BCB_E1AF1F3209AB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPipeDlg dialog

class CPipeDlg : public CDialog
{
// Construction
public:
    CPipeDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CPipeDlg)
    enum { IDD = IDD_PIPE_DIALOG };
    CString m_strCommand;
    CString m_strResult;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPipeDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CPipeDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONRun();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPEDLG_H__74325A11_74C9_4154_8BCB_E1AF1F3209AB__INCLUDED_)

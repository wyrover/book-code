// RedirectDlg.h : header file
//

#if !defined(AFX_REDIRECTDLG_H__9C3544F2_B005_4198_8CA2_E3551924AC0F__INCLUDED_)
#define AFX_REDIRECTDLG_H__9C3544F2_B005_4198_8CA2_E3551924AC0F__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRedirectDlg dialog

class CRedirectDlg : public CDialog
{
// Construction
public:
    CRedirectDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CRedirectDlg)
    enum { IDD = IDD_REDIRECT_DIALOG };
    CString m_strResult;
    CString m_strCommand;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRedirectDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CRedirectDlg)
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

#endif // !defined(AFX_REDIRECTDLG_H__9C3544F2_B005_4198_8CA2_E3551924AC0F__INCLUDED_)

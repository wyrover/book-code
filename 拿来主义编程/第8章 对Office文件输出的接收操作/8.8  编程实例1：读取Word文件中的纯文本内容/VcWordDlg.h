// VcWordDlg.h : header file
//

#if !defined(AFX_VCWORDDLG_H__6D189272_E4EB_47E5_B032_6C39C65E44A5__INCLUDED_)
#define AFX_VCWORDDLG_H__6D189272_E4EB_47E5_B032_6C39C65E44A5__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVcWordDlg dialog

class CVcWordDlg : public CDialog
{
// Construction
public:
    //CString m_strCurrentDirectory;
    CVcWordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CVcWordDlg)
    enum { IDD = IDD_VCWORD_DIALOG };
    CString m_strFilename;
    CString m_strContent;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcWordDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CVcWordDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    afx_msg void OnBUTTONBrowser();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCWORDDLG_H__6D189272_E4EB_47E5_B032_6C39C65E44A5__INCLUDED_)

// VCAutoClwDlg.h : header file
//
#include "atlsample.h"

#if !defined(AFX_VCAUTOCLWDLG_H__3B4F8E82_80C1_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_VCAUTOCLWDLG_H__3B4F8E82_80C1_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVCAutoClwDlg dialog

class CVCAutoClwDlg : public CDialog
{
// Construction
public:
    CVCAutoClwDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CVCAutoClwDlg)
    enum { IDD = IDD_VCAUTOCLW_DIALOG };
    long    m_amount;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVCAutoClwDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    IAccount m_account;

    // Generated message map functions
    //{{AFX_MSG(CVCAutoClwDlg)
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

#endif // !defined(AFX_VCAUTOCLWDLG_H__3B4F8E82_80C1_11D4_A23D_02608CACEDBB__INCLUDED_)

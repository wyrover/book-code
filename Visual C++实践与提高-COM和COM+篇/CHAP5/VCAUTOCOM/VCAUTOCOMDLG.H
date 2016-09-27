// VCAutoComDlg.h : header file
//

#if !defined(AFX_VCAUTOCOMDLG_H__55DB95C7_8631_11D4_8C1F_0000E83EBA18__INCLUDED_)
#define AFX_VCAUTOCOMDLG_H__55DB95C7_8631_11D4_8C1F_0000E83EBA18__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "..\..\chap5\ATLSample\ATLSample.h"
#include "..\..\chap5\ATLSample\ATLSample_i.c"
/////////////////////////////////////////////////////////////////////////////
// CVCAutoComDlg dialog

class CVCAutoComDlg : public CDialog
{
// Construction
public:
    CVCAutoComDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CVCAutoComDlg)
    enum { IDD = IDD_VCAUTOCOM_DIALOG };
    long    m_amount;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVCAutoComDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    IAccount* pAccount;

    // Generated message map functions
    //{{AFX_MSG(CVCAutoComDlg)
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

#endif // !defined(AFX_VCAUTOCOMDLG_H__55DB95C7_8631_11D4_8C1F_0000E83EBA18__INCLUDED_)

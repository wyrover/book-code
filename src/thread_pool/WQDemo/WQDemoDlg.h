// WQDemoDlg.h : header file
//

#if !defined(AFX_WQDEMODLG_H__9462F3B7_FCCE_46C3_82E5_87788CD2D03F__INCLUDED_)
#define AFX_WQDEMODLG_H__9462F3B7_FCCE_46C3_82E5_87788CD2D03F__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWQDemoDlg dialog
#include "WorkQueue.h"

#include "XListCtrl.h"

class CWQDemoDlg : public CDialog
{


    CWorkQueue m_WorkQueue;
// Construction
public:

    static unsigned long __stdcall ThreadFuncDestroy(void*  pParam);

    CWQDemoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CWQDemoDlg)
    enum { IDD = IDD_WQDEMO_DIALOG };
    CXListCtrl  m_ItemsOueueList;
    UINT    m_nNumberOfThreads;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWQDemoDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CWQDemoDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnCreate();
    afx_msg void OnDestroy();
    afx_msg void OnAddItemBtn();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WQDEMODLG_H__9462F3B7_FCCE_46C3_82E5_87788CD2D03F__INCLUDED_)

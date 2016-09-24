// ReadMemoryDlg.h : header file
//

#if !defined(AFX_READMEMORYDLG_H__533DCD51_26D3_4E56_AC67_C5E17ED115ED__INCLUDED_)
#define AFX_READMEMORYDLG_H__533DCD51_26D3_4E56_AC67_C5E17ED115ED__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CReadMemoryDlg dialog

class CReadMemoryDlg : public CDialog
{
// Construction
public:
    CReadMemoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CReadMemoryDlg)
    enum { IDD = IDD_READMEMORY_DIALOG };
    CListCtrl   m_ctlListContent;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReadMemoryDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CReadMemoryDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONOpen();
    afx_msg void OnBUTTONDebugStart();
    afx_msg void OnBUTTONDebugStop();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READMEMORYDLG_H__533DCD51_26D3_4E56_AC67_C5E17ED115ED__INCLUDED_)

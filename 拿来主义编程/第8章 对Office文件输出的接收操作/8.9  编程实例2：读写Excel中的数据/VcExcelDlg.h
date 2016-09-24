// VcExcelDlg.h : header file
//

#if !defined(AFX_VCEXCELDLG_H__A97E9B41_CDFE_4681_9506_6360158FCD31__INCLUDED_)
#define AFX_VCEXCELDLG_H__A97E9B41_CDFE_4681_9506_6360158FCD31__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVcExcelDlg dialog

class CVcExcelDlg : public CDialog
{
// Construction
public:

    CVcExcelDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CVcExcelDlg)
    enum { IDD = IDD_VCEXCEL_DIALOG };
    CListCtrl   m_ctlListExcel;
    CString m_strFilename;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcExcelDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CVcExcelDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONBrowser();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCEXCELDLG_H__A97E9B41_CDFE_4681_9506_6360158FCD31__INCLUDED_)

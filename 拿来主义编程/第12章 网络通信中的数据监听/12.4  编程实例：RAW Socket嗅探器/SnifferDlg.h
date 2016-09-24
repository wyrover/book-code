// SnifferDlg.h : header file
//

#if !defined(AFX_SNIFFERDLG_H__37DB02C7_B8E5_4EE9_805C_A857840FE753__INCLUDED_)
#define AFX_SNIFFERDLG_H__37DB02C7_B8E5_4EE9_805C_A857840FE753__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSnifferDlg dialog

class CSnifferDlg : public CDialog
{
// Construction
public:
    CSnifferDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CSnifferDlg)
    enum { IDD = IDD_SNIFFER_DIALOG };
    CListCtrl   m_ctlListReceive;
    CString m_strContent;
    int     m_iShowMode;
    CString m_strComment;
    CString m_strIPFilter;
    UINT    m_uPortFilter;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSnifferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CSnifferDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONStart();
    afx_msg void OnBUTTONStop();
    afx_msg void OnDblclkLISTReceive(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickLISTReceive(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBUTTONClearResult();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNIFFERDLG_H__37DB02C7_B8E5_4EE9_805C_A857840FE753__INCLUDED_)

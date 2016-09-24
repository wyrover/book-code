// PortSnifferDlg.h : header file
//

#if !defined(AFX_PORTSNIFFERDLG_H__008C8392_D044_4FB8_94B5_A62ECAFA76F4__INCLUDED_)
#define AFX_PORTSNIFFERDLG_H__008C8392_D044_4FB8_94B5_A62ECAFA76F4__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPortSnifferDlg dialog

class CPortSnifferDlg : public CDialog
{
// Construction
public:
    void WriteToList(BOOL bDir, CString strMsg);
    CString ComDisconnect(int iCom);
    CString ComConnect(int iCom);
    BOOL m_bReading;
    void Uninit();
    void Init();
    CPortSnifferDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CPortSnifferDlg)
    enum { IDD = IDD_PORTSNIFFER_DIALOG };
    CListCtrl   m_ctlListCommand;
    CString m_strComment;
    int     m_iComReal;
    int     m_iComVirtual;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPortSnifferDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CPortSnifferDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONStart();
    afx_msg void OnBUTTONDStop();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSNIFFERDLG_H__008C8392_D044_4FB8_94B5_A62ECAFA76F4__INCLUDED_)

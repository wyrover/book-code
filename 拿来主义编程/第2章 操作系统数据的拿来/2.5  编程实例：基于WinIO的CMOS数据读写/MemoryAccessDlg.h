// MemoryAccessDlg.h : header file
//

#if !defined(AFX_MEMORYACCESSDLG_H__CD116AD4_3891_44E1_B180_9DF9DAB97C29__INCLUDED_)
#define AFX_MEMORYACCESSDLG_H__CD116AD4_3891_44E1_B180_9DF9DAB97C29__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessDlg dialog

class CMemoryAccessDlg : public CDialog
{
// Construction
public:
    CString GetWeekName(UCHAR week);
    UCHAR ReadFromCMOS(int index);
    CMemoryAccessDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CMemoryAccessDlg)
    enum { IDD = IDD_MEMORYACCESS_DIALOG };
    CString m_strDateTime;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMemoryAccessDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CMemoryAccessDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBUTTONRead();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYACCESSDLG_H__CD116AD4_3891_44E1_B180_9DF9DAB97C29__INCLUDED_)

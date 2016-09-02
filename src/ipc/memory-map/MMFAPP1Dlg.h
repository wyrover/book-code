// MMFAPP1Dlg.h : header file
//

#if !defined(AFX_MMFAPP1DLG_H__631C0FD5_35E9_4EE1_98C9_74E71AD8C2BC__INCLUDED_)
#define AFX_MMFAPP1DLG_H__631C0FD5_35E9_4EE1_98C9_74E71AD8C2BC__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMMFAPP1Dlg dialog

class CMMFAPP1Dlg : public CDialog
{
// Construction
public:
    CMMFAPP1Dlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
    //{{AFX_DATA(CMMFAPP1Dlg)
    enum { IDD = IDD_MMFAPP1_DIALOG };
    CString m_Text;
    CString m_ReadText;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMMFAPP1Dlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    HANDLE m_hFileMMF;      // memory mapped file
    LPVOID m_pViewMMFFile;       // view of file, contains text in edit box

    // Generated message map functions
    //{{AFX_MSG(CMMFAPP1Dlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonWriteData();
    afx_msg void OnButtonReadData();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMFAPP1DLG_H__631C0FD5_35E9_4EE1_98C9_74E71AD8C2BC__INCLUDED_)

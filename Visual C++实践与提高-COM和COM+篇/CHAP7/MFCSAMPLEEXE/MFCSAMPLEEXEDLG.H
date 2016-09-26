// MFCSampleExeDlg.h : header file
//

#if !defined(AFX_MFCSAMPLEEXEDLG_H__CDB6A4E0_819B_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_MFCSAMPLEEXEDLG_H__CDB6A4E0_819B_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CMFCSampleExeDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlg dialog

class CMFCSampleExeDlg : public CDialog
{
    DECLARE_DYNAMIC(CMFCSampleExeDlg);
    friend class CMFCSampleExeDlgAutoProxy;

// Construction
public:
    CMFCSampleExeDlg(CWnd* pParent = NULL); // standard constructor
    virtual ~CMFCSampleExeDlg();

// Dialog Data
    //{{AFX_DATA(CMFCSampleExeDlg)
    enum { IDD = IDD_MFCSAMPLEEXE_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCSampleExeDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CMFCSampleExeDlgAutoProxy* m_pAutoProxy;
    HICON m_hIcon;

    BOOL CanExit();

    // Generated message map functions
    //{{AFX_MSG(CMFCSampleExeDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCSAMPLEEXEDLG_H__CDB6A4E0_819B_11D4_A23D_02608CACEDBB__INCLUDED_)

// MyCalculatorDlg.h : header file
//

#if !defined(AFX_MYCALCULATORDLG_H__7E4BACF9_6AB7_4D3D_B9E7_5550A7719BF8__INCLUDED_)
#define AFX_MYCALCULATORDLG_H__7E4BACF9_6AB7_4D3D_B9E7_5550A7719BF8__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCalculatorDlg dialog

class CMyCalculatorDlg : public CDialog
{
// Construction
public:
    CMyCalculatorDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CMyCalculatorDlg)
    enum { IDD = IDD_MYCALCULATOR_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyCalculatorDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CMyCalculatorDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCALCULATORDLG_H__7E4BACF9_6AB7_4D3D_B9E7_5550A7719BF8__INCLUDED_)

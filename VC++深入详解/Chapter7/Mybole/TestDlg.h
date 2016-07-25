#if !defined(AFX_TESTDLG_H__B68CE0B4_BF21_4020_8151_C73494334C0C__INCLUDED_)
#define AFX_TESTDLG_H__B68CE0B4_BF21_4020_8151_C73494334C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_edit3;
	CEdit	m_edit2;
	CEdit	m_edit1;
	int		m_num1;
	int		m_num2;
	int		m_num3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	afx_msg void OnBtnAdd();
	afx_msg void OnNumber1();
	afx_msg void OnButton1();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bIsCreated;
	CButton m_btn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__B68CE0B4_BF21_4020_8151_C73494334C0C__INCLUDED_)

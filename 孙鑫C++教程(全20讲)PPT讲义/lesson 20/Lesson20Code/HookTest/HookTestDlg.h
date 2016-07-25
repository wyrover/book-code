// HookTestDlg.h : header file
//

#if !defined(AFX_HOOKTESTDLG_H__569D5F33_3A9A_41C7_8FEF_64003BF13FEE__INCLUDED_)
#define AFX_HOOKTESTDLG_H__569D5F33_3A9A_41C7_8FEF_64003BF13FEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHookTestDlg dialog

class CHookTestDlg : public CDialog
{
// Construction
public:
	CHookTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHookTestDlg)
	enum { IDD = IDD_HOOKTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHookTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHookTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOKTESTDLG_H__569D5F33_3A9A_41C7_8FEF_64003BF13FEE__INCLUDED_)

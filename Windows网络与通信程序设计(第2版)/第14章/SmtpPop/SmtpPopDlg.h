// SmtpPopDlg.h : header file
//

#if !defined(AFX_SMTPPOPDLG_H__0795FE41_E431_4FC0_956B_3B7C061879FA__INCLUDED_)
#define AFX_SMTPPOPDLG_H__0795FE41_E431_4FC0_956B_3B7C061879FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSmtpPopDlg dialog

class CSmtpPopDlg : public CDialog
{
// Construction
public:
	CSmtpPopDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSmtpPopDlg)
	enum { IDD = IDD_SMTPPOP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmtpPopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSmtpPopDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSmtp();
	afx_msg void OnPop3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPPOPDLG_H__0795FE41_E431_4FC0_956B_3B7C061879FA__INCLUDED_)

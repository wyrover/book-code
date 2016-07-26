#if !defined(AFX_SMTPDLG_H__5A58404E_D8D2_42BE_918D_C5384F6B6980__INCLUDED_)
#define AFX_SMTPDLG_H__5A58404E_D8D2_42BE_918D_C5384F6B6980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmtpDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSmtpDlg dialog

class CSmtpDlg : public CDialog
{
// Construction
public:
	CSmtpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmtpDlg)
	enum { IDD = IDD_SMTP_DLG };
	CString	m_smtp;
	CString	m_username;
	CString	m_password;
	CString	m_soursemail;
	CString	m_desmail;
	CString	m_subject;
	CString	m_emailbody;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmtpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmtpDlg)
	afx_msg void OnSend();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPDLG_H__5A58404E_D8D2_42BE_918D_C5384F6B6980__INCLUDED_)

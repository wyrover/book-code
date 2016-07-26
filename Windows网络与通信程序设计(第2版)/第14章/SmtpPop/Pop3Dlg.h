#if !defined(AFX_POP3DLG_H__69F39318_E47B_4FC1_B1F9_0BD3D435F5FA__INCLUDED_)
#define AFX_POP3DLG_H__69F39318_E47B_4FC1_B1F9_0BD3D435F5FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pop3Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg dialog

class CPop3Dlg : public CDialog
{
// Construction
public:
	CPop3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPop3Dlg)
	enum { IDD = IDD_POP_DLG };
	CString	m_POP3;
	CString	m_password;
	CString	m_subject;
	CString	m_body;
	CString	m_username;
	UINT	m_num;
	CString	m_soursemail;
	CString	m_desmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPop3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPop3Dlg)
	afx_msg void OnCancal();
	afx_msg void OnReceive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POP3DLG_H__69F39318_E47B_4FC1_B1F9_0BD3D435F5FA__INCLUDED_)

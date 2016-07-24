#if !defined(AFX_ASKCONNECT_H__5A26633C_1ABC_49CA_A8A6_F9F0B092F3FC__INCLUDED_)
#define AFX_ASKCONNECT_H__5A26633C_1ABC_49CA_A8A6_F9F0B092F3FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// askconnect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// askconnect dialog

class askconnect : public CDialog
{
// Construction
public:
	askconnect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(askconnect)
	enum { IDD = IDD_DIALOG1 };
	CString	m_ip;
	CString	m_pro;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(askconnect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(askconnect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASKCONNECT_H__5A26633C_1ABC_49CA_A8A6_F9F0B092F3FC__INCLUDED_)

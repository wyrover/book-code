#if !defined(AFX_A_SETDIALOG_H__D4CCF482_FEC1_11D1_8334_00104B7B317F__INCLUDED_)
#define AFX_A_SETDIALOG_H__D4CCF482_FEC1_11D1_8334_00104B7B317F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// A_setDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// A_setDialog dialog

class A_setDialog : public CDialog
{
// Construction
public:
	A_setDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(A_setDialog)
	enum { IDD = IDD_DIALOG2 };
	BOOL	m_useVx;
	BOOL	m_useHp;
	BOOL	m_usePf;
	int		m_reinit_size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(A_setDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(A_setDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_A_SETDIALOG_H__D4CCF482_FEC1_11D1_8334_00104B7B317F__INCLUDED_)

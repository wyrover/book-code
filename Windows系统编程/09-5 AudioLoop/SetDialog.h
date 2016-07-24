#if !defined(AFX_SETDIALOG_H__D4CCF481_FEC1_11D1_8334_00104B7B317F__INCLUDED_)
#define AFX_SETDIALOG_H__D4CCF481_FEC1_11D1_8334_00104B7B317F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetDialog.h : header file
//

//??#include "A_setDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CSetDialog dialog

class CSetDialog : public CDialog
{
// Construction
public:
	CSetDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDialog)
	enum { IDD = IDD_DIALOG1 };
	CString	m_workrate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	public:
 //??        A_setDialog     m_ad_dlg;
//??
		//used to recieve the Advanced_Set Dialog parameters
//??		BOOL m_Hp;
//??		BOOL m_Vx;
//??		BOOL m_Pf;
//??		int m_InitSize;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDialog)
	afx_msg void OnAdvancedSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDIALOG_H__D4CCF481_FEC1_11D1_8334_00104B7B317F__INCLUDED_)

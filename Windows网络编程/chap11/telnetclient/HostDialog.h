#if !defined(AFX_HOSTDIALOG_H__FE95BC99_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)
#define AFX_HOSTDIALOG_H__FE95BC99_9860_11D3_8CD5_00C0F0405B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HostDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHostDialog dialog

class CHostDialog : public CDialog
{
// Construction
public:
	CHostDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHostDialog)
	enum { IDD = IDD_HOST };
	CString	m_HostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHostDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTDIALOG_H__FE95BC99_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)

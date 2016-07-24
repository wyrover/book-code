#if !defined(AFX_ADDDIALOG_H__0F62799B_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_ADDDIALOG_H__0F62799B_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddDialog dialog

class CAddDialog : public CDialog
{
// Construction
public:
	CAddDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddDialog)
	enum { IDD = IDD_ADDRESS };
	CString	m_strAddr;
	UINT	m_nPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDDIALOG_H__0F62799B_5A90_11D5_B587_00E04C39B036__INCLUDED_)

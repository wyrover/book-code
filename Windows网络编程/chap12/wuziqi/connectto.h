#if !defined(AFX_CONNECTTO_H__74FDCBF6_42B7_45DC_A4E1_1A6F1CADFB13__INCLUDED_)
#define AFX_CONNECTTO_H__74FDCBF6_42B7_45DC_A4E1_1A6F1CADFB13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// connectto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// connectto dialog

class connectto : public CDialog
{
// Construction
public:
	connectto(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(connectto)
	enum { IDD = IDD_DIALOG3 };
	CString	m_where;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(connectto)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(connectto)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTTO_H__74FDCBF6_42B7_45DC_A4E1_1A6F1CADFB13__INCLUDED_)

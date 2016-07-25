#if !defined(AFX_NEWBUTTON_H__52ABE599_5EBA_40F5_BE20_BCB2B8F67FA9__INCLUDED_)
#define AFX_NEWBUTTON_H__52ABE599_5EBA_40F5_BE20_BCB2B8F67FA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewButton window

class CNewButton : public CButton
{
// Construction
public:
	CNewButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	CNewButton* m_pBtn;
	virtual ~CNewButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNewButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWBUTTON_H__52ABE599_5EBA_40F5_BE20_BCB2B8F67FA9__INCLUDED_)

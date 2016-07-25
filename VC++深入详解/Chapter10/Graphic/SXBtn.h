#if !defined(AFX_WEIXINBUTTON_H__6A1ADB4D_B745_4F9B_B7BD_D0B151046A0A__INCLUDED_)
#define AFX_WEIXINBUTTON_H__6A1ADB4D_B745_4F9B_B7BD_D0B151046A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SXBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSXBtn window

class CSXBtn : public CButton
{
// Construction
public:
	CSXBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSXBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CSXBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSXBtn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEIXINBUTTON_H__6A1ADB4D_B745_4F9B_B7BD_D0B151046A0A__INCLUDED_)

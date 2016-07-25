#if !defined(AFX_DYNBAR_H__74BF169B_979F_4640_9DF5_3A33010B2D26__INCLUDED_)
#define AFX_DYNBAR_H__74BF169B_979F_4640_9DF5_3A33010B2D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDynBar window
class CDynBar : public CStatic
{
// Construction
public:
	CDynBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Stop();
	void Start();
	CBitmap m_bmp;
	virtual ~CDynBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDynBar)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNBAR_H__74BF169B_979F_4640_9DF5_3A33010B2D26__INCLUDED_)

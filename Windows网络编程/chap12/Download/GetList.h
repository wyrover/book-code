///////////////////////////////////////
// GetList
// ---------------
// Code by : Seo-WooSeok.
// Last Updated : 1999.10.12
///////////////////////////////////////


#if !defined(AFX_GETLIST_H__67187250_7D99_11D3_90F9_00105AA6C48C__INCLUDED_)
#define AFX_GETLIST_H__67187250_7D99_11D3_90F9_00105AA6C48C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "OleListDropTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CGetList window

class CGetList : public CListCtrl
{
// Construction
public:
	CGetList();

// Attributes
public:
	COleListDropTarget m_OleTarget;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetList)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddUrl(CString strUrl);
	virtual ~CGetList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGetList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Attributes
protected:
	int m_nCount;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETLIST_H__67187250_7D99_11D3_90F9_00105AA6C48C__INCLUDED_)

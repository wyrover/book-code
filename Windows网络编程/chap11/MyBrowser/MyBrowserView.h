// MyBrowserView.h : interface of the CMyBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBROWSERVIEW_H__24295128_8A9F_45B0_82F2_752DDA76F9C5__INCLUDED_)
#define AFX_MYBROWSERVIEW_H__24295128_8A9F_45B0_82F2_752DDA76F9C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyBrowserDoc.h"


class CMyBrowserView : public CEditView
{
protected: // create from serialization only
	CMyBrowserView();
	DECLARE_DYNCREATE(CMyBrowserView)

// Attributes
public:
	CMyBrowserDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBrowserView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyBrowserView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyBrowserView.cpp
inline CMyBrowserDoc* CMyBrowserView::GetDocument()
   { return (CMyBrowserDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBROWSERVIEW_H__24295128_8A9F_45B0_82F2_752DDA76F9C5__INCLUDED_)

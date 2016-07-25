// ASCView.h : interface of the CASCView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASCVIEW_H__CC414C12_6375_4F96_A025_84A527EFC1F6__INCLUDED_)
#define AFX_ASCVIEW_H__CC414C12_6375_4F96_A025_84A527EFC1F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CASCView : public CView
{
protected: // create from serialization only
	CASCView();
	DECLARE_DYNCREATE(CASCView)

// Attributes
public:
	CASCDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CASCView)
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
	virtual ~CASCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CASCView)
	afx_msg void OnSearchlog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ASCView.cpp
inline CASCDoc* CASCView::GetDocument()
   { return (CASCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASCVIEW_H__CC414C12_6375_4F96_A025_84A527EFC1F6__INCLUDED_)

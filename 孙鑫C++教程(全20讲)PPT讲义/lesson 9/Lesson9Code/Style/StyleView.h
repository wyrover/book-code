// StyleView.h : interface of the CStyleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLEVIEW_H__A71799D4_EFFE_47E2_8073_32F4C78052C2__INCLUDED_)
#define AFX_STYLEVIEW_H__A71799D4_EFFE_47E2_8073_32F4C78052C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStyleView : public CView
{
protected: // create from serialization only
	CStyleView();
	DECLARE_DYNCREATE(CStyleView)

// Attributes
public:
	CStyleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleView)
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
	virtual ~CStyleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStyleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in StyleView.cpp
inline CStyleDoc* CStyleView::GetDocument()
   { return (CStyleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEVIEW_H__A71799D4_EFFE_47E2_8073_32F4C78052C2__INCLUDED_)

// DSplitView.h : interface of the CDSplitView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPLITVIEW_H__76E949CF_9CAD_11D1_907F_00A024782894__INCLUDED_)
#define AFX_DSPLITVIEW_H__76E949CF_9CAD_11D1_907F_00A024782894__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DSplitDoc.h"

class CDSplitView : public CView
{
protected: // create from serialization only
	CDSplitView();
	DECLARE_DYNCREATE(CDSplitView)

// Attributes
public:
	CDSplitDoc* GetDocument();

// Operations
public:
 	CString filename;
    int state1;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDSplitView)
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
	virtual ~CDSplitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDSplitView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnYuantu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DSplitView.cpp
inline CDSplitDoc* CDSplitView::GetDocument()
   { return (CDSplitDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPLITVIEW_H__76E949CF_9CAD_11D1_907F_00A024782894__INCLUDED_)

// CDCStep3View.h : interface of the CCDCStep3View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDCSTEP3VIEW_H__B1D51790_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)
#define AFX_CDCSTEP3VIEW_H__B1D51790_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//{{seojt
#define NUM     1000
#define TWOPI   (2*3.14159)
//}}seojt

class CCDCStep3View : public CView
{
protected: // create from serialization only
	CCDCStep3View();
	DECLARE_DYNCREATE(CCDCStep3View)

// Attributes
public:
    //{{seojt
    POINT pt[NUM];
    int cxClient,cyClient;
    //}}seojt
	CCDCStep3Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDCStep3View)
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
	virtual ~CCDCStep3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCDCStep3View)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CDCStep3View.cpp
inline CCDCStep3Doc* CCDCStep3View::GetDocument()
   { return (CCDCStep3Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDCSTEP3VIEW_H__B1D51790_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)

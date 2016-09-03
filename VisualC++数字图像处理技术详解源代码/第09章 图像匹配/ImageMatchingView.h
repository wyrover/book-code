// ImageMatchingView.h : interface of the CImageMatchingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEMATCHINGVIEW_H__DB8C33BC_CEC5_4B77_BE7D_6BD7C10AEF1F__INCLUDED_)
#define AFX_IMAGEMATCHINGVIEW_H__DB8C33BC_CEC5_4B77_BE7D_6BD7C10AEF1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageMatchingView : public CView
{
protected: // create from serialization only
	CImageMatchingView();
	DECLARE_DYNCREATE(CImageMatchingView)

// Attributes
public:
	CImageMatchingDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMatchingView)
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
	virtual ~CImageMatchingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageMatchingView)
	afx_msg void OnImagematchSsda();
	afx_msg void OnImagematchMoment();
	afx_msg void OnImagematchDis();
	afx_msg void OnImagematchGyh();
	afx_msg void OnImagematchLsc();
	afx_msg void OnImagematch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageMatchingView.cpp
inline CImageMatchingDoc* CImageMatchingView::GetDocument()
   { return (CImageMatchingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMATCHINGVIEW_H__DB8C33BC_CEC5_4B77_BE7D_6BD7C10AEF1F__INCLUDED_)

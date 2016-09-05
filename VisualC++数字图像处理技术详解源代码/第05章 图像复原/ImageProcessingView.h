// ImageProcessingView.h : interface of the CImageProcessingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessingView : public CScrollView
{
protected: // create from serialization only
	CImageProcessingView();
	DECLARE_DYNCREATE(CImageProcessingView)

// Attributes
public:
	BOOL DIBDWTStep(CDib* pDib, int nInv);
	CImageProcessingDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	double* m_pDbImage;
	int m_nSupp;
	int m_nDWTCurDepth;
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingView)
	afx_msg void OnDegenerationInverse();
	afx_msg void OnDegenerationMotion();
	afx_msg void OnRestoreInverse();
	afx_msg void OnRestoreMotion();
	afx_msg void OnRestoreWinner();
	afx_msg void OnRestoreMindbl();
	afx_msg void OnRestoreMaxpro();
	afx_msg void OnRestoreProject();
	afx_msg void OnRestoreBlind();
	afx_msg void OnRestoreGeo();
	afx_msg void OnRestoreMaxentr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageProcessingView.cpp
inline CImageProcessingDoc* CImageProcessingView::GetDocument()
   { return (CImageProcessingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGVIEW_H__613B030F_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)

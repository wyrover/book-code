// DImageProcessView.h : interface of the CDImageProcessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMAGEPROCESSVIEW_H__011E8826_49D3_49DB_8855_0599DECA58E9__INCLUDED_)
#define AFX_DIMAGEPROCESSVIEW_H__011E8826_49D3_49DB_8855_0599DECA58E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DImageProcessDoc.h"

class CDImageProcessView : public CView
{
protected: // create from serialization only
	CDImageProcessView();
	DECLARE_DYNCREATE(CDImageProcessView)

	// Attributes
public:
	CDImageProcessDoc* GetDocument();


	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDImageProcessView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CDImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
protected:
	double* m_pDbImage;
	int m_nDWTCurDepth;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDImageProcessView)
	afx_msg void OnTranspose();
	afx_msg void OnMirror();
	//afx_msg void OnHistogram();
	//afx_msg void OnGrayEqual();
	afx_msg void OnRotation();
	afx_msg void OnTranslation();
	afx_msg void OnZoom();
	//afx_msg void OnLiner();
	//afx_msg void OnStretch();
	//afx_msg void OnThreshold();
	//afx_msg void OnWindow();
	afx_msg void OnDct();
	afx_msg void OnWalhHar();
	afx_msg void OnLog();
	afx_msg void OnFourier();
	afx_msg void OnKL();
	//afx_msg void OnInverRestore();
	//afx_msg void OnWinnerRestore();
	//afx_msg void OnGrayEnhance();
	//afx_msg void OnHistEnhance();
	//afx_msg void OnSmoothEnhance();
	//afx_msg void OnMedianFilter();
	//afx_msg void OnLaplasSharp();
	//afx_msg void OnLowpassFiler();
	//afx_msg void OnHighpassFiler();
	//afx_msg void OnErosionCalculation();
	//afx_msg void OnDilationCalculation();
	//afx_msg void OnOpencloseCalculation();
	//afx_msg void OnThiningCalculation();
	//afx_msg void OnEdgeDetection();
	//afx_msg void OnHoughTrans();
	//afx_msg void OnOutline();
	//afx_msg void OnContourtrace();
	//afx_msg void OnThresholdDivide();
	//afx_msg void OnRegionGrow();
	//afx_msg void OnProjection();
	//afx_msg void OnSubtract();
	//afx_msg void OnTempletematch();
	//afx_msg void OnHuffmancoding();
	//afx_msg void OnShannoncoding();
	//afx_msg void OnRlecoding();
	//afx_msg void OnGiflzw();
	afx_msg void OnWl();
    afx_msg void OnMirror2();
	//}}AFX_MSG
	//afx_msg void OnImageStunt(UINT nID);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnSvd();
public:
	afx_msg void OnGeomRota();
};

#ifndef _DEBUG  // debug version in DImageProcessView.cpp
inline CDImageProcessDoc* CDImageProcessView::GetDocument()
{ return (CDImageProcessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIMAGEPROCESSVIEW_H__011E8826_49D3_49DB_8855_0599DECA58E9__INCLUDED_)

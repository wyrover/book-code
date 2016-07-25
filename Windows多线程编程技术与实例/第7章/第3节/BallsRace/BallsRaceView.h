// BallsRaceView.h : interface of the CBallsRaceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALLSRACEVIEW_H__C38E3E1A_C2E7_4130_9F33_CD255649EB35__INCLUDED_)
#define AFX_BALLSRACEVIEW_H__C38E3E1A_C2E7_4130_9F33_CD255649EB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBallsRaceView : public CView
{
protected: // create from serialization only
	CBallsRaceView();
	DECLARE_DYNCREATE(CBallsRaceView)

// Attributes
public:
	CBallsRaceDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBallsRaceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual ~CBallsRaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBallsRaceView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBall1wait();
	afx_msg void OnBall0wait();
	afx_msg void OnBall2wait();
	afx_msg void OnBall3wait();
	afx_msg void OnBall4wait();
	afx_msg void OnTerminateall();
	afx_msg void OnBall0continue();
	afx_msg void OnBall1continue();
	afx_msg void OnBall2continue();
	afx_msg void OnBall3continue();
	afx_msg void OnBall4continue();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	BOOL isTerminate;
	HGLRC m_hGLContext;
	int m_GLPixelIndex;
};

#ifndef _DEBUG  // debug version in BallsRaceView.cpp
inline CBallsRaceDoc* CBallsRaceView::GetDocument()
   { return (CBallsRaceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLSRACEVIEW_H__C38E3E1A_C2E7_4130_9F33_CD255649EB35__INCLUDED_)

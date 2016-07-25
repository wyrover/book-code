// PhilosophersView.h : interface of the CPhilosophersView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHILOSOPHERSVIEW_H__7CA3E597_19FF_4754_8A3E_9E8FD7646D8B__INCLUDED_)
#define AFX_PHILOSOPHERSVIEW_H__7CA3E597_19FF_4754_8A3E_9E8FD7646D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

#define DEGRESS_TO_RADIAN(x) (x) * 3.14 / 180
#define CHOP_COORDINATE(x, y)																  \
	chopStartX[0] = (x) + 3 * DESK_DIAMETER / 4;					                          \
	chopStartY[0] = (y) + DESK_DIAMETER / 2;												  \
	chopEndX[0] = (x) + DESK_DIAMETER;									                      \
    chopEndY[0] = (y) + DESK_DIAMETER / 2;								                      \
	chopStartX[1] = (int)((x) + DESK_DIAMETER / 2 + DESK_DIAMETER / 4 * sin(DEGRESS_TO_RADIAN(30))); \
	chopStartY[1] = (int)((y) + DESK_DIAMETER / 2 - DESK_DIAMETER / 4 * cos(DEGRESS_TO_RADIAN(30))); \
	chopEndX[1] = (int)((x) + DESK_DIAMETER / 2 + DESK_DIAMETER / 2 * sin(DEGRESS_TO_RADIAN(30)));   \
    chopEndY[1] = (int)((y) + DESK_DIAMETER / 2 - DESK_DIAMETER / 2 * cos(DEGRESS_TO_RADIAN(30)));   \
    chopStartX[2] = (int)((x) + DESK_DIAMETER / 2 - DESK_DIAMETER / 4 * sin(DEGRESS_TO_RADIAN(30))); \
    chopStartY[2] = (int)((y) + DESK_DIAMETER / 2 - DESK_DIAMETER / 4 * cos(DEGRESS_TO_RADIAN(30))); \
    chopEndX[2] = (int)((x) + DESK_DIAMETER / 2 - DESK_DIAMETER / 2 * sin(DEGRESS_TO_RADIAN(30)));   \
    chopEndY[2] = (int)((y) + DESK_DIAMETER / 2 - DESK_DIAMETER / 2 * cos(DEGRESS_TO_RADIAN(30)));   \
	chopStartX[3] = (x) + DESK_DIAMETER / 4;							                      \
	chopStartY[3] = (y) + DESK_DIAMETER / 2;							                      \
    chopEndX[3] = (x);													                      \
	chopEndY[3] = (y) + DESK_DIAMETER / 2; 								                      \
	chopStartX[4] = (int)((x) + DESK_DIAMETER / 2 - DESK_DIAMETER / 4 * sin(DEGRESS_TO_RADIAN(30))); \
    chopStartY[4] = (int)((y) + DESK_DIAMETER / 2 + DESK_DIAMETER / 4 * cos(DEGRESS_TO_RADIAN(30))); \
    chopEndX[4] = (int)((x) + DESK_DIAMETER / 2 - DESK_DIAMETER / 2 * sin(DEGRESS_TO_RADIAN(30)));   \
    chopEndY[4] = (int)((y) + DESK_DIAMETER / 2 + DESK_DIAMETER / 2 * cos(DEGRESS_TO_RADIAN(30)));   \
	chopStartX[5] = (int)((x) + DESK_DIAMETER / 2 + DESK_DIAMETER / 4 * sin(DEGRESS_TO_RADIAN(30))); \
    chopStartY[5] = (int)((y) + DESK_DIAMETER / 2 + DESK_DIAMETER / 4 * cos(DEGRESS_TO_RADIAN(30))); \
    chopEndX[5] = (int)((x) + DESK_DIAMETER / 2 + DESK_DIAMETER / 2 * sin(DEGRESS_TO_RADIAN(30)));   \
    chopEndY[5] = (int)((y) + DESK_DIAMETER / 2 + DESK_DIAMETER / 2 * cos(DEGRESS_TO_RADIAN(30)));

#define PHIL_COORDINATE(x, y)											       \
	philX[0] = (int)((x) + DESK_DIAMETER /2 +										   \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);												   \
    philY[0] = (int)((y) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
	philX[1] = (int)((x) + DESK_DIAMETER /2 +									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(90)) - \
			PHIL_DIAMETER / 2);											       \
    philY[1] = (int)((y) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(90)) - \
			PHIL_DIAMETER / 2);											       \
	philX[2] = (int)((x) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
    philY[2] = (int)((y) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
	philX[3] = (int)((x) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
    philY[3] = (int)((y) + DESK_DIAMETER /2 +									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
	philX[4] = (int)((x) + DESK_DIAMETER /2 -									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(90)) - \
			PHIL_DIAMETER / 2);											       \
    philY[4] = (int)((y) + DESK_DIAMETER /2 +									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(90)) - \
			PHIL_DIAMETER / 2);											       \
	philX[5] = (int)((x) + DESK_DIAMETER /2 +									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * cos(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);											       \
    philY[5] = (int)((y) + DESK_DIAMETER /2 +									       \
			(DESK_DIAMETER / 2 + PHIL_DIAMETER) * sin(DEGRESS_TO_RADIAN(30)) - \
			PHIL_DIAMETER / 2);

typedef struct
{
	HWND m_hWnd;
	int chopXY[8];
	int philX, philY;
	int Id;
}PARAM;
	
const int DESK_DIAMETER = 200;
const int PHIL_DIAMETER = 30;
const int CHOP_LENGHT = 50;
const int PHIL_NUM = 6;
const int TIME = 10000;

class CPhilosophersView : public CView
{
protected: // create from serialization only
	CPhilosophersView();
	DECLARE_DYNCREATE(CPhilosophersView)

// Attributes
public:
	CPhilosophersDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhilosophersView)
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
	virtual ~CPhilosophersView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPhilosophersView)
	afx_msg void OnPlayOnce();
	afx_msg void OnPlayMany();
	afx_msg void OnUpdatePlayMany(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWinThread *m_pThread[6];
};

#ifndef _DEBUG  // debug version in PhilosophersView.cpp
inline CPhilosophersDoc* CPhilosophersView::GetDocument()
   { return (CPhilosophersDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHILOSOPHERSVIEW_H__7CA3E597_19FF_4754_8A3E_9E8FD7646D8B__INCLUDED_)

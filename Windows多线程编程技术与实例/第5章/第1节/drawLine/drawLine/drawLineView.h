// drawLineView.h : interface of the CDrawLineView class
//
/////////////////////////////////////////////////////////////////////////////
#include <afxmt.h>

#if !defined(AFX_DRAWLINEVIEW_H__4E81C72C_AC7A_46F7_AEC0_D20264EFB297__INCLUDED_)
#define AFX_DRAWLINEVIEW_H__4E81C72C_AC7A_46F7_AEC0_D20264EFB297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define THREAD_NUM 3//线程个数

typedef struct{
	int m_nID;//标识是第几个启动的线程
	HWND m_hWnd;//为了画图，需传入视图类CDrawLineView的窗口句柄
}PARAM;

UINT DrawLine(LPVOID);//线程入口函数

class CDrawLineView : public CView
{
protected: // create from serialization only
	CDrawLineView();
	DECLARE_DYNCREATE(CDrawLineView)

// Attributes
public:
	CDrawLineDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawLineView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawLineView)
	afx_msg void OnDrawLineThread();
	afx_msg void OnUpdateDrawLineThread(CCmdUI* pCmdUI);
	afx_msg void OnDrawLineTimer();
	afx_msg void OnUpdateDrawLineTimer(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CWinThread *m_pThread[THREAD_NUM];//创建线程后，产生的线程指针
public:
};

#ifndef _DEBUG  // debug version in drawLineView.cpp
inline CDrawLineDoc* CDrawLineView::GetDocument()
   { return (CDrawLineDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWLINEVIEW_H__4E81C72C_AC7A_46F7_AEC0_D20264EFB297__INCLUDED_)

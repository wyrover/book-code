// wuziqiView.h : interface of the CWuziqiView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WUZIQIVIEW_H__1C7E40BF_C7BF_47AC_B34E_94F3DF62A9F6__INCLUDED_)
#define AFX_WUZIQIVIEW_H__1C7E40BF_C7BF_47AC_B34E_94F3DF62A9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WM_CONNECT WM_USER+5
#include "chess.h"
#include "Overall.h"
#include "resultwin.h"
#include "askconnect.h"
class CWuziqiView : public CView
{
protected: // create from serialization only
	CWuziqiView();
	DECLARE_DYNCREATE(CWuziqiView)

// Attributes
public:
	CWuziqiDoc* GetDocument();
	chess white;
	chess black;
	Overall all;
	int who,whotemp,whohasjinshou,connect,youcansendmessage,whoiam,startserver;
	
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWuziqiView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWuziqiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWuziqiView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBackBlack();
	afx_msg void OnUpdateBackBlack(CCmdUI* pCmdUI);
	afx_msg void OnBackWhite();
	afx_msg void OnUpdateBackWhite(CCmdUI* pCmdUI);
	afx_msg void OnRuleChange();
	afx_msg void OnUpdateRuleChange(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRule3344();
	afx_msg void OnRuleClean();
	afx_msg void OnConnect();
	afx_msg void OnStartserver();
	afx_msg void OnUpdateStartserver(CCmdUI* pCmdUI);
	afx_msg void OnDisconnect();
	afx_msg void OnUpdateDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRuleRestart(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnConnect(WPARAM wParam,LPARAM lParam);
	public:
	afx_msg void OnRuleRestart();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in wuziqiView.cpp
inline CWuziqiDoc* CWuziqiView::GetDocument()
   { return (CWuziqiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WUZIQIVIEW_H__1C7E40BF_C7BF_47AC_B34E_94F3DF62A9F6__INCLUDED_)

// MultiCastChatView.h : interface of the CMultiCastChatView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICASTCHATVIEW_H__0F62798D_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_MULTICASTCHATVIEW_H__0F62798D_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "multicast.h"

class CMultiCastChatCntrItem;

class CMultiCastChatView : public CRichEditView
{
protected: // create from serialization only
	CMultiCastChatView();
	DECLARE_DYNCREATE(CMultiCastChatView)

// Attributes
public:
	CMultiCastChatDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiCastChatView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void TextOut(LPCTSTR lpszMessage, COLORREF clr);
	virtual ~CMultiCastChatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	CHARFORMAT cfm;
	//{{AFX_MSG(CMultiCastChatView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg LONG OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MultiCastChatView.cpp
inline CMultiCastChatDoc* CMultiCastChatView::GetDocument()
   { return (CMultiCastChatDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTICASTCHATVIEW_H__0F62798D_5A90_11D5_B587_00E04C39B036__INCLUDED_)

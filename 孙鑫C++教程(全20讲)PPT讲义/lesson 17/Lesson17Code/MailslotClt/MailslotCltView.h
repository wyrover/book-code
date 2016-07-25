// MailslotCltView.h : interface of the CMailslotCltView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILSLOTCLTVIEW_H__520CD3B2_DD52_427B_9E10_400EA33C384D__INCLUDED_)
#define AFX_MAILSLOTCLTVIEW_H__520CD3B2_DD52_427B_9E10_400EA33C384D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMailslotCltView : public CView
{
protected: // create from serialization only
	CMailslotCltView();
	DECLARE_DYNCREATE(CMailslotCltView)

// Attributes
public:
	CMailslotCltDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailslotCltView)
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
	virtual ~CMailslotCltView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMailslotCltView)
	afx_msg void OnMailslotSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MailslotCltView.cpp
inline CMailslotCltDoc* CMailslotCltView::GetDocument()
   { return (CMailslotCltDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILSLOTCLTVIEW_H__520CD3B2_DD52_427B_9E10_400EA33C384D__INCLUDED_)

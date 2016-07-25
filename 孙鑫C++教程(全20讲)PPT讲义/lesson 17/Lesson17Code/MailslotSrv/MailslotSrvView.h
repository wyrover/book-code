// MailslotSrvView.h : interface of the CMailslotSrvView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILSLOTSRVVIEW_H__31C1CC9C_4511_4B26_BD51_9DA72AD69F27__INCLUDED_)
#define AFX_MAILSLOTSRVVIEW_H__31C1CC9C_4511_4B26_BD51_9DA72AD69F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMailslotSrvView : public CView
{
protected: // create from serialization only
	CMailslotSrvView();
	DECLARE_DYNCREATE(CMailslotSrvView)

// Attributes
public:
	CMailslotSrvDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailslotSrvView)
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
	virtual ~CMailslotSrvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMailslotSrvView)
	afx_msg void OnMailslotRecv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MailslotSrvView.cpp
inline CMailslotSrvDoc* CMailslotSrvView::GetDocument()
   { return (CMailslotSrvDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILSLOTSRVVIEW_H__31C1CC9C_4511_4B26_BD51_9DA72AD69F27__INCLUDED_)

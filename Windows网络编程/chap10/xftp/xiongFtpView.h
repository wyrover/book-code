// xiongFtpView.h : interface of the CXiongFtpView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XIONGFTPVIEW_H__EA4C86EF_F083_4105_90FC_048C401A0DC6__INCLUDED_)
#define AFX_XIONGFTPVIEW_H__EA4C86EF_F083_4105_90FC_048C401A0DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXiongFtpDoc;
class CXiongFtpView : public CListView
{
protected: // create from serialization only
	CXiongFtpView();
	DECLARE_DYNCREATE(CXiongFtpView)

// Attributes
public:
	CXiongFtpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXiongFtpView)
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
	virtual ~CXiongFtpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXiongFtpView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in xiongFtpView.cpp
inline CXiongFtpDoc* CXiongFtpView::GetDocument()
   { return (CXiongFtpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XIONGFTPVIEW_H__EA4C86EF_F083_4105_90FC_048C401A0DC6__INCLUDED_)

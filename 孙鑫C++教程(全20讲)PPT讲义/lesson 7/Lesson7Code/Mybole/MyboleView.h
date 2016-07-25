// MyboleView.h : interface of the CMyboleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBOLEVIEW_H__5E6A22F0_1CCB_43D3_A91D_B0D13CD2FADD__INCLUDED_)
#define AFX_MYBOLEVIEW_H__5E6A22F0_1CCB_43D3_A91D_B0D13CD2FADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyboleView : public CView
{
protected: // create from serialization only
	CMyboleView();
	DECLARE_DYNCREATE(CMyboleView)

// Attributes
public:
	CMyboleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyboleView)
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
	virtual ~CMyboleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyboleView)
	afx_msg void OnDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyboleView.cpp
inline CMyboleDoc* CMyboleView::GetDocument()
   { return (CMyboleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBOLEVIEW_H__5E6A22F0_1CCB_43D3_A91D_B0D13CD2FADD__INCLUDED_)

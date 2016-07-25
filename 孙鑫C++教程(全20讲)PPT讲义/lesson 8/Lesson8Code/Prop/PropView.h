// PropView.h : interface of the CPropView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPVIEW_H__05470299_E729_4310_9325_70EC37AFFBAD__INCLUDED_)
#define AFX_PROPVIEW_H__05470299_E729_4310_9325_70EC37AFFBAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPropView : public CView
{
protected: // create from serialization only
	CPropView();
	DECLARE_DYNCREATE(CPropView)

// Attributes
public:
	CPropDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPropView)
	afx_msg void OnPropertysheet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iOccupation;
	CString m_strWorkAddr;
	BOOL m_bLike[4];
	CString m_strSalary;
};

#ifndef _DEBUG  // debug version in PropView.cpp
inline CPropDoc* CPropView::GetDocument()
   { return (CPropDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPVIEW_H__05470299_E729_4310_9325_70EC37AFFBAD__INCLUDED_)

// DBView.h : interface of the CDBView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBVIEW_H__84FCEE84_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_DBVIEW_H__84FCEE84_5F09_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CDBView : public CView
{
protected: // create from serialization only
    CDBView();
    DECLARE_DYNCREATE(CDBView)

// Attributes
public:
    CDBDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDBView)
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
    virtual ~CDBView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CDBView)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DBView.cpp
inline CDBDoc* CDBView::GetDocument()
{
    return (CDBDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBVIEW_H__84FCEE84_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)

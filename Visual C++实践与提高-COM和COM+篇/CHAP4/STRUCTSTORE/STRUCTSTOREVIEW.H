// StructStoreView.h : interface of the CStructStoreView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRUCTSTOREVIEW_H__BD1E880B_767E_11D4_8BD4_680DDD000000__INCLUDED_)
#define AFX_STRUCTSTOREVIEW_H__BD1E880B_767E_11D4_8BD4_680DDD000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CStructStoreView : public CView
{
protected: // create from serialization only
    CStructStoreView();
    DECLARE_DYNCREATE(CStructStoreView)

// Attributes
public:
    CStructStoreDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStructStoreView)
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
    virtual ~CStructStoreView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CStructStoreView)
    afx_msg void OnStorageread();
    afx_msg void OnStoragewrite();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in StructStoreView.cpp
inline CStructStoreDoc* CStructStoreView::GetDocument()
{
    return (CStructStoreDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTSTOREVIEW_H__BD1E880B_767E_11D4_8BD4_680DDD000000__INCLUDED_)

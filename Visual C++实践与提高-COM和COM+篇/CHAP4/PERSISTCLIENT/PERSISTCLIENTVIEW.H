// PersistClientView.h : interface of the CPersistClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTCLIENTVIEW_H__5D49D00D_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)
#define AFX_PERSISTCLIENTVIEW_H__5D49D00D_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CPersistClientView : public CView
{
protected: // create from serialization only
    CPersistClientView();
    DECLARE_DYNCREATE(CPersistClientView)

// Attributes
public:
    CPersistClientDoc* GetDocument();
    CString* pInfo;
    CString info;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPersistClientView)
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
    virtual ~CPersistClientView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CPersistClientView)
    afx_msg void OnStorageread();
    afx_msg void OnStoragewrite();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PersistClientView.cpp
inline CPersistClientDoc* CPersistClientView::GetDocument()
{
    return (CPersistClientDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTCLIENTVIEW_H__5D49D00D_76FC_11D4_8BDC_0000E83EBA18__INCLUDED_)

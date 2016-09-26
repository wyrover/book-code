// ScribVw.h : interface of the CScribbleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIBVW_H__A9156DFF_C61A_418A_B26D_771CC9C49607__INCLUDED_)
#define AFX_SCRIBVW_H__A9156DFF_C61A_418A_B26D_771CC9C49607__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class CScribbleView : public CScrollView
{
protected: // create from serialization only
    CScribbleView();
    DECLARE_DYNCREATE(CScribbleView)

// Attributes
public:
    CScribbleDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScribbleView)
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
    virtual ~CScribbleView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CScribbleView)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    afx_msg void OnCancelEditSrvr();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ScribVw.cpp
inline CScribbleDoc* CScribbleView::GetDocument()
{
    return (CScribbleDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIBVW_H__A9156DFF_C61A_418A_B26D_771CC9C49607__INCLUDED_)

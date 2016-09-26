// ContainerActiveXView.h : interface of the CContainerActiveXView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTAINERACTIVEXVIEW_H__0E34032C_930B_11D4_896A_9349D3D7A679__INCLUDED_)
#define AFX_CONTAINERACTIVEXVIEW_H__0E34032C_930B_11D4_896A_9349D3D7A679__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CContainerActiveXCntrItem;

class CContainerActiveXView : public CView
{
protected: // create from serialization only
    CContainerActiveXView();
    DECLARE_DYNCREATE(CContainerActiveXView)

// Attributes
public:
    CContainerActiveXDoc* GetDocument();
    // m_pSelection holds the selection to the current CContainerActiveXCntrItem.
    // For many applications, such a member variable isn't adequate to
    //  represent a selection, such as a multiple selection or a selection
    //  of objects that are not CContainerActiveXCntrItem objects.  This selection
    //  mechanism is provided just to help you get started.

    // TODO: replace this selection mechanism with one appropriate to your app.
    CContainerActiveXCntrItem* m_pSelection;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContainerActiveXView)
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CContainerActiveXView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CContainerActiveXView)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    afx_msg void OnDestroy();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnInsertObject();
    afx_msg void OnCancelEditCntr();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ContainerActiveXView.cpp
inline CContainerActiveXDoc* CContainerActiveXView::GetDocument()
{
    return (CContainerActiveXDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINERACTIVEXVIEW_H__0E34032C_930B_11D4_896A_9349D3D7A679__INCLUDED_)

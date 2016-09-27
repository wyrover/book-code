// OleClipView.h : interface of the COleClipView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLECLIPVIEW_H__C4CDBED0_616B_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_OLECLIPVIEW_H__C4CDBED0_616B_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000


class COleClipView : public CView
{
    CRectTracker m_tracker;
    CRect m_rectTracker; // logical coordinates
protected: // create from serialization only
    COleClipView();
    DECLARE_DYNCREATE(COleClipView)

// Attributes
public:
    COleClipDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COleClipView)
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

// Implementation
public:
    COleDataSource* SaveDib();
    BOOL DoPasteDib(COleDataObject* pDataObject);
    virtual ~COleClipView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(COleClipView)
    afx_msg void OnEditCut();
    afx_msg void OnEditCopy();
    afx_msg void OnEditPaste();
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg LONG OnViewPaletteChanged(UINT wParam, LONG lParam);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnEditPastefrom();
    afx_msg void OnEditCopyto();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OleClipView.cpp
inline COleClipDoc* COleClipView::GetDocument()
{
    return (COleClipDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLECLIPVIEW_H__C4CDBED0_616B_11D4_A23D_02608CACEDBB__INCLUDED_)

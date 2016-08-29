// PrintExplorerView.h : interface of the CPrintExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CPrintExplorerView : public CView
{
protected: // create from serialization only
        CPrintExplorerView();
        DECLARE_DYNCREATE(CPrintExplorerView)

// Attributes
public:
        CPrintExplorerDoc* GetDocument();

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPrintExplorerView)
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
        virtual ~CPrintExplorerView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CPrintExplorerView)
        afx_msg void OnPrintDirect();
        afx_msg void OnPrintsetupDirect();
        afx_msg void OnPrintdlg();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PrintExplorerView.cpp
inline CPrintExplorerDoc* CPrintExplorerView::GetDocument()
   { return (CPrintExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

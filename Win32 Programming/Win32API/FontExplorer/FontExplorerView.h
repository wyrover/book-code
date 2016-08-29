// FontExplorerView.h : interface of the CFontExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CFontExplorerView : public CView
{
protected: // create from serialization only
        CFontExplorerView();
        DECLARE_DYNCREATE(CFontExplorerView)

// Attributes
public:
        CFontExplorerDoc* GetDocument();

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFontExplorerView)
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
        virtual ~CFontExplorerView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CFontExplorerView)
                // NOTE - the ClassWizard will add and remove member functions here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FontExplorerView.cpp
inline CFontExplorerDoc* CFontExplorerView::GetDocument()
   { return (CFontExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

// CtlColorExplorerView.h : interface of the CCtlColorExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CCtlColorExplorerView : public CView
{
protected: // create from serialization only
        CCtlColorExplorerView();
        DECLARE_DYNCREATE(CCtlColorExplorerView)

// Attributes
public:
        CCtlColorExplorerDoc* GetDocument();

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCtlColorExplorerView)
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
        virtual ~CCtlColorExplorerView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CCtlColorExplorerView)
                // NOTE - the ClassWizard will add and remove member functions here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CtlColorExplorerView.cpp
inline CCtlColorExplorerDoc* CCtlColorExplorerView::GetDocument()
   { return (CCtlColorExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

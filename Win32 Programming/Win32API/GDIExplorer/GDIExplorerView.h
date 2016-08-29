// GDI ExplorerView.h : interface of the CGDIExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CGDIExplorerView : public CView
{
protected: // create from serialization only
        CGDIExplorerView();
        DECLARE_DYNCREATE(CGDIExplorerView)

// Attributes
public:
        CGDIExplorerDoc* GetDocument();
        int style;
        XFORM printscale;
        int description;
        int icon;
        RandomData rd;
        PolyPolyData pd;
        CRect bb;
        CRect * currentbb;
        BOOL viewbb;

// Operations
public:
        BOOL GetFileName(CString & fileName, UINT nIDSTitle, DWORD lFlags,
                        BOOL bOpenFileDialog, BOOL enhanced);
        void WriteMetaFile(BOOL enhanced);
        void SetParameters();
        void SetPrinterScale(float n) 
              { printscale.eM11 = n;          printscale.eM12 = (float)0.0;
                printscale.eM21 = (float)0.0; printscale.eM22 = n;}

        void SetPrinterOffset(float x, float y) 
                { printscale.eDx  = x; printscale.eDy  = y;}

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CGDIExplorerView)
        public:
        virtual void OnDraw(CDC* pDC);  // overridden to draw this view
        virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        protected:
        virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
        virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
        virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CGDIExplorerView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
        //{{AFX_MSG(CGDIExplorerView)
        afx_msg void OnSelect();
        afx_msg void OnWriteMetaFile();
        afx_msg void OnWriteEnhancedMetaFile();
        afx_msg void OnSetcontrolpoints();
        afx_msg void OnSetpolyfillAlternate();
        afx_msg void OnSetpolyfillWinding();
        afx_msg void OnUpdateSetpolyfillAlternate(CCmdUI* pCmdUI);
        afx_msg void OnUpdateSetpolyfillWinding(CCmdUI* pCmdUI);
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnSetpolyfillNone();
        afx_msg void OnEditCopy();
        afx_msg void OnViewBb();
        afx_msg void OnUpdateViewBb(CCmdUI* pCmdUI);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

        BOOL DoContextMenu(CPoint point);
};

#ifndef _DEBUG  // debug version in GDI ExplorerView.cpp
inline CGDIExplorerDoc* CGDIExplorerView::GetDocument()
   { return (CGDIExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

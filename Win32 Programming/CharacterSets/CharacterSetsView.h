// CharacterSetsView.h : interface of the CCharacterSetsView class
//
/////////////////////////////////////////////////////////////////////////////

class CCharacterSetsView : public CScrollView
{
protected: // create from serialization only
        CCharacterSetsView();
        DECLARE_DYNCREATE(CCharacterSetsView)

// Attributes
public:
        CCharacterSetsDoc* GetDocument();

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCharacterSetsView)
        public:
        virtual void OnDraw(CDC* pDC);  // overridden to draw this view
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        virtual void OnInitialUpdate();
        protected:
        virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
        virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
        virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CCharacterSetsView();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:
        CFont font;

// Generated message map functions
protected:
        void setScaling();
        //{{AFX_MSG(CCharacterSetsView)
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnEditCopy();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CharacterSetsView.cpp
inline CCharacterSetsDoc* CCharacterSetsView::GetDocument()
   { return (CCharacterSetsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

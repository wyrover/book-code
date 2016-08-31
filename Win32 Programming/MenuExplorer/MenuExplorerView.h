// MenuExplorerView.h : interface of the CMenuExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CMenuExplorerView : public CView
{
protected: // create from serialization only
	CMenuExplorerView();
	DECLARE_DYNCREATE(CMenuExplorerView)

// Attributes
public:
	CMenuExplorerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuExplorerView)
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
	virtual ~CMenuExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMenuExplorerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MenuExplorerView.cpp
inline CMenuExplorerDoc* CMenuExplorerView::GetDocument()
   { return (CMenuExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

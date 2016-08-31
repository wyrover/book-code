// ControlExplorerView.h : interface of the CControlExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CControlExplorerView : public CView
{
protected: // create from serialization only
	CControlExplorerView();
	DECLARE_DYNCREATE(CControlExplorerView)

// Attributes
public:
	CControlExplorerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlExplorerView)
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
	virtual ~CControlExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CControlExplorerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ControlExplorerView.cpp
inline CControlExplorerDoc* CControlExplorerView::GetDocument()
   { return (CControlExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

// Window ExplorerView.h : interface of the CWndExpView class
//
/////////////////////////////////////////////////////////////////////////////

class CWndExpView : public CView
{
protected: // create from serialization only
	CWndExpView();
	DECLARE_DYNCREATE(CWndExpView)

// Attributes
public:
	CWndExpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndExpView)
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
	virtual ~CWndExpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWndExpView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WndExpView.cpp
inline CWndExpDoc* CWndExpView::GetDocument()
   { return (CWndExpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

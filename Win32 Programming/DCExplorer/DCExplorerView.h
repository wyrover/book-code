// DC ExplorerView.h : interface of the CDCExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

class CDCExplorerView : public CView
{
protected: // create from serialization only
	CDCExplorerView();
	DECLARE_DYNCREATE(CDCExplorerView)

// Attributes
public:
	CDCExplorerDoc* GetDocument();
	CDCValues dcv;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCExplorerView)
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
	virtual ~CDCExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	int page;  // property page index for dialog
	int sampleid;

	int dump_seq;  // current position in dump sequence

	void drawPolygon(CDC * dc);
	void drawLines(CDC * dc);
	void drawText(CDC * dc);
	void drawFilledFig(CDC * dc);
	void doContextMenu(CPoint point);
	BOOL dropit(CPoint point);
	CFont * getScaledFont(CDC * dc, CString & s, CRect & bb);

	//{{AFX_MSG(CDCExplorerView)
	afx_msg void OnDcExp();
	afx_msg void OnPolygon();
	afx_msg void OnLines();
	afx_msg void OnFilledfig();
	afx_msg void OnText();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DCExplorerView.cpp
inline CDCExplorerDoc* CDCExplorerView::GetDocument()
   { return (CDCExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

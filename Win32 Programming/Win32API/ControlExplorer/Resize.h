// Resize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResizableTree window

class CResizableTree : public CTreeCtrl
{
// Construction
public:
	CResizableTree();

// Attributes
public:

// Operations
public:
	void setResize(BOOL mode);
	void setLimits(CRect * r);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableTree();

	// Generated message map functions
protected:
	CScrollBar sizer;
	CRect * limits;
	CStatic * coords;
	CPoint bias;

	void createCoordWindow(CPoint wr);
	void showCoord(CPoint p);


	//{{AFX_MSG(CResizableTree)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg LRESULT OnExitSizeMove(WPARAM, LPARAM);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

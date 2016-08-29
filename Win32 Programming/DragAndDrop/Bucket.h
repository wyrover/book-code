// Bucket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBucket window

class CBucket : public CStatic
{
// Construction
public:
	CBucket();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBucket)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBucket();
	static int QueryDrop;
	static int DoDrop;

	// Generated message map functions
protected:
	static HCURSOR fulldipper;
	static HCURSOR emptydipper;
	HCURSOR oldcursor;

	HWND findTarget(CPoint point);

	//{{AFX_MSG(CBucket)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT dropIt(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

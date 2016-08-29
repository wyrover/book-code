// Glass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGlass window

class CGlass : public CStatic
{
// Construction
public:
	CGlass();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGlass();

	// Generated message map functions
protected:
	int filled;

	//{{AFX_MSG(CGlass)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT dropIt(WPARAM, LPARAM);
	afx_msg LRESULT queryDrop(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

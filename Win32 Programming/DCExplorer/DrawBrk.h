// DrawBrk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawBricks window

class CDrawBricks : public CStatic
{
// Construction
public:
	CDrawBricks();

// Attributes
public:
	static UWM_QUERY_BKMODE;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawBricks)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawBricks();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawBricks)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

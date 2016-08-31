// Drawing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawing window

class CDrawing : public CStatic
{
// Construction
public:
	CDrawing();

// Attributes
public:
	static int UWM_QUERY_XOFFSET;
	static int UWM_QUERY_YOFFSET;
	static int UWM_QUERY_SIZE;
	static int UWM_QUERY_PENSIZE;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawing)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawing();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawing)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

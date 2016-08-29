// HatchSmp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHatchBrushSample window

class CHatchBrushSample : public CStatic
{
// Construction
public:
	CHatchBrushSample();
	LOGBRUSH br;
	POINT org;
	int bkmode;
	COLORREF bkcolor;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHatchBrushSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHatchBrushSample();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHatchBrushSample)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

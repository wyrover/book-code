// PenSamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPenSample window

class CPenSample : public CStatic
{
// Construction
public:
	CPenSample();
	CPen * pen;
	int  bkmode;
	COLORREF bkcolor;
	float miterlimit;
	int join;
	int angle;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPenSample();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPenSample)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

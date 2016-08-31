// Hatch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHatchSample window

class CHatchSample : public CStatic
{
// Construction
public:
	CHatchSample();
	COLORREF bkg;
	int BkMode;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHatchSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHatchSample();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHatchSample)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

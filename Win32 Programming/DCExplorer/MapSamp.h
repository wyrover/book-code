// MapSamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMappingSample window

class CMappingSample : public CStatic
{
// Construction
public:
	CMappingSample();
	int ticks;  // tick interval
	CPoint vorg;
	CPoint vext;
	CPoint worg;
	CPoint wext;
	int mapmode;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMappingSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMappingSample();

	// Generated message map functions
protected:
	void badValue(CDC &dc);
	int scrollValue(int cpos, UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//{{AFX_MSG(CMappingSample)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

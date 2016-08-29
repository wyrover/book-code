// LineJoin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineJoin window

class CLineJoin : public CGraphicCombo
{
// Construction
public:
	CLineJoin();

// Attributes
public:

// Operations
public:

// Overrides
	virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineJoin)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineJoin();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineJoin)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

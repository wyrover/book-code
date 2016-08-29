// EndCaps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEndCaps window

class CEndCaps : public CGraphicCombo
{
// Construction
public:
	CEndCaps();

// Attributes
public:

// Operations
public:

// Overrides
	virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEndCaps)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEndCaps();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEndCaps)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

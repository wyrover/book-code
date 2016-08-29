// PenStyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPenStyle window

class CPenStyle : public CGraphicCombo
{
// Construction
public:
	CPenStyle();

// Attributes
public:

// Operations
public:

// Overrides
	virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPenStyle)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPenStyle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPenStyle)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

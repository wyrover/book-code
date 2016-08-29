// BrushStyle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrushStyle window

class CBrushStyle : public CGraphicCombo
{
// Construction
public:
	CBrushStyle();
	int Load(int sel);
// Attributes
public:

// Operations
public:

// Overrides
	virtual void drawSample(LPRECT r, LPDRAWITEMSTRUCT dis);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrushStyle)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBrushStyle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBrushStyle)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

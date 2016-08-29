// pCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteCombo window

class CPaletteCombo : public CColor
{
// Construction
public:
	CPaletteCombo();
	int Load(CPalette * p = NULL);
	int AddString(int i, LPPALETTEENTRY pe);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaletteCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

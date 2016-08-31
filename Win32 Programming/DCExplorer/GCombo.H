// GraphicCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphicCombo window


class CGraphicCombo : public CIDCombo
{
// Construction
public:
	CGraphicCombo();
	void SetWidth(int n);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void drawSample(RECT * r, LPDRAWITEMSTRUCT lpdis) = 0;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicCombo)
	//}}AFX_MSG
	int width;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

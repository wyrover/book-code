// BitmapSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapSelect window

class CBitmapSelect : public CImageCombo
{
// Construction
public:
	CBitmapSelect();
	
// Attributes
public:

// Operations
public:
	int Load(const LPINT bitmaps, int sel = 0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapSelect)
	virtual void DrawItem(LPDRAWITEMSTRUCT dis);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT mis);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapSelect();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapSelect)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

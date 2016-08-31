// Palette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteDisplay dialog

class CPaletteDisplay : public CPropertyPage
{
	DECLARE_DYNCREATE(CPaletteDisplay)

// Construction
public:
	CPaletteDisplay();
	~CPaletteDisplay();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CPaletteDisplay)
	enum { IDD = IDD_PALETTE };
	CPaletteCombo	c_Palette;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPaletteDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void addPaletteEntry(int i, LPPALETTEENTRY pe);
	// Generated message map functions
	//{{AFX_MSG(CPaletteDisplay)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// DevCaps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevCaps dialog

class CDevCaps : public CPropertyPage
{
	DECLARE_DYNCREATE(CDevCaps)

// Construction
public:
	CDevCaps();
	~CDevCaps();
	CDCValues * dcv;
	CDC * dc;	// display context

// Dialog Data
	//{{AFX_DATA(CDevCaps)
	enum { IDD = IDD_DEVICECAPS };
	CMessage	c_Result;
	CBitCombo	c_TEXTCAPS;
	CBitCombo	c_POLYGONALCAPS;
	CBitCombo	c_LINECAPS;
	CBitCombo	c_CURVECAPS;
	CBitCombo	c_RASTERCAPS;
	CStatic	c_c_SIZEPALETTE;
	CNumericEdit	c_DRIVERVERSION;
	CNumericEdit	c_SIZEPALETTE;
	CEnumEdit	c_TECHNOLOGY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDevCaps)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowControls(UINT mode);
	void EnableControls(BOOL mode);
	void pd_to_controls();
	// Generated message map functions
	//{{AFX_MSG(CDevCaps)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

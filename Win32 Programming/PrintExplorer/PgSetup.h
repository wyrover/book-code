// PgSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPgSetup dialog

class CPgSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CPgSetup)

// Construction
public:
	CPgSetup();
	~CPgSetup();

// Dialog Data
	PAGESETUPDLG psd;
	LPPrintIt prt;		// print object that gets psd

	DWORD result;

	//{{AFX_DATA(CPgSetup)
	enum { IDD = IDD_PAGESETUP };
	CMessageLog	c_Messages;
	CStatic	c_c_lpfnPagePaintHook;
	CNumericEdit	c_lpfnPagePaintHook;
	CCommErr	c_Result;
	CHDevNames	c_hDevNames;
	CHDevMode	c_hDevMode;
	CStatic	c_c_rtMinMargin;
	CStatic	c_crtMargin;
	CStatic	c_c_ptPapersize;
	CNumericEdit	c_rtMinMargin_right;
	CNumericEdit	c_rtMinMargin_top;
	CNumericEdit	c_rtMinMargin_left;
	CNumericEdit	c_rtMinMargin_bottom;
	CNumericEdit	c_rtMargin_right;
	CNumericEdit	c_rtMargin_top;
	CNumericEdit	c_rtMargin_left;
	CNumericEdit	c_rtMargin_bottom;
	CNumericEdit	c_ptPaperSize_Y;
	CNumericEdit	c_ptPaperSize_X;
	CButton	c_PSD_ShowHelp;
	CButton	c_PSD_ReturnDefault;
	CButton	c_PSD_NoWarning;
	CButton	c_PSD_MinMargins;
	CButton	c_PSD_Margins;
	CButton	c_PSD_InThousandthsOfInches;
	CButton	c_PSD_InHundredthsOfMillimeters;
	CButton	c_PSD_EnablePageSetupTemplateHandle;
	CButton	c_PSD_EnablePageSetupTemplate;
	CButton	c_PSD_EnablePageSetupHook;
	CButton	c_PSD_EnablePagePaintingHook;
	CButton	c_PSD_DisablePrinter;
	CButton	c_PSD_DisablePaper;
	CButton	c_PageSetup;
	CButton	c_PSD_DisablePagePainting;
	CButton	c_PSD_DisableOrientation;
	CButton	c_PSD_DisableMargins;
	CButton	c_PSD_DefaultMargins;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPgSetup)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void psd_to_controls();
	void controls_to_psd();
	void RequiredSetup();
	// Generated message map functions
	//{{AFX_MSG(CPgSetup)
	afx_msg LRESULT onLogPaint(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDefaults();
	afx_msg void OnPagesetup();
	afx_msg void OnPsdInhundredthsofmillimeters();
	afx_msg void OnPsdInthousandthsofinches();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClear();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPsdEnablepagepainthook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

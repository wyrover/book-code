// PrintDlgSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintDlgSetup dialog

class CPrintDlgSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrintDlgSetup)

// Construction
public:
	CPrintDlgSetup();
	~CPrintDlgSetup();
	PRINTDLG pd;
	LPINT result;

// Dialog Data
	//{{AFX_DATA(CPrintDlgSetup)
	enum { IDD = IDD_PRINTDLG };
	CButton	c_QuickDate;
	CResourceName	c_lpSetupTemplateName;
	CResourceName	c_lpPrintTemplateName;
	CButton	c_Print;
	CCommErr	c_Result;
	CNumericEdit	c_nMaxPage;
	CHandleEdit	c_hwndOwner;
	CHandleEdit	c_hSetupTemplate;
	CHandleEdit	c_hPrintTemplate;
	CNumericEdit	c_nToPage;
	CNumericEdit	c_nMinPage;
	CNumericEdit	c_nFromPage;
	CNumericEdit	c_lStructSize;
	CNumericEdit	c_nCopies;
	CHandleEdit	c_lpfnSetupHook;
	CHandleEdit	c_lpfnPrintHook;
	CNumericEdit	c_lCustData;
	CHandleEdit	c_hInstance;
	CHDevNames	c_hDevNames;
	CHDevMode	c_hDevMode;
	CHandleEdit	c_hDC;
	CNumericEdit	c_Flags;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrintDlgSetup)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void pd_to_controls();
	void controls_to_pd();
	void RequiredSetup();

	// Generated message map functions
	//{{AFX_MSG(CPrintDlgSetup)
	afx_msg void OnClear();
	afx_msg void OnDefaults();
	virtual BOOL OnInitDialog();
	afx_msg void OnPrintdlg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPrint();
	afx_msg void OnQuickdc();
	afx_msg void OnQuickdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

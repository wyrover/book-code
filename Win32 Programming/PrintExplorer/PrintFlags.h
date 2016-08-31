// PrintFlags.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintFlags dialog

class CPrintFlags : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrintFlags)

// Construction
public:
	CPrintFlags();
	~CPrintFlags();
	PRINTDLG * pd;
	LPINT result;

// Dialog Data
	//{{AFX_DATA(CPrintFlags)
	enum { IDD = IDD_FLAGS };
	CButton	c_EnablePrintTemplate;
	CButton	c_EnablePrintHook;
	CButton	c_CustomDlg;
	CCommErr c_Result;
	CButton	c_PD_Selection;
	CButton	c_PD_PageNums;
	CButton	c_PD_AllPages;
	CButton	c_PD_ReturnIC;
	CButton	c_PD_ReturnDC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrintFlags)
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

	// Generated message map functions
	//{{AFX_MSG(CPrintFlags)
	virtual BOOL OnInitDialog();
	afx_msg void OnPdReturndc();
	afx_msg void OnPdReturnic();
	afx_msg void OnPdAllpages();
	afx_msg void OnPdSelection();
	afx_msg void OnPdPagenums();
	afx_msg void OnPrintdlg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCustomdlg();
	afx_msg void OnPdEnableprinthook();
	afx_msg void OnPdEnableprinttemplate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

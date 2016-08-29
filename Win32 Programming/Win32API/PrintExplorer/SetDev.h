// SetDev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDev dialog

class CSetDev : public CPropertyPage
{
	DECLARE_DYNCREATE(CSetDev)

// Construction
public:
	CSetDev();
	~CSetDev();

// Dialog Data
	PRINTDLG * pd;

	//{{AFX_DATA(CSetDev)
	enum { IDD = IDD_SETDEVMODE };
	CMessage	c_Result;
	CButton	c_dmTTOption;
	CButton	c_DMTT_SUBDEV;
	CButton	c_DMTT_None;
	CButton	c_DMTT_DOWNLOAD;
	CButton	c_DMTT_BITMAP;
	CButton	c_DMORIENT_LANDSCAPE;
	CButton	c_DMORIENT_PORTRAIT;
	CButton	c_DMORIENT_None;
	CButton	c_dmOrientation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetDev)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void pd_to_controls();
	void controls_to_pd();
	void EnableControls(BOOL mode);
	// Generated message map functions
	//{{AFX_MSG(CSetDev)
	afx_msg void OnDmorientLandscape();
	afx_msg void OnDMORIENTNone();
	afx_msg void OnDmorientPortrait();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

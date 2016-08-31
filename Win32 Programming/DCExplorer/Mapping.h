// Mapping.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapping dialog

class CMapping : public CPropertyPage
{
	DECLARE_DYNCREATE(CMapping)

// Construction
public:
	CMapping();
	~CMapping();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CMapping)
	enum { IDD = IDD_MAPPING };
	CScrollBar	c_VScrollSample;
	CScrollBar	c_HScrollSample;
	CStatic	c_c_Wext;
	CStatic	c_c_Vext;
	CSystemMessage	c_Error;
	CMappingSample	c_Sample;
	CButton	c_1000;
	CButton	c_100;
	CButton	c_10;
	CFloatEdit	c_SY;
	CFloatEdit	c_SX;
	CButton	c_Scaling;
	CIntEdit	c_WindowOrgY;
	CIntEdit	c_WindowOrgX;
	CIntEdit	c_WindowExtY;
	CIntEdit	c_WindowExtX;
	CIntEdit	c_ViewportOrgY;
	CIntEdit	c_ViewportOrgX;
	CIntEdit	c_ViewportExtY;
	CIntEdit	c_ViewportExtX;
	CIDCombo	c_MappingMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMapping)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateControls();
	void updateSample(int n = 0);
	LRESULT OnUpdateError(WPARAM wParam, LPARAM);
	LRESULT OnChangeVext(WPARAM x, LPARAM y);
	LRESULT OnChangeWext(WPARAM x, LPARAM y);
	void setSampleParms();
	// Generated message map functions
	//{{AFX_MSG(CMapping)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokMappingmode();
	afx_msg void OnChangeVextx();
	afx_msg void OnChangeVexty();
	afx_msg void OnChangeVorgx();
	afx_msg void OnChangeVorgy();
	afx_msg void OnChangeWextx();
	afx_msg void OnChangeWexty();
	afx_msg void OnChangeWorgx();
	afx_msg void OnChangeWorgy();
	afx_msg void On10();
	afx_msg void On100();
	afx_msg void On1000();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

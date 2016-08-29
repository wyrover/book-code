// DDState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDState dialog

class CDDState : public CComboOps
{
	DECLARE_DYNCREATE(CDDState)

// Construction
public:
	CDDState();
	~CDDState();

// Dialog Data
	//{{AFX_DATA(CDDState)
	enum { IDD = IDD_DROPDOWNMESSAGES };
	CLocales	c_Locales;
	CIntEdit	c_TopIndex;
	CSpinButtonCtrl	c_SpinTopIndex;
	CSpinButtonCtrl	c_SpinSelection;
	CSpinButtonCtrl	c_SpinLimit;
	CSpinButtonCtrl	c_SpinHorizontalExtent;
	CTinyButton	c_SetTopIndex;
	CTinyButton	c_SetLocale;
	CTinyButton	c_SetHorizontalExtent;
	CTinyButton	c_SetCurSel;
	CIntEdit	c_Selection;
	CTinyButton	c_GetCount;
	CTinyButton	c_GetCurSel;
	CTinyButton	c_GetHorizontalExtent;
	CTinyButton	c_GetLocale;
	CTinyButton	c_GetTopIndex;
	CIntEdit	c_HorizontalExtent;
	CTinyButton	c_LimitText;
	CIntEdit	c_LimitVal;
	CTinyButton	c_ResetContent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDDState)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateControls();

	// Generated message map functions
	//{{AFX_MSG(CDDState)
	afx_msg void OnGetcount();
	afx_msg void OnGethorizontalextent();
	afx_msg void OnGetlocale();
	afx_msg void OnGettopindex();
	afx_msg void OnLimittext();
	afx_msg void OnGetcursel();
	afx_msg void OnSetcursel();
	afx_msg void OnSetlocale();
	afx_msg void OnSettopindex();
	afx_msg void OnResetcontent();
	afx_msg void OnSethorizontalextent();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

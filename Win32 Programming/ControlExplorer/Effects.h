// Effects.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDEffects dialog

class CDDEffects : public CComboOps
{
	DECLARE_DYNCREATE(CDDEffects)

// Construction
public:
	CDDEffects();
	~CDDEffects();

// Dialog Data
	//{{AFX_DATA(CDDEffects)
	enum { IDD = IDD_DDEFFECTS };
	CSpinButtonCtrl	c_SpinDroppedWidth;
	CTinyButton	c_ShowDropDown;
	CTinyButton	c_SetExtendedUI;
	CTinyButton	c_SetDroppedWidth;
	CEdit	c_Rect;
	CTinyButton	c_GetExtendedUI;
	CTinyButton	c_GetDroppedWidth;
	CTinyButton	c_GetDroppedState;
	CTinyButton	c_GetDroppedControlRect;
	CTinyButton	c_ExtendedUI;
	CIntEdit	c_DroppedWidth;
	CButton	c_DropDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDDEffects)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDDEffects)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdroppedcontrolrect();
	afx_msg void OnGetdroppedstate();
	afx_msg void OnGetdroppedwidth();
	afx_msg void OnGetextendedui();
	afx_msg void OnSetdroppedwidth();
	afx_msg void OnSetextendedui();
	afx_msg void OnShowdropdown();
	afx_msg void OnDropdown();
	afx_msg void OnExtendedui();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// EditTabs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditTabs dialog

class CEditTabs : public CEditOps
{
	DECLARE_DYNCREATE(CEditTabs)

// Construction
public:
	CEditTabs();
	~CEditTabs();

// Dialog Data
	//{{AFX_DATA(CEditTabs)
	enum { IDD = IDD_EDITTABS };
	CSpinButtonCtrl	c_SpinTab;
	CButton	c_Remove;
	CTinyButton	c_TabStops;
	CIntEdit	c_TabVal;
	CNumericList	c_Tabs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditTabs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();
	// Generated message map functions
	//{{AFX_MSG(CEditTabs)
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabstops();
	afx_msg void OnSelchangeTabs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

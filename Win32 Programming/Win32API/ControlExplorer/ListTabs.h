// ListTabs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListTabs dialog

class CListTabs : public CListOps
{
	DECLARE_DYNCREATE(CListTabs)

// Construction
public:
	CListTabs();
	~CListTabs();

// Dialog Data
	//{{AFX_DATA(CListTabs)
	enum { IDD = IDD_LISTBOXTABS };
	CSpinButtonCtrl	c_SpinTab;
	CButton	c_Remove;
	CTinyButton	c_TabStops;
	CIntEdit	c_TabVal;
	CNumericList	c_Tabs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListTabs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();
	// Generated message map functions
	//{{AFX_MSG(CListTabs)
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabstops();
	afx_msg void OnSelchangeTabs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

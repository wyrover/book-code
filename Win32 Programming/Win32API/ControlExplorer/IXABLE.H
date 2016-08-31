// ixable.h : header file
//

//*****************************************************************************
#define UWM_SETPAGEFOCUS (WM_USER + 1)
//	wParam: ID of control that is to receive focus
//	lParam: unused, 0
//	Result: void
//	Effect: Sets the focus to the control whose ID is specified
//*****************************************************************************

/////////////////////////////////////////////////////////////////////////////
// CIndexablePage dialog

class CIndexablePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIndexablePage)

// Construction
public:
	CIndexablePage();
	CIndexablePage(int idd);
	~CIndexablePage();
	int id;        // page ID for use by table-of-contents
	int focus;	// first control which should get focus

// Dialog Data
	//{{AFX_DATA(CIndexablePage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIndexablePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIndexablePage)
	afx_msg LRESULT OnFocusRequest(WPARAM wParam, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

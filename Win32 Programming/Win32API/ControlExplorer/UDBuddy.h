// UDBuddy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpDownBuddy dialog

class CUpDownBuddy : public CUDOps
{
	DECLARE_DYNCREATE(CUpDownBuddy)

// Construction
public:
	CUpDownBuddy();
	~CUpDownBuddy();

// Dialog Data
	CArray<CZ *, CZ *> * czarray;

	//{{AFX_DATA(CUpDownBuddy)
	enum { IDD = IDD_UPDOWNBUD };
	CTinyButton	c_SetBase;
	CTinyButton	c_SetBuddy;
	CTinyButton	c_GetBuddy;
	CTinyButton	c_GetBase;
	CComboBox	c_Buddy;
	CComboBox	c_Base;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUpDownBuddy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int findSel(int n);
	CString getWindowString(HWND hwnd);

	// Generated message map functions
	//{{AFX_MSG(CUpDownBuddy)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetbase();
	afx_msg void OnGetbuddy();
	afx_msg void OnSetbuddy();
	afx_msg void OnSetbase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

//=============================================================================
#define UWM_DRAWITEM (WM_USER + 120)
//	wParam: window handle of window
//	lParam: message string
//	Result: 0 (void)
//=============================================================================

class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg LRESULT OnQuery3DControls(WPARAM, LPARAM);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

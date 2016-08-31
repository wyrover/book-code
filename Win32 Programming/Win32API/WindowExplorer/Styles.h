// Styles.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStyles dialog

class CStyles : public CPropertyPage
{
	DECLARE_DYNCREATE(CStyles)

// Construction
public:
	CStyles();
	~CStyles();

// Dialog Data
	//{{AFX_DATA(CStyles)
	enum { IDD = IDD_STYLES };
	CNumericEdit	c_Flags;
	CButton	c_WS_POPUP;
	CButton	c_WS_OVERLAPPED;
	CButton	c_WS_CHILD;
	CButton	c_WS_POPUPWINDOW;
	CButton	c_WS_OVERLAPPEDWINDOW;
	CButton	c_WS_CHILDWINDOW;
	CButton	c_WS_VSCROLL;
	CButton	c_WS_VISIBLE;
	CButton	c_WS_THICKFRAME;
	CButton	c_WS_TABSTOP;
	CButton	c_WS_SYSMENU;
	CButton	c_WS_MINIMIZEBOX;
	CButton	c_WS_MINIMIZE;
	CButton	c_WS_MAXIMIZEBOX;
	CButton	c_WS_MAXIMIZE;
	CButton	c_WS_HSCROLL;
	CButton	c_WS_GROUP;
	CButton	c_WS_DLGFRAME;
	CButton	c_WS_CLIPSIBLINGS;
	CButton	c_WS_CLIPCHILDREN;
	CButton	c_WS_CAPTION;
	CButton	c_WS_BORDER;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStyles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void setOverlappedWindow();
	void setPopupWindow();
	void showStyles();
	// Generated message map functions
	//{{AFX_MSG(CStyles)
	afx_msg LRESULT OnQuerySiblings(WPARAM, LPARAM);
	afx_msg void OnWsBorder();
	afx_msg void OnWsCaption();
	afx_msg void OnWsClipchildren();
	afx_msg void OnWsClipsiblings();
	afx_msg void OnWsDlgrame();
	afx_msg void OnWsGroup();
	afx_msg void OnWsHscroll();
	afx_msg void OnWsMaximize();
	afx_msg void OnWsMaximizebox();
	afx_msg void OnWsMinimize();
	afx_msg void OnWsMinimizebox();
	afx_msg void OnWsSysmenu();
	afx_msg void OnWsTabstop();
	afx_msg void OnWsThickframe();
	afx_msg void OnWsVisible();
	afx_msg void OnWsVscroll();
	virtual BOOL OnInitDialog();
	afx_msg void OnWsPopupwindow();
	afx_msg void OnWsOverlappedwindow();
	afx_msg void OnWsChildwindow();
	afx_msg void OnWsChild();
	afx_msg void OnWsOverlapped();
	afx_msg void OnWsPopup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

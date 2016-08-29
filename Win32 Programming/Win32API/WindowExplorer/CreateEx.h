// CreateEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateWindowEx dialog

class CCreateWindowEx : public CPropertyPage
{
	DECLARE_DYNCREATE(CCreateWindowEx)

// Construction
public:
	CCreateWindowEx();
	~CCreateWindowEx();

// Dialog Data
	//{{AFX_DATA(CCreateWindowEx)
	enum { IDD = IDD_CREATEWINDOWEX };
	CSpinButtonCtrl	c_SpinID;
	CComboBox	c_Menu;
	CNumericEdit	c_ID;
	CButton	c_API3Only;
	CWindowList	c_Windows;
	CEdit	c_Show;
	CMessage	c_Message;
	CComboBox	c_Classname;
	CSpinButtonCtrl	c_HSpin;
	CComboBox	c_Parent;
	CEdit	c_WindowName;
	CSpinButtonCtrl	c_YSpin;
	CSpinButtonCtrl	c_XSpin;
	CSpinButtonCtrl	c_WSpin;
	CButton	c_WS_EX_WINDOWEDGE;
	CButton	c_WS_EX_TRANSPARENT;
	CButton	c_WS_EX_TOPMOST;
	CButton	c_WS_EX_TOOLWINDOW;
	CButton	c_WS_EX_STATICEDGE;
	CButton	c_WS_EX_RTLREADING;
	CButton	c_WS_EX_RIGHTSCROLLBAR;
	CButton	c_WS_EX_RIGHT;
	CButton	c_WS_EX_PALETTEWINDOW;
	CButton	c_WS_EX_OVERLAPPEDWINDOW;
	CButton	c_WS_EX_ACCEPTFILES;
	CButton	c_WS_EX_NOPARENTNOTIFY;
	CButton	c_WS_EX_MDICHILD;
	CButton	c_WS_EX_LTRREADING;
	CButton	c_WS_EX_DLGMODALFRAME;
	CButton	c_WS_EX_CONTROLPARENT;
	CButton	c_WS_EX_CONTEXTHELP;
	CButton	c_WS_EX_CLIENTEDGE;
	CButton	c_WS_EX_APPWINDOW;
	CButton	c_WS_EX_LEFTSCROLLBAR;
	CButton	c_WS_EX_LEFT;
	CNumericEdit	c_Y;
	CNumericEdit	c_X;
	CNumericEdit	c_W;
	CNumericEdit	c_H;
	CButton	c_Destroy;
	CButton	c_Create;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCreateWindowEx)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL initialized;
	int initClassNames();
	int addClassName(int id); 
	void selectWindow(HWND hwnd);
	void doCreate(BOOL doit);
	CWindowManager windowlist;
	void updateWindowList();
	void enableControls();
	void addParent(int id, HWND hwnd);
	void addParent(CString s, HWND hwnd);
	void initParents();
	void initMenu();

	// Generated message map functions
	//{{AFX_MSG(CCreateWindowEx)
	afx_msg void OnCreateEx();
	afx_msg void OnDoDestroy();
	afx_msg void OnWsExAcceptfiles();
	afx_msg void OnWsExAppwindow();
	afx_msg void OnWsExClientedge();
	afx_msg void OnWsExContexthelp();
	afx_msg void OnWsExControlparent();
	afx_msg void OnWsExDlgmodalframe();
	afx_msg void OnWsExLeft();
	afx_msg void OnWsExLeftscrollbar();
	afx_msg void OnWsExLtrreading();
	afx_msg void OnWsExMdichild();
	afx_msg void OnWsExNoparentnotify();
	afx_msg void OnWsExOverlappedwindow();
	afx_msg void OnWsExPalettewindow();
	afx_msg void OnWsExRight();
	afx_msg void OnWsExRightscrollbar();
	afx_msg void OnWsExRtlreading();
	afx_msg void OnWsExStaticedge();
	afx_msg void OnWsExToolwindow();
	afx_msg void OnWsExTopmost();
	afx_msg void OnWsExTransparent();
	afx_msg void OnWsExWindowedge();
	afx_msg void OnChangeH();
	afx_msg void OnChangeW();
	afx_msg void OnChangeX();
	afx_msg void OnChangeY();
	afx_msg void OnSelendokClassname();
	afx_msg void OnChangeWindowname();
	afx_msg void OnDblclkWindows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusWindows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusWindows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickWindows(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApi3Only();
	afx_msg void OnSelendokParent();
	afx_msg void OnDestroy();
	afx_msg void OnSelendokMenu();
	afx_msg void OnChangeId();
	afx_msg LRESULT OnMyActivate(WPARAM, LPARAM);
	afx_msg LRESULT OnMyKill(WPARAM, LPARAM);
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

};

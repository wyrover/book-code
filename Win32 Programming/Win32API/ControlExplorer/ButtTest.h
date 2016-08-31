// ButtTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonTest dialog

class CButtonTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CButtonTest)

// Construction
public:
	CButtonTest();
	~CButtonTest();

// Dialog Data
	//{{AFX_DATA(CButtonTest)
	enum { IDD = IDD_BUTTON };
	CButton	c_Group1;
	CButton	c_Group3;
	CButton	c_Group2;
	CStatic	c_Working3;
	CStatic	c_Working2;
	CStatic	c_Working1;
	CButton	c_NoAnimation;
	CButton	c_WSBorder;
	CEdit	c_Edit;
	CStatic	c_Static;
	CMessageLog	c_Messages;
	CButton	c_BSFlat;
	CButton	c_Enable;
	CMyButton	c_Button3;
	CMyButton	c_Button2;
	CMyButton	c_Button1;
	CButton	c_BSVCenter;
	CButton	c_BSTop;
	CButton	c_BSText;
	CButton	c_BSRightButton;
	CButton	c_BSRight;
	CButton	c_BSRadioButton;
	CButton	c_BSPushLike;
	CButton	c_BSPushButton;
	CButton	c_BSOwnerDraw;
	CButton	c_BSNotify;
	CButton	c_BSMultiLine;
	CButton	c_BSLeftText;
	CButton	c_BSLeft;
	CButton	c_BSIcon;
	CButton	c_BSGroupBox;
	CButton	c_BSDefPushButton;
	CButton	c_BSCheckBox;
	CButton	c_BSCenter;
	CButton	c_BSBottom;
	CButton	c_BSBitmap;
	CButton	c_BSAutoRadioButton;
	CButton	c_BSAutoCheckBox;
	CButton	c_BSAuto3State;
	CButton	c_BS3State;
	CButton	c_Alignment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CButtonTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void recreateWindows(DWORD style);
 	void recreateWindow(CButton * button, int id, CRect rect, DWORD style);
	void buttonEvent(CButton * button, int id, int event);
	void addStyle(CButton * button, DWORD style, BOOL add);
	void setHAlignment(CButton * button, DWORD style);
	void setVAlignment(CButton * button, DWORD style);
	void setHAlignments(DWORD style);
	void setVAlignments(DWORD style);
	DWORD getAlignment();
	DWORD removeHAlignment(DWORD style);
	DWORD removeVAlignment(DWORD style);
	BOOL hasState(CButton * button);
	void setButtonText(CButton * button, int id);
	CString transform(CString s);
	DWORD getButtonType(DWORD style);
	void enableControls();
	DWORD removeDisplayType(DWORD style);
	void setDisplayType(CButton * button, DWORD style);
	void setDisplayTypes(DWORD style);
	DWORD getDisplayType();
	void setIcon(CButton * button, int id);

	CFont * orgfont;
	CRect orgsize;
	CRect size1;
	CRect size2;
	CRect size3;

	CWnd * ControlParent;  // parent of any dynamically-created controls

	// Generated message map functions
	//{{AFX_MSG(CButtonTest)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnLogDrawItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBs3state();
	afx_msg void OnBsAuto3state();
	afx_msg void OnBsAutocheckbox();
	afx_msg void OnBsAutoradiobutton();
	afx_msg void OnBsBitmap();
	afx_msg void OnBsBottom();
	afx_msg void OnBsCenter();
	afx_msg void OnBsCheckbox();
	afx_msg void OnBsDefpushbutton();
	afx_msg void OnBsGroupbox();
	afx_msg void OnBsIcon();
	afx_msg void OnBsLeft();
	afx_msg void OnBsLefttext();
	afx_msg void OnBsMultiline();
	afx_msg void OnBsNotify();
	afx_msg void OnBsOwnerdraw();
	afx_msg void OnBsPushbutton();
	afx_msg void OnBsPushlike();
	afx_msg void OnBsRadiobutton();
	afx_msg void OnBsRight();
	afx_msg void OnBsRightbutton();
	afx_msg void OnBsText();
	afx_msg void OnBsTop();
	afx_msg void OnBsVcenter();
	afx_msg void OnButton();
	afx_msg void OnDlgcode();
	afx_msg void OnEnabled();
	afx_msg void OnMessages();
	afx_msg void OnNoAnimation();
	afx_msg void OnSendmessages();
	afx_msg void OnSettext1();
	afx_msg void OnSettext2();
	afx_msg void OnSettext3();
	afx_msg void OnWsBorder();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnBsFlat();
	afx_msg void OnDoubleclickedButton1();
	afx_msg void OnDoubleclickedButton2();
	afx_msg void OnDoubleclickedButton3();
	afx_msg void OnSetFocusButton1();
	afx_msg void OnSetFocusButton2();
	afx_msg void OnSetFocusButton3();
	afx_msg void OnKillFocusButton1();
	afx_msg void OnKillFocusButton2();
	afx_msg void OnKillFocusButton3();
	afx_msg void OnHdefault();
	afx_msg void OnVdefault();
	afx_msg void OnGroup1();
	afx_msg void OnGroup2();
	afx_msg void OnGroup3();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

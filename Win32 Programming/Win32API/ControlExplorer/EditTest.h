// EditTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditTest dialog

class CEditTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CEditTest)

// Construction
public:
	CEditTest();
	~CEditTest();
	CButton	c_WantReturn;
	CButton	c_VScroll;
	CButton	c_Uppercase;
	CButton	c_AutoVScroll;
	CButton	c_AutoHScroll;
	CButton	c_Border;
	CButton	c_ReadOnly;
	CButton	c_Number;
	CButton	c_NoHideSel;
	CButton	c_Multiline;
	CButton	c_Lowercase;
	CButton	c_HScroll;
	CButton	c_Enable;
	CDlgCodeEdit	c_Edit;

	DWORD style;

// Dialog Data
	//{{AFX_DATA(CEditTest)
	enum { IDD = IDD_EDIT };
	CTinyButton	c_OEMSample;
	CButton	c_NoAnimation;
	CButton	c_Left;
	CButton	c_Center;
	CButton	c_Right;
	CButton	c_Password;
	CButton	c_OEMConvert;
	CButton	c_UseLimit;
	CSpinButtonCtrl	c_SpinLimit;
	CIntEdit	c_LimitVal;
	CMessageLog	c_Messages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditTest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
 	void recreateWindow();
	void clearStyleBit(CButton * ctl, DWORD newstyle);
	void setStyleBit(CButton * ctl, DWORD newstyle);
	void styleToControls();
	void logEdit(int id);
	void enableLimit();
	void setTextLimit();
	void enableMultiLine(BOOL multi);
	void enableControls();

	// Generated message map functions
	//{{AFX_MSG(CEditTest)
	afx_msg void OnSendMessages();
	afx_msg void OnAutohscroll();
	afx_msg void OnAutovscroll();
	afx_msg void OnBorder();
	afx_msg void OnEnable();
	afx_msg void OnHscroll();
	afx_msg void OnLowercase();
	afx_msg void OnMultiline();
	afx_msg void OnNohidesel();
	afx_msg void OnNumber();
	afx_msg void OnReadonly();
	afx_msg void OnUppercase();
	afx_msg void OnVscroll();
	afx_msg void OnWantreturn();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnChangeEdit();
	afx_msg void OnErrspaceEdit();
	afx_msg void OnHscrollEdit();
	afx_msg void OnKillfocusEdit();
	afx_msg void OnMaxtextEdit();
	afx_msg void OnSetfocusEdit();
	afx_msg void OnUpdateEdit();
	afx_msg void OnVscrollEdit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUselimit();
	afx_msg void OnCenter();
	afx_msg void OnLeft();
	afx_msg void OnChangeLimitval();
	afx_msg void OnOemconvert();
	afx_msg void OnPassword();
	afx_msg void OnRight();
	afx_msg void OnDlgcode();
	afx_msg void OnOemSample();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

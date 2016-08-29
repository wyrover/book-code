// BtnMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonMessages dialog

class CButtonMessages : public CDialog
{
// Construction
public:
	CButtonMessages(CWnd* pParent = NULL);   // standard constructor
	CMessageLog * c_Messages;
	CButton * c_Button1;
	CButton * c_Button2;
	CButton * c_Button3;
	

// Dialog Data
	//{{AFX_DATA(CButtonMessages)
	enum { IDD = IDD_BUTTONMSG };
	CButton	c_StateVal;
	CButton	c_Button3Select;
	CButton	c_Button2Select;
	CButton	c_Button1Select;
	CTinyButton	c_SetStyle;
	CTinyButton	c_SetState;
	CTinyButton	c_SetImage;
	CTinyButton	c_SetCheck;
	CTinyButton	c_GetState;
	CTinyButton	c_GetImage;
	CTinyButton	c_GetCheck;
	CTinyButton	c_Click;
	CButton	c_2;
	CButton	c_1;
	CButton	c_0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonMessages)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString getButtonID(CButton * button);
	CButton * getButton();
	int getCheckVal();
	void setIcon(CButton * button, HICON icon);
	HICON loadIcon(int id);
	HICON icon1;
	HICON icon2;
	HICON icon3;

	// Generated message map functions
	//{{AFX_MSG(CButtonMessages)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnClick();
	afx_msg void OnGetcheck();
	afx_msg void OnGetimage();
	afx_msg void OnGetstate();
	afx_msg void OnSetcheck();
	afx_msg void OnSetimage();
	afx_msg void OnSetstate();
	afx_msg void OnSetstyle();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

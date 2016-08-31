// DlgCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetDlgCode dialog

class CGetDlgCode : public CDialog
{
// Construction
public:
	CGetDlgCode(CWnd* pParent = NULL);   // standard constructor

// Parameters
	CWnd * ctl;  // control whose dialog codes are to be set
	
// Dialog Data
	//{{AFX_DATA(CGetDlgCode)
	enum { IDD = IDD_DLGCODE };
	CButton	c_UndefPushbutton;
	CButton	c_Static;
	CButton	c_UseDefault;
	CButton	c_PushButton;
	CButton	c_WantTab;
	CButton	c_WantMessage;
	CButton	c_WantChars;
	CButton	c_WantArrows;
	CButton	c_WantAllKeys;
	CButton	c_RadioButton;
	CButton	c_HasSetSel;
	CButton	c_DefPushbutton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDlgCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT controlsToCode();
	void codeToControls(UINT codes);

	// Generated message map functions
	//{{AFX_MSG(CGetDlgCode)
	afx_msg void OnDlgcWantallkeys();
	afx_msg void OnDlgcWantmessage();
	afx_msg void OnUsedefault();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

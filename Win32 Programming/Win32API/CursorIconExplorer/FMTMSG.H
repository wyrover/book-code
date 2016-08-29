// FmtMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFmtMsg dialog

class CFmtMsg : public CDialog
{
// Construction
public:
	CFmtMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFmtMsg)
	enum { IDD = IDD_FMTMSG };
	CButton	c_TestBuff;
	CStatic	c_Result;
	CComboBox	c_P3;
	CComboBox	c_P2;
	CComboBox	c_P1;
	CComboBox	c_Fmt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmtMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFmtMsg)
	afx_msg void OnSelchangeFmt();
	afx_msg void OnSelchangeP1();
	afx_msg void OnSelchangeP2();
	afx_msg void OnSelchangeP3();
	virtual BOOL OnInitDialog();
	afx_msg void OnMsgbox();
	afx_msg void OnTestbuff();
	afx_msg void OnHelp();
	//}}AFX_MSG

	void Reformat();

	DECLARE_MESSAGE_MAP()
};

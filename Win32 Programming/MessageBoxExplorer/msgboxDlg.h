// msgboxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgboxDlg dialog

class CMsgboxDlg : public CDialog
{
// Construction
public:
	CMsgboxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgboxDlg)
	enum { IDD = IDD_MSGBOX_DIALOG };
	CButton	c_NullParent;
	CStatic	c_Flags;
	CStatic	c_IconWarningIcon;
	CStatic	c_IconQuestionIcon;
	CStatic	c_IconInformationIcon;
	CStatic	c_IconErrorIcon;
	CButton	c_NoIcon;
	CEdit	c_Result;
	CButton	c_MsgBox;
	CButton	c_YesNoCancel;
	CButton	c_YesNo;
	CButton	c_TaskModal;
	CButton	c_SystemModal;
	CButton	c_SetForeground;
	CButton	c_ServiceNotification;
	CButton	c_RTLReading;
	CButton	c_Right;
	CButton	c_RetryCancel;
	CButton	c_OKCancel;
	CButton	c_OK;
	CButton	c_IconWarning;
	CButton	c_IconQuestion;
	CButton	c_IconInformation;
	CButton	c_IconError;
	CButton	c_Help;
	CButton	c_DefButton4;
	CButton	c_DefButton3;
	CButton	c_DefButton2;
	CButton	c_DefButton1;
	CButton	c_DefaultDesktopOnly;
	CButton	c_ApplModal;
	CButton	c_AbortRetryIgnore;
	CEdit	c_Caption;
	CEdit	c_Body;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgboxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int buttons;
	int icon;
	int modality;
	int def;
	int misc;

	// Generated message map functions
	//{{AFX_MSG(CMsgboxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMsgbox();
	afx_msg void OnMbYesnocancel();
	afx_msg void OnMbYesno();
	afx_msg void OnMbTaskmodal();
	afx_msg void OnMbSystemmodal();
	afx_msg void OnMbSetforeground();
	afx_msg void OnMbServiceNotification();
	afx_msg void OnMbRtlreading();
	afx_msg void OnMbRight();
	afx_msg void OnMbRetrycancel();
	afx_msg void OnMbOkcancel();
	afx_msg void OnMbOk();
	afx_msg void OnMbIconwarning();
	afx_msg void OnMbIconquestion();
	afx_msg void OnMbIconinformation();
	afx_msg void OnMbIconerror();
	afx_msg void OnMbHelp();
	afx_msg void OnMbDefbutton4();
	afx_msg void OnMbDefbutton3();
	afx_msg void OnMbDefbutton2();
	afx_msg void OnMbDefbutton1();
	afx_msg void OnMbDefaultDesktopOnly();
	afx_msg void OnMbApplmodal();
	afx_msg void OnMbAbortretryignore();
	afx_msg void OnNoIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

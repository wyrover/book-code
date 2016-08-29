// dipperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDipperDlg dialog

class CDipperDlg : public CDialog
{
// Construction
public:
	CDipperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDipperDlg)
	enum { IDD = IDD_DIPPER_DIALOG };
	CStatic	c_Status;
	CGlass	c_Glass5;
	CGlass	c_Glass4;
	CGlass	c_Glass3;
	CGlass	c_Glass2;
	CGlass	c_Glass1;
	CBucket	c_Bucket;
	CButton	c_Close;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDipperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HCURSOR dipper;  // current dipper

	// Generated message map functions
	//{{AFX_MSG(CDipperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT doStatus(WPARAM id, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

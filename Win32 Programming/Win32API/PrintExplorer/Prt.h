// Prt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrt dialog

class CPrt : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrt)

// Construction
public:
	CPrt();
	~CPrt();

// Dialog Data
	PRINTDLG * pd;
	PrintIt prt;

	//{{AFX_DATA(CPrt)
	enum { IDD = IDD_PRINTDLL };
	CButton	c_50;
	CButton	c_500;
	CButton	c_200;
	CButton	c_1000;
	CNumericEdit	c_Right;
	CNumericEdit	c_Left;
	CButton	c_Hook;
	CNumericEdit	c_YPos;
	CNumericEdit	c_Bottom;
	CNumericEdit	c_Top;
	CNumericEdit	c_PageNo;
	CNumericEdit	c_DY;
	CButton	c_Print;
	CButton	c_Query;
	CHdng	c_TopRight;
	CHdng	c_TopLeft;
	CHdng	c_TopCenter;
	CHdng	c_BottomRight;
	CHdng	c_BottomLeft;
	CHdng	c_BottomCenter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrt)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void prt_to_controls();
	void controls_to_prt();
	void prd_to_controls();
	// Generated message map functions
	//{{AFX_MSG(CPrt)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnableQuery();
	afx_msg void OnPrint();
	afx_msg LRESULT OnUserUpdate(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

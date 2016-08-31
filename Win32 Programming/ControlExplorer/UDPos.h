// UDPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpDownPos dialog

class CUpDownPos : public CUDOps
{
	DECLARE_DYNCREATE(CUpDownPos)

// Construction
public:
	CUpDownPos();
	~CUpDownPos();

// Dialog Data
	//{{AFX_DATA(CUpDownPos)
	enum { IDD = IDD_UPDOWNPOS };
	CButton	c_Swap;
	CSpinButtonCtrl	c_SpinPos;
	CSpinButtonCtrl	c_SpinLow;
	CSpinButtonCtrl	c_SpinHigh;
	CTinyButton	c_SetRange;
	CTinyButton	c_SetPos;
	CNumericEdit	c_Pos;
	CNumericEdit	c_Low;
	CNumericEdit	c_High;
	CTinyButton	c_GetRange;
	CTinyButton	c_GetPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUpDownPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void setSpinRange(CSpinButtonCtrl * ctl, int low, int high);
	// Generated message map functions
	//{{AFX_MSG(CUpDownPos)
	afx_msg void OnSwap();
	afx_msg void OnSetrange();
	afx_msg void OnSetpos();
	afx_msg void OnGetpos();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetrange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

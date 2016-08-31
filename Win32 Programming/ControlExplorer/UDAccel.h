// UDAccel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpDownAccelerators dialog

class CUpDownAccelerators : public CUDOps
{
	DECLARE_DYNCREATE(CUpDownAccelerators)

// Construction
public:
	CUpDownAccelerators();
	~CUpDownAccelerators();

// Dialog Data
	//{{AFX_DATA(CUpDownAccelerators)
	enum { IDD = IDD_UPDOWNACC };
	CListCtrl	c_Accels;
	CTinyButton	c_SetAccel;
	CTinyButton	c_GetAccel;
	CStatic	c_Count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUpDownAccelerators)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void initAccels();

	// Generated message map functions
	//{{AFX_MSG(CUpDownAccelerators)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetaccel();
	afx_msg void OnSetaccel();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

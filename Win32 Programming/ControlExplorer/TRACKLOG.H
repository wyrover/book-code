// tracklog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogarithmic dialog

class CLogarithmic : public CDialog
{
// Construction
public:
	CLogarithmic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogarithmic)
	enum { IDD = IDD_TRACKBAR_LOG };
	CNumericEdit	c_Value;
	CSliderCtrl	c_Trackbar;
	CNumericEdit	c_Pos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogarithmic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void setValue();
	BOOL recursion;

	// Generated message map functions
	//{{AFX_MSG(CLogarithmic)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

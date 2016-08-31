// BkgSamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkgSample dialog

class CBkgSample : public CDialog
{
// Construction
public:
	CBkgSample(CWnd* pParent = NULL);   // standard constructor
	CPoint org;
	int bkmode;
	int bkcolor;
	CBrush brush;

// Dialog Data
	//{{AFX_DATA(CBkgSample)
	enum { IDD = IDD_BKGSAMPLE };
	CStatic	c_Sample;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBkgSample)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBkgSample)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

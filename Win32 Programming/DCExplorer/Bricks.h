// Bricks.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBricks dialog

class CBricks : public CDialog
{
// Construction
public:
	CBricks(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBricks)
	enum { IDD = IDD_BRICKS };
	CButton	c_Transparent;
	CButton	c_Opaque;
	CDrawBricks c_Bricks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBricks)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBricks)
	afx_msg void OnTransparent();
	afx_msg void OnOpaque();
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	afx_msg LRESULT OnQueryBkMode(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

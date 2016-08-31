// Icon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIcon dialog

class CIcon : public CPropertyPage
{
        DECLARE_DYNCREATE(CIcon)

// Construction
public:
	CIcon();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIcon)
	enum { IDD = IDD_ICON };
	CButton	c_Copy;
	CButton	c_AutoCopy;
	CStatic	c_Icon;
	//}}AFX_DATA
	HICON ico;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIcon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIcon)
	afx_msg void OnCopy();
	virtual BOOL OnInitDialog();
	afx_msg void OnAutocopy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	afx_msg void IconSelect(UINT id);

	DECLARE_MESSAGE_MAP()

};

// EditScrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditScroll dialog

class CEditScroll : public CEditOps
{
	DECLARE_DYNCREATE(CEditScroll)

// Construction
public:
	CEditScroll();
	~CEditScroll();

// Dialog Data
	//{{AFX_DATA(CEditScroll)
	enum { IDD = IDD_EDITSCROLL };
	CButton	c_SBPageUp;
	CButton	c_SBPageDown;
	CButton	c_SBLineUp;
	CButton	c_SBLineDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditScroll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditScroll)
	virtual BOOL OnInitDialog();
	afx_msg void OnSbLinedown();
	afx_msg void OnSbLineup();
	afx_msg void OnSbPagedown();
	afx_msg void OnSbPageup();
	afx_msg void OnScroll();
	afx_msg void OnScrollcaret();
	afx_msg void OnLinescroll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// Printing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrinting dialog

class CPrinting : public CDialog
{
// Construction
public:
	CPrinting( CDC * dc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrinting)
	enum { IDD = IDD_PRINTER };
	CStatic	c_PageNo;
	CButton	c_EndPage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int pagecount;
	CDC * hDC;

	void UpdatePageCount();
	void FillPage(int id);
	// Generated message map functions
	//{{AFX_MSG(CPrinting)
	afx_msg void OnEndpage();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

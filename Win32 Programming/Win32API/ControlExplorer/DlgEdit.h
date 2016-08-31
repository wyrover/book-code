// DlgEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCodeEdit window

class CDlgCodeEdit : public CEdit
{
// Construction
public:
	CDlgCodeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCodeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDlgCodeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgCodeEdit)
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

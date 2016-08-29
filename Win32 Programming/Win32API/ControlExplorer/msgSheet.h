// msgSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageSheet

class CMessageSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMessageSheet)

// Construction
public:
	CMessageSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMessageSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMessageSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMessageSheet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

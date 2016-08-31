// DCProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCProperties

class CDCProperties : public CPropertySheet
{
	DECLARE_DYNAMIC(CDCProperties)

// Construction
public:
	CDCProperties(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDCProperties(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	int page;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCProperties)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDCProperties();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDCProperties)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

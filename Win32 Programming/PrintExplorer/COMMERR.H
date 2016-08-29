// CommErr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommerr window

class CCommErr : public CEdit
{
// Construction
public:
	CCommErr();

// Attributes
public:

// Operations
public:
	void SetWindowText(int val);
	HBRUSH OnCtlColor(CDC * pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommErr)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommErr();

	// Generated message map functions
protected:
	LPCTSTR deffmt;

	//{{AFX_MSG(CCommErr)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

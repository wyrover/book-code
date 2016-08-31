// resname.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResourceName window

class CResourceName : public CEdit
{
// Construction
public:
	CResourceName();
	void SetWindowText(LPCTSTR name);
	void Blank();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceName)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResourceName();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResourceName)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

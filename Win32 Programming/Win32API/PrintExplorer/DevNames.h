// DevNames.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHDevNames window

class CHDevNames : public CEdit
{
// Construction
public:
	CHDevNames();

// Attributes
public:

// Operations
public:
	void SetWindowText(HANDLE hDevNames);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHDevNames)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHDevNames();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHDevNames)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

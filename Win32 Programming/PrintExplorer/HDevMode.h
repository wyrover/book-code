// HDevMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHDevMode window

class CHDevMode : public CTextHandleEdit
{
// Construction
public:
	CHDevMode();

// Attributes
public:

// Operations
public:
    void SetWindowText(HANDLE hDevMode);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHDevMode)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHDevMode();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHDevMode)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

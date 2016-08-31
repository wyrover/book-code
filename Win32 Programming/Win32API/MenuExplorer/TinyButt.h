// TinyButt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTinyButton window

class CTinyButton : public CButton
{
// Construction
public:
	CTinyButton();
	void GetWindowText(CString & s);
	BOOL EnableWindow(BOOL newval);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTinyButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTinyButton();

	// Generated message map functions
protected:
	CStatic * findCaption();

	//{{AFX_MSG(CTinyButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

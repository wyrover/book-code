// Locales.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLocales window

class CLocales : public CComboBox
{
// Construction
public:
	CLocales();

// Attributes
public:

// Operations
public:
	void load();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocales)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLocales();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLocales)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

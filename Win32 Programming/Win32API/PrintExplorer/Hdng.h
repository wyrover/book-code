// Hdng.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHdng window


typedef struct { int id;  // menu ID 
	 LPPrintitHeaderFunc func;
       } HdngLoad, *LPHdngLoad;

class CHdng : public CComboBox
{
// Construction
public:
	CHdng();
	void LoadFuncs(LPHdngLoad tbl);
	int SelectFunc(LPPrintitHeaderFunc func);
	LPPrintitHeaderFunc GetFunc();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHdng)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHdng();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHdng)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

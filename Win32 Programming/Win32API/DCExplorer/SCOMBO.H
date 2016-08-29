// scombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSizeCombo window

class CSizeCombo : public CComboBox
{
// Construction
public:
	CSizeCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSizeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSizeCombo)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

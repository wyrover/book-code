// BitCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitCombo window

typedef struct {
	DWORD bit;
	int id; 
	       } BITCOMBO, * LPBITCOMBO;

class CBitCombo : public CComboBox
{
// Construction
public:
	CBitCombo();
	CBitCombo(LPBITCOMBO fmt);
	int AddBits(DWORD bit, LPBITCOMBO fmt = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitCombo();

	// Generated message map functions
protected:
	LPBITCOMBO deffmt;
	//{{AFX_MSG(CBitCombo)
	afx_msg void OnSelendcancel();
	afx_msg void OnSelendok();
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

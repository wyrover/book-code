// IDCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIDCombo window

typedef struct {
	int id;  // IDS_ for string, or 0 for end of table for AddStrings
	int val; // value for itemdata
	       } IDData;


class CIDCombo : public CComboBox
{
// Construction
public:
	CIDCombo();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIDCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIDCombo();
	int AddString(IDData * data);
	BOOL AddStrings(IDData * data, int def = 0);

	// Generated message map functions
protected:
	//{{AFX_MSG(CIDCombo)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

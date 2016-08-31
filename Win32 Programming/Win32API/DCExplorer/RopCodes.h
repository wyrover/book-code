// RopCodes.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRopCodes window

class CRopCodes : public CIDCombo
{
// Construction
public:
	CRopCodes();
	void Load(int sel);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRopCodes)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRopCodes();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRopCodes)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// NumericList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumericList window

class CNumericList : public CListBox
{
// Construction
public:
	CNumericList();

// Attributes
public:

// Operations
public:
	int AddString(int i);
	int FindString(int start, int i);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericList)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNumericList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNumericList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// PName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaperNameData window

class CPaperNameData : public CPaperData
{
// Construction
public:
	CPaperNameData();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaperNameData)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaperNameData();

	// Generated message map functions
protected:
	virtual void recompute_widths();
	virtual void DrawLine(CDC *, LPPaperInfo, LPDRAWITEMSTRUCT);
	//{{AFX_MSG(CPaperNameData)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

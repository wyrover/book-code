// Papers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPapers window

class CPapers : public CPaperData
{
// Construction
public:
	CPapers();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPapers)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPapers();

	// Generated message map functions
protected:
	virtual void recompute_widths();
	virtual void DrawLine(CDC *, LPPaperInfo, LPDRAWITEMSTRUCT);
	//{{AFX_MSG(CPapers)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

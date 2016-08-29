// PSizes.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaperSizes window

class CPaperSizes : public CPaperData
{
// Construction
public:
	CPaperSizes();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaperSizes)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaperSizes();

	// Generated message map functions
protected:
	virtual void recompute_widths();
	virtual void DrawLine(CDC *, LPPaperInfo, LPDRAWITEMSTRUCT);
	//{{AFX_MSG(CPaperSizes)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

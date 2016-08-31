// Poly.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPoly window

class CPoly : public CStatic
{
// Construction
public:
	CPoly();
	int fillmode;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPoly)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPoly();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPoly)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

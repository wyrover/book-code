// MatText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatrixText window

class CMatrixText : public CMatrixSample
{
// Construction
public:
	CMatrixText();

// Attributes
public:
	int align;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixText)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMatrixText();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixText)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

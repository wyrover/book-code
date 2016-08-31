// MatGraf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatrixGraphic window

class CMatrixGraphic : public CMatrixSample
{
// Construction
public:
	CMatrixGraphic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixGraphic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMatrixGraphic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixGraphic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

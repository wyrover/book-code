// MatSamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatrixSample window

class CMatrixSample : public CStatic
{
// Construction
public:
	CMatrixSample();

// Attributes
public:
	XFORM matrix;
	DWORD error;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMatrixSample();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixSample)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

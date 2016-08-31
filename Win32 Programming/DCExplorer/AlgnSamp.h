// AlgnSamp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlignSample window

class CAlignSample : public CStatic
{
// Construction
public:
	CAlignSample();
	int align;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlignSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlignSample();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAlignSample)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// RecEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecreatableEdit window

class CRecreatableEdit : public CEdit
{
// Construction
public:
	CRecreatableEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecreatableEdit)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRecreatableEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRecreatableEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

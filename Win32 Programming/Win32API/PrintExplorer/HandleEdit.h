// HandleEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHandleEdit window

class CHandleEdit : public CEdit
{
// Construction
public:
	CHandleEdit();
	void Blank();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandleEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHandleEdit();
	void SetWindowText(HANDLE h);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHandleEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTextHandleEdit window

class CTextHandleEdit : public CHandleEdit
{
// Construction
public:
	CTextHandleEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandleEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextHandleEdit();
	void SetWindowText(HANDLE h);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextHandleEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// NumericEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NumericEdit window

class CNumericEdit : public CEdit
{
// Construction
public:
	CNumericEdit();
	CNumericEdit(LPCTSTR fmt);
	void SetWindowText(int val, LPCTSTR fmt = NULL);
	void SetWindowText(LPCTSTR val) { CEdit::SetWindowText(val); }
	int GetWindowInt();
	void Blank();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNumericEdit();

	// Generated message map functions
protected:
	LPCTSTR deffmt;

	//{{AFX_MSG(CNumericEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

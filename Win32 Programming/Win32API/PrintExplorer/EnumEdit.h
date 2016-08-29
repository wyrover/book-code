// EnumEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EnumEdit window

typedef struct {
	int val;
	int id;
	       } ENUMEDIT, * LPENUMEDIT;

class CEnumEdit : public CEdit
{
// Construction
public:
	CEnumEdit();
	CEnumEdit(LPENUMEDIT fmt);
	void SetWindowText(int val, LPENUMEDIT fmt = NULL);
	GetWindowInt(LPENUMEDIT fmt = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnumEdit();
	void Blank();

	// Generated message map functions
protected:
	LPENUMEDIT deffmt;

	//{{AFX_MSG(CEnumEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

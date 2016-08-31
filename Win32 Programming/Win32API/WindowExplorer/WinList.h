// WinList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWindowList window

class CWindowList : public CListCtrl
{
// Construction
public:
	CWindowList();
	void initialize(int n);
	HWND GetCurItem();
	void Add(HWND hwnd);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWindowList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWindowList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

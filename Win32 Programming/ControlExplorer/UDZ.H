// UDZ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZ window

class CZ : public CStatic
{
// Construction
public:
	CZ();
	CZ& operator=(const CZ& src);
	
// Attributes
public:
	//-----------------------------------------------------------
	// These are copied by the assignment operator

	CWnd * wnd;
	CWnd * cap;
	CString caption;
	CRect rect;

	//-----------------------------------------------------------
// Operations
public:
	void setZ(CWnd * wnd, CWnd * caption);
	void enable(HWND desired);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZ)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZ();

	// Generated message map functions
protected:
	//{{AFX_MSG(CZ)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

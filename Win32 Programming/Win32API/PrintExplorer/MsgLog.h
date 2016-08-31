// MessageLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageLog window

class CMessageLog : public CListCtrl
{
// Construction
public:
	CMessageLog();
	void initialize(int n);
	int addMessage(HANDLE hwnd, CString type, CString event);
	BOOL setWrap(BOOL val) { BOOL old = wrap; wrap = val; return old;}
	BOOL getWrap() {return wrap; }
	BOOL setAutoExtend(BOOL val) { BOOL old = autoextend; autoextend = val;
						return old;}
        BOOL getAutoExtend() {return autoextend; }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageLog)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMessageLog();

	// Generated message map functions
protected:
	CEdit * edit;
	BOOL wrap;
	BOOL autoextend;
	CEdit * getEditControl(int width);
	//{{AFX_MSG(CMessageLog)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// HexDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHexDisplay window

class CHexDisplay : public CEdit
{
// Construction
public:
	CHexDisplay();

// Attributes
public:

// Operations
public:
	void SetWindowText(LPCTSTR s);
	void SetWindowTextAscii(LPCTSTR s);
	void SetWindowTextHex(LPCTSTR s);
	int  GetWindowText(LPTSTR s, int count);
	void GetWindowText(CString &s);
	BOOL toHex();
	BOOL toAscii();
	BOOL IsHex();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexDisplay)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexDisplay();

	// Generated message map functions
protected:
	CString makeHex(LPCTSTR s);
	//{{AFX_MSG(CHexDisplay)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

// SysMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemMessage window

class CSystemMessage : public CMessage
{
// Construction
public:
	CSystemMessage();

// Attributes
public:

// Operations
public:
	void SetWindowText(DWORD error);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemMessage)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSystemMessage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSystemMessage)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

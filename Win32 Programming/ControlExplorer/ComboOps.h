// comboops.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboOps dialog

class CComboOps : public CLoggingPage
{
	DECLARE_DYNCREATE(CComboOps)

// Construction
public:
	CComboOps();
	CComboOps(int idd);
	~CComboOps();
	CComboBox * combo;

// Dialog Data
	//{{AFX_DATA(CComboOps)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CComboOps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void addMessage(CString s);
	// Generated message map functions
	//{{AFX_MSG(CComboOps)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

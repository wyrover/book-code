// listops.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListOps dialog

class CListOps : public CLoggingPage
{
	DECLARE_DYNCREATE(CListOps)

// Construction
public:
	CListOps();
	CListOps(int idd);
	~CListOps();
	static int selection;
	CListBox * c_Single;
	CListBox * c_Multiple;
	CListBox * c_Extended;
	CListBox * c_None;

	CButton	c_SendSingle;
	CButton	c_SendNone;
	CButton	c_SendMultiple;
	CButton	c_SendExtended;

// Dialog Data
	//{{AFX_DATA(CListOps)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListOps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListBox * getListBox();
	CString getListBoxName();
	virtual void addMessage(CString s);
	virtual BOOL OnSetActive();
	// Generated message map functions
	//{{AFX_MSG(CListOps)
	afx_msg void OnSendSingleClicked();
	afx_msg void OnSendMultipleClicked();
	afx_msg void OnSendExtendedClicked();
	afx_msg void OnSendNoneClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

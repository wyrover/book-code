// MessageTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageTable dialog

class CMessageTable : public CDialog
{
// Construction
public:
	CMessageTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageTable)
	enum { IDD = IDD_MSGTABLE };
	CStatic	c_Icon;
	CStatic	c_MsgID;
	CStatic	c_Msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void makeMessage();
	DWORD lang;
	DWORD msgid;
	int icon;
	// Generated message map functions
	//{{AFX_MSG(CMessageTable)
	afx_msg void OnEnglish();
	afx_msg void OnFrench();
	afx_msg void OnGerman();
	afx_msg void OnGoodbye();
	afx_msg void OnHello();
	afx_msg void OnCat();
	afx_msg void OnFish();
	afx_msg void OnSpanish();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

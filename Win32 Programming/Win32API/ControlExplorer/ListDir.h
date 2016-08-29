// ListDir.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListDir dialog

class CListDir : public CListOps
{
	DECLARE_DYNCREATE(CListDir)

// Construction
public:
	CListDir();
	~CListDir();

// Dialog Data
	//{{AFX_DATA(CListDir)
	enum { IDD = IDD_LISTBOXDIR };
	CTinyButton	c_GetShortPathName;
	CEdit	c_Filename;
	CTinyButton	c_Dir;
	CButton	c_System;
	CButton	c_ReadWrite;
	CButton	c_ReadOnly;
	CButton	c_Hidden;
	CButton	c_Exclusive;
	CButton	c_Drives;
	CButton	c_Directory;
	CButton	c_Archive;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListDir)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int getFileAttr();

	// Generated message map functions
	//{{AFX_MSG(CListDir)
	virtual BOOL OnInitDialog();
	afx_msg void OnDir();
	afx_msg void OnGetshortpathname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

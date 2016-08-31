// ListStr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListStrings dialog

class CListStrings : public CListOps
{
	DECLARE_DYNCREATE(CListStrings)

// Construction
public:
	CListStrings();
	~CListStrings();

// Dialog Data
	//{{AFX_DATA(CListStrings)
	enum { IDD = IDD_LISTBOXSTRING };
	CSpinButtonCtrl	c_SpinFindStart;
	CIntEdit	c_FindStart;
	CTinyButton	c_GetTextLength;
	CTinyButton	c_GetText;
	CTinyButton	c_GetSel;
	CTinyButton	c_GetItemHeight;
	CTinyButton	c_GetItemData;
	CTinyButton	c_FindStringExact;
	CTinyButton	c_FindString;
	CTinyButton	c_DeleteString;
	CTinyButton	c_AddString;
	CEdit	c_String;
	CSpinButtonCtrl	c_SpinIndex;
	CTinyButton	c_SetSel;
	CTinyButton	c_SetItemData;
	CTinyButton	c_SetCurSel;
	CTinyButton	c_SelectString;
	CIntEdit	c_Index;
	CButton	c_SetSelVal;
	CIntEdit	c_ItemDataVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListStrings)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void doFind(int msg, int id);

	// Generated message map functions
	//{{AFX_MSG(CListStrings)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddstring();
	afx_msg void OnDeletestring();
	afx_msg void OnFindstring();
	afx_msg void OnFindstringexact();
	afx_msg void OnGetitemdata();
	afx_msg void OnGetitemheight();
	afx_msg void OnGetsel();
	afx_msg void OnGettext();
	afx_msg void OnGettextlength();
	afx_msg void OnSelectstring();
	afx_msg void OnSetcursel();
	afx_msg void OnSetitemdata();
	afx_msg void OnSetsel();
	afx_msg void OnInsertstring();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

// DDText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDText dialog

class CDDText : public CComboOps
{
	DECLARE_DYNCREATE(CDDText)

// Construction
public:
	CDDText();
	~CDDText();

// Dialog Data
	//{{AFX_DATA(CDDText)
	enum { IDD = IDD_DDTEXT };
	CEdit	c_String;
	CIntEdit	c_ItemData;
	CTinyButton	c_AddString;
	CTinyButton	c_InsertString;
	CTinyButton	c_GetLBTextLength;
	CTinyButton	c_GetLBText;
	CTinyButton	c_GetItemHeight;
	CTinyButton	c_GetItemData;
	CTinyButton	c_GetEditSel;
	CTinyButton	c_FindStringExact;
	CTinyButton	c_FindString;
	CSpinButtonCtrl	c_SpinSelStart;
	CSpinButtonCtrl	c_SpinSelEnd;
	CSpinButtonCtrl	c_SpinSelection;
	CSpinButtonCtrl	c_SpinItemHeight;
	CTinyButton	c_SetItemHeight;
	CTinyButton	c_SetItemData;
	CTinyButton	c_SetEditSel;
	CIntEdit	c_SelStart;
	CIntEdit	c_SelEnd;
	CIntEdit	c_Selection;
	CIntEdit	c_ItemHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDDText)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void updateControls();

	// Generated message map functions
	//{{AFX_MSG(CDDText)
	virtual BOOL OnInitDialog();
	afx_msg void OnFindstring();
	afx_msg void OnFindstringexact();
	afx_msg void OnGeteditsel();
	afx_msg void OnGetitemdata();
	afx_msg void OnGetitemheight();
	afx_msg void OnGetlbtext();
	afx_msg void OnGetlbtextlength();
	afx_msg void OnInsertstring();
	afx_msg void OnSeteditsel();
	afx_msg void OnSetitemdata();
	afx_msg void OnSetitemheight();
	afx_msg void OnAddstring();
	afx_msg void OnDeletestring();
	afx_msg void OnGetwindowtext();
	afx_msg void OnSelectstring();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

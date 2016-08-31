// TreeIns.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsertItem dialog

class CInsertItem : public CTV_ITEM
{
	DECLARE_DYNCREATE(CInsertItem)

// Construction
public:
	CInsertItem();
	~CInsertItem();

	static CString formatInsertItem(CTreeCtrl * c_Tree, TV_INSERTSTRUCT * is);

// Dialog Data
	//{{AFX_DATA(CInsertItem)
	enum { IDD = IDD_TREEINSERT };
	CComboBox	c_hParent;
	CComboBox	c_hInsertAfter;
	CButton	c_TVI_SORT;
	CButton	c_TVI_LAST;
	CButton	c_TVI_FIRST;
	CButton	c_InsertHandle;
	CTinyButton c_InsertItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInsertItem)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void enableControls();


// Implementation
protected:
	void loadParent();

	// Generated message map functions
	//{{AFX_MSG(CInsertItem)
	afx_msg void OnTviFirst();
	afx_msg void OnTviLast();
	afx_msg void OnTviSort();
	virtual BOOL OnInitDialog();
	afx_msg void OnInserthandle();
	afx_msg void OnInsertitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

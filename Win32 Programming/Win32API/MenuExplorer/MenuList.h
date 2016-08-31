// MenuList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuList window

class CMenuList : public CTreeCtrl
{
// Construction
public:
	CMenuList();
	BOOL SetItemData(HTREEITEM ti, menuitem * mi);
	menuitem * GetItemData(HTREEITEM ti);
	HTREEITEM findItem(HMENU menu, HTREEITEM parent = NULL); 
	menuitem * findMenuItem(HMENU menu, HTREEITEM parent = NULL);
	HTREEITEM InsertItem(menuitem * mi, HTREEITEM parent);
	HTREEITEM InsertItem(CString s, HTREEITEM parent) { return CTreeCtrl::InsertItem(s, parent); }
	void deleteChildren(HTREEITEM parent);
	void insertMenuItems(CMenu * menu, HTREEITEM root);
	int getItemPos(HTREEITEM ti);
// Attributes
public:
	BOOL autoresize;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuList();

	// Generated message map functions
protected:
	CSize newsize;
	CSize oldsize;
	BOOL initialized;
	BOOL zoomed;
	HWND predecessor;

	//{{AFX_MSG(CMenuList)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

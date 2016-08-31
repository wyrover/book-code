// Tree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTree dialog

class CTree : public CPropertyPage
{
	DECLARE_DYNCREATE(CTree)

// Construction
public:
	CTree();
	~CTree();

// Dialog Data
	//{{AFX_DATA(CTree)
	enum { IDD = IDD_TREE };
	CButton	c_LabelEdit;
	CTinyButton	c_QuickImageList;
	CButton	c_Down;
	CButton	c_Up;
	CButton	c_SmartDragDrop;
	CButton	c_Callbacks;
	CButton	c_UseImages;
	CButton	c_DragDrop;
	CButton	c_NoSelect;
	CButton	c_NoExpand;
	CButton	c_Resize;
	CButton	c_WS_Border;
	CButton	c_TVS_ShowSelAlways;
	CButton	c_TVS_HasLinesAtRoot;
	CButton	c_TVS_HasLines;
	CButton	c_TVS_HasButtons;
	CButton	c_TVS_EditLabels;
	CButton	c_TVS_DisableDragDrop;
	CResizableTree	c_Tree;
	CMessageLog	c_Messages;
	CTinyButton	c_Demo1;
	CTinyButton	c_Copy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTree)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

// Implementation
protected:
	int inGetDispInfo;  // TVN_GETDISPINFO recursion counter

	CList<TV_ITEM *, TV_ITEM *> cache;
	void logSetWindowLong(DWORD style);
	CString styleToString(DWORD style);
	void addStyle(CString & stylestr, DWORD styles, DWORD style, int styleid);

	BOOL retrieveFromCache(TV_ITEM * item);
	void addToCache(TV_ITEM * item);

	void OnBotanyExpanded(NM_TREEVIEW * pNMHDR);
	CImageList statelist;
	CImageList imagelist;
	CImageList pixel1;
	void setStyle(BOOL set, DWORD style);
	void showResult_N_s_x(int msg, CString l, int result, int id = IDS_DRAGDROP);
	void showResult_N_x_x(int msg, DWORD l, DWORD result);
	void showResult_N_N_N(int msg);
	void showResult_d_s_b(int msg, int w, CString l, BOOL result);
	void showResult_d_d_b(int msg, int w, int l, BOOL result);
	void showResult_N_hti_h(int msg, HTREEITEM result);
	void showResult_N_s_d(int msg, CString l, int result, int id = IDS_DRAGDROP);
	void showResult_N_s_s(int msg, CString l, CString result);
	void showResult_b_R_b(int msg, BOOL w, LPRECT r, BOOL result);
	void logShowCursor(BOOL arg);
	void logDragEnter(HWND hwnd, int x, int y);
	void logDragLeave(HWND hwnd);
	void logBeginDrag(HIMAGELIST list, int index, int x, int y, BOOL result);
	void logEndDrag();
	void logKillTimer(int timerid);
	void logautoScroll(int dir, int y, LPCTSTR compare, int zid, int limit);
	void logImageListDestroy(HIMAGELIST lsit);
	void showResult_s_s_x(int msg, CString w, CString l, DWORD result);
	void logCapture(int id);
	void logNMHDR(int msg, LPNMHDR hdr, CString ext, CString * result = NULL);
	void logNMHDR_b(int msg, LPNMHDR hdr, CString ext, BOOL result);
	void logLoadImage(HINSTANCE hinst, LPCTSTR resid, int cx, int grow, COLORREF mask, UINT type, UINT flags, HIMAGELIST result);

	CString getStateShort(UINT state);
	CString itemString(LPTV_ITEM item, UINT mask);

	void enableControls();

	treedata * insertDemo(treedata * data, HTREEITEM parent, int depth);

	// The following support the drag-and-drop
	HIMAGELIST DragImage;
	BOOL isDragging;      // TRUE if we are dragging an image
	HTREEITEM DragItem;   // handle of item being dragged
	BOOL dragCopy;	      // FALSE to move, TRUE to copy
	void copyChildren(HTREEITEM to, HTREEITEM from);
	void autoScroll(int SBcode, int timecode);
	BOOL isParent(HTREEITEM target, HTREEITEM dragee);
	BOOL isDownSibling(HTREEITEM source, HTREEITEM target);
	BOOL isUpSibling(HTREEITEM source, HTREEITEM target);
	int  treeLevel(HTREEITEM item);
	int  timer;
	int  ScrollDirection;
	static void CALLBACK timerProc(HWND hwnd, UINT, UINT, DWORD);
	static int UWM_SCROLL;
	void checkAutoScroll(int x, int y);

	afx_msg LRESULT OnRecordDispInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnScroll(WPARAM, LPARAM);
	// Generated message map functions
	//{{AFX_MSG(CTree)
	afx_msg void OnCopy();
	afx_msg void OnSendmessages();
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvsDisabledragdrop();
	afx_msg void OnTvsEditlabels();
	afx_msg void OnTvsHasbuttons();
	afx_msg void OnTvsHaslines();
	afx_msg void OnTvsHaslinesatroot();
	afx_msg void OnTvsShowselalways();
	afx_msg void OnWsBorder();
	virtual BOOL OnInitDialog();
	afx_msg void OnDemo1();
	afx_msg void OnResize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUseimages();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDragdrop();
	afx_msg void OnDown();
	afx_msg void OnUp();
	afx_msg void OnImagelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

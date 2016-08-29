// ListStat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListState dialog

class CListState : public CListOps
{
	DECLARE_DYNCREATE(CListState)

// Construction
public:
	CListState();
	~CListState();

// Dialog Data
	//{{AFX_DATA(CListState)
	enum { IDD = IDD_LISTBOXSTATE };
	CIntEdit	c_HorizontalExtent;
	CButton	c_SelItemRangeFlag;
	CSpinButtonCtrl	c_SpinRangeLow;
	CSpinButtonCtrl	c_SpinRangeHigh;
	CSpinButtonCtrl	c_SpinItemHeight;
	CSpinButtonCtrl	c_SpinIndex;
	CSpinButtonCtrl	c_SpinColumnWidth;
	CIntEdit	c_Index;
	CLocales	c_Locales;
	CIntEdit	c_ItemHeight;
	CIntEdit	c_ColumnWidth;
	CTinyButton	c_ResetContent;
	CIntEdit	c_RangeLow;
	CIntEdit	c_RangeHigh;
	CButton	c_RedrawVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListState)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CListState)
	virtual BOOL OnInitDialog();
	afx_msg void OnResetcontent();
	afx_msg void OnSelitemrange();
	afx_msg void OnSelitemrangeex();
	afx_msg void OnSetanchorindex();
	afx_msg void OnSetcaretindex();
	afx_msg void OnSetcolumnwidth();
	afx_msg void OnSetitemheight();
	afx_msg void OnSetlocale();
	afx_msg void OnSetredraw();
	afx_msg void OnSettopindex();
	afx_msg void OnSetcursel();
	afx_msg void OnSethorizontalextent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

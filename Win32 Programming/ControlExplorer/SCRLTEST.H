// scrltest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollBarTest dialog

class CScrollBarTest : public CPropertyPage
{
	DECLARE_DYNCREATE(CScrollBarTest)

// Construction
public:
	CScrollBarTest();
	~CScrollBarTest();

// Dialog Data
	//{{AFX_DATA(CScrollBarTest)
	enum { IDD = IDD_SCROLLBAR };
	CIntEdit	c_Page;
	CSpinButtonCtrl	c_SpinPage;
	CSpinButtonCtrl	c_SpinPos;
	CSpinButtonCtrl	c_SpinLow;
	CSpinButtonCtrl	c_SpinHigh;
	CIntEdit	c_Pos;
	CIntEdit	c_Low;
	CIntEdit	c_High;
	CButton	c_FullSize;
	CButton	c_SBS_SizeBoxTopLeftAlign;
	CButton	c_SBS_SizeBoxBottomRightAlign;
	CMessageLog	c_Messages;
	CButton	c_None;
	CButton	c_SBS_Vert;
	CButton	c_SBS_TopAlign;
	CButton	c_SBS_SizeBox;
	CButton	c_SBS_SizeGrip;
	CButton	c_SBS_RightAlign;
	CButton	c_SBS_LeftAlign;
	CButton	c_SBS_Horz;
	CButton	c_SBS_BottomAlign;
	CButton	c_Create;
	CScrollParent	c_ScrollSpace;
	//}}AFX_DATA

	CScrollBar c_ScrollBar;
	DWORD style;  // style for next create operation

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScrollBarTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL initialized;
	BOOL inchange;

	void moveScrollBar(UINT nSBCode, UINT pos, CScrollBar * pScrollBar);
	void enableAlignment(BOOL horz, BOOL vert, BOOL size);
	void enableControls();
	void styleToAlignment();
	void enableParameters(BOOL enable);
	void controlsToStyle();
	void enableFullSize();
	void setScrollStyle();
	void setAlignmentStyle();

	CString getCtlName();
	// Generated message map functions
	//{{AFX_MSG(CScrollBarTest)
	afx_msg void OnCreate();
	afx_msg void OnSbsBottomalign();
	afx_msg void OnSbsHorz();
	afx_msg void OnSbsNone();
	afx_msg void OnSbsLeftalign();
	afx_msg void OnSbsRightalign();
	afx_msg void OnSbsSizebox();
	afx_msg void OnSbsSizegrip();
	afx_msg void OnSbsTopalign();
	afx_msg void OnSbsVert();
	afx_msg void OnSbsSizeboxtopleftalign();
	afx_msg void OnSbsSizeboxbottomrightalign();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLow();
	afx_msg void OnChangeHigh();
	afx_msg void OnChangePos();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnSimulateSize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangePage();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

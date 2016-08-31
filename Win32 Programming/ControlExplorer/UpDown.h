// UpDown.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpDown dialog

class CUpDown : public CPropertyPage
{
	DECLARE_DYNCREATE(CUpDown)

// Construction
public:
	CUpDown();
	~CUpDown();

// Dialog Data
	//{{AFX_DATA(CUpDown)
	enum { IDD = IDD_UPDOWN };
	CButton	c_WS_TabStop;
	CStatic	c_Copy3Rect;
	CButton	c_WS_Disabled;
	CStatic	c_Copy2Rect;
	CButton	c_NoAnimation;
	CButton	c_UDS_AlignLeft;
	CButton	c_UDS_AlignRight;
	CButton	c_UDS_ArrowKeys;
	CButton	c_UDS_AutoBuddy;
	CButton	c_UDS_Horz;
	CButton	c_UDS_NoThousands;
	CButton	c_UDS_SetBuddyInt;
	CButton	c_UDS_Wrap;
	CListBox	c_BuddyD;
	CMessageLog	c_Messages;
	CNumericEdit	c_BuddyA;
	CNumericEdit	c_BuddyB;
	CNumericEdit	c_High;
	CNumericEdit	c_Low;
	CNumericEdit	c_Value;
	CSpinButtonCtrl	c_UpDown;
	CStatic	c_BuddyC;
	CStatic	c_c_BuddyA;
	CStatic	c_c_BuddyB;
	CStatic	c_c_BuddyC;
	CStatic	c_c_BuddyD;
	CZ		c_Z1;
	CZ		c_Z2;
	CZ		c_Z3;
	CZ		c_Z4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUpDown)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect udrect;

	void buildZOrder();
	void enableBuddies();
	void setStyle(BOOL set, DWORD flag);
	DWORD getStyles();
	void recreate();
	CArray<CZ *, CZ *> czarray;

	void onScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar, int scrollid);
	void onScrollCtl(UINT nSBCode, UINT nPos, CSpinButtonCtrl * pScrollBar, int scrollid);

	// Generated message map functions
	//{{AFX_MSG(CUpDown)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeBuddyA();
	afx_msg void OnChangeBuddyB();
	afx_msg void OnChangeHigh();
	afx_msg void OnChangeLow();
	afx_msg void OnSwap();
	afx_msg void OnUdsAlignleft();
	afx_msg void OnUdsAlignright();
	afx_msg void OnUdsArrowkeys();
	afx_msg void OnUdsAutobuddy();
	afx_msg void OnUdsHorz();
	afx_msg void OnUdsNothousands();
	afx_msg void OnUdsSetbuddyint();
	afx_msg void OnUdsWrap();
	afx_msg void OnDeltaposUpdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSendmessages();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnZshift();
	afx_msg void OnCopy1();
	afx_msg void OnCopy2();
	afx_msg void OnDestroy();
	afx_msg void OnWSDisabled();
	afx_msg void OnCopy3();
	afx_msg void OnWsTabstop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

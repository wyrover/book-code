// TrackBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackBar dialog

class CTrackBar : public CPropertyPage
{
	DECLARE_DYNCREATE(CTrackBar)

// Construction
public:
	CTrackBar();
	~CTrackBar();

// Dialog Data
	//{{AFX_DATA(CTrackBar)
	enum { IDD = IDD_TRACKBAR };
	CTinyButton	c_InvalidateRect;
	CButton	c_AutoInvalidate;
	CTinyButton	c_Copy;
	CButton	c_WS_Disabled;
	CButton	c_WS_Border;
	CStatic	c_c_WH;
	CNumericEdit	c_WH;
	CSpinButtonCtrl	c_SpinWH;
	CSpinButtonCtrl	c_SpinPos;
	CSpinButtonCtrl	c_SpinLow;
	CSpinButtonCtrl	c_SpinHigh;
	CNumericEdit	c_Pos;
	CNumericEdit	c_High;
	CNumericEdit	c_Low;
	CStatic	c_WorkArea;
	CButton	c_TBS_Vert;
	CButton	c_TBS_Top;
	CButton	c_TBS_Right;
	CButton	c_TBS_NoTicks;
	CButton	c_TBS_NoThumb;
	CButton	c_TBS_Left;
	CButton	c_TBS_Horz;
	CButton	c_TBS_FixedLength;
	CButton	c_TBS_EnableSelRange;
	CButton	c_TBS_Bottom;
	CButton	c_TBS_Both;
	CButton	c_TBS_AutoTicks;
	CButton	c_NoAnimation;
	CMessageLog	c_Messages;
	//}}AFX_DATA
	CSliderCtrl c_Slider;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void setStyles();
	void createControl();
	DWORD getStyle();
	void enableControls();
	BOOL initialized;
	void onScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar, int scrollid);

	// Used to preserve position over the re-creation of the window
	int low;
	int high;
	int pos;
	int page;
	int line;
	BOOL recursive;  // avoid recursive calls on change handler

	// Generated message map functions
	//{{AFX_MSG(CTrackBar)
	afx_msg void OnInvalidaterect();
	afx_msg void OnNoAnimation();
	afx_msg void OnSendmessages();
	afx_msg void OnTbsAutoticks();
	afx_msg void OnTbsBoth();
	afx_msg void OnTbsBottom();
	afx_msg void OnTbsEnableselrange();
	afx_msg void OnTbsFixedlength();
	afx_msg void OnTbsHorz();
	afx_msg void OnTbsLeft();
	afx_msg void OnTbsNothumb();
	afx_msg void OnTbsNoticks();
	afx_msg void OnTbsRight();
	afx_msg void OnTbsTop();
	afx_msg void OnTbsVert();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnChangeHigh();
	afx_msg void OnChangeLow();
	afx_msg void OnChangePos();
	afx_msg void OnChangeWh();
	afx_msg void OnWsBorder();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnWsDisabled();
	afx_msg void OnCopy();
	afx_msg void OnLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

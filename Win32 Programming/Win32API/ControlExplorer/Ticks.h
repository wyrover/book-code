// Ticks.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTicks dialog

class CTicks : public CTrackOps
{
	DECLARE_DYNCREATE(CTicks)

// Construction
public:
	CTicks();
	~CTicks();

// Dialog Data
	//{{AFX_DATA(CTicks)
	enum { IDD = IDD_TRACKBAR_TICKS };
	CButton	c_Redraw;
	CListBox	c_Ticks;
	CNumericEdit	c_NumTicks;
	CNumericEdit	c_Pos;
	CNumericEdit	c_Tick;
	CNumericEdit	c_TickArray;
	CNumericEdit	c_TickFreq;
	CNumericEdit	c_TickIndex;
	CNumericEdit	c_TickPosX;
	CNumericEdit	c_TickPosY;
	CSpinButtonCtrl	c_SpinTick;
	CSpinButtonCtrl	c_SpinTickFreq;
	CSpinButtonCtrl	c_SpinTickIndex;
	CSpinButtonCtrl	c_SpinTickPos;
	CStatic	c_c_Freq;
	CStatic	c_c_Pos;
	CTinyButton	c_ClearTicks;
	CTinyButton	c_GetNumTicks;
	CTinyButton	c_GetPTicks;
	CTinyButton	c_GetTick;
	CTinyButton	c_GetTickPos;
	CTinyButton	c_SetTick;
	CTinyButton	c_SetTickFreq;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTicks)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableControls();

	// Generated message map functions
	//{{AFX_MSG(CTicks)
	afx_msg void OnCleartics();
	afx_msg void OnGetnumtics();
	afx_msg void OnGetptics();
	afx_msg void OnGettic();
	afx_msg void OnGetticpos();
	afx_msg void OnSettic();
	afx_msg void OnSetticfreq();
	afx_msg void OnDblclkTicks();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

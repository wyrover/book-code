// sdemo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSliderDemo dialog

class CSliderDemo : public CDialog
{
// Construction
public:
	CSliderDemo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSliderDemo)
	enum { IDD = IDD_SLIDERS };
	CDrawing	c_Picture;
	CSpinButtonCtrl	c_SpinValue;
	CSliderCtrl	c_YSlider;
	CSliderCtrl	c_XSlider;
	CScrollBar	c_VScroll;
	CNumericEdit	c_Value;
	CProgressCtrl	c_Progress;
	CScrollBar	c_HScroll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSliderDemo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void setScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	int timer;
	BOOL initialized;

	int progress;
	int delta;
	int limit;

	
	// Generated message map functions
	//{{AFX_MSG(CSliderDemo)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeValue();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnQueryXOffset(WPARAM, LPARAM);
	afx_msg LRESULT OnQueryYOffset(WPARAM, LPARAM);
	afx_msg LRESULT OnQuerySize(WPARAM, LPARAM);
	afx_msg LRESULT OnQueryPenSize(WPARAM, LPARAM rp);
	DECLARE_MESSAGE_MAP()
};

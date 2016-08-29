// TrackPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackPos dialog

class CTrackPos : public CTrackOps
{
	DECLARE_DYNCREATE(CTrackPos)

// Construction
public:
	CTrackPos();
	~CTrackPos();

// Dialog Data
	//{{AFX_DATA(CTrackPos)
	enum { IDD = IDD_TRACKBAR_POS };
	CButton	c_Move;
	CButton	c_Redraw;
	CNumericEdit	c_LineSize;
	CNumericEdit	c_PageSize;
	CNumericEdit	c_Pos;
	CNumericEdit	c_RangeMax;
	CNumericEdit	c_RangeMin;
	CSpinButtonCtrl	c_SpinLineSize;
	CSpinButtonCtrl	c_SpinPageSize;
	CSpinButtonCtrl	c_SpinPos;
	CSpinButtonCtrl	c_SpinRangeMax;
	CSpinButtonCtrl	c_SpinRangeMin;
	CTinyButton	c_GetLineSize;
	CTinyButton	c_GetPageSize;
	CTinyButton	c_GetPos;
	CTinyButton	c_GetRangeMax;
	CTinyButton	c_GetRangeMin;
	CTinyButton	c_SetLineSize;
	CTinyButton	c_SetPageSize;
	CTinyButton	c_SetPos;
	CTinyButton	c_SetRange;
	CTinyButton	c_SetRangeMax;
	CTinyButton	c_SetRangeMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackPos)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void enableSetRange();

	// Generated message map functions
	//{{AFX_MSG(CTrackPos)
	afx_msg void OnGetlinesize();
	afx_msg void OnGetpagesize();
	afx_msg void OnGetpos();
	afx_msg void OnGetrangemax();
	afx_msg void OnGetrangemin();
	afx_msg void OnSetlinesize();
	afx_msg void OnSetpagesize();
	afx_msg void OnSetpos();
	afx_msg void OnSetrangemax();
	afx_msg void OnSetrangemin();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetrange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

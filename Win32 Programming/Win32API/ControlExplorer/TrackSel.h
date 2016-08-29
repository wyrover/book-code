// TrackSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackSelect dialog

class CTrackSelect : public CTrackOps
{
	DECLARE_DYNCREATE(CTrackSelect)

// Construction
public:
	CTrackSelect();
	~CTrackSelect();

// Dialog Data
	//{{AFX_DATA(CTrackSelect)
	enum { IDD = IDD_TRACKBAR_SEL };
	CTinyButton	c_SetThumbLength;
	CTinyButton	c_SetSelStart;
	CTinyButton	c_SetSelEnd;
	CTinyButton	c_SetSel;
	CTinyButton	c_GetThumbLength;
	CTinyButton	c_GetSelStart;
	CTinyButton	c_GetSelEnd;
	CTinyButton	c_ClearSel;
	CNumericEdit	c_SelStart;
	CNumericEdit	c_SelEnd;
	CNumericEdit	c_ThumbLength;
	CButton	c_Redraw;
	CSpinButtonCtrl	c_SpinSelEnd;
	CSpinButtonCtrl	c_SpinSelStart;
	CSpinButtonCtrl	c_SpinThumbLength;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrackSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrackSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearsel();
	afx_msg void OnGetselend();
	afx_msg void OnGetselstart();
	afx_msg void OnGetthumblength();
	afx_msg void OnSetsel();
	afx_msg void OnSetselend();
	afx_msg void OnSetselstart();
	afx_msg void OnSetthumblength();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

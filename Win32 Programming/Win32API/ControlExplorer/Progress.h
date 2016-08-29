// Progress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class CProgress : public CLoggingPage
{
	DECLARE_DYNCREATE(CProgress)

// Construction
public:
	CProgress();
	~CProgress();

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CSpinButtonCtrl	c_SpinPos;
	CSpinButtonCtrl	c_SpinStep;
	CSpinButtonCtrl	c_SpinLow;
	CSpinButtonCtrl	c_SpinHigh;
	CSpinButtonCtrl	c_SpinDelta;

	CNumericEdit	c_Step;
	CNumericEdit	c_PosVal;
	CNumericEdit	c_Delta;
	CNumericEdit	c_RangeHigh;
	CNumericEdit	c_RangeLow;

	CTinyButton	c_StepIt;
	CTinyButton	c_DeltaPos;
	CTinyButton	c_SetStep;
	CTinyButton	c_SetPos;
	CTinyButton	c_SetRange;

	CProgressCtrl	c_Progress;
	CMessageLog	c_Messages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL initialized;
	virtual void addMessage(CString s);

	// Generated message map functions
	//{{AFX_MSG(CProgress)
	afx_msg void OnDeltapos();
	afx_msg void OnSetpos();
	afx_msg void OnSetrange();
	afx_msg void OnSetstep();
	afx_msg void OnStepit();
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

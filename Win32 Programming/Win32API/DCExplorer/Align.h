// Align.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlign dialog

class CAlign : public CPropertyPage
{
	DECLARE_DYNCREATE(CAlign)

// Construction
public:
	CAlign();
	~CAlign();
	CDCValues * dcv;

// Dialog Data
	//{{AFX_DATA(CAlign)
	enum { IDD = IDD_ALIGNMENT };
	CAlignSample	c_Sample;
	CButton	c_RTLReading;
	CButton	c_Right;
	CButton	c_Center;
	CButton	c_Bottom;
	CButton	c_BaseLine;
	CButton	c_UpdateCP;
	CButton	c_Top;
	CButton	c_Left;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAlign)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int getAlign();
	void updateSample();

	// Generated message map functions
	//{{AFX_MSG(CAlign)
	virtual BOOL OnInitDialog();

	afx_msg void OnTop();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnCenter();
	afx_msg void OnBottom();
	afx_msg void OnRtlreading();
	afx_msg void OnUpdatecp();
	afx_msg void OnBaseline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

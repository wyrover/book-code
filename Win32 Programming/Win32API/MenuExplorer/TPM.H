// TPM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTPM dialog

class CTPM : public CPropertyPage
{
	DECLARE_DYNCREATE(CTPM)

// Construction
public:
	CTPM();
	~CTPM();

// Dialog Data
	//{{AFX_DATA(CTPM)
	enum { IDD = IDD_TRACKPOPUPMENU };
	CEdit	c_Log;
	CButton	c_VerticalAlignment;
	CButton	c_ReturnCmd;
	CButton	c_NoNotify;
	CButton	c_VCenterAlign;
	CButton	c_TopAlign;
	CButton	c_BottomAlign;
	CButton	c_Vertical;
	CButton	c_RightButton;
	CButton	c_RightAlign;
	CButton	c_LeftButton;
	CButton	c_LeftAlign;
	CButton	c_Horizontal;
	CButton	c_Preference;
	CButton	c_CenterAlign;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTPM)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD getFlags();
	// Generated message map functions
	//{{AFX_MSG(CTPM)
	virtual BOOL OnInitDialog();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnLogTrackPopup(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

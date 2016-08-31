// HdrLayout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHdrLayout dialog

class CHdrLayout : public CLoggingPage
{
	DECLARE_DYNCREATE(CHdrLayout)

// Construction
public:
	CHdrLayout();
	~CHdrLayout();
	CHeaderCtrl * c_Header;

// Dialog Data
	//{{AFX_DATA(CHdrLayout)
	enum { IDD = IDD_HEADERLAYOUT };
	CTinyButton	c_SetWindowPos;
	CStatic	c_c_y;
	CStatic	c_c_x;
	CStatic	c_c_top;
	CStatic	c_c_right;
	CButton	c_c_LPWINDOWPOS;
	CButton	c_c_LPRECT;
	CStatic	c_c_left;
	CButton	c_c_Layout;
	CStatic	c_c_hwndInsertAfter;
	CStatic	c_c_hwnd;
	CStatic	c_c_flags;
	CStatic	c_c_cy;
	CStatic	c_c_cx;
	CStatic	c_c_bottom;
	CNumericEdit	c_y;
	CNumericEdit	c_x;
	CNumericEdit	c_top;
	CNumericEdit	c_right;
	CNumericEdit	c_left;
	CNumericEdit	c_hwndInsertAfter;
	CNumericEdit	c_hwnd;
	CBitCombo	c_flags;
	CNumericEdit	c_cy;
	CNumericEdit	c_cx;
	CNumericEdit	c_bottom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHdrLayout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RECT rc;
	WINDOWPOS pos;
	HD_LAYOUT layout;
	void enableControls(BOOL enable);
	void layoutToControls();
	void clearControls();
	void controlsToLayout();

	// Generated message map functions
	//{{AFX_MSG(CHdrLayout)
	virtual BOOL OnInitDialog();
	afx_msg void OnHdLayout();
	afx_msg void OnSetwindowpos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

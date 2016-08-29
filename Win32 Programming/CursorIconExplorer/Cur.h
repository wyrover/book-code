// Cur.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCur dialog

class CIco : public CStatic
    {
public:
     CIco::CIco();
     HCURSOR cur;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIco)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    };

class CCur : public CPropertyPage
{
	DECLARE_DYNCREATE(CCur)

// Construction
public:
	CCur();
	~CCur();

// Dialog Data
	//{{AFX_DATA(CCur)
	enum { IDD = IDD_CURSORS };
	CButton	c_Copy;
	CButton	c_AutoCopy;
	CIco	c_Icon;
	//}}AFX_DATA
	HCURSOR tcur;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCur)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCur)
	afx_msg void OnCopy();
	virtual BOOL OnInitDialog();
	afx_msg void OnAutocopy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	afx_msg void CurSelect(UINT id);

	DECLARE_MESSAGE_MAP()

};

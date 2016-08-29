// ScParent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollParent window

class CScrollParent : public CWnd
{
// Construction
public:
	CScrollParent();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollParent)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScrollParent();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollParent)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT ForwardFocus(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

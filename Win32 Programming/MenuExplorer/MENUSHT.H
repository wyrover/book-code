// MenuSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuSheet


class CMenuSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMenuSheet)

// Construction
public:
	CMenuSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMenuSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	void retransmit(int msg, WPARAM wParam, LPARAM lParam);

// Attributes
public:
	UINT tpmFlags;
	HMENU trackMenu;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuSheet)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuSheet();

	// Generated message map functions
protected:
	void tpmInit();
	CString tpmstr(DWORD flags);
	//{{AFX_MSG(CMenuSheet)
	afx_msg void OnClose();
	afx_msg LRESULT OnMainCommand(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainInitMenu(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainInitMenuPopup(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainMenuSelect(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainMenuChar(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainEnterMenuLoop(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMainExitMenuLoop(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnLogMsg(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnTrackPopupMenu(WPARAM, LPARAM);
	afx_msg LRESULT OnSetFlags(WPARAM, LPARAM);
	afx_msg LRESULT OnAddOwnerDraw(WPARAM, LPARAM);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

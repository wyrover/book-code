// MenuView.h : interface of the CMenuView class
//
/////////////////////////////////////////////////////////////////////////////
......
class CMenuView : public CView
{
protected: // create from serialization only
	CMenuView();
	DECLARE_DYNCREATE(CMenuView)

	......

// Generated message map functions
protected:
	//{{AFX_MSG(CMenuView)
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

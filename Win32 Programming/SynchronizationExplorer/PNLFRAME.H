// PnlFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelFrame frame

class CPanelFrame : public CChildFrame
{
        DECLARE_DYNCREATE(CPanelFrame)
protected:
        CPanelFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPanelFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
        virtual ~CPanelFrame();

        // Generated message map functions
        //{{AFX_MSG(CPanelFrame)
        afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

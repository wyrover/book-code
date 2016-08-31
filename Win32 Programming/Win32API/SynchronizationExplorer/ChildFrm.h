// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CMDIChildWnd
{
        DECLARE_DYNCREATE(CChildFrame)
public:
        CChildFrame();

// Attributes
public:

// Operations
public:
        void setSize();
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CChildFrame)
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CChildFrame();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
        POINT sizelimit;
        BOOL  sizeknown;  // sizelimit is valid

        //{{AFX_MSG(CChildFrame)
        afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

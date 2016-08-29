// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMenuSheet;
class CMenuExplorerDlg;
class CMenuInfo;
class COD;

class CMainFrame : public CMDIFrameWnd
{
        DECLARE_DYNAMIC(CMainFrame)
public:
        CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMainFrame)
        public:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        protected:
        virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CMainFrame();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
        CStatusBar  m_wndStatusBar;
        CToolBar    m_wndToolBar;

// Generated message map functions
protected:
        CMenuSheet * sheet;
        CMenuExplorerDlg * exp;
        CTPM * tpm;
        CMenuInfo * info;
        COD * od;

        void changeBreakState(int id, UINT flag);

        COwnerDrawMenu odmenu;

        //{{AFX_MSG(CMainFrame)
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnMenuexplorer();
        afx_msg void OnInitMenu(CMenu* pMenu);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
        afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnEnterMenuLoop(BOOL popup);
        afx_msg void OnExitMenuLoop(BOOL popup);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnEnableBar();
        afx_msg void OnUpdateEnableBar(CCmdUI* pCmdUI);
        afx_msg void OnEnableBreak();
        afx_msg void OnUpdateEnableBreak(CCmdUI* pCmdUI);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

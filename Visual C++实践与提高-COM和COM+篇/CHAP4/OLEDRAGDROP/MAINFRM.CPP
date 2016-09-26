// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OleClip.h"

#include "MainFrm.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                               | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
                                      sizeof(indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);
    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CMDIFrameWnd::PreCreateWindow(cs))
        return FALSE;

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnQueryNewPalette()
{
    TRACE("CMainFrame::OnQueryNewPalette\n");
    CClientDC dc(this);

    // don't bother if we're not using a palette
    if ((dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE) == 0) return TRUE;

    // deterimine the active view
    HWND hActiveView = NULL;
    CFrameWnd* pActiveFrm = GetActiveFrame();

    if (pActiveFrm != NULL) {
        CView* pActiveView = pActiveFrm->GetActiveView();

        if (pActiveView != NULL) {
            hActiveView = pActiveView->GetSafeHwnd();
        }
    }

    // iterate through all views
    BOOL bBackground;
    CView* pView;
    CDocument* pDoc;
    CDocTemplate* pTemplate;
    POSITION posView;
    POSITION posDoc;
    POSITION posTemplate = AfxGetApp()->GetFirstDocTemplatePosition();

    while (posTemplate != NULL) {
        pTemplate = AfxGetApp()->GetNextDocTemplate(posTemplate);
        posDoc = pTemplate->GetFirstDocPosition();

        while (posDoc != NULL) {
            pDoc = pTemplate->GetNextDoc(posDoc);
            posView = pDoc->GetFirstViewPosition();

            while (posView != NULL) {
                pView = pDoc->GetNextView(posView);
                bBackground = !(hActiveView == pView->GetSafeHwnd());
                // bacground mode if view is not the active view
                pView->SendMessage(WM_VIEWPALETTECHANGED, bBackground);
            }
        }
    }

    /*

    */
    return TRUE;
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
    TRACE("CMainFrame::OnPaletteChanged\n");

    if (GetSafeHwnd() != pFocusWnd->GetSafeHwnd()) {
        OnQueryNewPalette();
    }
}

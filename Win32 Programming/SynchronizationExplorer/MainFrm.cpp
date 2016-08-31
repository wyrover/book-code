// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"

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
        ON_COMMAND(ID_WINDOW_CASCADE, OnWindowCascade)
        //}}AFX_MSG_MAP
        // Global help commands
        ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
        ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
        ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
        ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
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
        
        if (!m_wndToolBar.Create(this) ||
                !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
        {
                TRACE0("Failed to create toolbar\n");
                return -1;      // fail to create
        }

        if (!m_wndStatusBar.Create(this) ||
                !m_wndStatusBar.SetIndicators(indicators,
                  sizeof(indicators)/sizeof(UINT)))
        {
                TRACE0("Failed to create status bar\n");
                return -1;      // fail to create
        }

        // TODO: Remove this if you don't want tool tips or a resizeable toolbar
        m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
                CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

        // TODO: Delete these three lines if you don't want the toolbar to
        //  be dockable
        m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
        EnableDocking(CBRS_ALIGN_ANY);
        DockControlBar(&m_wndToolBar);

        return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CMDIFrameWnd::PreCreateWindow(cs);
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


/****************************************************************************
*                               OnWindowCascade
* Result: void
*       
* Effect: 
*       This intercepts the "cascade" menu item and sends the MDI Client
*       frame a message (or ideally we end up sending it to each of the
*       MDI child windows).  The purpose of these two messages is to prep
*       the windows for their motion.  The Controller window is assigned an
*       ID of 0; the remaining child windows in Z-axis order are assigned
*       integers 1..n.  In addition, the size of the controller window and
*       the appropriate offset for the child windows is computed.  A flag
*       is set indicating we are in a Cascade operation
*
*       During the move, the goal is to use the fact that we are in a cascade
*       to force, via the OnWindowPosChanging method, the controller window
*       to anchor to the top left corner and subsequent windows to fan out
*       to the right.  When the first row is filled, the next window moves down
*       to the next row.
*
*       The UWM_POSTCASCADE message clears the flag so that ordinary window
*       movement is not inhibited by the algorithm.
* Notes:
*       This is going to take a lot of time to implement, so it may be left
*       as an exercise for the reader.
****************************************************************************/

void CMainFrame::OnWindowCascade() 
{
 // ?->SendMessage(UWM_PRECASCADE);

 CMDIFrameWnd::MDICascade();

 // ?->SendMessage(UWM_POSTCASCADE);
        
}

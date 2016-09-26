// IpFrame.cpp : implementation of the CInPlaceFrame class
//

#include "stdafx.h"
#include "Scribble.h"

#include "IpFrame.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
    //{{AFX_MSG_MAP(CInPlaceFrame)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    // Global help commands
    ON_COMMAND(ID_HELP_FINDER, COleDocIPFrameWnd::OnHelpFinder)
    ON_COMMAND(ID_HELP, COleDocIPFrameWnd::OnHelp)
    ON_COMMAND(ID_DEFAULT_HELP, COleDocIPFrameWnd::OnHelpFinder)
    ON_COMMAND(ID_CONTEXT_HELP, COleDocIPFrameWnd::OnContextHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // CResizeBar implements in-place resizing.
    if (!m_wndResizeBar.Create(this)) {
        TRACE0("Failed to create resize bar\n");
        return -1;      // fail to create
    }

    // By default, it is a good idea to register a drop-target that does
    //  nothing with your frame window.  This prevents drops from
    //  "falling through" to a container that supports drag-drop.
    m_dropTarget.Register(this);
    return 0;
}

// OnCreateControlBars is called by the framework to create control bars on the
//  container application's windows.  pWndFrame is the top level frame window of
//  the container and is always non-NULL.  pWndDoc is the doc level frame window
//  and will be NULL when the container is an SDI application.  A server
//  application can place MFC control bars on either window.
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
    // Remove this if you use pWndDoc
    UNREFERENCED_PARAMETER(pWndDoc);
    // Set owner to this window, so messages are delivered to correct app
    m_wndToolBar.SetOwner(this);

    // Create toolbar on client's frame window
    if (!m_wndToolBar.CreateEx(pWndFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                               | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_SCRIBBTYPE_SRVR_IP)) {
        TRACE0("Failed to create toolbar\n");
        return FALSE;
    }

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    pWndFrame->EnableDocking(CBRS_ALIGN_ANY);
    pWndFrame->DockControlBar(&m_wndToolBar);
    return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame diagnostics

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
    COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
    COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame commands

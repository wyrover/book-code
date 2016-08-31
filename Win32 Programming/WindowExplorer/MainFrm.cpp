// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "afxtempl.h"
#include "WndExp.h"
#include "idcombo.h"
#include "numericEdit.h"
#include "message.h"
#include "winList.h"
#include "WinMgr.h"
#include "createex.h"
#include "styles.h"

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
	ON_REGISTERED_MESSAGE(UWM_ACTIVATE, OnMyActivate)
	ON_COMMAND(IDM_WNDEXP, OnWndexp)
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

/*****************************************************************************
*				 UWM_ACTIVATE
* Inputs:
*	WPARAM: Handle of window being activated
*	LPARAM: unused, 0
* Result: 0
*	ignored
* Effect:
*	Notifies the main window that one of the created child windows
*	is being activated.  This lets us do things like select it from
*	the list control
*****************************************************************************/
int CMainFrame::UWM_ACTIVATE = 0;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
 UWM_ACTIVATE = ::RegisterWindowMessage(_T("Window Explorer.Activate"));
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

void CMainFrame::OnWndexp() 
{
 CPropertySheet sht(IDS_CREATEWINDOWEX_CAPTION);
 CCreateWindowEx cex;
 CStyles styles;

 sht.AddPage(&cex);
 sht.AddPage(&styles);
 sht.DoModal();
}

/****************************************************************************
*                          CMainFrame::OnMyActivate
* Inputs:
*       WPARAM wParam: (WPARAM)HWND: Window handle of window being activated
*	LPARAM: unused, but passed through
* Result: 0
*       ignored
* Effect: 
*       Sends the registered message to all descendants.  Note that we can't
*	limit it to immediate descendants because we want it to be received
*	by the property page, which is a descendant of the property sheet.
* Notes:
*	Because we are sending to all descendants, which includes the dozens
*	of special controls underneath us, it is not safe to use a WM_USER
*	message since this could conflict with an existing message (it has
*	happened to us!)  Instead, we register a unique message so nobody who
*	sees it will react -- assuming they are all well-behaved.
****************************************************************************/

LRESULT CMainFrame::OnMyActivate(WPARAM wParam, LPARAM lParam)
    {
     SendMessageToDescendants(UWM_ACTIVATE, wParam, lParam);
     return 0;
    }

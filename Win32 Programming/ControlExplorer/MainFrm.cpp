// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <afxtempl.h>
#include "ControlExplorer.h"

#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "intedit.h"
#include "scparent.h"
#include "mybutton.h"
#include "TinyButt.h"
#include "UDZ.h"
#include "resize.h"

#include "NumericEdit.h"
#include "dlgedit.h"
#include "dropdown.h"
#include "butttest.h"
#include "listbox.h"
#include "recedit.h"
#include "edittest.h"
#include "scrltest.h"
#include "MyStatic.h"
#include "stattest.h"
#include "header.h"
#include "trackbar.h"
#include "3d.h"
#include "drawing.h"
#include "progress.h"
#include "sdemo.h"
#include "tracklog.h"
#include "updown.h"
#include "demo.h"
#include "tree.h"

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
	ON_COMMAND(ID_CTLEXP, OnCtlexp)
	ON_COMMAND(IDM_SLIDERS, OnSliders)
	ON_COMMAND(IDM_LOGARITHM, OnLogarithm)
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

void CMainFrame::OnCtlexp() 
{
 CPropertySheet ps(IDS_CONTROL_EXPLORER);

 CEditTest edit;
 CListBoxTest lb;
 CDropDownTest ddt;
 CScrollBarTest scr;
 CStaticTest stat;
 CButtonTest butt;
 CHeaderTest hdr;
 CTrackBar tb;
 CProgress prg;
 CTree tree;
 CUpDown updown;
 C3D threeD;

 ps.AddPage(&butt);
 ps.AddPage(&ddt);
 ps.AddPage(&edit);
 ps.AddPage(&hdr);
 ps.AddPage(&lb);
 ps.AddPage(&prg);
 ps.AddPage(&scr);
 ps.AddPage(&stat);
 ps.AddPage(&tb);
 ps.AddPage(&tree);
 ps.AddPage(&updown);
 ps.AddPage(&threeD);

 ps.DoModal(); 
	
}

void CMainFrame::OnSliders() 
{
 CSliderDemo dlg;
 dlg.DoModal();
	
}

void CMainFrame::OnLogarithm() 
{
 CLogarithmic dlg;
 dlg.DoModal();
	
}

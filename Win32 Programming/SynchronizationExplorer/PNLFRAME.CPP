// PnlFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"
#include "childfrm.h"
#include "PnlFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelFrame

IMPLEMENT_DYNCREATE(CPanelFrame, CChildFrame)

CPanelFrame::CPanelFrame()
{
}

CPanelFrame::~CPanelFrame()
{
}


BEGIN_MESSAGE_MAP(CPanelFrame, CChildFrame)
        //{{AFX_MSG_MAP(CPanelFrame)
        ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelFrame message handlers

void CPanelFrame::OnClose() 
{
 return;  // We don't allow this window to close
}

BOOL CPanelFrame::PreCreateWindow(CREATESTRUCT& cs) 
{

 	cs.style &= ~(WS_MAXIMIZE);
	
	return CChildFrame::PreCreateWindow(cs);
}


void CPanelFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
 switch(nID & ~0xF)
     {
      case SC_CLOSE:
      case SC_SIZE:
      case SC_MAXIMIZE:
	      return;
     }
	
	CChildFrame::OnSysCommand(nID, lParam);
}

void CPanelFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	pPopupMenu->EnableMenuItem(SC_SIZE, MF_GRAYED);
	pPopupMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
	pPopupMenu->EnableMenuItem(SC_MAXIMIZE, MF_GRAYED);

	CChildFrame::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

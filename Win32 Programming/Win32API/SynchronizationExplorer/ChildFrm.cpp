// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
        //{{AFX_MSG_MAP(CChildFrame)
        ON_WM_GETMINMAXINFO()
	ON_WM_INITMENUPOPUP()
	ON_WM_SYSCOMMAND()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
        sizeknown = FALSE;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
        CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
        CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
        if(sizeknown)
           { /* use known size */
            lpMMI->ptMaxSize = sizelimit;
            lpMMI->ptMinTrackSize = sizelimit;
            lpMMI->ptMaxTrackSize = sizelimit;
            return;
           } /* use known size */

        CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}

/****************************************************************************
*                          void CChildFrame::setSize
* Result: void
*       
* Effect: 
*       Sets the size to limit resizing options
****************************************************************************/

void CChildFrame::setSize()
    {
     CRect r;
     GetWindowRect(&r);

     sizelimit.x = r.Width();
     sizelimit.y = r.Height();
     sizeknown = TRUE;
    }

void CChildFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIChildWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	pPopupMenu->EnableMenuItem(SC_SIZE, MF_GRAYED);
	pPopupMenu->EnableMenuItem(SC_MAXIMIZE, MF_GRAYED);
	
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
 switch(nID & ~0xF)
    {
     case SC_SIZE:
     case SC_MAXIMIZE:
	     return;
    }
 CMDIChildWnd::OnSysCommand(nID, lParam);
}

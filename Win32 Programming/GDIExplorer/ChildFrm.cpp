// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "GDIExplorer.h"

#include "ChildFrm.h"
#include "uwm.h"

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
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code !
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
        // TODO: add member initialization code here
        
}

CChildFrame::~CChildFrame()
{
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

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~FWS_ADDTOTITLE;
    return CMDIChildWnd::PreCreateWindow(cs);
}


/****************************************************************************
*                             CChildFrame::OnUserError
* Inputs:
*       WPARAM: body of text
*       LPARAM: heading of text
* Result: LRESULT
*       0, always
* Effect: 
*       Issues an error message
****************************************************************************/

LRESULT CChildFrame::OnUserError(WPARAM wParam, LPARAM lParam)
    {
     // NYI
     return 0;
    }

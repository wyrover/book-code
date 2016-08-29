// MsgSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msgSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageSheet

IMPLEMENT_DYNAMIC(CMessageSheet, CPropertySheet)

CMessageSheet::CMessageSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
        :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMessageSheet::CMessageSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
        :CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CMessageSheet::~CMessageSheet()
{
}


BEGIN_MESSAGE_MAP(CMessageSheet, CPropertySheet)
        //{{AFX_MSG_MAP(CMessageSheet)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageSheet message handlers


/****************************************************************************
*                         CMessageSheet::OnInitDialog
* Result: BOOL
*       As returned by CPropertySheet::OnInitDialog
* Effect: 
*       Makes sure the window is in the top right of the screen
****************************************************************************/

BOOL CMessageSheet::OnInitDialog()
    {
     BOOL result = CPropertySheet::OnInitDialog();

     CRect r;
     GetWindowRect(&r);
     SetWindowPos(NULL, ::GetSystemMetrics(SM_CXSCREEN) - r.Width(), 0,
                        0, 0, SWP_NOSIZE | SWP_NOZORDER);
     return result;
    }

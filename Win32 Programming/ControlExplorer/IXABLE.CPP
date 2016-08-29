// ixable.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexablePage property page

IMPLEMENT_DYNCREATE(CIndexablePage, CPropertyPage)

CIndexablePage::CIndexablePage(int idd) : CPropertyPage(idd)
{
 id = idd;  // save id for table of contents
}

CIndexablePage::CIndexablePage() : CPropertyPage((int)0)  // should never call default constructor!
{
 focus = 0;
        //{{AFX_DATA_INIT(CIndexablePage)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CIndexablePage::~CIndexablePage()
{
}

void CIndexablePage::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CIndexablePage)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexablePage, CPropertyPage)
        //{{AFX_MSG_MAP(CIndexablePage)
        ON_MESSAGE(UWM_SETPAGEFOCUS, OnFocusRequest)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexablePage message handlers

/****************************************************************************
*                       CIndexablePage::OnFocusRequest
* Inputs:
*       WPARAM wParam: ID of control to receive focus
* Result: LRESULT
*       logically void, 0 always
* Effect: 
*       Causes the focus to be set to the specified control
* Notes:
*       Sent to the page by the "index" page
****************************************************************************/

LRESULT CIndexablePage::OnFocusRequest(WPARAM wParam, LPARAM)
    {
     focus = wParam;
     
     if(focus != 0)
        { /* want focus */
         CWnd * wnd = GetDlgItem(focus);

         if(wnd != NULL)
            wnd->SetFocus();
        } /* want focus */

     return 0;
    }

/****************************************************************************
*                         CIndexablePage::OnSetActive
* Result: BOOL
*       Result of superclass call
* Effect: 
*       If the focus is not 0, sets the focus to the control that has that
*       id
****************************************************************************/

BOOL CIndexablePage::OnSetActive()
    {
     BOOL result = CPropertyPage::OnSetActive();

     if(focus != 0)
        { /* want focus */
         PostMessage(UWM_SETPAGEFOCUS, focus);
        } /* want focus */

     return result;
    }

/****************************************************************************
*                         CIndexablePage::OnKillActive
* Result: BOOL
*       Result of superclass call
* Effect: 
*       If the focus is not 0, sets the focus to the control that has that
*       id
****************************************************************************/

BOOL CIndexablePage::OnKillActive()
    {
     CWnd * wnd = GetFocus();

     if(wnd != NULL)
        { /* had focus */
         focus = wnd->GetDlgCtrlID();
        } /* had focus */

     return CPropertyPage::OnKillActive();
    }

// Box.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "Box.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBox

CBox::CBox()
{
}

CBox::~CBox()
{
}


BEGIN_MESSAGE_MAP(CBox, CButton)
        //{{AFX_MSG_MAP(CBox)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBox message handlers

void CBox::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 // A checked box is drawn as a white square; an unchecked box is
 // drawn as a black square

 // If the item is selected, a selection rectangle is drawn.

 CDC * dc = CDC::FromHandle(dis->hDC);

 CRect r = dis->rcItem;

 int saved = dc->SaveDC();

 CBrush brush;

 if(GetCheck())
    brush.CreateStockObject(WHITE_BRUSH);
 else
    brush.CreateStockObject(BLACK_BRUSH);

 dc->FillRect(&r, &brush);

 if(dis->itemState & ODS_FOCUS)
    { /* show selection */
     InflateRect(&r, -2, -2);
     dc->DrawFocusRect(&r);
    } /* show selection */

 dc->RestoreDC(saved);
}

/****************************************************************************
*                               CBox::SetCheck
* Inputs:
*       BOOL val: Value to check
* Result: void
*       
* Effect: 
*       Marks the control as "checked" (TRUE) or "unchecked" (FALSE)
* Notes:
*       The only valid values are 1 and 0, so we use a Boolean expression
*       to force the value stored to be exactly 1 or 0.
****************************************************************************/

void CBox::SetCheck(BOOL val)
    {
     BOOL old = GetCheck();
     ::SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)(val ? 1 : 0));
     if(old != val)
        InvalidateRect(NULL);
    }

/****************************************************************************
*                               CBox::GetCheck
* Result: BOOL
*       TRUE if checked
*       FALSE if unchecked
****************************************************************************/

BOOL CBox::GetCheck()
    {
     return (BOOL)::GetWindowLong(m_hWnd, GWL_USERDATA);
    }

/****************************************************************************
*                                CBox::toggle
* Result: void
*       
* Effect: 
*       Toggles the state of the box
****************************************************************************/

void CBox::toggle()
    {
     SetCheck(!GetCheck());
     InvalidateRect(NULL);
    }

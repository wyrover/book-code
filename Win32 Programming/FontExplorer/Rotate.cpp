// Rotate.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "degrees.h"
#include "Rotate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotation

CRotation::CRotation()
{
 buddy = NULL;
}

CRotation::~CRotation()
{
}


BEGIN_MESSAGE_MAP(CRotation, CSpinButtonCtrl)
        //{{AFX_MSG_MAP(CRotation)
        ON_WM_VSCROLL()
        ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotation message handlers

void CRotation::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    CSpinButtonCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
    pos_to_buddy();
}

BOOL CRotation::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
 BOOL result = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

 if(!result)
    return FALSE;

 SetRange(-3600, 3600);
 SetPos(0);
 pos_to_buddy();

 return TRUE;
}

/****************************************************************************
*                           CRotation::pos_to_buddy
* Result: void
*       
* Effect: 
*       Transfers the contents of the window to the buddy display
****************************************************************************/

void CRotation::pos_to_buddy()
    {
     if(buddy == NULL)
        return;

     int n = (int)(short)GetPos();
     buddy->SetWindowInt(n);
    }

/****************************************************************************
*                             CRotation::SetBuddy
* Inputs:
*       CDegrees * newbuddy: Buddy window
* Result: CWnd *
*       Old buddy
* Effect: 
*       Sets the new buddy window.  Forces the value to be formatted
* Notes:
*       We have to handle buddy windows on our own because of how we format
*       the values
****************************************************************************/

CWnd * CRotation::SetBuddy(CDegrees * newbuddy)
    {
     CWnd * old = buddy;
     buddy = newbuddy;
     pos_to_buddy();
     return old;
    }

/****************************************************************************
*                             CRotation::SetRange
* Inputs:
*       int lower:
*       int upper:
* Result: void
*       
* Effect: 
*       Sets the spin control range
****************************************************************************/

void CRotation::SetRange(int lower, int upper)
    {
     CSpinButtonCtrl::SetRange(lower, upper);
     pos_to_buddy();
    }

/****************************************************************************
*                              CRotation::SetPos
* Inputs:
*       int pos: Position to set
*       BOOL updatebuddy = TRUE: updates buddy control.  Set to false if
*                       calling from EN_UPDATE handler of buddy control
* Result: int
*       Previous value
* Effect: 
*       Sets the position and updates the buddy control
****************************************************************************/

int CRotation::SetPos(int pos, BOOL updatebuddy)
    {
     int result = CSpinButtonCtrl::SetPos(pos);
     if(updatebuddy)
        pos_to_buddy();
     return result;
    }

/****************************************************************************
*                            CRotation::OnDeltapos
* Inputs:
*       NMHDR * pNMHDR: ignored
*       LRESULT * pResult: Set to 0 to permit control to change
* Result: void
*       
* Effect: 
*       If there is a buddy, reads the value and sets it in the control
*       before allowing the operation to proceed
****************************************************************************/

void CRotation::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
        // TODO: Add your control notification handler code here
        
        if(buddy != NULL)
           { /* read buddy */
            int pos = buddy->GetWindowInt();
            CSpinButtonCtrl::SetPos(pos);           
           } /* read buddy */
        *pResult = 0;
}

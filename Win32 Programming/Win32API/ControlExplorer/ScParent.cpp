// ScParent.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ScParent.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollParent

CScrollParent::CScrollParent()
{
}

CScrollParent::~CScrollParent()
{
}


BEGIN_MESSAGE_MAP(CScrollParent, CWnd)
        //{{AFX_MSG_MAP(CScrollParent)
        ON_WM_SIZE()
        ON_WM_VSCROLL()
        ON_WM_HSCROLL()
        ON_WM_GETMINMAXINFO()
        ON_WM_SETFOCUS()
        ON_MESSAGE(UWM_SETFOCUS, ForwardFocus)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollParent message handlers

void CScrollParent::OnSize(UINT nType, int cx, int cy) 
{
        CWnd::OnSize(nType, cx, cy);
        
        CRect sbr;
        // This code depends upon the fact that our scrollbar has a
        // known ID

        CScrollBar * sb = (CScrollBar *)GetDlgItem(IDC_SCROLLBAR);

        // We want the box to maintain its alignment.  If it is a bottom
        // right box, it tracks the bottom right.  If it is a top left
        // box, it stays anchored.

        if(sb->GetStyle() &  SBS_SIZEBOXBOTTOMRIGHTALIGN)
           { /* keep right */
            CRect r;

            sb->GetWindowRect(&r);
            ScreenToClient(&r);

            sb->MoveWindow(cx - r.Width(), cy - r.Height(), 
                                        r.Width(), r.Height(), TRUE);
            InvalidateRect(NULL);
           } /* keep right */
        else
        if(sb->GetStyle() & SBS_SIZEBOXTOPLEFTALIGN)
           { /* leave top left */
            // do nothing
           } /* leave top left */
        else
           { /* neither, make sure still visible */
            // Currently we don't have to do anything
            // because we limit, in the GetMinMaxInfo handler, the size
            // to not get smaller than would cover our window
           } /* neither, make sure still visible */


        // Send a message to the parent so we can report the WM_SIZE as
        // having taken place.  This is strictly for the Explorer demo
        // and would not be needed for a real example

        GetParent()->SendMessage(UWM_SIMULATE_WM_SIZE, nType, MAKELONG(cx, cy));
        
}

/****************************************************************************
*                          CScrollParent::OnVScroll
* Inputs:
*       UINT nSBCode:
*       UINT nPos:
*       CScrollBar * pScrollBar:
* Result: void
*       
* Effect: 
*       This fakes, for purposes of the Explorer, the effect of the scroll
*       bar being in our parent window, but allows the sizing operations
*       to work on the simulated parent window.  This is entirely gratuitous
*       for purposes of being a nice demo.  Normally we would create sizing
*       boxes in the parent window itself, but that would not be visually
*       attractive for the Explorer.
* Notes:
*       There is no good "forwarding" macro so we have to assemble the
*       parameters "by hand" to get the effect.
****************************************************************************/

void CScrollParent::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        GetParent()->SendMessage(WM_VSCROLL, MAKELONG(nSBCode, nPos),
                                                (LPARAM)pScrollBar->m_hWnd);
}

void CScrollParent::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        GetParent()->SendMessage(WM_HSCROLL, MAKELONG(nSBCode, nPos),
                                                (LPARAM)pScrollBar->m_hWnd);
}

void CScrollParent::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
 // We have to limit the size.  if the GWL_USERDATA is nonzero, we use
 // its values to limit the maximum x and y dimensions
        
 DWORD size = ::GetWindowLong(m_hWnd, GWL_USERDATA);
 if(size == 0)
    CWnd::OnGetMinMaxInfo(lpMMI);
 else
    { /* impose limits */
     CWnd * wnd;
     wnd = GetDlgItem(IDC_SCROLLBAR);
     if(wnd != NULL)
        { /* keep min size */
         CRect r;
         if(wnd->GetStyle() & (SBS_SIZEBOXTOPLEFTALIGN | SBS_SIZEBOXBOTTOMRIGHTALIGN))
            { /* cornered */
             wnd->GetWindowRect(&r);
             lpMMI->ptMinTrackSize.x = r.Width();
             lpMMI->ptMinTrackSize.y = r.Height();
            } /* cornered */
         else
            { /* other */
             wnd->GetWindowRect(&r);
             ScreenToClient(&r);
             lpMMI->ptMinTrackSize.x = r.right;
             lpMMI->ptMinTrackSize.y = r.bottom;
            } /* other */
        } /* keep min size */
     
     lpMMI->ptMaxSize.x = LOWORD(size);
     lpMMI->ptMaxSize.y = HIWORD(size);
     lpMMI->ptMaxTrackSize.x = LOWORD(size);
     lpMMI->ptMaxTrackSize.y = HIWORD(size);
    } /* impose limits */
}

/****************************************************************************
*                          CScrollParent::OnSetFocus
* Inputs:
*       CWnd * pOldWnd:
* Result: void
*       
* Effect: 
*       This is necessary for the Explorer application, which places the
*       scrollbar within another window so the size boxes work nicely.
*       However, what we have to do here is if the window ever gets focus,
*       we want to immediately hand off focus to the scrollbar child if
*       there is one.  Otherwise we want to pass the focus on to the
*       next window in sequence
****************************************************************************/

void CScrollParent::OnSetFocus(CWnd* pOldWnd) 
{
        CWnd::OnSetFocus(pOldWnd);
        
        CWnd * scrollbar = GetDlgItem(IDC_SCROLLBAR);
        if(scrollbar != NULL)
           this->PostMessage(UWM_SETFOCUS, (WPARAM)scrollbar);
        else
           { /* try for next */
            CWnd * next = GetNextDlgTabItem(this);
            if(next != NULL)
               this->PostMessage(UWM_SETFOCUS, (WPARAM)next);
           } /* try for next */
        
}

/****************************************************************************
*                         CScrollParent::ForwardFocus
* Inputs:
*       WPARAM wParam: Window handle of window to receive focus
*       LPARAM: not used
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Sets the focus to the window whose handle is given
****************************************************************************/

LRESULT CScrollParent::ForwardFocus(WPARAM wParam, LPARAM)
    {
     if(wParam != NULL)
        ((CWnd *)wParam)->SetFocus();

     return 0;
    }

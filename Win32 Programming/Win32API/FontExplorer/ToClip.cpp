#include "stdafx.h"
#include "toclip.h" 


/****************************************************************************
*                                 toClipboard
* Inputs:
*       CWnd * wnd: Window whose contents are to be sent to clipboard
*       BOOL FullWnd: TRUE for entire window, FALSE for client area
* Result: void
*       
* Effect: 
*       Copies the contents of the client area or the window to the clipboard
*       in CF_BITMAP format.
****************************************************************************/

void toClipboard(CWnd * wnd, BOOL FullWnd)
    {
     CDC dc;

     if(FullWnd)
        { /* full window */
         HDC hdc = ::GetWindowDC(wnd->m_hWnd);
         dc.Attach(hdc);
        } /* full window */
     else
        { /* client area only */
         HDC hdc = ::GetDC(wnd->m_hWnd);
         dc.Attach(hdc);
        } /* client area only */

     CDC memDC;

     memDC.CreateCompatibleDC(&dc);

     CBitmap bm;
     CRect r;

     if(FullWnd)
        wnd->GetWindowRect(&r);
     else
        { /* client only */
         wnd->GetClientRect(&r);
        } /* client only */

     CString s;
     wnd->GetWindowText(s);

     CSize sz(r.Width(), r.Height());

     bm.CreateCompatibleBitmap(&dc, sz.cx, sz.cy);

     CBitmap * oldbm = memDC.SelectObject(&bm);

     memDC.BitBlt(0, 0, sz.cx, sz.cy, &dc, 0, 0, SRCCOPY);

     wnd->GetParent()->OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
    }

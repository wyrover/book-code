#include "stdafx.h"
#include "copy.h" 


/****************************************************************************
*                                 toClipboard
* Inputs:
*       CWnd * wnd: Window whose contents are to be sent to clipboard
*       CRect * bb: Bounding rectangle, or NULL for entire client area
*                       Units are in device units.
* Result: void
*       
* Effect: 
*       Copies the contents of the client area or a piece of it to the
*       clipboard in CF_BITMAP format.
****************************************************************************/

void toClipboard(CWnd * wnd, CRect * bb)
    {
     CClientDC dc(wnd);

     CDC memDC;

     memDC.CreateCompatibleDC(&dc);

     CBitmap bm;
     CRect r;
     CPoint pt;

     wnd->GetClientRect(&r);

     CString s;
     wnd->GetWindowText(s);

     CSize sz;

     if(bb == NULL)
        { /* entire client area */
         sz.cx = r.Width();
         sz.cy = r.Height();
         pt.x = 0;
         pt.y = 0;
        } /* entire client area */
     else
        { /* use BB */
         sz.cx = bb->Width();
         sz.cy = bb->Height();
         pt.x = bb->left;
         pt.y = bb->top;
        } /* use BB */

     bm.CreateCompatibleBitmap(&dc, sz.cx, sz.cy);

     CBitmap * oldbm = memDC.SelectObject(&bm);

     memDC.BitBlt(0, 0, sz.cx, sz.cy, &dc, pt.x, pt.y, SRCCOPY);

     wnd->GetParent()->OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
    }

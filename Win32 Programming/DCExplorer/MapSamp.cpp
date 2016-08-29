// MapSamp.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>

#include "DCExplorer.h"
#include "MapSamp.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMappingSample

CMappingSample::CMappingSample()
{
 ticks = 0;  // no ticks, no drawing
}

CMappingSample::~CMappingSample()
{
}


BEGIN_MESSAGE_MAP(CMappingSample, CStatic)
	//{{AFX_MSG_MAP(CMappingSample)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMappingSample message handlers

/****************************************************************************
*                          CMappingSample::badValue
* Inputs:
*       CDC & dc: Display context
* Result: void
*       
* Effect: 
*       Draws the window as a blank and posts the error back
****************************************************************************/

void CMappingSample::badValue(CDC & dc)
    {
     DWORD error = GetLastError();

     CRect r;
     GetClientRect(&r);

     CBrush fillbr(GetSysColor(COLOR_3DFACE));
     dc.FillRect(&r, &fillbr);     
     
     GetParent()->PostMessage(UWM_UPDATE_ERROR, error);
    }

void CMappingSample::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(ticks == 0)
	   return; // no ticks, no drawing

	CRect r;
	GetClientRect(&r);


	dc.SetMapMode(mapmode);
	// Note: SetWindowExt must be called *before* SetViewportExt
	// in case we are in MM_ISOTROPIC mode

	// We have to use the :: forms because the MFC versions don't
	// give us error codes; they just internally generate assert
	// failures, which is not acceptable behavior

	// We must also call the SetLastError call because the
	// API functions being checked do not set LastError on their own

	if(!::SetWindowExtEx(dc.m_hDC, wext.x, wext.y, NULL))
	   { /* bad wext */
	    SetLastError(ERROR_INVALID_PARAMETER);
	    badValue(dc);
	    return;
	   } /* bad wext */

	if(!::SetViewportExtEx(dc.m_hDC, vext.x, vext.y, NULL))
	   { /* bad vext */
	    SetLastError(ERROR_INVALID_PARAMETER);
	    badValue(dc);
	    return;
	   } /* bad vext */

	// Since we have no explicit return values below, we can safely
	// set the batch limit (for debugging convenience, so we can see
	// the lines being drawn immediately while debugging this code)
#ifdef _DEBUG
	int oldbatch = GdiSetBatchLimit(1); // needed only for NT 3.5x
#endif

#if 0 // This code is probably confused; this is because the Windows
      // documentation itself is EXTREMELY unclear as to what is really
      // happening here...

	// Note that according to the rules, setting the viewport ext
	// may change the window ext or be constrained by the
	// window ext in MM_ISOTROPIC mode so that isotropy is maintained
	// Here we see if that happened, and if so, we force the new
	// values back into the controls.

	CSize sz = dc.GetViewportExt();
	if(vext.x != sz.cx || vext.y != sz.cy)
	   GetParent()->PostMessage(UWM_CHANGE_VEXT, sz.cx, sz.cy);

	sz = dc.GetWindowExt();
	if(wext.x != sz.cx || wext.y != sz.cy)
	   GetParent()->PostMessage(UWM_CHANGE_WEXT, sz.cx, sz.cy);
#endif

	dc.SetWindowOrg(worg.x, worg.y);
	dc.SetViewportOrg(vorg.x, vorg.y);

	CRgn rgn;

	// Because controls don't clip, we need to force clipping
	rgn.CreateRectRgnIndirect(r);
	dc.SelectClipRgn(&rgn);

	CRect lrect;  // logical rectangle

	lrect = r;

	// We have to use the ::DPtoLP call directly to avoid a serious
	// flaw in the MFC design, wherein it assert-fails instead of
	// returning a boolean value indicating success or failure
	if(!::DPtoLP(dc.m_hDC, (LPPOINT)&lrect, 2))
	   { /* failed */
	    // According to the manual, this will fail if the device
	    // coordinates exceed 27 bits, or the converted coordinates
	    // would exceed 32 bits.
	    
	    // If this happens, we blank out the sample window to indicate
	    // an error and post the last error back
	    badValue(dc);
	   } /* failed */
	else
	   { /* converted */
	    CBrush fillbr(GetSysColor(COLOR_WINDOW));
	    dc.FillRect(&lrect, &fillbr);

	    CPen negpen(PS_SOLID, 0, RGB(255,0,0));


	    // Remember that based on the coordinates, we may have an
	    // ascending or descending delta.  In the case of descending,
	    // we need to draw the coordinates the opposite direction
	    // so our coordinate ssytem as portrayed is always in the
	    // positive-x, positive-y direction

	    // Draw a horizontal line from edge-to-edge, y=0 to positive y
	    {
	     int save = dc.SaveDC();
	     if(lrect.left < 0)
		dc.SelectObject(&negpen);
	     else
		dc.SelectStockObject(BLACK_PEN);

	     dc.MoveTo(lrect.left, 0);
	     dc.LineTo(0, 0);

	     if(lrect.right < 0)
		dc.SelectObject(&negpen);
	     else
		dc.SelectStockObject(BLACK_PEN);

	     dc.LineTo(lrect.right, 0);
	     dc.RestoreDC(save);
	    }


	    // Draw a vertical line from edge-to-edge, x=0 to positive x
	    {
	     int save = dc.SaveDC();
	     if(lrect.top < 0)
		dc.SelectObject(&negpen);
	     else
		dc.SelectStockObject(BLACK_PEN);

	     dc.MoveTo(0, lrect.top);
	     dc.LineTo(0, 0);

	     if(lrect.bottom < 0)
		dc.SelectObject(&negpen);
	     else
		dc.SelectStockObject(BLACK_PEN);

	     dc.LineTo(0, lrect.bottom);
	     dc.RestoreDC(save);
	    }

	    // Now find the leftmost point which is in the range and is a
	    // multiple of the ticks, and draw ticks across the screen
	    // We want the ticks to be the same size in both x and y,
	    // even in anisotropic mode.  So we first compute the sizes
	    // in physical units and compute the minimum value of the two.

#define divisor 20
	    CPoint ticksize(r.Height() / divisor, r.Width() / divisor) ;

	    // We now have physical points that represent a constant
	    // size in physical units.

	    ticksize.x = min(ticksize.x, ticksize.y);
	    ticksize.y = min(ticksize.x, ticksize.y);

	    // Now convert these points to logical units in both x and y
	    // To do this, we need to set the origin to 0,0 so that the
	    // scaling works correctly, since we basically want only
	    // scaling, and if we don't have 0,0 set, we get the origin
	    // offsets included in the computation

	    {
	     int save = dc.SaveDC();
	     dc.SetWindowOrg(0, 0);
	     dc.SetViewportOrg(0, 0);

	     ::DPtoLP(dc.m_hDC, &ticksize, 1);
	     dc.RestoreDC(save);
	    }



	    // Draw the coordinates in the direction of increasing x,y value
	    // Remember that some of the modes generate inverted axes, and
	    // vext and wext values could also be negative.

	    // We don't worry about the moveto/lineto signs for drawing the
	    // tickmarks because they are drawn symmetrically around the
	    // axis line, e.g., for y, either left-to-right or right-to-left
	    // but it doesn't matter which direction they are drawn.

	    int t1    = (lrect.left / ticks) * ticks;
	    int t2    = (lrect.right / ticks) * ticks;
	    int delta = ticks;
	    int start;
	    int end;

	    if(t2 < t1)
	       { /* invert */
	        start = t2;
		end   = t1;
	       } /* invert */
	    else
	       { /* normal */
	        start = t1;
		end   = t2;
	       } /* normal */

	    for(int x = start; x <= end; x += delta)
	       { /* hticks */
		if(x == 0)
		   continue;  // no tick at origin

		if(x < 0)
		   dc.SelectObject(&negpen);
		else
		   dc.SelectStockObject(BLACK_PEN);

		dc.MoveTo(x, -ticksize.y);
		dc.LineTo(x, ticksize.y);
	       } /* hticks */

	    t1    = (lrect.top / ticks) * ticks;
	    t2    = (lrect.bottom / ticks) * ticks;
	    delta = abs(ticks);
	     
	    if(t2 < t1)
	       { /* invert */
	        start = t2;
		end   = t1;
	       } /* invert */
	    else
	       { /* normal */
	        start = t1;
		end   = t2;
	       } /* normal */

	    for(int y = start; y <= end; y += delta)
	       { /* yticks */
		if(y == 0)
		   continue;  // no tick mark at origin

		if(y < 0)
		   dc.SelectObject(&negpen);
		else
		   dc.SelectStockObject(BLACK_PEN);

		dc.MoveTo(-ticksize.x, y);
		dc.LineTo(ticksize.x,  y);
	       } /* yticks */

	    // Because we are successful, we wish to clear the error message...
	     
	    GetParent()->PostMessage(UWM_UPDATE_ERROR, 0);
	   } /* converted */

#ifdef _DEBUG
	GdiSetBatchLimit(oldbatch);  // for NT 3.5x only
#endif

	// Do not call CStatic::OnPaint() for painting messages
}

/****************************************************************************
*                         CMappingSample::scrollValue
* Inputs:
*       int cpos: Current position of scroll object value
*	UINT nSBCode: Scrollbar code
*	UINT nPos: Scrollbar position for THUMB operations
*	CScrollBar * pScrollBar: Scroll bar handle
* Result: int
*       Updated cpos value
* Effect: 
*       Modifies the scrollbar position to reflect the new value
****************************************************************************/

int CMappingSample::scrollValue(int cpos, UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 int low;
 int high;

 pScrollBar->GetScrollRange(&low, &high);

 switch(nSBCode)
    { /* decode */
     case SB_PAGELEFT:
	     cpos -= ticks;
	     break;
     case SB_PAGERIGHT:
	     cpos += ticks;
	     break;
     case SB_LINELEFT:
	     cpos--;
	     break;
     case SB_LINERIGHT:
	     cpos++;
	     break;
     case SB_THUMBPOSITION:
	     cpos = nPos;
	     break;
    } /* decode */
 cpos = max(low, cpos);
 cpos = min(high, cpos);
 
 pScrollBar->SetScrollPos(cpos);
 return cpos;
}

void CMappingSample::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
 worg.x = scrollValue(worg.x, nSBCode, nPos, pScrollBar);
}

void CMappingSample::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
 worg.y = scrollValue(worg.y, nSBCode, nPos, pScrollBar);
}

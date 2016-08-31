// Sample.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "Sample.h"
#include "idcombo.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWnd * CSample::capture;

/////////////////////////////////////////////////////////////////////////////
// CSample

CSample::CSample()
{
 rop = R2_COPYPEN;
 hv = hv_Hq | hv_Vq;
 lastcolor = (COLORREF)-1;  // bogus value
 capture = NULL;
 actual_rect.SetRectEmpty();
}

CSample::~CSample()
{
}


BEGIN_MESSAGE_MAP(CSample, CStatic)
	//{{AFX_MSG_MAP(CSample)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSample message handlers


/****************************************************************************
*                              CSample::hStripe
* Inputs:
*       int i: Index position of stripe
*	COLORREF color: Color value to use
*	int count;
* Result: void
*       
* Effect: 
*       Paints a horizontal stripe in the selected color
* Notes:
*	Uses a path so the same code works on both NT and 95; otherwise we
*	don't get the flat end caps
****************************************************************************/

void CSample::hStripe(CDC * dc, int i, COLORREF color, int count, BOOL quantize)
    {
     CRect r;

     GetClientRect(&r);

     int height = (r.Height() / count);
     int width  = (r.Width() / count);

     int save = dc->SaveDC();

     CRect stripe;
     CBrush br(color);
     int right = (quantize ? width * count : r.right); // recompute because of truncation

     stripe.left = 0;
     stripe.top  = height * i;
     stripe.right = right + 1;
     stripe.bottom = stripe.top + height + 1;

     actual_rect.top = r.top;
     actual_rect.left = r.left;
     actual_rect.right = stripe.right;
     actual_rect.bottom = max(actual_rect.bottom, stripe.bottom);


     dc->SelectStockObject(NULL_PEN);
     dc->SelectObject(&br);
     dc->Rectangle(&stripe);
     dc->RestoreDC(save);
    }

/****************************************************************************
*                              CSample::vStripe
* Inputs:
*       int i: Index position of stripe
*	COLORREF color: Color value to use
*	int count: Number of colors to draw
*	BOOL quantize: TRUE to quantize length, FALSE to draw full length
* Result: void
*       
* Effect: 
*       Paints a vertical stripe in the selected color
* Notes:
*	Uses a path so the same code works on both NT and 95; otherwise we
*	don't get the flat end caps.
*	Computes vertical height of corresponding horizontal stripes and
*	then adjusts the line length so that it will not exceed the number
*	of horizontal stripes drawn underneath.
****************************************************************************/

void CSample::vStripe(CDC * dc, int i, COLORREF color, int count, BOOL quantize)
    {
     CRect r;

     GetClientRect(&r);

     int height = (r.Height() / count);
     int width  = (r.Width() / count);
     int save = dc->SaveDC();

     CRect stripe;
     CBrush br(color);
     int bottom = (quantize ? height * count : r.bottom); // recompute because of truncation

     stripe.left = width * i;
     stripe.top  = 0;
     stripe.right = stripe.left + width + 1;
     stripe.bottom = bottom + 1;

     actual_rect.top = r.top;
     actual_rect.bottom = stripe.bottom;
     actual_rect.left = r.left;
     actual_rect.right = max(actual_rect.right, stripe.right);

     dc->SelectStockObject(NULL_PEN);
     dc->SelectObject(&br);
     dc->Rectangle(&stripe);
     
     dc->RestoreDC(save);
    }

void CSample::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect r;
	int count;
	CPalette p;  // holder for palette
	p.CreateStockObject(DEFAULT_PALETTE);
	PALETTEENTRY pe[256];
	CRgn rgn;

	GetClientRect(&r);
	rgn.CreateRectRgnIndirect(r);
	dc.SelectClipRgn(&rgn);


	count = p.GetPaletteEntries(0, 256, pe);
	if(count == 0)
	   return; // should never fail

#ifdef _DEBUG
	int oldbatch = GdiSetBatchLimit(1);
#endif

	int i;

	int save = dc.SaveDC();

	if(hv & hv_2h)
	   dc.SetROP2(rop);

	if(hv & (hv_H | hv_Hq))
	    for(i = 0; i < count; i++)
	       { /* horizontal stripes */
		hStripe(&dc, i, RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue),
					count, hv & hv_Hq);
	       } /* horizontal stripes */

	dc.RestoreDC(save);

	save = dc.SaveDC();

	if(hv & hv_2v)
	   dc.SetROP2(rop);

	if(hv & (hv_V | hv_Vq))
	    for(i = 0; i < count; i++)
	       { /* vertical stripes */
		vStripe(&dc, i, RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue),
					count, hv & hv_Vq);
	       } /* vertical stripes */
	
	dc.RestoreDC(save);

#ifdef _DEBUG
	 GdiSetBatchLimit(oldbatch);
#endif
}

void CSample::OnDestroy() 
{
	CStatic::OnDestroy();
	
}

/****************************************************************************
*                             CSample::MouseMove
* Inputs:
*       UINT nFlags: Mouse button flags
*	CPoint point: Mouse coordinate in parent coordinates
* Result: void
*       
* Effect: 
*       Updates the color information in the parent.
* Notes:
*	A WM_MOUSEMOVE is not be routed to static child controls.
*	So we rely on the parent to route the mouse move messages to us.
*
****************************************************************************/

void CSample::MouseMove(UINT nFlags, CPoint point) 
{
 	COLORREF color;
	CClientDC dc(this);
	CRect r;

	GetClientRect(&r);

	// The point received is forwarded in parent client units.
	// We convert it to local client units by first converting
	// to screen units and then back to client units in the local
	// context

	GetParent()->ClientToScreen(&point);
	ScreenToClient(&point);

	// If we don't have capture, capture the mouse if it is in our
	// space.

	if(GetCapture() == NULL)
	   { /* capture mouse? */
	    if(!actual_rect.PtInRect(point))
	       return;  // mouse isn't in us
	    GetParent()->SetCapture();  // we got it...
	    capture = this;  // so record that we have it
	    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	   } /* capture mouse? */
	else
	   { /* in window */
	    // If we have the mouse, see if it is still in our space.
	    // If it isn't, release it and notify our parent that the
	    // color is no longer valid.

	    if(capture == this && !actual_rect.PtInRect(point))
	       { /* moved out */
	        GetParent()->PostMessage(UWM_COLOR_CLEAR);
		lastcolor = (COLORREF)-1; // for recapture
		ReleaseCapture();
		capture = NULL;
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	       } /* moved out */
	    else
	       { /* still in */
		// We are still in the window.  Get the pixel.
		color = dc.GetPixel(point);
		if(color != lastcolor)
		   { /* change */
		    GetParent()->PostMessage(UWM_COLOR_CHANGE, color);
		    lastcolor = color;
		   } /* change */
	       } /* still in */
	   } /* in window */

}

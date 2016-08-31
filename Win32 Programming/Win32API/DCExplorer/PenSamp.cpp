// PenSamp.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "DCExplorer.h"
#include "PenSamp.h"
#include "degrad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenSample

CPenSample::CPenSample()
{
 pen = NULL;
 miterlimit = 10.0f;
 join = FALSE;
 angle = 90;
}

CPenSample::~CPenSample()
{
}


BEGIN_MESSAGE_MAP(CPenSample, CStatic)
	//{{AFX_MSG_MAP(CPenSample)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenSample message handlers

void CPenSample::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Draws example images

	if(pen == NULL)
	   return;  // no drawing to do

	CRect r1;
	CRect r2;
	CRect r3;
	CRect r4;

	CRect r;
	GetClientRect(&r);

	dc.SetBkColor(bkcolor);

	CBrush fillbr(GetSysColor(COLOR_3DFACE));

	dc.FillRect(&r, &fillbr);

	r1 = r2 = r3 = r4 = r;

	r1.right = r1.left + r.Width() / 2;
	r1.bottom = r1.top + r.Height() / 2;

	r2.left = r1.right;
	r2.bottom = r1.bottom;

	r3.top = r1.bottom;
	r3.right = r1.right;

	r4.left = r2.left;
	r4.top = r3.top;

	/*
	   +--+--+
	   |r1|r2|
	   +--+--+
	   |r3|r4|
	   +--+--+
	*/
	   

	CRgn rgn1;
	CRgn rgn2;
	CRgn rgn3;
	CRgn rgn4;

	rgn1.CreateRectRgnIndirect(&r1);
	rgn2.CreateRectRgnIndirect(&r2);
	rgn3.CreateRectRgnIndirect(&r3);
	rgn4.CreateRectRgnIndirect(&r4);


	int border = r1.Width() / 7;
	r1.InflateRect(- border, - border);
	r2.InflateRect(- border, - border);
	r3.InflateRect(- border, - border);
	r4.InflateRect(- border, - border);


	int saved;

	// r1 box: a single line

	saved = dc.SaveDC();

	dc.SelectObject(pen);
	dc.SelectClipRgn(&rgn1);
	dc.SetBkMode(bkmode);
	dc.SetBkColor(RGB(255,255,255));

	dc.BeginPath();
	dc.MoveTo(r1.left, r1.top + r1.Height() / 2);
	dc.LineTo(r1.right, r1.top + r1.Height() / 2);
	dc.EndPath();
	dc.StrokePath();
	
	dc.RestoreDC(saved);

	// r2 box: two lines meet

	saved = dc.SaveDC();

	dc.SelectObject(pen);
	dc.SelectClipRgn(&rgn2);
	dc.SetBkMode(bkmode);
	dc.SetBkColor(RGB(255,255,255));

	dc.BeginPath();
	dc.MoveTo(r2.left, r2.top);
	dc.LineTo(r2.right, r2.bottom);
	dc.LineTo(r2.left, r2.bottom);
	dc.EndPath();
	dc.StrokePath();
	
	dc.RestoreDC(saved);


	// R3 box: an ellipse inside a box

	saved = dc.SaveDC();

	dc.SelectObject(pen);
	dc.SelectClipRgn(&rgn3);
	dc.SetBkMode(bkmode);
	dc.SetBkColor(RGB(255,255,255));

	dc.Ellipse(&r3);
	
	dc.RestoreDC(saved);


	saved = dc.SaveDC();

	dc.SelectStockObject(BLACK_PEN);
	dc.SelectStockObject(HOLLOW_BRUSH);
	dc.Rectangle(&r3);

	dc.RestoreDC(saved);

	// r4 box: miter limit

	// Draw the miter-limit providing the mode is a mitered joint

	if(join)
	   { /* do join */

	    saved = dc.SaveDC();

	    dc.SelectObject(pen);
	    dc.SelectClipRgn(&rgn4);
	    dc.SetBkMode(bkmode);
	    dc.SetBkColor(RGB(255,255,255));
	    dc.SetMiterLimit(miterlimit);

	    dc.BeginPath();

	    CPoint center(r4.left + r4.Width() / 2, r4.top + r4.Height() / 2);
	    int radius = min(r4.Width() / 2, r4.Height() / 2);
	    int x = center.x + 
	    	   (int) ((float)radius * (float)cos(deg_to_rad(angle)));
	    int y = center.y - 
	    	   (int) ((float)radius * (float)sin(deg_to_rad(angle)));

	    dc.MoveTo(center.x + radius, center.y);
	    dc.LineTo(center.x, center.y);
	    dc.LineTo(x, y);
	    dc.EndPath();
	    dc.StrokePath();

	    dc.RestoreDC(saved);
	   } /* do join */
}

// HatchSmp.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "HatchSmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHatchBrushSample

CHatchBrushSample::CHatchBrushSample()
{
}

CHatchBrushSample::~CHatchBrushSample()
{
}


BEGIN_MESSAGE_MAP(CHatchBrushSample, CStatic)
	//{{AFX_MSG_MAP(CHatchBrushSample)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHatchBrushSample message handlers

void CHatchBrushSample::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;

	GetClientRect(&r);

	CRect left = r;
	CRect right = r;

	left.right = left.Width() / 2;
	right.left = left.right;
	left.right++;

	/*
	   The left hand side uses the brush origin specified
	   The right hand side uses a brush origin of 0,0
	   If we had more vertical space we could have embedded the
	   custom origin brush in the interior of the 0,0 brush.

	   +------------+------------+
	   |  \   \   \ |\   \   \   |
	   |   \   \   \| \   \   \  |
           |\   \   \   |  \   \   \ |
           | \   \   \  |   \   \   \|
           |  \   \   \ |\   \   \   |
	   +------------+------------+
	*/

	CBrush fillbr(GetSysColor(COLOR_3DFACE));
	dc.FillRect(&r, &fillbr);

	dc.SetBkColor(bkcolor);
	dc.SetBkMode(bkmode);

	CBrush brush;
	brush.CreateBrushIndirect(&br);

	dc.SelectObject(&brush);
	dc.SelectStockObject(NULL_PEN);

	dc.Rectangle(&right);

	dc.SetBrushOrg(org.x, org.y);
	dc.Rectangle(&left);
	
	// Do not call CStatic::OnPaint() for painting messages
}

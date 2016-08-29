// EndCaps.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "EndCaps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEndCaps

CEndCaps::CEndCaps()
{
}

CEndCaps::~CEndCaps()
{
}


BEGIN_MESSAGE_MAP(CEndCaps, CGraphicCombo)
	//{{AFX_MSG_MAP(CEndCaps)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEndCaps message handlers

void CEndCaps::drawSample(LPRECT r, LPDRAWITEMSTRUCT dis) 
{
     CDC * dc = CDC::FromHandle(dis->hDC);
     CPen pen;
     LOGBRUSH lb;
     CRgn rgn;

     rgn.CreateRectRgnIndirect(r);

     lb.lbStyle = BS_SOLID;
     lb.lbColor = RGB(0, 0, 0);
     lb.lbHatch = dis->itemData;

     pen.CreatePen(PS_GEOMETRIC | dis->itemData, (2 * (r->bottom - r->top)) / 3, &lb);

     int saved = dc->SaveDC();

     dc->SelectObject(&pen);
     dc->SelectClipRgn(&rgn);

     // Start with the left said far off to the left, relying on the
     // clipping region to 

     int center = r->top + (r->bottom - r->top) / 2;  
     int startx = r->left - r->right;
     int endx   = r->left + 2 * (r->right - r->left) / 3;

     dc->BeginPath();
     dc->MoveTo(startx, center);
     dc->LineTo(endx, center);
     dc->EndPath();
     dc->StrokePath();
     
     dc->BeginPath();
     dc->SelectStockObject(WHITE_PEN);
     dc->MoveTo(startx, center);
     dc->LineTo(endx, center);
     dc->EndPath();
     dc->StrokePath();

     dc->RestoreDC(saved);

     pen.DeleteObject();
}

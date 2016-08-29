// LineJoin.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "LineJoin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineJoin

CLineJoin::CLineJoin()
{
}

CLineJoin::~CLineJoin()
{
}


BEGIN_MESSAGE_MAP(CLineJoin, CGraphicCombo)
	//{{AFX_MSG_MAP(CLineJoin)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineJoin message handlers

void CLineJoin::drawSample(LPRECT r0, LPDRAWITEMSTRUCT dis) 
{
     CDC * dc = CDC::FromHandle(dis->hDC);
     CPen pen;
     LOGBRUSH lb;
     CRgn rgn;
     CRect r = *r0;

     rgn.CreateRectRgnIndirect(r);

     lb.lbStyle = BS_SOLID;
     lb.lbColor = RGB(0, 0, 0);
     lb.lbHatch = 0; // ignored for BS_SOLID

     pen.CreatePen(PS_GEOMETRIC | dis->itemData, r.Height() / 4, &lb);

     int saved = dc->SaveDC();

     dc->SelectObject(&pen);
     dc->SelectClipRgn(&rgn);

     int top    = r.top + r.Height() / 4;
     int bottom = r.top + 3 * r.Height() / 4;  
     int left   = r.left + r.Width() / 4;
     int right  = r.left + 3 * r.Width() / 4;


     dc->BeginPath();
     dc->MoveTo(left, top);
     dc->LineTo(right, bottom);
     dc->LineTo(left, bottom);
     dc->EndPath();
     dc->StrokePath();
     
     dc->RestoreDC(saved);

     pen.DeleteObject();
}

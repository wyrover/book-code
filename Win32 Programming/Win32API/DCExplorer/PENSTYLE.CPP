// PenStyle.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "PenStyle.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenStyle

CPenStyle::CPenStyle()
{
}

CPenStyle::~CPenStyle()
{
}


BEGIN_MESSAGE_MAP(CPenStyle, CGraphicCombo)
	//{{AFX_MSG_MAP(CPenStyle)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenStyle message handlers

void CPenStyle::drawSample(LPRECT r, LPDRAWITEMSTRUCT dis) 
{
     CDC * dc = CDC::FromHandle(dis->hDC);
     CPen pen;
     LOGBRUSH lb;
     CRgn rgn;

     rgn.CreateRectRgnIndirect(r);

     lb.lbStyle = BS_SOLID;
     lb.lbColor = RGB(0, 0, 0);
     lb.lbHatch = 0; // ignored for BS_SOLID

     pen.CreatePen(PS_GEOMETRIC | dis->itemData, 
     		(Is95() ? 1 : 2 * ::GetSystemMetrics(SM_CXBORDER)), &lb);

     int saved = dc->SaveDC();

     dc->SelectObject(&pen);
     dc->SelectClipRgn(&rgn);

     // Start with the left said far off to the left, relying on the
     // clipping region to 

     int center = r->top + (r->bottom - r->top) / 2;  
     dc->BeginPath();
     dc->MoveTo(r->left - r->right, center);
     dc->LineTo(r->left + 2 * (r->right - r->left) / 3, center);
     dc->EndPath();
     dc->StrokePath();
     
     dc->RestoreDC(saved);

     pen.DeleteObject();
}

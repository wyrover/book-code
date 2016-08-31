// BrushStyle.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "BrStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData brushstyles[] = {
	{ IDS_BS_SOLID,      MAKELONG(BS_SOLID, 0)},
	{ IDS_HS_HORIZONTAL, MAKELONG(BS_HATCHED, HS_HORIZONTAL)},
	{ IDS_HS_VERTICAL,   MAKELONG(BS_HATCHED, HS_VERTICAL)},
	{ IDS_HS_FDIAGONAL,  MAKELONG(BS_HATCHED, HS_FDIAGONAL)},
	{ IDS_HS_BDIAGONAL,  MAKELONG(BS_HATCHED, HS_BDIAGONAL)},
	{ IDS_HS_CROSS,      MAKELONG(BS_HATCHED, HS_CROSS)},
	{ IDS_HS_DIAGCROSS,  MAKELONG(BS_HATCHED, HS_DIAGCROSS)},
	
	{ 0, 0} // EOT
			      };

/////////////////////////////////////////////////////////////////////////////
// CBrushStyle

CBrushStyle::CBrushStyle()
{
}

CBrushStyle::~CBrushStyle()
{
}


BEGIN_MESSAGE_MAP(CBrushStyle, CGraphicCombo)
	//{{AFX_MSG_MAP(CBrushStyle)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrushStyle message handlers

void CBrushStyle::drawSample(LPRECT r0, LPDRAWITEMSTRUCT dis) 
{
     CDC * dc = CDC::FromHandle(dis->hDC);


     LOGBRUSH lb;
     CRgn rgn;
     CRect r = *r0;

     rgn.CreateRectRgnIndirect(r);

     lb.lbStyle = LOWORD(dis->itemData);
     lb.lbColor = RGB(0, 0, 0);
     lb.lbHatch = HIWORD(dis->itemData);

     CBrush br;
     br.CreateBrushIndirect(&lb);

     int saved = dc->SaveDC();
     
     dc->SelectObject(&br);
     dc->SelectStockObject(NULL_PEN);
     dc->SelectClipRgn(&rgn);

     dc->Rectangle(&r);

     dc->RestoreDC(saved);
     br.DeleteObject();
}

/****************************************************************************
*                             CBrushStyle::Load
* Inputs:
*	int sel: Selection to make after loading
* Result: int
*       Index of selection, or CB_ERR if no selection made
* Effect: 
*       Loads the control with the colors from the color table
****************************************************************************/

int CBrushStyle::Load(int sel)
    {
     AddStrings(brushstyles);
     return Select(sel);
    }

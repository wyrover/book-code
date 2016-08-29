// Stock.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "Stock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData stock[] = {
	{ IDS_WHITE_BRUSH,   WHITE_BRUSH},
	{ IDS_LTGRAY_BRUSH,  LTGRAY_BRUSH},
	{ IDS_GRAY_BRUSH,    GRAY_BRUSH},
	{ IDS_DKGRAY_BRUSH,  DKGRAY_BRUSH},
	{ IDS_BLACK_BRUSH,   BLACK_BRUSH},
	{ IDS_NULL_BRUSH,    NULL_BRUSH},
	{0, 0} // EOT
			};

/////////////////////////////////////////////////////////////////////////////
// CStock

CStock::CStock()
{
}

CStock::~CStock()
{
}


BEGIN_MESSAGE_MAP(CStock, CGraphicCombo)
	//{{AFX_MSG_MAP(CStock)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/****************************************************************************
*                                CStock::Load
* Inputs:
*       int sel: Selection code
* Result: void
*       
* Effect: 
*       Loads up the combo box with the stock colors, and selects the one
*	indicated as 'sel'
****************************************************************************/

void CStock::Load(int sel)
    {
     AddStrings(stock);
     Select(sel);
    }


/////////////////////////////////////////////////////////////////////////////
// CStock message handlers
void CStock::drawSample(LPRECT r0, LPDRAWITEMSTRUCT dis) 
{
     CDC * dc = CDC::FromHandle(dis->hDC);


     CRgn rgn;
     CRect r = *r0;

     rgn.CreateRectRgnIndirect(r);

     int saved = dc->SaveDC();
     
     dc->SelectStockObject(dis->itemData);
     dc->SelectStockObject(BLACK_PEN);
     dc->SelectClipRgn(&rgn);

     dc->Rectangle(&r);

     dc->RestoreDC(saved);
}

// PenSel.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "dcvals.h"
#include "intedit.h"
#include "fltedit.h"
#include "pensamp.h"
#include "penstyle.h"
#include "LineJoin.h"
#include "BrStyle.h"
#include "endcaps.h"
#include "PenSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData pens[] = {
	{ IDS_BLACK_PEN, BLACK_PEN},
	{ IDS_WHITE_PEN, WHITE_PEN},
	{ IDS_NULL_PEN,  NULL_PEN},
	{ 0, 0 } // EOT
		       };

static IDData styles_cosmetic_1[] = {
	{ IDS_PS_SOLID,   PS_SOLID},
	{ IDS_PS_DASH,    PS_DASH},
        { IDS_PS_DOT,     PS_DOT},
        { IDS_PS_DASHDOT, PS_DASHDOT},
	{ IDS_PS_DASHDOTDOT, PS_DASHDOTDOT},
	{ IDS_PS_NULL,    PS_NULL},
	{ IDS_PS_INSIDEFRAME, PS_INSIDEFRAME},
	{ 0, 0} // EOT
				    };

static IDData styles_cosmetic_n[] = {
	{ IDS_PS_SOLID,   PS_SOLID},
	{ IDS_PS_NULL,    PS_NULL},
	{ IDS_PS_INSIDEFRAME, PS_INSIDEFRAME},
	{ 0, 0} // EOT
				    };

static IDData styles_geometric[] = {
	{ IDS_PS_SOLID,   PS_SOLID},
	{ IDS_PS_DASH,    PS_DASH},
        { IDS_PS_DOT,     PS_DOT},
        { IDS_PS_DASHDOT, PS_DASHDOT},
	{ IDS_PS_DASHDOTDOT, PS_DASHDOTDOT},
	{ IDS_PS_NULL,    PS_NULL},
	{ IDS_PS_INSIDEFRAME, PS_INSIDEFRAME},
	{ IDS_PS_ALTERNATE,   PS_ALTERNATE},
     //	{ IDS_PS_USERSTYLE,   PS_USERSTYLE},
	{ 0, 0} // EOT
				    };

static IDData joins[] = {
	{ IDS_PS_JOIN_BEVEL, PS_JOIN_BEVEL},
	{ IDS_PS_JOIN_MITER, PS_JOIN_MITER},
	{ IDS_PS_JOIN_ROUND, PS_JOIN_ROUND},
	{ 0, 0} // EOT
			};

static IDData endcaps[] = {
	{ IDS_PS_ENDCAP_ROUND, PS_ENDCAP_ROUND},
	{ IDS_PS_ENDCAP_SQUARE, PS_ENDCAP_SQUARE},
	{ IDS_PS_ENDCAP_FLAT,  PS_ENDCAP_FLAT},
	{ 0, 0} // EOT
			  };

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
// CPenSel property page

IMPLEMENT_DYNCREATE(CPenSel, CPropertyPage)

CPenSel::CPenSel() : CPropertyPage(CPenSel::IDD)
{
	//{{AFX_DATA_INIT(CPenSel)
	//}}AFX_DATA_INIT
	current_pen = NULL;
	initialized = FALSE;
}

CPenSel::~CPenSel()
{
}	    


/****************************************************************************
*                           CPenSel::changePenStyle
* Result: void
*       
* Effect: 
*       Changes the loading of the pen styles
****************************************************************************/

void CPenSel::changePenStyle()
    {
     // Note: we might be called because of the autobuddy changes
     // on the spin control, and all the controls may not be yet
     // initialized.  So we have to check for this case

     if(c_StockPen.m_hWnd == NULL)
	return;
     if(c_Cosmetic.m_hWnd == NULL)
	return;
     if(c_Geometric.m_hWnd == NULL)
	return;

     // If there is a style selected, we may have to reload the
     // styles selected
     if(!c_StockPen.GetCheck())
        { /* non-stock */
	 if(c_Geometric.GetCheck())
	    { /* geometric */
	     // If we are using a geometric pen, we need to make sure that
	     // the correct table is loaded.  
	     // Right now, the geometric pen is a superset of all others

	     DWORD item = c_PenStyles.GetCurItem();
	     if(c_PenStyles.GetCurSel() == CB_ERR)
		item = dcv->penstyle;
	     c_PenStyles.ResetContent();
	     c_PenStyles.AddStrings(styles_geometric);
	     c_PenStyles.Select(item);
	     return; // that was easy!
	    } /* geometric */

	 if(c_Cosmetic.GetCheck())
	    { /* cosmetic */
	     // Cosmetic pens produce more problems.  If we had a geometric
	     // pen, we won't necessarily have a corresponding entry for
	     // a cosmetic pen; if we had a cosmetic pen of size < 2, we
	     // won't necessarily have all the options available if the
	     // reason we are called was a pen size change and it is now > 1.

	     DWORD item = c_PenStyles.GetCurItem();
	     if(c_PenStyles.GetCurSel() == CB_ERR)
		item = dcv->penstyle;

	     c_PenStyles.ResetContent();
	     c_PenStyles.AddStrings(c_PenWidth.GetVal() > 1
	     				? styles_cosmetic_n 
					: styles_cosmetic_1);

	     // Make the same selection we had, unless it is no longer
	     // available, in which case we default to BS_SOLID
	     if(c_PenStyles.Select(item) == CB_ERR)
		c_PenStyles.Select(BS_SOLID);
	    } /* cosmetic */
	} /* non-stock */
     
    }

/****************************************************************************
*                           CPenSel::updateControls
* Result: void
*       
* Effect: 
*       Enables conditional controls
****************************************************************************/

void CPenSel::updateControls()
    {
     c_StockPens.EnableWindow(c_StockPen.GetCheck());

     c_SolidPens.EnableWindow(c_Geometric.GetCheck() || c_Cosmetic.GetCheck());
     c_c_Color.EnableWindow(c_Geometric.GetCheck() || c_Cosmetic.GetCheck());

     c_c_PenStyle.EnableWindow(!c_StockPen.GetCheck());

     c_c_EndCaps.EnableWindow(c_Geometric.GetCheck());
     c_EndCaps.EnableWindow(c_Geometric.GetCheck());

     c_BrushStyle.EnableWindow(c_Geometric.GetCheck());
     c_c_BrushStyle.EnableWindow(c_Geometric.GetCheck());

     c_LineJoins.EnableWindow(c_Geometric.GetCheck());
     c_c_LineJoin.EnableWindow(c_Geometric.GetCheck());

     c_c_Width.EnableWindow(!c_StockPen.GetCheck());
     c_PenSpin.EnableWindow(!c_StockPen.GetCheck());
     c_PenWidth.EnableWindow(!c_StockPen.GetCheck());

     BOOL ismiter = (c_LineJoins.GetCurItem() == PS_JOIN_MITER) 
     					     && c_Geometric.GetCheck();
     c_MiterSpin.EnableWindow(ismiter);
     c_MiterAngle.EnableWindow(ismiter);
     c_MiterLimit.EnableWindow(ismiter);
     c_MiterAngleSpin.EnableWindow(ismiter);
     c_c_MiterLimit.EnableWindow(ismiter);
     c_c_MiterAngle.EnableWindow(ismiter);
    }

void CPenSel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPenSel)
	DDX_Control(pDX, IDC_MITERSPIN, c_MiterSpin);
	DDX_Control(pDX, IDC_MITERANGLE, c_MiterAngle);
	DDX_Control(pDX, IDC_MITERLIMIT, c_MiterLimit);
	DDX_Control(pDX, IDC_MITERANGLESPIN, c_MiterAngleSpin);
	DDX_Control(pDX, IDC_MITER_CAPTION, c_c_MiterLimit);
	DDX_Control(pDX, IDC_ANGLE_CAPTION, c_c_MiterAngle);
	DDX_Control(pDX, IDC_WIDTH_CAPTION, c_c_Width);
	DDX_Control(pDX, IDC_TRANSPARENT, c_Transparent);
	DDX_Control(pDX, IDC_OPAQUE, c_Opaque);
	DDX_Control(pDX, IDC_PENSTYLE_CAPTION, c_c_PenStyle);
	DDX_Control(pDX, IDC_LINEJOIN_CAPTION, c_c_LineJoin);
	DDX_Control(pDX, IDC_BRUSH_CAPTION, c_c_BrushStyle);
	DDX_Control(pDX, IDC_ENDCAPS_CAPTION, c_c_EndCaps);
	DDX_Control(pDX, IDC_COLOR_CAPTION, c_c_Color);
	DDX_Control(pDX, IDC_PENORGY, c_PenOrgY);
	DDX_Control(pDX, IDC_PENORGX, c_PenOrgX);
	DDX_Control(pDX, IDC_PENSPIN, c_PenSpin);
	DDX_Control(pDX, IDC_PENWIDTH, c_PenWidth);
	DDX_Control(pDX, IDC_GEOMETRIC, c_Geometric);
	DDX_Control(pDX, IDC_COSMETIC, c_Cosmetic);
	DDX_Control(pDX, IDC_PENSAMPLE, c_Sample);
	DDX_Control(pDX, IDC_STOCKPENS, c_StockPens);
	DDX_Control(pDX, IDC_STOCKPEN, c_StockPen);
	DDX_Control(pDX, IDC_SOLIDPENS, c_SolidPens);
 	DDX_Control(pDX, IDC_PENSTYLES, c_PenStyles);
 	DDX_Control(pDX, IDC_ENDCAPS, c_EndCaps);
 	DDX_Control(pDX, IDC_LINEJOIN, c_LineJoins);
 	DDX_Control(pDX, IDC_BRUSHSTYLE, c_BrushStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPenSel, CPropertyPage)
	//{{AFX_MSG_MAP(CPenSel)
	ON_BN_CLICKED(IDC_STOCKPEN, OnStockpen)
	ON_CBN_SELENDOK(IDC_ENDCAPS, OnSelendokEndcaps)
	ON_CBN_SELENDOK(IDC_STOCKPENS, OnSelendokStockpens)
	ON_BN_CLICKED(IDC_GEOMETRIC, OnGeometric)
	ON_BN_CLICKED(IDC_COSMETIC, OnCosmetic)
	ON_CBN_SELENDOK(IDC_BRUSHSTYLE, OnSelendokBrushstyle)
	ON_CBN_SELENDOK(IDC_LINEJOIN, OnSelendokLinejoin)
	ON_CBN_SELENDOK(IDC_PENSTYLES, OnSelendokPenstyles)
	ON_EN_CHANGE(IDC_PENWIDTH, OnChangePenwidth)
	ON_CBN_SELENDOK(IDC_SOLIDPENS, OnSelendokSolidpens)
	ON_BN_CLICKED(IDC_OPAQUE, OnOpaque)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	ON_EN_CHANGE(IDC_MITERANGLE, OnChangeMiterangle)
	ON_EN_CHANGE(IDC_MITERLIMIT, OnChangeMiterlimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenSel message handlers
void CPenSel::updateSample()
{
 if(c_Sample.m_hWnd == NULL)
    return;  // autobuddy startup

 c_Sample.pen = current_pen;
 c_Sample.bkcolor = dcv->current_bkcolor;
 c_Sample.bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE;
 c_Sample.miterlimit = c_MiterLimit.GetVal();
 c_Sample.join = (c_LineJoins.GetCurItem() == PS_JOIN_MITER) 
     					     && c_Geometric.GetCheck();
 c_Sample.angle = c_MiterAngle.GetVal();

 c_Sample.InvalidateRect(NULL);
}

void CPenSel::setPen()
{
 if(!initialized)
    return;
 // create a new pen

 if(current_pen != NULL)
    delete current_pen;
 
 current_pen = NULL;

 if(c_StockPen.GetCheck())
    { /* stock pen */
     current_pen = new CPen;
     current_pen->CreateStockObject(c_StockPens.GetCurItem());
    } /* stock pen */
 else
    { /* custom pen */
     current_pen = new CPen;
     if(c_Geometric.GetCheck())
        { /* geometric */
	 LOGBRUSH lb;

	 lb.lbStyle = LOWORD(c_BrushStyle.GetCurItem());
	 lb.lbColor = c_SolidPens.GetCurItem();
	 lb.lbHatch = HIWORD(c_BrushStyle.GetCurItem());

	 current_pen->CreatePen(PS_GEOMETRIC | c_EndCaps.GetCurItem() |
	 			c_PenStyles.GetCurItem() |
				c_LineJoins.GetCurItem(), 
	 				c_PenWidth.GetVal(), &lb);

	} /* geometric */
     else
        { /* cosmetic */
	 current_pen->CreatePen(PS_COSMETIC | c_PenStyles.GetCurItem(),
	 				c_PenWidth.GetVal(), 
					c_SolidPens.GetCurItem());

	} /* cosmetic */
    } /* custom pen */

 updateSample();
}



void CPenSel::OnOK()
{
 CPropertyPage::OnOK();

 dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();
 dcv->pensel = SEL_STOCK;
 if(c_Cosmetic.GetCheck())
    dcv->pensel = SEL_COSMETIC;
 if(c_Geometric.GetCheck())
    dcv->pensel = SEL_GEOMETRIC;
 if(current_pen != NULL)
    { /* replacement pen */
     if(dcv->pen != NULL)
	delete dcv->pen;
     dcv->pen = current_pen;
    } /* replacement pen */

 dcv->stockpen = c_StockPens.GetCurItem();
 dcv->pensolid = c_SolidPens.GetCurItem();
 dcv->endcaps = c_EndCaps.GetCurItem();
 dcv->linejoin = c_LineJoins.GetCurItem();
 dcv->lbstyle  = c_BrushStyle.GetCurItem();
 dcv->penwidth = c_PenWidth.GetVal();
 dcv->penstyle = c_PenStyles.GetCurItem();
 dcv->penposition.x = c_PenOrgX.GetVal();
 dcv->penposition.y = c_PenOrgY.GetVal();
 dcv->miterlimit = c_MiterLimit.GetVal();
 dcv->angle = c_MiterAngle.GetVal();
 dcv->BkMode = dcv->current_bkmode;
}
	
	    

BOOL CPenSel::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	int sel = IDC_STOCKPEN;

 	switch(dcv->pensel)
	   { /* pensel */
	    case SEL_STOCK:
		    sel = IDC_STOCKPEN;
		    break;
	    case SEL_COSMETIC:
		    sel = IDC_COSMETIC;
		    break;
	    case SEL_GEOMETRIC:
		    sel = IDC_GEOMETRIC;
		    break;
	   } /* pensel */


	int bkmode = (dcv->current_bkmode == 0 ? dcv->BkMode : dcv->current_bkmode);

	CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, bkmode == OPAQUE
				? IDC_OPAQUE : IDC_TRANSPARENT);

 	CheckRadioButton(IDC_STOCKPEN, IDC_GEOMETRIC, sel);

	c_PenOrgX.SetVal(dcv->penposition.x);
	c_PenOrgY.SetVal(dcv->penposition.y);
	
 	c_SolidPens.Load(dcv->pensolid);
	
	c_StockPens.AddStrings(pens);
	c_StockPens.Select(dcv->stockpen);

	c_BrushStyle.AddStrings(brushstyles);
	c_BrushStyle.Select(dcv->lbstyle);

	c_EndCaps.AddStrings(endcaps);
	c_EndCaps.Select(dcv->endcaps);

	c_LineJoins.AddStrings(joins);
	c_LineJoins.Select(dcv->linejoin);

	c_PenSpin.SetPos(dcv->penwidth);
	c_PenSpin.SetRange(0, 20);  // random range, 0..something

	c_MiterAngleSpin.SetRange(0,359);
	c_MiterAngle.SetVal(dcv->angle);
	c_MiterSpin.SetRange(0, 20);

	// Spin controls assume integers, so we first set the position so
	// that the buddy control gets set to an integer, then we set the
	// real floating point value.
	c_MiterSpin.SetPos((int)dcv->miterlimit); 
	c_MiterLimit.SetVal(dcv->miterlimit);

	{
	 CClientDC dc(this);
	 SIZE w = dc.GetTextExtent(_T("WWW"));
	 c_PenStyles.SetWidth(w.cx);
	}

	initialized = TRUE;

	changePenStyle();

	updateControls();

	setPen();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPenSel::OnStockpen() 
{
 updateControls();
 setPen();
}

void CPenSel::OnSelendokEndcaps() 
{
 setPen();
}


void CPenSel::OnSelendokStockpens() 
{
 setPen();
}

void CPenSel::OnGeometric() 
{
 changePenStyle();
 updateControls();
 setPen();
}

void CPenSel::OnCosmetic() 
{
 changePenStyle();
 updateControls();
 setPen();
}

void CPenSel::OnSelendokBrushstyle() 
{
 setPen();
}

void CPenSel::OnSelendokLinejoin() 
{
 setPen();
 updateControls();	
}

void CPenSel::OnSelendokPenstyles() 
{
 setPen();
}

void CPenSel::OnChangePenwidth() 
{
 setPen();
 changePenStyle();
}

void CPenSel::OnSelendokSolidpens() 
{
 setPen();
}

/****************************************************************************
*                             CPenSel::OnSetActive
* Result: BOOL
*       As returned by superclass
* Effect: 
*       Makes sure current control settings are consistent with other pages.
****************************************************************************/

BOOL CPenSel::OnSetActive() 
{
	switch(dcv->current_bkmode)
	   { /* change mode */
	    case OPAQUE:
		    CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, IDC_OPAQUE);
		    break;
	    case TRANSPARENT:
		    CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, IDC_TRANSPARENT);
		    break;
	   } /* change mode */
	
	setPen();
	return CPropertyPage::OnSetActive();
}

void CPenSel::OnOpaque() 
{
 setPen();
}

void CPenSel::OnTransparent() 
{
 setPen();
}

BOOL CPenSel::OnKillActive() 
{
 dcv->current_bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE; 
	
	return CPropertyPage::OnKillActive();
}

void CPenSel::OnChangeMiterangle() 
{
 updateSample();
	
}

void CPenSel::OnChangeMiterlimit() 
{
 updateSample();
	
}

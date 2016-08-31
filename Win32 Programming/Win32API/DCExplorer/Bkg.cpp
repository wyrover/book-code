// Bkg.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "dcvals.h"
#include "is95.h"

#include "idcombo.h"
#include "gcombo.h"
#include "brstyle.h"
#include "color.h"
#include "intedit.h"
#include "poly.h"
#include "hatch.h"
#include "hatchsmp.h"
#include "stock.h"
#include "ropcodes.h"
#include "syscolor.h"

#include "Bkg.h"
#include "sample.h"
#include "rop2.h"
#include "bkgsamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBkg property page

IMPLEMENT_DYNCREATE(CBkg, CPropertyPage)

CBkg::CBkg() : CPropertyPage(CBkg::IDD)
{
	//{{AFX_DATA_INIT(CBkg)
	//}}AFX_DATA_INIT
}

CBkg::~CBkg()
{
}

void CBkg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkg)
	DDX_Control(pDX, IDC_WNDCOLOR, c_WindowColor);
	DDX_Control(pDX, IDC_SPINY, c_SpinY);
	DDX_Control(pDX, IDC_SPINX, c_SpinX);
	DDX_Control(pDX, IDC_HATCHSAMPLE, c_HatchBrushSample);
	DDX_Control(pDX, IDC_ORIGIN_CAPTION, c_c_Origin);
	DDX_Control(pDX, IDC_BRUSHEXAMPLE, c_HatchSample);
	DDX_Control(pDX, IDC_BKCOLOR, c_BkColor);
	DDX_Control(pDX, IDC_HATCHBRUSH, c_HatchBrush);
	DDX_Control(pDX, IDC_SOLIDBRUSH, c_SolidBrush);
	DDX_Control(pDX, IDC_STOCKBRUSH, c_StockBrush);
	DDX_Control(pDX, IDC_POLYEXAMPLE, c_Poly);
	DDX_Control(pDX, IDC_BRUSHORGY, c_BrushOrgY);
	DDX_Control(pDX, IDC_BRUSHORGX, c_BrushOrgX);
	DDX_Control(pDX, IDC_RASTEROP, c_RasterOp);
	DDX_Control(pDX, IDC_WINDING, c_Winding);
	DDX_Control(pDX, IDC_TRANSPARENT, c_Transparent);
	DDX_Control(pDX, IDC_STOCKBRUSHES, c_StockBrushes);
	DDX_Control(pDX, IDC_SOLIDBRUSHES, c_SolidBrushes);
	DDX_Control(pDX, IDC_OPAQUE, c_Opaque);
	DDX_Control(pDX, IDC_HATCHBRUSHES, c_HatchBrushes);
	DDX_Control(pDX, IDC_ALTERNATE, c_Alternate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBkg, CPropertyPage)
	//{{AFX_MSG_MAP(CBkg)
	ON_CBN_SELENDOK(IDC_HATCHBRUSHES, OnSelendokHatchbrushes)
	ON_CBN_SELENDOK(IDC_RASTEROP, OnSelendokRasterop)
	ON_CBN_SELENDOK(IDC_SOLIDBRUSHES, OnSelendokSolidbrushes)
	ON_CBN_SELENDOK(IDC_STOCKBRUSHES, OnSelendokStockbrushes)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	ON_BN_CLICKED(IDC_WINDING, OnWinding)
	ON_BN_CLICKED(IDC_STOCKBRUSH, OnStockbrush)
	ON_BN_CLICKED(IDC_SOLIDBRUSH, OnSolidbrush)
	ON_BN_CLICKED(IDC_OPAQUE, OnOpaque)
	ON_BN_CLICKED(IDC_HATCHBRUSH, OnHatchbrush)
	ON_BN_CLICKED(IDC_ALTERNATE, OnAlternate)
	ON_BN_CLICKED(IDC_SHOWROP, OnShowrop)
	ON_CBN_SELENDOK(IDC_BKCOLOR, OnSelendokBkcolor)
	ON_EN_CHANGE(IDC_BRUSHORGX, OnChangeBrushorgx)
	ON_EN_CHANGE(IDC_BRUSHORGY, OnChangeBrushorgy)
	ON_CBN_SELENDOK(IDC_WNDCOLOR, OnSelendokWndcolor)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBkg message handlers

void CBkg::updateControls()
   {
    c_StockBrushes.EnableWindow(c_StockBrush.GetCheck());
    c_SolidBrushes.EnableWindow(c_SolidBrush.GetCheck() || c_HatchBrush.GetCheck());
    c_HatchBrushes.EnableWindow(c_HatchBrush.GetCheck());
    c_HatchSample.BkMode = c_Opaque.GetCheck() ? OPAQUE : TRANSPARENT;
    c_c_Origin.EnableWindow(c_HatchBrush.GetCheck());
    c_BrushOrgX.EnableWindow(c_HatchBrush.GetCheck());
    c_BrushOrgY.EnableWindow(c_HatchBrush.GetCheck());
    c_SpinX.EnableWindow(c_HatchBrush.GetCheck());
    c_SpinY.EnableWindow(c_HatchBrush.GetCheck());
    c_HatchBrushSample.ShowWindow(c_HatchBrush.GetCheck() ? SW_SHOW : SW_HIDE);
   }

void CBkg::OnSelendokHatchbrushes() 
{
 updateBrushSample();
	
}

void CBkg::OnSelendokRasterop() 
{
	// TODO: Add your control notification handler code here
	
}

void CBkg::OnSelendokSolidbrushes() 
{
 updateBrushSample();
}

void CBkg::OnSelendokStockbrushes() 
{
	// TODO: Add your control notification handler code here
	
}

void CBkg::OnTransparent() 
{
 c_HatchSample.BkMode = TRANSPARENT;
 c_HatchSample.InvalidateRect(NULL);
 updateBrushSample();
	
}

void CBkg::OnWinding() 
{
 c_Poly.fillmode = WINDING;
 c_Poly.InvalidateRect(NULL, TRUE);
}

void CBkg::OnStockbrush() 
{
 updateControls();
}

void CBkg::OnSolidbrush() 
{
 updateControls();
}

void CBkg::OnOpaque() 
{
 c_HatchSample.BkMode = OPAQUE;
 c_HatchSample.InvalidateRect(NULL);
 updateBrushSample();
}

void CBkg::OnHatchbrush() 
{
 updateControls();
	
}

void CBkg::OnAlternate() 
{
 c_Poly.fillmode = ALTERNATE;
 c_Poly.InvalidateRect(NULL, TRUE);
}

void CBkg::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnCancel();
}

/****************************************************************************
*                          CBkg::createSelectedBrush
* Result: CBrush *
*       A brush that corresponds to the selection made by the user
* Effect: 
*       Creates the brush
****************************************************************************/

CBrush * CBkg::createSelectedBrush()
    {
     CBrush * br = new CBrush;
     if(c_StockBrush.GetCheck())
        { /* stock brush */
	 br->CreateStockObject(c_StockBrushes.GetCurItem());
	 return br;
	} /* stock brush */
     if(c_SolidBrush.GetCheck())
        { /* solid brush */
	 br->CreateSolidBrush(c_SolidBrushes.GetCurItem());
	 return br;
	} /* solid brush */
     if(c_HatchBrush.GetCheck())
        { /* hatch brush */
	 LOGBRUSH lb;
	 DWORD brush = c_HatchBrushes.GetCurItem();
	 lb.lbStyle = LOWORD(brush);
	 lb.lbColor = c_SolidBrushes.GetCurItem();
	 lb.lbHatch = HIWORD(brush);
	 br->CreateBrushIndirect(&lb);
	 return br;
	} /* hatch brush */
     return NULL;  // should never get here
    }

void CBkg::OnOK() 
{
	CPropertyPage::OnOK();

 	dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();

 	dcv->BkMode = dcv->current_bkmode;
	dcv->fillmode = c_Winding.GetCheck() ? WINDING : ALTERNATE;
	dcv->rop = c_RasterOp.GetCurItem();
	dcv->hatchsel = c_HatchBrushes.GetCurItem();
	dcv->stocksel = c_StockBrushes.GetCurItem();
	dcv->solidsel = c_SolidBrushes.GetCurItem();

	dcv->brushsel = c_StockBrush.GetCheck() ? SEL_STOCK :
			c_SolidBrush.GetCheck() ? SEL_SOLID : SEL_HATCH;

        dcv->BkColor  = c_BkColor.GetCurItem();

	dcv->wcolor   = c_WindowColor.GetCurItem();

	if(dcv->brush != NULL)
	   delete dcv->brush;
        dcv->brush = createSelectedBrush();
}

/****************************************************************************
*                             CBkg::OnInitDialog
* Result: BOOL
*       TRUE, always (use default focus)
* Effect: 
*       Initializes all controls
****************************************************************************/

BOOL CBkg::OnInitDialog() 
{

	CPropertyPage::OnInitDialog();
	
 	int bkmode = (dcv->current_bkmode == 0 ? dcv->BkMode 
					       : dcv->current_bkmode);
 	CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, 
		(bkmode == OPAQUE ? IDC_OPAQUE : IDC_TRANSPARENT));

        CheckRadioButton(IDC_ALTERNATE, IDC_WINDING,
		(dcv->fillmode == ALTERNATE ? IDC_ALTERNATE : IDC_WINDING));
	
 	int sel = 0;

        switch(dcv->brushsel)
	   { /* selection */
	    case SEL_STOCK:
		    sel = IDC_STOCKBRUSH;
		    break;
	    case SEL_SOLID:
		    sel = IDC_SOLIDBRUSH;
		    break;
	    case SEL_HATCH:
		    sel = IDC_HATCHBRUSH;
		    break;
	   } /* selection */

	CheckRadioButton(IDC_STOCKBRUSH, IDC_HATCHBRUSH, sel);

	c_RasterOp.Load(dcv->rop);

	c_HatchBrushes.Load(dcv->hatchsel);

	c_StockBrushes.Load(dcv->stocksel);

	c_SolidBrushes.Load(dcv->solidsel);

	c_BkColor.Load(dcv->BkColor);

	c_WindowColor.Load(dcv->wcolor);

	c_Poly.fillmode = dcv->fillmode;
	updateControls();

	c_BrushOrgX.SetVal(dcv->brushorg.x);
	c_BrushOrgY.SetVal(dcv->brushorg.y);

	if(Is95())
	   { /* win95 */
	    c_SpinX.SetRange(0,7);
	    c_SpinY.SetRange(0,7);
	    // For Win95 only, force 0..7 wrap
	    // Note that there is no CWnd::SetStyle function!
	    ::SetWindowLong(c_SpinX.m_hWnd, GWL_STYLE, 
	    			c_SpinX.GetStyle() | UDS_WRAP);
	    ::SetWindowLong(c_SpinY.m_hWnd, GWL_STYLE, 
	    			c_SpinY.GetStyle() | UDS_WRAP);
	   } /* win95 */
	else
	   { /* NT */
	    c_SpinX.SetRange(-200,200);
	    c_SpinY.SetRange(-200,200);
	   } /* NT */

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBkg::OnShowrop() 
{
 CRop2 dlg;
 int sel;

 sel = c_RasterOp.GetCurSel();
 c_RasterOp.GetLBText(sel, dlg.ropname);
 dlg.rop = c_RasterOp.GetCurItem();

 dlg.DoModal();
	
 c_RasterOp.Select(dlg.rop);
}

void CBkg::OnSelendokBkcolor() 
{
 c_HatchSample.bkg = c_BkColor.GetCurItem();
 c_HatchSample.InvalidateRect(NULL);
 updateBrushSample();
}

BOOL CBkg::OnSetActive() 
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
	
	return CPropertyPage::OnSetActive();
}

BOOL CBkg::OnKillActive() 
{
 dcv->current_bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE; 
 dcv->current_bkcolor = c_BkColor.GetCurItem();
	
	return CPropertyPage::OnKillActive();
}

/****************************************************************************
*                           CBkg::updateBrushSample
* Result: void
*       
* Effect: 
*       Sets up the brush parameters for the brush sample and forces it
*	to redraw.
****************************************************************************/

void CBkg::updateBrushSample()
{

 // Because the autobuddy control can cause change events as it is loaded
 // by its spin control, we can get updates arriving before all the
 // controls are initialized.  

 if(!::IsWindow(c_HatchBrushes.m_hWnd))
    return;
 if(!::IsWindow(c_SolidBrushes.m_hWnd))
    return;
 if(!::IsWindow(c_Transparent.m_hWnd))
    return;
 if(!::IsWindow(c_BkColor.m_hWnd))
    return;

 DWORD brush = c_HatchBrushes.GetCurItem();
 c_HatchBrushSample.br.lbStyle = LOWORD(brush);
 c_HatchBrushSample.br.lbColor = c_SolidBrushes.GetCurItem();
 c_HatchBrushSample.br.lbHatch = HIWORD(brush);
 c_HatchBrushSample.org.x = c_BrushOrgX.GetVal();
 c_HatchBrushSample.org.y = c_BrushOrgY.GetVal();
 c_HatchBrushSample.bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE;
 c_HatchBrushSample.bkcolor = c_BkColor.GetCurItem();
 c_HatchBrushSample.InvalidateRect(NULL);
}


void CBkg::OnChangeBrushorgx() 
{
 updateBrushSample();
}

void CBkg::OnChangeBrushorgy() 
{
 updateBrushSample();
}

void CBkg::OnSelendokWndcolor() 
{
	// TODO: Add your control notification handler code here
	
}

void CBkg::OnShow() 
{
 CBkgSample dlg;

 
 if(c_StockBrush.GetCheck())
    { /* stock */
     dlg.brush.CreateStockObject(c_StockBrushes.GetCurItem());
    } /* stock */

 if(c_SolidBrush.GetCheck())
    { /* solid */
     dlg.brush.CreateSolidBrush(c_SolidBrushes.GetCurItem());
    } /* solid */

 if(c_HatchBrush.GetCheck())
    { /* hatched */
     DWORD brush  = c_HatchBrushes.GetCurItem();
     LOGBRUSH br;
     br.lbStyle = LOWORD(brush);
     br.lbColor = c_SolidBrushes.GetCurItem();
     br.lbHatch = HIWORD(brush);
     dlg.brush.CreateBrushIndirect(&br);
    } /* hatched */
 
 dlg.org.x = c_BrushOrgX.GetVal();
 dlg.org.y = c_BrushOrgY.GetVal();
 dlg.bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE;
 dlg.bkcolor = c_BkColor.GetCurItem();

 dlg.DoModal();

 dlg.brush.DeleteObject();
	
}

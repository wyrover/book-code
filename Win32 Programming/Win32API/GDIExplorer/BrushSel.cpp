// BrushSel.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "idcombo.h"
#include "gcombo.h"
#include "stock.h"
#include "color.h"
#include "syscolor.h"
#include "intedit.h"
#include "hatchsmp.h"
#include "brstyle.h"
#include "is95.h"

#include "BrushSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrushSelector property page

IMPLEMENT_DYNCREATE(CBrushSelector, CPropertyPage)

CBrushSelector::CBrushSelector() : CPropertyPage(CBrushSelector::IDD)
{
        //{{AFX_DATA_INIT(CBrushSelector)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CBrushSelector::~CBrushSelector()
{
}


void CBrushSelector::updateControls()
   {
    c_StockBrushes.EnableWindow(c_StockBrush.GetCheck());
    c_SolidBrushes.EnableWindow(c_SolidBrush.GetCheck() || c_HatchBrush.GetCheck());
    c_HatchBrushes.EnableWindow(c_HatchBrush.GetCheck());
    //c_HatchSample.BkMode = c_Opaque.GetCheck() ? OPAQUE : TRANSPARENT;
    c_c_Origin.EnableWindow(c_HatchBrush.GetCheck());
    c_BrushOrgX.EnableWindow(c_HatchBrush.GetCheck());
    c_BrushOrgY.EnableWindow(c_HatchBrush.GetCheck());
    c_SpinX.EnableWindow(c_HatchBrush.GetCheck());
    c_SpinY.EnableWindow(c_HatchBrush.GetCheck());
    c_HatchBrushSample.ShowWindow(c_HatchBrush.GetCheck() ? SW_SHOW : SW_HIDE);
   }

void CBrushSelector::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CBrushSelector)
        DDX_Control(pDX, IDC_HATCHSAMPLE, c_HatchBrushSample);
        DDX_Control(pDX, IDC_ORIGIN_CAPTION, c_c_Origin);
        DDX_Control(pDX, IDC_STOCKBRUSHES, c_StockBrushes);
        DDX_Control(pDX, IDC_HATCHBRUSHES, c_HatchBrushes);
        DDX_Control(pDX, IDC_SOLIDBRUSHES, c_SolidBrushes);
        DDX_Control(pDX, IDC_HATCHBRUSH, c_HatchBrush);
        DDX_Control(pDX, IDC_SOLIDBRUSH, c_SolidBrush);
        DDX_Control(pDX, IDC_STOCKBRUSH, c_StockBrush);
        DDX_Control(pDX, IDC_BRUSHORGY, c_BrushOrgY);
        DDX_Control(pDX, IDC_BRUSHORGX, c_BrushOrgX);
        DDX_Control(pDX, IDC_SPINY, c_SpinY);
        DDX_Control(pDX, IDC_SPINX, c_SpinX);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrushSelector, CPropertyPage)
        //{{AFX_MSG_MAP(CBrushSelector)
        ON_CBN_SELENDOK(IDC_HATCHBRUSHES, OnSelendokHatchbrushes)
        ON_CBN_SELENDOK(IDC_SOLIDBRUSHES, OnSelendokSolidbrushes)
        ON_CBN_SELENDOK(IDC_STOCKBRUSHES, OnSelendokStockbrushes)
        ON_BN_CLICKED(IDC_HATCHBRUSH, OnHatchbrush)
        ON_BN_CLICKED(IDC_STOCKBRUSH, OnStockbrush)
        ON_BN_CLICKED(IDC_SOLIDBRUSH, OnSolidbrush)
        ON_EN_CHANGE(IDC_BRUSHORGX, OnChangeBrushorgx)
        ON_EN_CHANGE(IDC_BRUSHORGY, OnChangeBrushorgy)
        ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrushSelector message handlers

BOOL CBrushSelector::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        c_StockBrushes.Load();
        c_StockBrushes.Select(BLACK_BRUSH);

        c_SolidBrushes.Load(IDS_BLACK);  // use default black brush

        c_HatchBrushes.Load();
        c_HatchBrushes.Select(HS_FDIAGONAL);

        CheckRadioButton(IDC_STOCKBRUSH, IDC_HATCHBRUSH, IDC_STOCKBRUSH);

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

        updateControls();
        updateBrushSample();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrushSelector::OnHatchbrush() 
{
 updateControls();
}

void CBrushSelector::OnStockbrush() 
{
 updateControls();
}

void CBrushSelector::OnSolidbrush() 
{
 updateControls();
}

void CBrushSelector::OnSelendokStockbrushes() 
{
        // TODO: Add your control notification handler code here
        
}

void CBrushSelector::OnSelendokHatchbrushes() 
{
 updateBrushSample();
        
}

void CBrushSelector::OnSelendokSolidbrushes() 
{
 updateBrushSample();
}

void CBrushSelector::OnChangeBrushorgx() 
{
 updateBrushSample();
}

void CBrushSelector::OnChangeBrushorgy() 
{
 updateBrushSample();
}

/****************************************************************************
*                      CBrushSelector::updateBrushSample
* Result: void
*       
* Effect: 
*       Sets up the brush parameters for the brush sample and forces it
*       to redraw.
****************************************************************************/

void CBrushSelector::updateBrushSample()
{

 // Because the autobuddy control can cause change events as it is loaded
 // by its spin control, we can get updates arriving before all the
 // controls are initialized.  

 if(!::IsWindow(c_HatchBrushes.m_hWnd))
    return;
 if(!::IsWindow(c_SolidBrushes.m_hWnd))
    return;
 // if(!::IsWindow(c_Transparent.m_hWnd))
 //   return;
 // if(!::IsWindow(c_BkColor.m_hWnd))
 //   return;

 DWORD brush = c_HatchBrushes.GetCurItem();
 c_HatchBrushSample.br.lbStyle = LOWORD(brush);
 c_HatchBrushSample.br.lbColor = c_SolidBrushes.GetCurItem();
 c_HatchBrushSample.br.lbHatch = HIWORD(brush);
 c_HatchBrushSample.org.x = c_BrushOrgX.GetVal();
 c_HatchBrushSample.org.y = c_BrushOrgY.GetVal();
 //c_HatchBrushSample.bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE;
 //c_HatchBrushSample.bkcolor = c_BkColor.GetCurItem();

 c_HatchBrushSample.bkmode = TRANSPARENT;
 c_HatchBrushSample.bkcolor = RGB(255,255,255);

 c_HatchBrushSample.InvalidateRect(NULL);
}

BOOL CBrushSelector::OnKillActive() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        return CPropertyPage::OnKillActive();
}

/****************************************************************************
*                       CBrushSelector::OnQuerySiblings
* Inputs:
*       WPARAM: unused
*       LPARAM: (LOGBRUSH *) pointer to LOGBRUSH structure to be filled in
* Result: LRESULT
*       TRUE, always (the FALSE return should never happen!)
* Effect: 
*       Fills in the LOGBRUSH structure.  See Notes.
* Notes:
*       The lbStyle value is set to -1 for a stock brush, and the lbColor
*       is the stock brush index
****************************************************************************/

LRESULT CBrushSelector::OnQuerySiblings(WPARAM, LPARAM lbp)
    {
     LOGBRUSH * lb = (LOGBRUSH *)lbp;

     if(c_StockBrush.GetCheck())
        { /* solid */
         lb->lbStyle = (UINT)-1;   // bogus value
         lb->lbColor = c_StockBrushes.GetCurItem();
         lb->lbHatch = 0;   // actually, this is ignored
         return TRUE;
        } /* solid */
     else
     if(c_SolidBrush.GetCheck())
        { /* solid */
         lb->lbStyle = BS_SOLID;
         lb->lbColor = c_SolidBrushes.GetCurItem();
         lb->lbHatch = 0;   // actually, this is ignored
         return TRUE;
        } /* solid */
     else
     if(c_HatchBrush.GetCheck())
        { /* hatched */
         DWORD brush = c_HatchBrushes.GetCurItem();
         lb->lbStyle = LOWORD(brush);
         lb->lbColor = c_SolidBrushes.GetCurItem();
         lb->lbHatch = HIWORD(brush);
         return TRUE;
        } /* hatched */

     return FALSE;  // should never happen
    }

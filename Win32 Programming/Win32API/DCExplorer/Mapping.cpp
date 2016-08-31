// Mapping.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "dcvals.h"
#include "fltedit.h"
#include "intedit.h"
#include "message.h"
#include "sysmsg.h"
#include "idcombo.h"
#include "mapsamp.h"
#include "Mapping.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData mappingmodes[] = {
	{IDS_MM_TEXT, MM_TEXT},
	{IDS_MM_LOENGLISH, MM_LOENGLISH},
	{IDS_MM_HIENGLISH, MM_HIENGLISH},
	{IDS_MM_LOMETRIC,  MM_LOMETRIC},
	{IDS_MM_HIMETRIC,  MM_HIMETRIC},
	{IDS_MM_TWIPS,     MM_TWIPS},
	{IDS_MM_ISOTROPIC, MM_ISOTROPIC},
	{IDS_MM_ANISOTROPIC, MM_ANISOTROPIC},
	{0, 0} // EOT
			       };

/////////////////////////////////////////////////////////////////////////////
// CMapping property page

IMPLEMENT_DYNCREATE(CMapping, CPropertyPage)

CMapping::CMapping() : CPropertyPage(CMapping::IDD)
{
	//{{AFX_DATA_INIT(CMapping)
	//}}AFX_DATA_INIT
}

CMapping::~CMapping()
{
}

void CMapping::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapping)
	DDX_Control(pDX, IDC_VSCROLL, c_VScrollSample);
	DDX_Control(pDX, IDC_HSCROLL, c_HScrollSample);
	DDX_Control(pDX, IDC_WEXT_CAPTION, c_c_Wext);
	DDX_Control(pDX, IDC_VEXT_CAPTION, c_c_Vext);
	DDX_Control(pDX, IDC_ERROR, c_Error);
	DDX_Control(pDX, IDC_SAMPLE, c_Sample);
	DDX_Control(pDX, IDC_1000, c_1000);
	DDX_Control(pDX, IDC_100, c_100);
	DDX_Control(pDX, IDC_10, c_10);
	DDX_Control(pDX, IDC_SY, c_SY);
	DDX_Control(pDX, IDC_SX, c_SX);
	DDX_Control(pDX, IDC_SCALING, c_Scaling);
	DDX_Control(pDX, IDC_WORGY, c_WindowOrgY);
	DDX_Control(pDX, IDC_WORGX, c_WindowOrgX);
	DDX_Control(pDX, IDC_WEXTY, c_WindowExtY);
	DDX_Control(pDX, IDC_WEXTX, c_WindowExtX);
	DDX_Control(pDX, IDC_VORGY, c_ViewportOrgY);
	DDX_Control(pDX, IDC_VORGX, c_ViewportOrgX);
	DDX_Control(pDX, IDC_VEXTY, c_ViewportExtY);
	DDX_Control(pDX, IDC_VEXTX, c_ViewportExtX);
	DDX_Control(pDX, IDC_MAPPINGMODE, c_MappingMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapping, CPropertyPage)
	//{{AFX_MSG_MAP(CMapping)
	ON_CBN_SELENDOK(IDC_MAPPINGMODE, OnSelendokMappingmode)
	ON_EN_CHANGE(IDC_VEXTX, OnChangeVextx)
	ON_EN_CHANGE(IDC_VEXTY, OnChangeVexty)
	ON_EN_CHANGE(IDC_VORGX, OnChangeVorgx)
	ON_EN_CHANGE(IDC_VORGY, OnChangeVorgy)
	ON_EN_CHANGE(IDC_WEXTX, OnChangeWextx)
	ON_EN_CHANGE(IDC_WEXTY, OnChangeWexty)
	ON_EN_CHANGE(IDC_WORGX, OnChangeWorgx)
	ON_EN_CHANGE(IDC_WORGY, OnChangeWorgy)
	ON_BN_CLICKED(IDC_10, On10)
	ON_BN_CLICKED(IDC_100, On100)
	ON_BN_CLICKED(IDC_1000, On1000)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UWM_UPDATE_ERROR, OnUpdateError)
	ON_MESSAGE(UWM_CHANGE_VEXT, OnChangeVext)
	ON_MESSAGE(UWM_CHANGE_WEXT, OnChangeWext)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapping message handlers

void CMapping::updateControls()
    {
     BOOL enablex;
     BOOL enabley;
     BOOL showx;
     BOOL showy;

     int sel;

     sel = c_MappingMode.GetCurSel();
     if(sel == CB_ERR)
	showx = showy = FALSE;
     else
        { /* has selection */
	 int mode = c_MappingMode.GetItemData(sel);
	 showx = (mode == MM_ISOTROPIC || mode == MM_ANISOTROPIC);
	 showy = mode == MM_ANISOTROPIC;
	 // Unless we are in an (an-)isotropic mode, we ignore the
	 // viewport extent and window extent
	 
	 c_WindowExtX.EnableWindow(showx);
	 c_WindowExtY.EnableWindow(showx);
	 c_ViewportExtX.EnableWindow(showx);
	 c_ViewportExtY.EnableWindow(showx);
	 c_c_Vext.EnableWindow(showx);
	 c_c_Wext.EnableWindow(showx);
	} /* has selection */



     // The scaling is viewportExt/WindowExt
     if(c_WindowExtX.GetVal() == 0)
        { /* infinite */
	 c_SX.SetWindowText(_T(""));
	 enablex = FALSE;
	} /* infinite */
     else
        { /* change value */
	 enablex = TRUE;
	 c_SX.SetVal((float)c_ViewportExtX.GetVal() 
	 			/ (FLOAT)c_WindowExtX.GetVal());
	} /* change value */

     if(c_WindowExtY.GetVal() == 0)
        { /* infinite */
	 c_SY.SetWindowText(_T(""));
	 enabley = FALSE;
	} /* infinite */
     else
        { /* change value */
	 enabley = TRUE;
	 c_SY.SetVal((float)c_ViewportExtY.GetVal() 
	 			/ (FLOAT)c_WindowExtY.GetVal());
	} /* change value */
	 

     c_SX.ShowWindow(showx ? SW_SHOW : SW_HIDE);
     c_SY.ShowWindow(showy ? SW_SHOW : SW_HIDE);

     c_SX.EnableWindow(enablex);
     c_SY.EnableWindow(enabley);

     c_Scaling.ShowWindow(showx || showy ? SW_SHOW : SW_HIDE);

     updateSample(0);
    }
	    

void CMapping::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();

 	dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();

	dcv->wext.x = c_WindowExtX.GetVal();
	dcv->wext.y = c_WindowExtY.GetVal();
	dcv->worg.x = c_WindowOrgX.GetVal();
	dcv->worg.y = c_WindowOrgY.GetVal();

	dcv->vext.x = c_ViewportExtX.GetVal();
	dcv->vext.y = c_ViewportExtY.GetVal();
	dcv->vorg.x = c_ViewportOrgX.GetVal();
	dcv->vorg.y = c_ViewportOrgY.GetVal();

	dcv->mapmode = (int)c_MappingMode.GetCurItem();
	dcv->ticks = c_Sample.ticks ;

}

BOOL CMapping::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_MappingMode.AddStrings(mappingmodes);
	c_MappingMode.Select((DWORD)dcv->mapmode);
	
	c_WindowExtX.SetVal(dcv->wext.x);
	c_WindowExtY.SetVal(dcv->wext.y);
	c_WindowOrgX.SetVal(dcv->worg.x);
	c_WindowOrgY.SetVal(dcv->worg.y);

	c_ViewportExtX.SetVal(dcv->vext.x);
	c_ViewportExtY.SetVal(dcv->vext.y);
	c_ViewportOrgX.SetVal(dcv->vorg.x);
	c_ViewportOrgY.SetVal(dcv->vorg.y);

	c_HScrollSample.SetScrollRange(-32766, 32766);
	c_VScrollSample.SetScrollRange(-32766, 32766);

	int tickoption = 0; // no valid option
	switch(dcv->ticks)
	   { /* which one */
	    case 10:
		    tickoption = IDC_10;
		    break;
	    case 100:
		    tickoption = IDC_100;
		    break;
	    case 1000:
		    tickoption = IDC_1000;
		    break;
	   } /* which one */

	if(tickoption != 0)
	   { /* valid option */
	    CheckRadioButton(IDC_10, IDC_1000, tickoption);
	    c_Sample.ticks = dcv->ticks;
	   } /* valid option */
	else
	   { /* bogus option */
	    c_Sample.ticks = 0;
	   } /* bogus option */

	updateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                          CMapping::setSampleParms
* Result: void
*       
* Effect: 
*       Copies all the parameters to the sample window members
****************************************************************************/

void CMapping::setSampleParms()
    {
     c_Sample.wext.x = c_WindowExtX.GetVal();
     c_Sample.wext.y = c_WindowExtY.GetVal();
     c_Sample.worg.x = c_WindowOrgX.GetVal();
     c_Sample.worg.y = c_WindowOrgY.GetVal();

     c_Sample.vext.x = c_ViewportExtX.GetVal();
     c_Sample.vext.y = c_ViewportExtY.GetVal();
     c_Sample.vorg.x = c_ViewportOrgX.GetVal();
     c_Sample.vorg.y = c_ViewportOrgY.GetVal();

     c_Sample.mapmode = (int)c_MappingMode.GetCurItem();
     
    }

/****************************************************************************
*                           CMapping::UpdateSample
* Inputs:
*       int n: Number of ticks
* Result: void
*       
* Effect: 
*       Ships all the parameters to the sample and forces it to update
****************************************************************************/

void CMapping::updateSample(int n)
    {
     if(n > 0)
	c_Sample.ticks = n;

     setSampleParms();
     c_Sample.InvalidateRect(NULL);    
    }

void CMapping::OnSelendokMappingmode() 
{
 	updateControls();
}

void CMapping::OnChangeVextx() 
{
 	updateControls();
	
}

void CMapping::OnChangeVexty() 
{
 	updateControls();
	
}

void CMapping::OnChangeVorgx() 
{
 updateSample();
	
}

void CMapping::OnChangeVorgy() 
{
 updateSample();
}

void CMapping::OnChangeWextx() 
{
 updateControls();
	
}

void CMapping::OnChangeWexty() 
{
 updateControls();
	
}

void CMapping::OnChangeWorgx() 
{
 updateSample();
}

void CMapping::OnChangeWorgy() 
{
 updateSample();
}

void CMapping::On10() 
{
 updateSample(10);
	
}

void CMapping::On100() 
{
 updateSample(100);
	
}

void CMapping::On1000() 
{
 updateSample(1000);
	
}

/****************************************************************************
*                           CMapping::OnUpdateError
* Inputs:
*       WPARAM wParam: Error code
*	LPARAM: ignored
* Result: LRESULT
*       0, always (logically void)
* Effect: 
*       Updates the error message
****************************************************************************/

LRESULT CMapping::OnUpdateError(WPARAM wParam, LPARAM)
    {
     c_Error.SetWindowText(wParam);
     return 0;
    }

/****************************************************************************
*                           CMapping::OnChangeWext
* Inputs:
*       WPARAM x: new X-value for window extent
*	LPARAM y: new Y-value for window extent
* Result: LRESULT (void, logically)
*       0, always
* Effect: 
*       Resets the window extent
****************************************************************************/

LRESULT CMapping::OnChangeWext(WPARAM x, LPARAM y)
    {
     return 0;
    }

/****************************************************************************
*                           CMapping::OnChangeVext
* Inputs:
*       WPARAM x: new X-value for window extent
*	LPARAM y: new Y-value for window extent
* Result: LRESULT (void, logically)
*       0, always
* Effect: 
*       Resets the window extent
****************************************************************************/

LRESULT CMapping::OnChangeVext(WPARAM x, LPARAM y)
    {
     return 0;
    }

void CMapping::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_HScrollSample.m_hWnd)
    { /* sample hscroll */
     setSampleParms();

     c_Sample.SendMessage(WM_HSCROLL, MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
     int prev = c_WindowOrgX.GetVal();
     int newval = c_Sample.worg.x;

     if(newval != prev)
	{ /* changed */
	 c_WindowOrgX.SetVal(c_Sample.worg.x); // generates EN_CHANGE forcing update
	} /* changed */
    } /* sample hscroll */
}

void CMapping::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_VScrollSample.m_hWnd)
    { /* sample vscroll */
     setSampleParms();

     c_Sample.SendMessage(WM_VSCROLL, MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
     int prev = c_WindowOrgY.GetVal();
     int newval = c_Sample.worg.y;

     if(newval != prev)
	{ /* changed */
	 c_WindowOrgY.SetVal(c_Sample.worg.y); // generates EN_CHANGE forcing update
	} /* changed */
    } /* sample vscroll */

}

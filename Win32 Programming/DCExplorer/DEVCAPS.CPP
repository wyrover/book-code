// DevCaps.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "NumericEdit.h"
#include "enumedit.h"
#include "bitcombo.h"
#include "message.h"
#include "dcvals.h"
#include "DevCaps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevCaps property page

IMPLEMENT_DYNCREATE(CDevCaps, CPropertyPage)

CDevCaps::CDevCaps() : CPropertyPage(CDevCaps::IDD)
{
 dc = NULL;
	//{{AFX_DATA_INIT(CDevCaps)
	//}}AFX_DATA_INIT
}

CDevCaps::~CDevCaps()
{
}

void CDevCaps::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDevCaps)
	DDX_Control(pDX, IDC_RESULT, c_Result);
	DDX_Control(pDX, IDC_TEXTCAPS, c_TEXTCAPS);
	DDX_Control(pDX, IDC_POLYGONALCAPS, c_POLYGONALCAPS);
	DDX_Control(pDX, IDC_LINECAPS, c_LINECAPS);
	DDX_Control(pDX, IDC_CURVECAPS, c_CURVECAPS);
	DDX_Control(pDX, IDC_RASTERCAPS, c_RASTERCAPS);
	DDX_Control(pDX, IDC_C_SIZEPALETTE, c_c_SIZEPALETTE);
	DDX_Control(pDX, IDC_DRIVERVERSION, c_DRIVERVERSION);
	DDX_Control(pDX, IDC_SIZEPALETTE, c_SIZEPALETTE);
	DDX_Control(pDX, IDC_TECHNOLOGY, c_TECHNOLOGY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDevCaps, CPropertyPage)
	//{{AFX_MSG_MAP(CDevCaps)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// This table lists each control ID and the GetDeviceCaps value that
// is used to set its contents.  All of these controls take integer values.

struct { int id;
	 int cap;} controls[] = {
			{IDC_HORZSIZE,    HORZSIZE}, // TODO: mm and in
			{IDC_VERTSIZE,    VERTSIZE}, // TODO: mm and in
			{IDC_HORZRES,     HORZRES},
			{IDC_VERTRES,     VERTRES},
			{IDC_LOGPIXELSX,  LOGPIXELSX},
			{IDC_LOGPIXELSY,  LOGPIXELSY},
			{IDC_BITSPIXEL,   BITSPIXEL},
			{IDC_PLANES,      PLANES},
			{IDC_NUMBRUSHES,  NUMBRUSHES},
			{IDC_NUMPENS,     NUMPENS},
			{IDC_NUMFONTS,    NUMFONTS},
			{IDC_NUMCOLORS,   NUMCOLORS},
			{IDC_ASPECTX,     ASPECTX},
			{IDC_ASPECTY,     ASPECTY},
			{IDC_ASPECTXY,    ASPECTXY},
			{IDC_PDEVICESIZE, PDEVICESIZE},
			{IDC_CLIPCAPS,    CLIPCAPS},
			{IDC_NUMRESERVED, NUMRESERVED},
			{IDC_COLORRES,    COLORRES},
			{0, 0} // EOT
				};

static BITCOMBO t_RASTERCAPS[] = {
		{ RC_BITBLT,  IDS_RC_BITBLT},            // .... .... .... ...1
		{ RC_BANDING, IDS_RC_BANDING},		 // .... .... .... ..1.
		{ RC_SCALING,   IDS_RC_SCALING},	 // .... .... .... .1..
		{ RC_BITMAP64, IDS_RC_BITMAP64},	 // .... .... .... 1...
		{ RC_GDI20_OUTPUT, IDS_RC_GDI20_OUTPUT}, // .... .... ...1 ....
		{ RC_GDI20_STATE, IDS_RC_GDI20_STATE},	 // .... .... ..1. ....
		{ RC_SAVEBITMAP, IDS_RC_SAVEBITMAP},     // .... .... .1.. ....
		{ RC_DI_BITMAP, IDS_RC_DI_BITMAP},	 // .... .... 1... ....
		{ RC_PALETTE,   IDS_RC_PALETTE},         // .... ...1 .... ....
		{ RC_DIBTODEV,   IDS_RC_DIBTODEV},       // .... ..1. .... ....
		{ RC_BIGFONT,  IDS_RC_BIGFONT},		 // .... .1.. .... ....
		{ RC_STRETCHBLT, IDS_RC_STRETCHBLT},     // .... 1... .... ....
		{ RC_FLOODFILL, IDS_RC_FLOODFILL},	 // ...1 .... .... ....
		{ RC_STRETCHDIB, IDS_RC_STRETCHDIB},     // ..1. .... .... ....
		{ RC_OP_DX_OUTPUT, IDS_RC_OP_DX_OUTPUT}, // .1.. .... .... ....
		{ RC_DEVBITS,    IDS_RC_DEVBITS},        // 1... .... .... ....
		{ 0, 0} // EOT
				 };
static BITCOMBO t_CURVECAPS[] = {
		{ CC_NONE, IDS_CC_NONE},
		{ CC_CIRCLES, IDS_CC_CIRCLES},		 // .... .... .... ...1
		{ CC_PIE, IDS_CC_PIE},			 // .... .... .... ..1.
		{ CC_CHORD, IDS_CC_CHORD},		 // .... .... .... .1..
		{ CC_ELLIPSES, IDS_CC_ELLIPSES},	 // .... .... .... 1...
		{ CC_WIDE, IDS_CC_WIDE},		 // .... .... ...1 ....
		{ CC_STYLED, IDS_CC_STYLED},		 // .... .... ..1. ....
		{ CC_WIDESTYLED, IDS_CC_WIDESTYLED},	 // .... .... .1.. ....
		{ CC_INTERIORS, IDS_CC_INTERIORS},	 // .... .... 1... ....
		{ CC_ROUNDRECT, IDS_CC_ROUNDRECT},	 // .... ...1 .... ....
		{ 0, 0} // EOT
				};

static BITCOMBO t_LINECAPS[] = {
		{ LC_NONE, IDS_LC_NONE},
		{ LC_POLYLINE, IDS_LC_POLYLINE},
		{ LC_MARKER, IDS_LC_MARKER},
		{ LC_POLYMARKER, IDS_LC_POLYMARKER},
		{ LC_WIDE, IDS_LC_WIDE},
		{ LC_STYLED, IDS_LC_STYLED},
		{ LC_WIDESTYLED, IDS_LC_WIDESTYLED},
		{ LC_INTERIORS, IDS_LC_INTERIORS},
		{ 0, 0} // EOT
			       };

static BITCOMBO t_POLYGONALCAPS[] = {
		{ PC_NONE, IDS_PC_NONE},
		{ PC_POLYGON, IDS_PC_POLYGON},		 // .... .... .... ...1
		{ PC_RECTANGLE, IDS_PC_RECTANGLE},	 // .... .... .... ..1.
		{ PC_WINDPOLYGON, IDS_PC_WINDPOLYGON},	 // .... .... .... .1..
		{ PC_TRAPEZOID, IDS_PC_TRAPEZOID},       // .... .... .... .1..
		{ PC_SCANLINE, IDS_PC_SCANLINE},	 // .... .... .... 1...
		{ PC_WIDE, IDS_PC_WIDE},		 // .... .... ...1 ....
		{ PC_STYLED, IDS_PC_STYLED},		 // .... .... ..1. ....
		{ PC_WIDESTYLED, IDS_PC_WIDESTYLED},	 // .... .... .1.. ....
		{ PC_INTERIORS, IDS_PC_INTERIORS},	 // .... .... 1... ....
		{ PC_POLYPOLYGON, IDS_PC_POLYPOLYGON},   // .... ...1 .... ....
		{ PC_PATHS, IDS_PC_PATHS},               // .... ..1. .... ....
		{ 0, 0} // EOT
				    };

static BITCOMBO t_TEXTCAPS[] = {
		{ TC_OP_CHARACTER, IDS_TC_OP_CHARACTER},//. .... .... .... ...1
		{ TC_OP_STROKE, IDS_TC_OP_STROKE},	//. .... .... .... ..1.
		{ TC_CP_STROKE, IDS_TC_CP_STROKE},	//. .... .... .... .1..
		{ TC_CR_90, IDS_TC_CR_90},		//. .... .... .... 1...
		{ TC_CR_ANY, IDS_TC_CR_ANY},		//. .... .... ...1 ....
		{ TC_SF_X_YINDEP, IDS_TC_SF_X_YINDEP},	//. .... .... ..1. ....
		{ TC_SA_DOUBLE, IDS_TC_SA_DOUBLE},	//. .... .... .1.. ....
		{ TC_SA_INTEGER, IDS_TC_SA_INTEGER},	//. .... .... 1... ....
		{ TC_SA_CONTIN, IDS_TC_SA_CONTIN},	//. .... ...1 .... ....
		{ TC_EA_DOUBLE, IDS_TC_EA_DOUBLE},	//. .... ..1. .... ....
		{ TC_IA_ABLE, IDS_TC_IA_ABLE},		//. .... .1.. .... ....
		{ TC_UA_ABLE, IDS_TC_UA_ABLE},		//. .... 1... .... ....
		{ TC_SO_ABLE, IDS_TC_SO_ABLE},		//. ...1 .... .... ....
		{ TC_RA_ABLE, IDS_TC_RA_ABLE},		//. ..1. .... .... ....
		{ TC_VA_ABLE, IDS_TC_VA_ABLE},		//. .1.. .... .... ....
		{ TC_RESERVED, IDS_TC_RESERVED},	//. 1... .... .... ....
		{ TC_SCROLLBLT, IDS_TC_SCROLLBLT},	//1 .... .... .... ....
		{ 0, 0} // EOT
			       };


/****************************************************************************
*                          CDevCaps::pd_to_controls
* Result: void
*       
* Effect: 
*       Transfers the information in the DC of the pd to the controls
****************************************************************************/


static ENUMEDIT t_technology[] = {
	{ DT_PLOTTER, IDS_DT_PLOTTER},
	{ DT_RASDISPLAY, IDS_DT_RASDISPLAY},
	{ DT_RASPRINTER, IDS_DT_RASPRINTER},
	{ DT_RASCAMERA, IDS_DT_RASCAMERA},
	{ DT_CHARSTREAM, IDS_DT_CHARSTREAM},
	{ DT_METAFILE, IDS_DT_METAFILE},
	{ DT_DISPFILE, IDS_DT_DISPFILE},
	{ 0, 0} // EOT
				 };

void CDevCaps::pd_to_controls()
    {
     if(dc == NULL || dc->m_hDC == NULL)
        { /* nothing */
	 EnableControls(FALSE);
	 c_Result.EnableWindow(TRUE);
	 c_Result.SetWindowText(MAKEINTRESOURCE(IDS_NO_DC));
	} /* nothing */
     else
        { /* something */
	 CNumericEdit wnd;

	 c_Result.EnableWindow(FALSE);
	 c_Result.SetWindowText(_T(""));

	 c_DRIVERVERSION.SetWindowText(dc->GetDeviceCaps(DRIVERVERSION),
	 					_T("0x%x"));

	 EnableControls(TRUE);
	 // Copy the info to the numeric controls
	 for(int i = 0; controls[i].id != 0; i++)
	    { /* init each */
	     HWND hwnd = ::GetDlgItem(m_hWnd, controls[i].id);
	     wnd.Attach(hwnd);
	     wnd.SetWindowText(dc->GetDeviceCaps(controls[i].cap));
	     wnd.Detach();
	    } /* init each */

	 // set the TECHNOLOGY field
	 c_TECHNOLOGY.SetWindowText(dc->GetDeviceCaps(TECHNOLOGY), t_technology);

	 // set the SIZEPALETTE
	 if(dc->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	    { /* has palette */
	     c_c_SIZEPALETTE.EnableWindow(TRUE);
	     c_SIZEPALETTE.ShowWindow(SW_SHOW);
	     c_SIZEPALETTE.SetWindowText(dc->GetDeviceCaps(SIZEPALETTE));
	    } /* has palette */
	 else
	    { /* no palette */
	     c_c_SIZEPALETTE.EnableWindow(FALSE);
	     c_SIZEPALETTE.ShowWindow(SW_HIDE);
	    } /* no palette */

	 // Set the RASTERCAPS
	 c_RASTERCAPS.AddBits(dc->GetDeviceCaps(RASTERCAPS), t_RASTERCAPS);

	 c_CURVECAPS.AddBits(dc->GetDeviceCaps(CURVECAPS), t_CURVECAPS);
	 c_LINECAPS.AddBits(dc->GetDeviceCaps(LINECAPS), t_LINECAPS);
	 c_POLYGONALCAPS.AddBits(dc->GetDeviceCaps(POLYGONALCAPS), t_POLYGONALCAPS);
	 c_TEXTCAPS.AddBits(dc->GetDeviceCaps(TEXTCAPS), t_TEXTCAPS);
	} /* something */
    }

/****************************************************************************
*                           CDevCaps::ShowControls
* Inputs:
*       UINT mode: Mode to show
* Result: void
*       
* Effect: 
*       Shows/hides controls based on mode
****************************************************************************/

void CDevCaps::ShowControls(UINT mode)
    {
     int i;
     for(i = 0; controls[i].id != 0; i++)
        { /* set each */
	 GetDlgItem(controls[i].id)->ShowWindow(mode);
	} /* set each */

     c_TECHNOLOGY.ShowWindow(mode);
     c_SIZEPALETTE.ShowWindow(mode);
     c_DRIVERVERSION.ShowWindow(mode);     
     c_TEXTCAPS.ShowWindow(mode);
     c_POLYGONALCAPS.ShowWindow(mode);
     c_LINECAPS.ShowWindow(mode);
     c_CURVECAPS.ShowWindow(mode);
     c_RASTERCAPS.ShowWindow(mode);
     
    }

/****************************************************************************
*                          CDevCaps::EnableControls
* Inputs:
*       BOOL mode: TRUE or FALSE
* Result: void
*       
* Effect: 
*       Enables or disables all the child controls of the window
****************************************************************************/

void CDevCaps::EnableControls(BOOL mode)
    {
     CWnd * wnd = GetWindow(GW_CHILD);
     do
        { /* handle each window */
	 wnd->EnableWindow(mode);
	 wnd = wnd->GetWindow(GW_HWNDNEXT);
	 ShowControls(SW_SHOW);
	} /* handle each window */
     while (wnd != NULL);

     if(!mode)
        { /* blank all controls */
	 for(int i = 0; controls[i].id != 0; i++)
	    { /* blank one control */
	     CEdit * wnd = (CEdit *)GetDlgItem(controls[i].id);
	     wnd->SetWindowText(_T(""));
	    } /* blank one control */
	 c_DRIVERVERSION.Blank();
	 c_TECHNOLOGY.Blank();
	 c_RASTERCAPS.ResetContent();
	 ShowControls(SW_HIDE);
	} /* blank all controls */
    }

/////////////////////////////////////////////////////////////////////////////
// CDevCaps message handlers

BOOL CDevCaps::OnSetActive() 
    {
     dc = new CClientDC(this);
     dcv->setDC(dc);
     pd_to_controls();
	
     return CPropertyPage::OnSetActive();
    }

HBRUSH CDevCaps::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_EDIT && pWnd->m_hWnd == c_Result.m_hWnd)
	   return ((CMessage *)pWnd)->OnCtlColor(pDC);
	return hbr;
}

BOOL CDevCaps::OnKillActive() 
{
 delete dc;
 dc = NULL;
	
	return CPropertyPage::OnKillActive();
}

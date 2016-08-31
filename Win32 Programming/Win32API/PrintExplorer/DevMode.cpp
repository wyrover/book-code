// DevMode.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "numericEdit.h"
#include "EnumEdit.h"
#include "message.h"
#include "DevMode.h"
#include "paper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevMode property page

IMPLEMENT_DYNCREATE(CDevMode, CPropertyPage)

CDevMode::CDevMode() : CPropertyPage(CDevMode::IDD)
{
        //{{AFX_DATA_INIT(CDevMode)
        //}}AFX_DATA_INIT
}

CDevMode::~CDevMode()
{
}

void CDevMode::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDevMode)
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_c_dmSpecVersion, c_c_dmSpecVersion);
        DDX_Control(pDX, IDC_c_dmSize, c_c_dmSize);
        DDX_Control(pDX, IDC_c_dmFields, c_c_dmFields);
        DDX_Control(pDX, IDC_c_dmDriverVersion, c_c_dmDriverVersion);
        DDX_Control(pDX, IDC_c_dmDriverExtra, c_c_dmDriverExtra);
        DDX_Control(pDX, IDC_c_dmDeviceName, c_c_dmDeviceName);
        DDX_Control(pDX, IDC_dmPaperSize, c_dmPaperSize);
        DDX_Control(pDX, IDC_dmYResolution, c_dmYResolution);
        DDX_Control(pDX, IDC_c_dmYResolution, c_c_dmYResolution);
        DDX_Control(pDX, IDC_dmDefaultSource, c_dmDefaultSource);
        DDX_Control(pDX, IDC_c_dmCollate, c_c_dmCollate);
        DDX_Control(pDX, IDC_c_dmPrintQuality, c_c_dmPrintQuality);
        DDX_Control(pDX, IDC_dmPrintQuality, c_dmPrintQuality);
        DDX_Control(pDX, IDC_dmTTOption, c_dmTTOption);
        DDX_Control(pDX, IDC_dmDisplayFrequency, c_dmDisplayFrequency);
        // DDX_Control(pDX, IDC_dmDisplayFlags, c_dmDisplayFlags);
        DDX_Control(pDX, IDC_dmCollate, c_dmCollate);
        DDX_Control(pDX, IDC_dmPelsWidth, c_dmPelsWidth);
        DDX_Control(pDX, IDC_dmDuplex, c_dmDuplex);
        DDX_Control(pDX, IDC_dmPelsHeight, c_dmPelsHeight);
        DDX_Control(pDX, IDC_dmLogPixels, c_dmLogPixels);
        DDX_Control(pDX, IDC_dmFormName, c_dmFormName);
        DDX_Control(pDX, IDC_dmBitsPerPel, c_dmBitsPerPel);
        DDX_Control(pDX, IDC_c_dmTTOption, c_c_dmTTOption);
        DDX_Control(pDX, IDC_c_dmPelsWidth, c_c_dmPelsWidth);
        DDX_Control(pDX, IDC_c_dmPelsHeight, c_c_dmPelsHeight);
        DDX_Control(pDX, IDC_c_dmDisplayFrequency, c_c_dmDisplayFrequency);
        // DDX_Control(pDX, IDC_c_dmDisplayFlags, c_c_dmDisplayFlags);
        DDX_Control(pDX, IDC_c_dmBitsPerPel, c_c_dmBitsPerPel);
        DDX_Control(pDX, IDC_c_dmDuplex, c_c_dmDuplex);
        DDX_Control(pDX, IDC_c_dmLogPixels, c_c_dmLogPixels);
        DDX_Control(pDX, IDC_c_dmFormName, c_c_dmFormName);
        DDX_Control(pDX, IDC_dmColor, c_dmColor);
        DDX_Control(pDX, IDC_dmScale, c_dmScale);
        DDX_Control(pDX, IDC_dmPaperWidth, c_dmPaperWidth);
        DDX_Control(pDX, IDC_dmPaperLength, c_dmPaperLength);
        DDX_Control(pDX, IDC_dmCopies, c_dmCopies);
        DDX_Control(pDX, IDC_dmSpecVersion, c_dmSpecVersion);
        DDX_Control(pDX, IDC_dmSize, c_dmSize);
        DDX_Control(pDX, IDC_dmOrientation, c_dmOrientation);
        DDX_Control(pDX, IDC_dmFields, c_dmFields);
        DDX_Control(pDX, IDC_dmDriverVresion, c_dmDriverVersion);
        DDX_Control(pDX, IDC_dmDriverExtra, c_dmDriverExtra);
        DDX_Control(pDX, IDC_dmDeviceName, c_dmDeviceName);
        DDX_Control(pDX, IDC_c_dmScale, c_c_dmScale);
        DDX_Control(pDX, IDC_c_dmPaperWidth, c_c_dmPaperWidth);
        DDX_Control(pDX, IDC_c_dmPaperSize, c_c_dmPaperSize);
        DDX_Control(pDX, IDC_c_dmPaperLength, c_c_dmPaperLength);
        DDX_Control(pDX, IDC_c_dmOrientation, c_c_dmOrientation);
        DDX_Control(pDX, IDC_c_dmDefaultSource, c_c_dmDefaultSource);
        DDX_Control(pDX, IDC_c_dmCopies, c_c_dmCopies);
        DDX_Control(pDX, IDC_c_dmColor, c_c_dmColor);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDevMode, CPropertyPage)
        //{{AFX_MSG_MAP(CDevMode)
        ON_WM_CTLCOLOR()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevMode message handlers
static ENUMEDIT t_dmOrientation[] = {
        { DMORIENT_LANDSCAPE, IDS_DMORIENT_LANDSCAPE},
        { DMORIENT_PORTRAIT,  IDS_DMORIENT_PORTRAIT},
        { 0, 0} // EOT
                                    };
static ENUMEDIT t_dmColor[] = {
        { DMCOLOR_COLOR, IDS_DMCOLOR_COLOR},
        { DMCOLOR_MONOCHROME, IDS_DMCOLOR_MONOCHROME},
        { 0, 0} // EOT
                              };

static ENUMEDIT t_dmPrintQuality[] = {
        { DMRES_HIGH, IDS_DMRES_HIGH},
        { DMRES_MEDIUM, IDS_DMRES_MEDIUM},
        { DMRES_LOW, IDS_DMRES_LOW},
        { DMRES_DRAFT, IDS_DMRES_DRAFT},
        { 0, 0 } // EOT
                                     };

static ENUMEDIT t_dmDuplex[] = {
        { DMDUP_SIMPLEX, IDS_DMDUP_SIMPLEX},
        { DMDUP_HORIZONTAL, IDS_DMDUP_HORIZONTAL},
        { DMDUP_VERTICAL, IDS_DMDUP_VERTICAL},
        { 0, 0} // EOT
                               };

static ENUMEDIT t_dmTTOption[] = {
        { DMTT_BITMAP, IDS_DMTT_BITMAP},
        { DMTT_DOWNLOAD, IDS_DMTT_DOWNLOAD},
        { DMTT_SUBDEV, IDS_DMTT_SUBDEV},
        { 0, 0 } // EOT
                                 };

static ENUMEDIT t_dmCollate[] = {
        { DMCOLLATE_TRUE, IDS_DMCOLLATE_TRUE},
        { DMCOLLATE_FALSE, IDS_DMCOLLATE_FALSE},
        { 0, 0} // EOT
                                };

// The VC++ 4.2 release marks these two flags as "No Longer Valid"
//static ENUMEDIT t_dmDisplayFlags[] = {
//        { DM_GRAYSCALE, IDS_DMGRAYSCALE},
//        { DM_INTERLACED, IDS_DMINTERLACED},
//        { 0, 0} // EOT
//                                     };


/****************************************************************************
*                            CDevMode::ShowFields
* Inputs:
*       �
* Result: �
*       �
* Effect: 
*       �
****************************************************************************/

void CDevMode::ShowFields(LPDEVMODE dm, DWORD Fields)
    {
     // ----- ORIENTATION
     c_c_dmOrientation.EnableWindow(Fields & DM_ORIENTATION);
     c_dmOrientation.ShowWindow(Fields & DM_ORIENTATION 
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_ORIENTATION) && dm)
        c_dmOrientation.SetWindowText(dm->dmOrientation, t_dmOrientation);

     // ----- PAPERSIZE
     c_c_dmPaperSize.EnableWindow(Fields & DM_PAPERSIZE);
     c_dmPaperSize.ShowWindow(Fields & DM_PAPERSIZE
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PAPERSIZE) && dm)
        c_dmPaperSize.SetWindowText(dm->dmPaperSize, t_dmPaperSize);

     // ----- PAPERLENGTH
     c_c_dmPaperLength.EnableWindow(Fields & DM_PAPERLENGTH);
     c_dmPaperLength.ShowWindow(Fields & DM_PAPERLENGTH
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PAPERLENGTH) &&dm)
         c_dmPaperLength.SetWindowText(dm->dmPaperSize,"%d x0.1mm");

     // ----- PAPERWIDTH
     c_c_dmPaperWidth.EnableWindow(Fields & DM_PAPERWIDTH);
     c_dmPaperWidth.ShowWindow(Fields & DM_PAPERWIDTH
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PAPERWIDTH) && dm)
         c_dmPaperWidth.SetWindowText(dm->dmPaperSize,"%d x0.1mm");
     ::GlobalUnlock(pd->hDevMode);

     // ----- SCALE
     c_c_dmScale.EnableWindow(Fields & DM_SCALE);
     c_dmScale.ShowWindow(Fields & DM_SCALE
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_SCALE) && dm)
        c_dmScale.SetWindowText(dm->dmScale,_T("%d%%"));

     // ----- COPIES
     c_c_dmCopies.EnableWindow(Fields & DM_COPIES);
     c_dmCopies.ShowWindow(Fields & DM_COPIES
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_COPIES) && dm)
        c_dmCopies.SetWindowText(dm->dmCopies);


     // ----- DEFAULTSOURCE
     c_c_dmDefaultSource.EnableWindow(Fields & DM_DEFAULTSOURCE);
     c_dmDefaultSource.ShowWindow(Fields & DM_DEFAULTSOURCE
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_DEFAULTSOURCE) && dm)
        c_dmDefaultSource.SetWindowText(dm->dmDefaultSource);

     // ----- PRINTQUALITY
     c_c_dmPrintQuality.EnableWindow(Fields & DM_PRINTQUALITY);
     c_dmPrintQuality.ShowWindow(Fields & DM_PRINTQUALITY
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PRINTQUALITY) && dm)
        c_dmPrintQuality.SetWindowText(dm->dmPrintQuality, t_dmPrintQuality);
     // ----- COLOR
     c_c_dmColor.EnableWindow(Fields & DM_COLOR);
     c_dmColor.ShowWindow(Fields & DM_COLOR
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_COLOR) && dm)
        c_dmColor.SetWindowText(dm->dmColor, t_dmColor);

     // ----- DUPLEX
     c_c_dmDuplex.EnableWindow(Fields & DM_DUPLEX);
     c_dmDuplex.ShowWindow(Fields & DM_DUPLEX
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_DUPLEX) && dm)
        c_dmDuplex.SetWindowText(dm->dmDuplex, t_dmDuplex);

     // ----- YRESOLUTION
     c_c_dmYResolution.EnableWindow(Fields & DM_YRESOLUTION);
     c_dmYResolution.ShowWindow(Fields & DM_YRESOLUTION
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_YRESOLUTION) && dm)
        c_dmYResolution.SetWindowText(dm->dmYResolution);

     // ----- TTOPTION
     c_c_dmTTOption.EnableWindow(Fields & DM_TTOPTION);
     c_dmTTOption.ShowWindow(Fields & DM_TTOPTION
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_TTOPTION) && dm)
        c_dmTTOption.SetWindowText(dm->dmTTOption, t_dmTTOption);

     // ----- COLLATE
     c_c_dmCollate.EnableWindow(Fields & DM_COLLATE);
     c_dmCollate.ShowWindow(Fields & DM_COLLATE
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_COLLATE) && dm)
        c_dmCollate.SetWindowText(dm->dmCollate, t_dmCollate);

     // ----- FORMNAME
     c_c_dmFormName.EnableWindow(Fields & DM_FORMNAME);
     c_dmFormName.ShowWindow(Fields & DM_FORMNAME
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_FORMNAME) && dm)
        c_dmFormName.SetWindowText((LPCTSTR)dm->dmFormName);

     // ----- LOGPIXELS
     c_c_dmLogPixels.EnableWindow(Fields & DM_LOGPIXELS);
     c_dmLogPixels.ShowWindow(Fields & DM_LOGPIXELS
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_LOGPIXELS) && dm)
        c_dmLogPixels.SetWindowText(dm->dmLogPixels);

     // ----- BITSPERPEL
     c_c_dmBitsPerPel.EnableWindow(Fields & DM_BITSPERPEL);
     c_dmBitsPerPel.ShowWindow(Fields & DM_BITSPERPEL
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_BITSPERPEL) && dm)
        c_dmBitsPerPel.SetWindowText(dm->dmBitsPerPel);

     // ----- PELSWIDTH
     c_c_dmPelsWidth.EnableWindow(Fields & DM_PELSWIDTH);
     c_dmPelsWidth.ShowWindow(Fields & DM_PELSWIDTH
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PELSWIDTH) && dm)
        c_dmPelsWidth.SetWindowText(dm->dmPelsWidth);

     // ----- PELSHEIGHT
     c_c_dmPelsHeight.EnableWindow(Fields & DM_PELSHEIGHT);
     c_dmPelsHeight.ShowWindow(Fields & DM_PELSHEIGHT
                                        ? SW_SHOW : SW_HIDE);
     if((Fields & DM_PELSHEIGHT) && dm)
        c_dmPelsHeight.SetWindowText(dm->dmPelsHeight);

     // As of the VC++ 4.2 release, these flags are maked as "No Longer Valid"
	 // // ----- DISPLAYFLAGS
     // c_c_dmDisplayFlags.EnableWindow(Fields & DM_DISPLAYFLAGS);
     // c_dmDisplayFlags.ShowWindow(Fields & DM_DISPLAYFLAGS
     //                                   ? SW_SHOW : SW_HIDE);
     // if((Fields & DM_DISPLAYFLAGS) && dm)
     //   c_dmDisplayFlags.SetWindowText(dm->dmDisplayFlags, t_dmDisplayFlags);

     // ----- DISPLAYFREQUENCY
     c_c_dmDisplayFrequency.EnableWindow(Fields & DM_DISPLAYFREQUENCY);
     c_dmDisplayFrequency.ShowWindow(Fields & DM_DISPLAYFREQUENCY
                                        ? SW_SHOW : SW_HIDE);
     if( (Fields & DM_DISPLAYFREQUENCY) && dm)
        c_dmDisplayFrequency.SetWindowText(dm->dmDisplayFrequency);
     
    }

void CDevMode::pd_to_controls()
    {
     LPDEVMODE dm;
     if(pd->hDevMode != NULL)
        { /* has demove */
         dm = (LPDEVMODE)::GlobalLock(pd->hDevMode);
         if(dm != NULL)
            { /* has pointer */
             c_Result.EnableWindow(FALSE);
             c_Result.SetWindowText(_T(""));

             c_c_dmDeviceName.EnableWindow(TRUE);
             c_dmDeviceName.ShowWindow(SW_SHOW);
             c_dmDeviceName.SetWindowText((LPCTSTR)dm->dmDeviceName);

             c_c_dmSpecVersion.EnableWindow(TRUE);
             c_dmSpecVersion.ShowWindow(SW_SHOW);
             c_dmSpecVersion.SetWindowText(dm->dmSpecVersion);

             c_c_dmDriverVersion.EnableWindow(TRUE);
             c_dmDriverVersion.ShowWindow(SW_SHOW);
             c_dmDriverVersion.SetWindowText(dm->dmDriverVersion, _T("0x%x"));

             c_c_dmSize.EnableWindow(TRUE);
             c_dmSize.ShowWindow(SW_SHOW);
             c_dmSize.SetWindowText(dm->dmSize);

             c_c_dmDriverExtra.EnableWindow(TRUE);
             c_dmDriverExtra.ShowWindow(SW_SHOW);
             c_dmDriverExtra.SetWindowText(dm->dmDriverExtra);

             c_c_dmFields.EnableWindow(TRUE);
             c_dmFields.ShowWindow(SW_SHOW);
             c_dmFields.SetWindowText(dm->dmFields, "%08x");

             ShowFields(dm, dm->dmFields);

             return;
            } /* has pointer */
        } /* has demove */

     // If we get here, either the pointer is NULL or the lock failed.
     // clear out everything...
     ShowFields();

     c_c_dmDeviceName.EnableWindow(FALSE);
     c_dmDeviceName.ShowWindow(SW_HIDE);

     c_c_dmSpecVersion.EnableWindow(FALSE);
     c_dmSpecVersion.ShowWindow(SW_HIDE);

     c_c_dmDriverVersion.EnableWindow(FALSE);
     c_dmDriverVersion.ShowWindow(SW_HIDE);
     
     c_c_dmSize.EnableWindow(FALSE);
     c_dmSize.ShowWindow(SW_HIDE);

     c_c_dmDriverExtra.EnableWindow(FALSE);
     c_dmDriverExtra.ShowWindow(SW_HIDE);

     c_c_dmFields.EnableWindow(FALSE);
     c_dmFields.ShowWindow(SW_HIDE);

     c_Result.SetWindowText(MAKEINTRESOURCE(IDS_NO_DEVMODE));
    }


BOOL CDevMode::OnSetActive() 
    {
     pd_to_controls();
        
     return CPropertyPage::OnSetActive();
    }

HBRUSH CDevMode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(nCtlColor == CTLCOLOR_EDIT && pWnd->m_hWnd == c_Result.m_hWnd)
           return ((CMessage *)pWnd)->OnCtlColor(pDC);

        return hbr;
}

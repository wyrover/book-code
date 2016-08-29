// SetDev.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "message.h"
#include "SetDev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDev property page

IMPLEMENT_DYNCREATE(CSetDev, CPropertyPage)

CSetDev::CSetDev() : CPropertyPage(CSetDev::IDD)
{
        //{{AFX_DATA_INIT(CSetDev)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CSetDev::~CSetDev()
{
}

void CSetDev::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSetDev)
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_dmTTOption, c_dmTTOption);
        DDX_Control(pDX, IDC_DMTT_SUBDEV, c_DMTT_SUBDEV);
        DDX_Control(pDX, IDC_DMTT_None, c_DMTT_None);
        DDX_Control(pDX, IDC_DMTT_DOWNLOAD, c_DMTT_DOWNLOAD);
        DDX_Control(pDX, IDC_DMTT_BITMAP, c_DMTT_BITMAP);
        DDX_Control(pDX, IDC_DMORIENT_LANDSCAPE, c_DMORIENT_LANDSCAPE);
        DDX_Control(pDX, IDC_DMORIENT_PORTRAIT, c_DMORIENT_PORTRAIT);
        DDX_Control(pDX, IDC_DMORIENT_None, c_DMORIENT_None);
        DDX_Control(pDX, IDC_dmOrientation, c_dmOrientation);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDev, CPropertyPage)
        //{{AFX_MSG_MAP(CSetDev)
        ON_BN_CLICKED(IDC_DMORIENT_LANDSCAPE, OnDmorientLandscape)
        ON_BN_CLICKED(IDC_DMORIENT_None, OnDMORIENTNone)
        ON_BN_CLICKED(IDC_DMORIENT_PORTRAIT, OnDmorientPortrait)
        ON_WM_CTLCOLOR()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDev message handlers


/****************************************************************************
*                           CSetDev::EnableControls
* Inputs:
*       BOOL mode: Mode to set
* Result: void
*       
* Effect: 
*       Enables or disables all the controls (used for global setting based
*       on availability of DEVMODE)
****************************************************************************/

void CSetDev::EnableControls(BOOL mode)
    {
     c_dmOrientation.EnableWindow(mode);
     c_DMORIENT_None.EnableWindow(mode);
     c_DMORIENT_LANDSCAPE.EnableWindow(mode);
     c_DMORIENT_PORTRAIT.EnableWindow(mode);

     c_dmTTOption.EnableWindow(mode);
     c_DMTT_None.EnableWindow(mode);
     c_DMTT_BITMAP.EnableWindow(mode);
     c_DMTT_DOWNLOAD.EnableWindow(mode);
     c_DMTT_SUBDEV.EnableWindow(mode);
    }

/****************************************************************************
*                           CSetDev::pd_to_controls
* Result: void
*       
* Effect: 
*       Transfers contents of DEVMODE in pd member to controls
****************************************************************************/

void CSetDev::pd_to_controls()
    {
     if(pd->hDevMode != NULL)
        { /* has devmode */
         LPDEVMODE dm;
         dm = (LPDEVMODE)GlobalLock(pd->hDevMode);
         if(dm != NULL)
            { /* lock worked */
             EnableControls(TRUE);
             if(dm->dmFields & DM_ORIENTATION)
                { /* DM_ORIENTATION */
                 int id = IDC_DMORIENT_None;
                 switch(dm->dmOrientation)
                    { /* what orientation */
                     case DMORIENT_PORTRAIT:
                             id = IDC_DMORIENT_PORTRAIT;
                             break;
                     case DMORIENT_LANDSCAPE:
                             id = IDC_DMORIENT_LANDSCAPE;
                             break;
                    } /* what orientation */
                 CheckRadioButton(IDC_DMORIENT_PORTRAIT, IDC_DMORIENT_None,
                                                id);
                } /* DM_ORIENTATION */
             else
                { /* no DM_ORIENTATION */
                 CheckRadioButton(IDC_DMORIENT_PORTRAIT, IDC_DMORIENT_None, 
                                        IDC_DMORIENT_None);
                } /* no DM_ORIENTATION */


             if(dm->dmFields & DM_TTOPTION)
                { /* DM_TTOPTION */
                 int id = IDC_DMTT_None;
                 switch(dm->dmTTOption)
                    { /* what TT */
                     case DMTT_BITMAP:
                             id = IDC_DMTT_BITMAP;
                             break;
                     case DMTT_DOWNLOAD:
                             id = IDC_DMTT_DOWNLOAD;
                             break;
                     case DMTT_SUBDEV:
                             id = IDC_DMTT_SUBDEV;
                             break;
                    } /* what TT */
                 CheckRadioButton(IDC_DMTT_None, IDC_DMTT_SUBDEV, id);
                } /* DM_TTOPTION */
             else
                { /* no DM_TTOPTION */
                 CheckRadioButton(IDC_DMTT_None, IDC_DMTT_SUBDEV, 
                                        IDC_DMTT_None);
                } /* no DM_TTOPTION */
             GlobalUnlock(pd->hDevMode);
             return;
            } /* lock worked */
        } /* has devmode */

     // no devmode or lock failed
     EnableControls(FALSE);
     c_Result.SetWindowText(MAKEINTRESOURCE(IDS_NO_DEVMODE));
    }


/****************************************************************************
*                           CSetDev::controls_to_pd
* Result: void
*       
* Effect: 
*       Transfers the control values back to the DEVMODE
****************************************************************************/

void CSetDev::controls_to_pd()
    {
     if(pd->hDevMode != NULL)
        { /* has devmode */
         LPDEVMODE dm = (LPDEVMODE)GlobalLock(pd->hDevMode);
         if(dm != NULL)
            { /* lock worked */
             c_Result.EnableWindow(FALSE);
             c_Result.SetWindowText(_T(""));
             if(c_DMORIENT_None.GetCheck())
                { /* no orientation */
                 dm->dmFields &= ~ DM_ORIENTATION;
                } /* no orientation */
             else
             if(c_DMORIENT_LANDSCAPE.GetCheck())
                { /* landscape */
                 dm->dmFields |= DM_ORIENTATION;
                 dm->dmOrientation = DMORIENT_LANDSCAPE;
                } /* landscape */
             else
             if(c_DMORIENT_PORTRAIT.GetCheck())
                { /* portrait */
                 dm->dmFields |= DM_ORIENTATION;
                 dm->dmOrientation = DMORIENT_PORTRAIT;
                } /* portrait */

             if(c_DMTT_None.GetCheck())
                { /* no tt */
                 dm->dmFields &= ~ DM_TTOPTION;
                } /* no tt */
             else
             if(c_DMTT_BITMAP.GetCheck())
                { /* bitmap */
                 dm->dmFields |= DM_TTOPTION;
                 dm->dmTTOption = DMTT_BITMAP;
                } /* bitmap */
             else
             if(c_DMTT_DOWNLOAD.GetCheck())
                { /* download */
                 dm->dmFields |= DM_TTOPTION;
                 dm->dmTTOption = DMTT_DOWNLOAD;
                } /* download */
             else
             if(c_DMTT_SUBDEV.GetCheck())
                { /* subdev */
                 dm->dmFields |= DM_TTOPTION;
                 dm->dmTTOption = DMTT_SUBDEV;
                } /* subdev */

             // TODO: more fields set here...
             GlobalUnlock(pd->hDevMode);
            } /* lock worked */
        } /* has devmode */
    }


void CSetDev::OnDmorientLandscape() 
{
        // TODO: Add your control notification handler code here
        
}

void CSetDev::OnDMORIENTNone() 
{
        // TODO: Add your control notification handler code here
        
}

void CSetDev::OnDmorientPortrait() 
{
        // TODO: Add your control notification handler code here
        
}

BOOL CSetDev::OnKillActive() 
    {
     controls_to_pd();
     return CPropertyPage::OnKillActive();
    }

BOOL CSetDev::OnSetActive() 
    {
 
     pd_to_controls();
     return CPropertyPage::OnSetActive();
    }

BOOL CSetDev::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        pd_to_controls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSetDev::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(pWnd->m_hWnd == c_Result.m_hWnd)
           return ((CMessage *)pWnd)->OnCtlColor(pDC);
        return hbr;
}

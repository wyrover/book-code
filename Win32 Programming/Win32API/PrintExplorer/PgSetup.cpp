// PgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "NumericEdit.h"
#include "commerr.h"
#include "message.h"
#include "handleEdit.h"
#include "hdevmode.h"
#include "devnames.h"
#include "printit.h"
#include "msglog.h"
#include "PgSetup.h"
#include "pglog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPgSetup property page

IMPLEMENT_DYNCREATE(CPgSetup, CPropertyPage)

CPgSetup::CPgSetup() : CPropertyPage(CPgSetup::IDD)
{
        //{{AFX_DATA_INIT(CPgSetup)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        memset(&psd, 0, sizeof(PAGESETUPDLG));
        result = 0;
        psd.lStructSize = sizeof(PAGESETUPDLG);
}

CPgSetup::~CPgSetup()
{
}

void CPgSetup::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPgSetup)
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_c_lpfnPagePaintHook, c_c_lpfnPagePaintHook);
        DDX_Control(pDX, IDC_lpfnPagePaintHook, c_lpfnPagePaintHook);
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_hDevNames, c_hDevNames);
        DDX_Control(pDX, IDC_hDevMode, c_hDevMode);
        DDX_Control(pDX, IDC_c_rtMinMargin, c_c_rtMinMargin);
        DDX_Control(pDX, IDC_c_rtMargin, c_crtMargin);
        DDX_Control(pDX, IDC_c_ptPaperSize, c_c_ptPapersize);
        DDX_Control(pDX, IDC_rtMinMaring_right, c_rtMinMargin_right);
        DDX_Control(pDX, IDC_rtMinMargin_top, c_rtMinMargin_top);
        DDX_Control(pDX, IDC_rtMinMargin_left, c_rtMinMargin_left);
        DDX_Control(pDX, IDC_rtMinMargin_bottom, c_rtMinMargin_bottom);
        DDX_Control(pDX, IDC_rtMaring_right, c_rtMargin_right);
        DDX_Control(pDX, IDC_rtMargin_top, c_rtMargin_top);
        DDX_Control(pDX, IDC_rtMargin_left, c_rtMargin_left);
        DDX_Control(pDX, IDC_rtMargin_bottom, c_rtMargin_bottom);
        DDX_Control(pDX, IDC_ptPaperSize_Y, c_ptPaperSize_Y);
        DDX_Control(pDX, IDC_ptPaperSize_X, c_ptPaperSize_X);
        DDX_Control(pDX, IDC_PSD_SHOWHELP, c_PSD_ShowHelp);
        DDX_Control(pDX, IDC_PSD_RETURNDEFAULT, c_PSD_ReturnDefault);
        DDX_Control(pDX, IDC_PSD_NOWARNING, c_PSD_NoWarning);
        DDX_Control(pDX, IDC_PSD_MINMARGINS, c_PSD_MinMargins);
        DDX_Control(pDX, IDC_PSD_MARGINS, c_PSD_Margins);
        DDX_Control(pDX, IDC_PSD_INTHOUSANDTHSOFINCHES, c_PSD_InThousandthsOfInches);
        DDX_Control(pDX, IDC_PSD_INHUNDREDTHSOFMILLIMETERS, c_PSD_InHundredthsOfMillimeters);
        DDX_Control(pDX, IDC_PSD_ENABLEPAGESETUPTEMPLATEHANDLE, c_PSD_EnablePageSetupTemplateHandle);
        DDX_Control(pDX, IDC_PSD_ENABLEPAGESETUPTEMPLATE, c_PSD_EnablePageSetupTemplate);
        DDX_Control(pDX, IDC_PSD_ENABLEPAGESETUPHOOK, c_PSD_EnablePageSetupHook);
        DDX_Control(pDX, IDC_PSD_ENABLEPAGEPAINTHOOK, c_PSD_EnablePagePaintingHook);
        DDX_Control(pDX, IDC_PSD_DISABLEPRINTER, c_PSD_DisablePrinter);
        DDX_Control(pDX, IDC_PSD_DISABLEPAPER, c_PSD_DisablePaper);
        DDX_Control(pDX, IDC_PAGESETUP, c_PageSetup);
        DDX_Control(pDX, IDC_PSD_DISABLEPAGEPAINTING, c_PSD_DisablePagePainting);
        DDX_Control(pDX, IDC_PSD_DISABLEORIENTATION, c_PSD_DisableOrientation);
        DDX_Control(pDX, IDC_PSD_DISABLEMARGINS, c_PSD_DisableMargins);
        DDX_Control(pDX, IDC_PSD_DEFAULTMINMARGINS, c_PSD_DefaultMargins);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPgSetup, CPropertyPage)
        //{{AFX_MSG_MAP(CPgSetup)
        ON_BN_CLICKED(IDC_DEFAULTS, OnDefaults)
        ON_BN_CLICKED(IDC_PAGESETUP, OnPagesetup)
        ON_BN_CLICKED(IDC_PSD_INHUNDREDTHSOFMILLIMETERS, OnPsdInhundredthsofmillimeters)
        ON_BN_CLICKED(IDC_PSD_INTHOUSANDTHSOFINCHES, OnPsdInthousandthsofinches)
        ON_WM_CTLCOLOR()
        ON_BN_CLICKED(IDC_CLEAR, OnClear)
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_PSD_ENABLEPAGEPAINTHOOK, OnPsdEnablepagepainthook)
        //}}AFX_MSG_MAP
        ON_REGISTERED_MESSAGE(UWM_LOG_PAINT, onLogPaint)
END_MESSAGE_MAP()
            

static struct {
        int id;
        DWORD flag;
               } bits[] = {
        { IDC_PSD_DISABLEORIENTATION, PSD_DISABLEORIENTATION},
        { IDC_PSD_DISABLEPAGEPAINTING, PSD_DISABLEPAGEPAINTING},
        { IDC_PSD_DISABLEPAPER, PSD_DISABLEPAPER},
        { IDC_PSD_DISABLEPRINTER, PSD_DISABLEPRINTER},
        { IDC_PSD_ENABLEPAGEPAINTHOOK, PSD_ENABLEPAGEPAINTHOOK},
        { IDC_PSD_ENABLEPAGESETUPHOOK, PSD_ENABLEPAGESETUPHOOK},
        { IDC_PSD_ENABLEPAGESETUPTEMPLATE, PSD_ENABLEPAGESETUPTEMPLATE},
        { IDC_PSD_INHUNDREDTHSOFMILLIMETERS, PSD_INHUNDREDTHSOFMILLIMETERS},
        { IDC_PSD_INTHOUSANDTHSOFINCHES, PSD_INTHOUSANDTHSOFINCHES},
        { IDC_PSD_MARGINS, PSD_MARGINS},
        { IDC_PSD_MINMARGINS, PSD_MINMARGINS},
        { IDC_PSD_NOWARNING, PSD_NOWARNING},
        { IDC_PSD_RETURNDEFAULT, PSD_RETURNDEFAULT},
        { IDC_PSD_SHOWHELP, PSD_SHOWHELP},
        { 0, 0} // EOT
                          };

void CPgSetup::psd_to_controls()
    {
     int i;
     for(i = 0; bits[i].id != 0; i++)
        { /* process each */
         ((CButton *)GetDlgItem(bits[i].id))->SetCheck(
                     bits[i].flag & psd.Flags ? BST_CHECKED : BST_UNCHECKED);
        } /* process each */
     c_Result.SetWindowText(result);
     c_hDevMode.SetWindowText(psd.hDevMode);
     c_hDevNames.SetWindowText(psd.hDevNames);

     c_rtMinMargin_right.SetWindowText(psd.rtMinMargin.right);
     c_rtMinMargin_top.SetWindowText(psd.rtMinMargin.top);
     c_rtMinMargin_left.SetWindowText(psd.rtMinMargin.left);
     c_rtMinMargin_bottom.SetWindowText(psd.rtMinMargin.bottom);

     c_rtMargin_right.SetWindowText(psd.rtMargin.right);
     c_rtMargin_top.SetWindowText(psd.rtMargin.top);
     c_rtMargin_left.SetWindowText(psd.rtMargin.left);
     c_rtMargin_bottom.SetWindowText(psd.rtMargin.bottom);

     c_ptPaperSize_X.SetWindowText(psd.ptPaperSize.x);
     c_ptPaperSize_Y.SetWindowText(psd.ptPaperSize.y);

     if(psd.Flags & PSD_ENABLEPAGEPAINTHOOK)
        { /* has page hook */
         c_c_lpfnPagePaintHook.EnableWindow(TRUE);
         c_Messages.ShowWindow(SW_SHOW);
         c_lpfnPagePaintHook.SetWindowText((int)psd.lpfnPagePaintHook, 
                                                        _T("0x%08x"));
        } /* has page hook */
     else
        { /* no page hook */
         c_c_lpfnPagePaintHook.EnableWindow(FALSE);
         c_lpfnPagePaintHook.SetWindowText((LPCTSTR)_T("NULL"));
         c_Messages.ShowWindow(SW_HIDE);
        } /* no page hook */
    }

void CPgSetup::controls_to_psd()
    {
     int i;
     for(i = 0; bits[i].id != 0; i++)
        { /* process each */
         CButton * button = (CButton  *)GetDlgItem(bits[i].id);
         if(button->GetCheck())
            psd.Flags |= bits[i].flag;
         else
            psd.Flags &= ~bits[i].flag;
        } /* process each */
     psd.rtMinMargin.right = c_rtMinMargin_right.GetWindowInt();
     psd.rtMinMargin.top = c_rtMinMargin_top.GetWindowInt();
     psd.rtMinMargin.left = c_rtMinMargin_left.GetWindowInt();
     psd.rtMinMargin.bottom = c_rtMinMargin_bottom.GetWindowInt();

     psd.rtMargin.right = c_rtMargin_right.GetWindowInt();
     psd.rtMargin.top = c_rtMargin_top.GetWindowInt();
     psd.rtMargin.left = c_rtMargin_left.GetWindowInt();
     psd.rtMargin.bottom = c_rtMargin_bottom.GetWindowInt();

     psd.ptPaperSize.x = c_ptPaperSize_X.GetWindowInt();
     psd.ptPaperSize.y = c_ptPaperSize_Y.GetWindowInt();

     if(psd.Flags & PSD_ENABLEPAGEPAINTHOOK)
        { /* enable page hook */
         psd.lpfnPagePaintHook = (LPPAGEPAINTHOOK)pagePaint;
         psd.lpfnPageSetupHook = (LPPAGESETUPHOOK)pageSetup;
         psd.lCustData = (LPARAM)m_hWnd;            // for logging
         psd.Flags |= PSD_ENABLEPAGESETUPHOOK;  // for logging, we use this
        } /* enable page hook */
     else
        { /* no page hook */
         psd.lpfnPagePaintHook = NULL;
         psd.lpfnPageSetupHook = NULL;
         psd.Flags &= ~PSD_ENABLEPAGESETUPHOOK;  // turn off internal hack
        } /* no page hook */
    }
/////////////////////////////////////////////////////////////////////////////
// CPgSetup message handlers


/****************************************************************************
*                            CPgSetup::onLogPaint
* Inputs:
*       WPARAM wParam: Handle of page setup dialog window
*       LPARAM lParam: (LPCTSTR) reference to string to log, or NULL
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Logs the message to the message window.  If lParam is NULL, clears
*       the window.
****************************************************************************/

LRESULT CPgSetup::onLogPaint(WPARAM wParam, LPARAM lParam)
    {
     if(lParam == NULL)
        { /* clear it */
         c_Messages.DeleteAllItems();
         return 0;
        } /* clear it */

     CString id;
     id.LoadString(IDS_PAINTHOOK);
     c_Messages.addMessage((HANDLE)wParam, id, CString((LPCTSTR)lParam));
     return 0;
    }

void CPgSetup::RequiredSetup()
    {
     memset(&psd, 0, sizeof(psd));
     psd.lStructSize = sizeof(psd);
    }


void CPgSetup::OnDefaults() 
{
 PAGESETUPDLG pdsetup;

 memset(&pdsetup, 0, sizeof(pdsetup));
 pdsetup.lStructSize = sizeof(pdsetup);
 pdsetup.Flags = PD_RETURNDEFAULT;

 pdsetup.Flags |= ( psd.Flags & ( PSD_INHUNDREDTHSOFMILLIMETERS |
                                  PSD_INTHOUSANDTHSOFINCHES ));
 ::PageSetupDlg(&pdsetup);

 if(psd.hDevMode != NULL && psd.hDevMode != pdsetup.hDevMode)
    ::GlobalFree(psd.hDevMode);
 psd.hDevMode = pdsetup.hDevMode;
        
 if(psd.hDevNames != NULL && psd.hDevNames != pdsetup.hDevNames)
    ::GlobalFree(psd.hDevNames);
 psd.hDevNames = pdsetup.hDevNames;

 psd_to_controls();
        
}

void CPgSetup::OnPagesetup() 
{
 controls_to_psd();


 if(::PageSetupDlg(&psd))
    { /* success */
     result = 0;
     prt->psu = &psd;
    } /* success */
 else
    { /* error */
     result = CommDlgExtendedError();
     MessageBeep(0);
    } /* error */


 psd_to_controls();
        
}

BOOL CPgSetup::OnSetActive() 
{
        psd_to_controls();
        
        return CPropertyPage::OnSetActive();
}

void CPgSetup::OnPsdInhundredthsofmillimeters() 
{
 if(c_PSD_InHundredthsOfMillimeters.GetCheck())
    { /* .01 mm */
     c_PSD_InThousandthsOfInches.SetCheck(BST_UNCHECKED);
    } /* .01 mm */
        
}

void CPgSetup::OnPsdInthousandthsofinches() 
{
 if(c_PSD_InThousandthsOfInches.GetCheck())
    { /* .001 in */
     c_PSD_InHundredthsOfMillimeters.SetCheck(BST_UNCHECKED);
    } /* .001 in */
        
}

HBRUSH CPgSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(nCtlColor == CTLCOLOR_EDIT && pWnd->m_hWnd == c_Result.m_hWnd)
           { /* result */
            return ((CCommErr *)pWnd)->OnCtlColor(pDC);
           } /* result */

        return hbr;
}

void CPgSetup::OnClear() 
{
 if(psd.hDevMode != NULL)
    ::GlobalFree(psd.hDevMode);
 if(psd.hDevNames != NULL)
    ::GlobalFree(psd.hDevNames);

 prt->psu = NULL;  // no setup if we've cleared it

 RequiredSetup();
 psd_to_controls();
        
}

BOOL CPgSetup::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        logPageInit();

        CString s;
        s.LoadString(IDS_PAINTHOOK);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        RequiredSetup();
        psd_to_controls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CPgSetup::OnDestroy() 
{
        CPropertyPage::OnDestroy();
        
        if(psd.hDevNames != NULL)
           ::GlobalFree(psd.hDevNames);
        if(psd.hDevMode != NULL)
           ::GlobalFree(psd.hDevMode);
        
}

void CPgSetup::OnPsdEnablepagepainthook() 
{
 controls_to_psd();
 psd_to_controls();
}

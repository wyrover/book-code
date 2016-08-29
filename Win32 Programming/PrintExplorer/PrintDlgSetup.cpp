// PrintDlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "handleedit.h"
#include "numericedit.h"
#include "commerr.h"
#include "devnames.h"
#include "hdevmode.h"
#include "resname.h"
#include "PrintDlgSetup.h"
#include "printdlg.h"
#include "PrintHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrintDlgSetup property page

IMPLEMENT_DYNCREATE(CPrintDlgSetup, CPropertyPage)

CPrintDlgSetup::CPrintDlgSetup() : CPropertyPage(CPrintDlgSetup::IDD)
{
        //{{AFX_DATA_INIT(CPrintDlgSetup)
        //}}AFX_DATA_INIT
}

CPrintDlgSetup::~CPrintDlgSetup()
{
}

void CPrintDlgSetup::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPrintDlgSetup)
        DDX_Control(pDX, IDC_QUICKDATE, c_QuickDate);
        DDX_Control(pDX, IDC_lpSetupTemplateName, c_lpSetupTemplateName);
        DDX_Control(pDX, IDC_lpPrintTamplateName, c_lpPrintTemplateName);
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_nMaxPage, c_nMaxPage);
        DDX_Control(pDX, IDC_hwndOwner, c_hwndOwner);
        DDX_Control(pDX, IDC_hSetupTemplate, c_hSetupTemplate);
        DDX_Control(pDX, IDC_hPrintTemplate, c_hPrintTemplate);
        DDX_Control(pDX, IDC_nToPage, c_nToPage);
        DDX_Control(pDX, IDC_nMinPage, c_nMinPage);
        DDX_Control(pDX, IDC_nFromPage, c_nFromPage);
        DDX_Control(pDX, IDC_lStructSize, c_lStructSize);
        DDX_Control(pDX, IDC_nCopies, c_nCopies);
        DDX_Control(pDX, IDC_lpfnSetupHook, c_lpfnSetupHook);
        DDX_Control(pDX, IDC_lpfnPrintHook, c_lpfnPrintHook);
        DDX_Control(pDX, IDC_lCustData, c_lCustData);
        DDX_Control(pDX, IDC_hInstance, c_hInstance);
        DDX_Control(pDX, IDC_hDevNames, c_hDevNames);
        DDX_Control(pDX, IDC_hDevMode, c_hDevMode);
        DDX_Control(pDX, IDC_hDC, c_hDC);
        DDX_Control(pDX, IDC_Flags, c_Flags);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintDlgSetup, CPropertyPage)
        //{{AFX_MSG_MAP(CPrintDlgSetup)
        ON_BN_CLICKED(IDC_CLEAR, OnClear)
        ON_BN_CLICKED(IDC_DEFAULTS, OnDefaults)
        ON_BN_CLICKED(IDC_PRINTDLG, OnPrintdlg)
        ON_WM_CTLCOLOR()
        ON_BN_CLICKED(IDC_QUICKDC, OnQuickdc)
        ON_BN_CLICKED(IDC_QUICKDATE, OnQuickdate)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/****************************************************************************
*                       CPrintDlgSetup::pd_to_controls
* Result: void
*       
* Effect: 
*       Transfers the PRINTDLG contents to the controls.  Like DDX but
*       uses smart algorithms
****************************************************************************/

void CPrintDlgSetup::pd_to_controls()
    {
     c_lStructSize.SetWindowText(pd.lStructSize, _T("%d"));
     c_hwndOwner.SetWindowText(pd.hwndOwner);
     c_hDevMode.SetWindowText(pd.hDevMode);

     c_hDevNames.SetWindowText(pd.hDevNames);

     c_hDC.SetWindowText(pd.hDC);
     c_Flags.SetWindowText(pd.Flags, _T("%08x"));

     c_nFromPage.SetWindowText(pd.nFromPage);
     c_nToPage.SetWindowText(pd.nToPage);

     c_nMinPage.SetWindowText(pd.nMinPage);
     c_nMaxPage.SetWindowText(pd.nMaxPage);
     c_nCopies.SetWindowText(pd.nCopies);

     c_hInstance.SetWindowText(pd.hInstance);
     c_lCustData.SetWindowText(pd.lCustData,_T("%x"));

     c_lpPrintTemplateName.SetWindowText(pd.lpPrintTemplateName);
     c_lpSetupTemplateName.SetWindowText(pd.lpSetupTemplateName);

     c_lpfnPrintHook.SetWindowText(pd.lpfnPrintHook);
     c_lpfnSetupHook.SetWindowText(pd.lpfnSetupHook);

     c_hPrintTemplate.SetWindowText(pd.hPrintTemplate);
     c_hSetupTemplate.SetWindowText(pd.hSetupTemplate);

     c_Result.SetWindowText(*result);

     c_QuickDate.EnableWindow((pd.Flags & PD_ENABLEPRINTTEMPLATE) ? TRUE : FALSE);
    }

/****************************************************************************
*                       CPrintDlgSetup::controls_to_pd
* Result: void
*       
* Effect: 
*       Copies contents of controls to the PD
****************************************************************************/

void CPrintDlgSetup::controls_to_pd()
    {
     pd.nFromPage = c_nFromPage.GetWindowInt();
     pd.nToPage = c_nToPage.GetWindowInt();
     pd.nMinPage = c_nMinPage.GetWindowInt();
     pd.nMaxPage = c_nMaxPage.GetWindowInt();
     pd.nCopies = c_nCopies.GetWindowInt();
     pd.lCustData = c_lCustData.GetWindowInt();
    }

/////////////////////////////////////////////////////////////////////////////
// CPrintDlgSetup message handlers

void CPrintDlgSetup::RequiredSetup()
    {
     pd.lStructSize = sizeof(pd);
     pd.hwndOwner = GetParent()->m_hWnd;
    }

void CPrintDlgSetup::OnClear() 
{
 if(pd.hDevMode != NULL)
    ::GlobalFree(pd.hDevMode);
 if(pd.hDevNames != NULL)
    ::GlobalFree(pd.hDevNames);

 // having released any resources we had, re-initialize the structure
 memset(&pd, 0, sizeof(pd));

 RequiredSetup();
 pd_to_controls();
        
}

void CPrintDlgSetup::OnDefaults() 
{
 PRINTDLG pdsetup;

 memset(&pdsetup, 0, sizeof(pdsetup));
 pdsetup.lStructSize = sizeof(pdsetup);
 pdsetup.Flags = PD_RETURNDEFAULT;

 ::PrintDlg(&pdsetup);

 if(pd.hDevMode != NULL && pd.hDevMode != pdsetup.hDevMode)
    ::GlobalFree(pd.hDevMode);
 pd.hDevMode = pdsetup.hDevMode;
        
 if(pd.hDevNames != NULL && pd.hDevNames != pdsetup.hDevNames)
    ::GlobalFree(pd.hDevNames);
 pd.hDevNames = pdsetup.hDevNames;

 pd_to_controls();
}

BOOL CPrintDlgSetup::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        RequiredSetup();
        pd_to_controls();
        

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPrintDlgSetup::OnSetActive() 
{
        pd_to_controls();
        
        return CPropertyPage::OnSetActive();
}

BOOL CPrintDlgSetup::OnKillActive() 
{
        controls_to_pd();
        
        return CPropertyPage::OnKillActive();
}

void CPrintDlgSetup::OnPrintdlg() 
    {
     controls_to_pd();

     CPrintDlg printdlg(&pd);
     if(printdlg.DoModal())
        *result = 0;
     else
        { /* error */
         MessageBeep(0);
#if 1
         *result = CommDlgExtendedError();
#else
         *result = GetLastError();
#endif
        } /* error */

     pd_to_controls();
    }

HBRUSH CPrintDlgSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(nCtlColor == CTLCOLOR_EDIT && pWnd->m_hWnd == c_Result.m_hWnd)
           { /* result */
            return ((CCommErr*)pWnd)->OnCtlColor(pDC);
           } /* result */
            
        return hbr;
}


void CPrintDlgSetup::OnQuickdc() 
{
 controls_to_pd();
 pd.Flags |= PD_RETURNDC | PD_PRINTTOFILE;
 pd_to_controls();
 OnPrintdlg(); 
        
}

void CPrintDlgSetup::OnQuickdate() 
{
 SYSTEMTIME t;

 GetLocalTime(&t);

 controls_to_pd();
 pd.nMinPage = encodeYYMM(t.wYear - 1, 1);  // Jan of last year
 pd.nMaxPage = encodeYYMM(t.wYear + 2,12);  // Dec of two years from now
 pd.nFromPage = encodeYYMM(t.wYear, 1);     // Jan of this year
 pd.nToPage =   encodeYYMM(t.wYear,  12);   // Dec of this year
 pd_to_controls();
}

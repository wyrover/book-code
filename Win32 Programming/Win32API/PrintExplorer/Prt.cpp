// Prt.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "NumericEdit.h"
#include "printit.h"
#include "hdng.h"
#include "Prt.h"
#include "custom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int fakeline;
static int fakelimit = 100;
#define UWM_UPDATE (WM_USER+100)



/****************************************************************************
*                                  HdrPageNo
* Inputs:
*       LPPrintData data: Print data block
*       LPTSTR buffer:  Place to format the number
*       int len: Maximum length of the buffer (ignored, actually)
* Result: int
*       PH_Continue, always
* Effect: 
*       Formats the page number into the buffer
****************************************************************************/

int CALLBACK HdrPageNo(LPPrintData data, LPTSTR buffer, int len)
    {
     wsprintf(buffer, _T("%d"), data->pageno);
     return PH_Continue;
    }

/****************************************************************************
*                                   HdrDate
*       LPPrintData data: Print data block
*       LPTSTR buffer:  Place to format the number
*       int len: Maximum length of the buffer (ignored, actually)
* Result: int
*       PH_Continue, always
* Effect: 
*       Formats the data 
* Notes:
*       Not yet implemented
****************************************************************************/

int CALLBACK HdrDate(LPPrintData data, LPTSTR buffer, int len)
    {
     lstrcpy(buffer, "00/00/96");
     return PH_Continue;
    }

/****************************************************************************
*                                 HdrFile
*       LPPrintData data: Print data block
*       LPTSTR buffer:  Place to format the number
*       int len: Maximum length of the buffer (ignored, actually)
* Result: int
*       PH_Continue, always
* Effect: 
*       Formats the file name into the buffer, if one is given
****************************************************************************/

int CALLBACK HdrFile(LPPrintData data, LPTSTR buffer, int len)
    {
     if(data->UserData != NULL)
        lstrcpyn(buffer, (LPCTSTR)data->UserData, len);
     else
        lstrcpy(buffer, _T(""));
     return PH_Continue;
    }


/****************************************************************************
*                                  QueryFunc
* Inputs:
*       LPPrintData data
* Result: int
*       PQ_Continue if we have not hit the end yet
*       PQ_Stop if we have hit EOF
****************************************************************************/

int CALLBACK QueryFunc(LPPrintData data)
    {
     if(data->UserData == NULL)
        { /* simulated */
         if(fakeline > fakelimit)
            return PQ_Stop;
         else
            return PQ_Continue;
        } /* simulated */
     else
        { /* real */
         return PH_Stop;  // TODO: write this code!
        } /* real */
    }


/****************************************************************************
*                                   fmtline
* Inputs:
*       LPPrintData data: Data representative
* Result: int
*       PI_Continue to continue
*       PI_Stop if EOF
* Effect: 
*       Formats the line and advances ypos
****************************************************************************/

int CALLBACK fmtline(LPPrintData data)
    {
     TCHAR buffer[256];
     if(data->UserData == NULL)
        { /* fake it */
         fakeline++;
         wsprintf(buffer, _T("Line %d"), fakeline); // TODO: get from LoadString
         
         ::TextOut(data->pd->hDC, data->page.left, data->ypos, buffer, lstrlen(buffer));
         data->ypos += data->dy;

         // log our changes to our parent debugging window
         ::SendMessage(data->Owner, UWM_UPDATE, 0, 0);

         return (fakeline < fakelimit ? PI_Continue : PI_Stop);
        } /* fake it */
     else
        { /* real */
         return PI_Stop;  // TODO: Write this code
        } /* real */
    }

HdngLoad heading_options[] = {
    { IDS_HDR_NONE,     (LPPrintitHeaderFunc) NULL },
    { IDS_HDR_FILE,     HdrFile },
    { IDS_HDR_DATE,     HdrDate },
    { IDS_HDR_PAGENO,   HdrPageNo },
    { 0, NULL } // EOT
                       };

/////////////////////////////////////////////////////////////////////////////
// CPrt property page

IMPLEMENT_DYNCREATE(CPrt, CPropertyPage)

CPrt::CPrt() : CPropertyPage(CPrt::IDD)
    {
     memset(&prt, 0, sizeof(prt));
     prt.lStructSize = sizeof(prt);

     //{{AFX_DATA_INIT(CPrt)
     //}}AFX_DATA_INIT
    }

CPrt::~CPrt()
{
}

void CPrt::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPrt)
        DDX_Control(pDX, IDC_50, c_50);
        DDX_Control(pDX, IDC_500, c_500);
        DDX_Control(pDX, IDC_200, c_200);
        DDX_Control(pDX, IDC_1000, c_1000);
        DDX_Control(pDX, IDC_RIGHT, c_Right);
        DDX_Control(pDX, IDC_LEFT, c_Left);
        DDX_Control(pDX, IDC_HOOK, c_Hook);
        DDX_Control(pDX, IDC_YPOS, c_YPos);
        DDX_Control(pDX, IDC_BOTTOM, c_Bottom);
        DDX_Control(pDX, IDC_TOP, c_Top);
        DDX_Control(pDX, IDC_PAGENO, c_PageNo);
        DDX_Control(pDX, IDC_DY, c_DY);
        DDX_Control(pDX, IDC_PRINT, c_Print);
        DDX_Control(pDX, IDC_ENABLE_QUERY, c_Query);
        DDX_Control(pDX, IDC_TOPRIGHT, c_TopRight);
        DDX_Control(pDX, IDC_TOPLEFT, c_TopLeft);
        DDX_Control(pDX, IDC_TOPCENTER, c_TopCenter);
        DDX_Control(pDX, IDC_BOTTOMRIGHT, c_BottomRight);
        DDX_Control(pDX, IDC_BOTTOMLEFT, c_BottomLeft);
        DDX_Control(pDX, IDC_BOTTOMCENTER, c_BottomCenter);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrt, CPropertyPage)
        //{{AFX_MSG_MAP(CPrt)
        ON_BN_CLICKED(IDC_ENABLE_QUERY, OnEnableQuery)
        ON_BN_CLICKED(IDC_PRINT, OnPrint)
        //}}AFX_MSG_MAP
        ON_MESSAGE(UWM_UPDATE, OnUserUpdate)
END_MESSAGE_MAP()

void CPrt::prt_to_controls()
    {
     c_TopLeft.SelectFunc(prt.TopLeft);
     c_TopCenter.SelectFunc(prt.TopCenter);
     c_TopRight.SelectFunc(prt.TopRight);
     c_BottomLeft.SelectFunc(prt.BottomLeft);
     c_BottomCenter.SelectFunc(prt.BottomCenter);
     c_BottomRight.SelectFunc(prt.BottomRight);
     c_Query.SetCheck(prt.Query != NULL ? BST_CHECKED : BST_UNCHECKED);
     c_Print.EnableWindow(pd->hDC != NULL);
     if(prt.Flags & (PrintIt_ENABLEHOOK | PrintIt_ENABLETEMPLATE))
        { /* flags enabled */
         // If any flag is set, force the correct values in
         prt.Flags |= (PrintIt_ENABLEHOOK | PrintIt_ENABLETEMPLATE);
         prt.hInst = AfxGetInstanceHandle();
         prt.Template = MAKEINTRESOURCE(IDD_CUSTOMPRINTING);
         prt.Hook = CustomHook;
         c_Hook.SetCheck(BST_CHECKED);  // indicate we are enabled
        } /* flags enabled */
     else
        c_Hook.SetCheck(BST_UNCHECKED);

     BOOL toPrinter = (prt.prdata.pd->Flags & PD_PRINTTOFILE) ? TRUE : FALSE;
     if(toPrinter)
        { /* all options */
         c_50.EnableWindow(TRUE);
         c_200.EnableWindow(TRUE);
         c_500.EnableWindow(TRUE);
         c_1000.EnableWindow(TRUE);
        } /* all options */
     else
        { /* small options */
         c_50.EnableWindow(TRUE);
         c_200.EnableWindow(TRUE);
         c_500.EnableWindow(FALSE);
         c_1000.EnableWindow(FALSE);
        } /* small options */


     int id;
     switch(fakelimit)
        { /* fakelimit */
         case 200:
                 id = IDC_200;
                 break;
         case 500:
                 id = (toPrinter ? IDC_500 : IDC_50);
                 break;
         case 1000:
                 id = (toPrinter ? IDC_1000 : IDC_50);
                 break;
         default:
                 id = IDC_50;
                 break;
        } /* fakelimit */
     CheckRadioButton(IDC_50, IDC_1000, id);

     prd_to_controls();
    }

void CPrt::controls_to_prt()
    {
     prt.TopLeft = c_TopLeft.GetFunc();
     prt.TopCenter = c_TopCenter.GetFunc();
     prt.TopRight = c_TopRight.GetFunc();
     prt.BottomLeft = c_BottomLeft.GetFunc();
     prt.BottomCenter = c_BottomCenter.GetFunc();
     prt.BottomRight = c_BottomRight.GetFunc();
     
     prt.Query = (c_Query.GetCheck() ? QueryFunc : NULL);
     if(c_Hook.GetCheck())
        { /* set hook */
         prt.Flags |= (PrintIt_ENABLEHOOK | PrintIt_ENABLETEMPLATE);
         prt.hInst = AfxGetInstanceHandle();
         prt.Template = MAKEINTRESOURCE(IDD_CUSTOMPRINTING);
         prt.Hook = CustomHook;
        } /* set hook */
     else
        { /* clear hook */
         prt.Flags &= ~(PrintIt_ENABLEHOOK | PrintIt_ENABLETEMPLATE);
         prt.hInst = NULL;
         prt.Template = NULL;
         prt.Hook = NULL;
        } /* clear hook */
     fakelimit = 50;
     if(c_200.GetCheck())
        fakelimit = 200;
     else
     if(c_500.GetCheck())
        fakelimit = 500;
     else
     if(c_1000.GetCheck())
        fakelimit = 1000;
     
    }

/****************************************************************************
*                            CPrt::prd_to_controls
* Result: void
*       
* Effect: 
*       Updates the prdata controls
****************************************************************************/

void CPrt::prd_to_controls()
    {
     c_YPos.SetWindowText(prt.prdata.ypos);
     c_Bottom.SetWindowText(prt.prdata.page.bottom);
     c_Top.SetWindowText(prt.prdata.page.top);
     c_Left.SetWindowText(prt.prdata.page.left);
     c_Right.SetWindowText(prt.prdata.page.right);
     c_PageNo.SetWindowText(prt.prdata.pageno);
     c_DY.SetWindowText(prt.prdata.dy);
    }

/////////////////////////////////////////////////////////////////////////////
// CPrt message handlers

BOOL CPrt::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        c_TopLeft.LoadFuncs(heading_options);

        c_TopCenter.LoadFuncs(heading_options);

        c_TopRight.LoadFuncs(heading_options);

        c_BottomLeft.LoadFuncs(heading_options);

        c_BottomCenter.LoadFuncs(heading_options);

        c_BottomRight.LoadFuncs(heading_options);
        
        
        prt.prdata.DocName = _T("Document"); // TODO: fix this
        prt.prdata.UserData = NULL; // TODO: file name of open file in view
        prt.prdata.pd = pd;
        prt.prdata.Owner = m_hWnd;
        prt.FormatLine = fmtline;

        fakelimit = (prt.prdata.pd->Flags & PD_PRINTTOFILE) ? 1000 : 200;
           
        prt_to_controls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrt::OnEnableQuery() 
{
        // TODO: Add your control notification handler code here
        
}

void CPrt::OnPrint() 
{
 if(prt.prdata.UserData == NULL)
    { /* fake it */
     fakeline = 0;
    } /* fake it */

 controls_to_prt();

 doPrintIt(&prt);

 prt_to_controls();
}

BOOL CPrt::OnSetActive() 
    {
     prt_to_controls();
        
     return CPropertyPage::OnSetActive();
    }

/****************************************************************************
*                             CPrt::OnUserUpdate
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Forces PrintData to be updated
****************************************************************************/

LRESULT CPrt::OnUserUpdate(WPARAM, LPARAM)
    {
     prd_to_controls();
     return 0;
    }

BOOL CPrt::OnKillActive() 
{
        controls_to_prt();
        
        return CPropertyPage::OnKillActive();
}

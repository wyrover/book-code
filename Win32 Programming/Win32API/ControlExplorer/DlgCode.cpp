// DlgCode.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "dlgcodes.h"
#include "DlgCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*****************************************************************************
// How to use this:
//
//      Set the ctl variable to the CWnd-derived object
//      Call the dialog
//
// The CWnd has to use the data word set by the dlgcodes.h file in its
// OnGetDlgCode handler.

/////////////////////////////////////////////////////////////////////////////
// CGetDlgCode dialog


CGetDlgCode::CGetDlgCode(CWnd* pParent /*=NULL*/)
        : CDialog(CGetDlgCode::IDD, pParent)
{
        //{{AFX_DATA_INIT(CGetDlgCode)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CGetDlgCode::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CGetDlgCode)
        DDX_Control(pDX, IDC_DLGC_UNDEFPUSHBUTTON, c_UndefPushbutton);
        DDX_Control(pDX, IDC_DLGC_STATIC, c_Static);
        DDX_Control(pDX, IDC_USEDEFAULT, c_UseDefault);
        DDX_Control(pDX, IDC_PUSHBUTTON, c_PushButton);
        DDX_Control(pDX, IDC_DLGC_WANTTAB, c_WantTab);
        DDX_Control(pDX, IDC_DLGC_WANTMESSAGE, c_WantMessage);
        DDX_Control(pDX, IDC_DLGC_WANTCHARS, c_WantChars);
        DDX_Control(pDX, IDC_DLGC_WANTARROWS, c_WantArrows);
        DDX_Control(pDX, IDC_DLGC_WANTALLKEYS, c_WantAllKeys);
        DDX_Control(pDX, IDC_DLGC_RADIOBUTTON, c_RadioButton);
        DDX_Control(pDX, IDC_DLGC_HASSETSEL, c_HasSetSel);
        DDX_Control(pDX, IDC_DLGC_DEFPUSHBUTTON, c_DefPushbutton);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetDlgCode, CDialog)
        //{{AFX_MSG_MAP(CGetDlgCode)
        ON_BN_CLICKED(IDC_DLGC_WANTALLKEYS, OnDlgcWantallkeys)
        ON_BN_CLICKED(IDC_DLGC_WANTMESSAGE, OnDlgcWantmessage)
        ON_BN_CLICKED(IDC_USEDEFAULT, OnUsedefault)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDlgCode message handlers

void CGetDlgCode::OnDlgcWantallkeys() 
{
 c_WantMessage.SetCheck(c_WantAllKeys.GetCheck());
        
}

void CGetDlgCode::OnDlgcWantmessage() 
{
 c_WantAllKeys.SetCheck(c_WantMessage.GetCheck());
        
}

void CGetDlgCode::OnUsedefault() 
{
        // TODO: Add your control notification handler code here
        
}

/****************************************************************************
*                         CGetDlgCode::controlsToCode
* Result: UINT
*       Bitmask representing the various codes. 
* Notes: 
*       If the "Use default" check box is set, the DLGCX_ACTIVE bit is
*       not set.  The DLGCX_VALID bit will always be set.
****************************************************************************/

UINT CGetDlgCode::controlsToCode()
    {
     UINT code = DLGCX_VALID;

     if(c_WantArrows.GetCheck())
        code |= DLGC_WANTARROWS;

     if(c_WantTab.GetCheck())
        code |= DLGC_WANTTAB;

     if(c_WantAllKeys.GetCheck())
        code |= DLGC_WANTALLKEYS;

     if(c_WantMessage.GetCheck())
        code |= DLGC_WANTMESSAGE;

     if(c_HasSetSel.GetCheck())
        code |= DLGC_HASSETSEL;

     if(c_DefPushbutton.GetCheck())
        code |= DLGC_DEFPUSHBUTTON;

     if(c_UndefPushbutton.GetCheck())
        code |= DLGC_UNDEFPUSHBUTTON;

     if(c_RadioButton.GetCheck())
        code |= DLGC_RADIOBUTTON;

     if(c_WantChars.GetCheck())
        code |= DLGC_WANTCHARS;

     if(c_Static.GetCheck())
        code |= DLGC_STATIC;

     if(c_PushButton.GetCheck())
        code |= DLGC_BUTTON;

     if(!c_UseDefault.GetCheck())
        code |= DLGCX_ACTIVE;

     return code;

    }

void CGetDlgCode::OnOK() 
{
 UINT codes = controlsToCode();
        
 setDlgCode(ctl->m_hWnd, codes);

 CDialog::OnOK();
}

/****************************************************************************
*                         CGetDlgCode::codeToControls
* Inputs:
*       UINT codes: Code to set
* Result: void
*       
* Effect: 
*       Transfers the code to the control.  If the code
****************************************************************************/

void CGetDlgCode::codeToControls(UINT codes)
    {
     c_WantArrows.SetCheck(codes & DLGC_WANTARROWS ? TRUE : FALSE);
     c_WantTab.SetCheck(codes & DLGC_WANTTAB ? TRUE : FALSE);
     c_WantAllKeys.SetCheck(codes & DLGC_WANTALLKEYS ? TRUE : FALSE);
     c_WantMessage.SetCheck(codes & DLGC_WANTMESSAGE ? TRUE : FALSE);
     c_HasSetSel.SetCheck(codes & DLGC_HASSETSEL ? TRUE : FALSE);
     c_DefPushbutton.SetCheck(codes & DLGC_DEFPUSHBUTTON ? TRUE : FALSE);
     c_UndefPushbutton.SetCheck(codes & DLGC_UNDEFPUSHBUTTON ? TRUE : FALSE);
     c_RadioButton.SetCheck(codes & DLGC_RADIOBUTTON ? TRUE : FALSE);
     c_WantChars.SetCheck(codes & DLGC_WANTCHARS ? TRUE : FALSE);
     c_Static.SetCheck(codes & DLGC_STATIC ? TRUE : FALSE);
     c_PushButton.SetCheck(codes & DLGC_BUTTON ? TRUE : FALSE);
    }

BOOL CGetDlgCode::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        UINT codes = getDlgCode(ctl->m_hWnd);
        
        if(!(codes & DLGCX_VALID))
            codes = ctl->SendMessage(WM_GETDLGCODE);

        codeToControls(codes);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

// Effects.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "intedit.h"
#include "tinybutt.h"
#include "ComboOps.h"
#include "Effects.h"
#include "Is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDDEffects property page

IMPLEMENT_DYNCREATE(CDDEffects, CComboOps)

CDDEffects::CDDEffects() : CComboOps(CDDEffects::IDD)
{
        //{{AFX_DATA_INIT(CDDEffects)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CDDEffects::~CDDEffects()
{
}

void CDDEffects::DoDataExchange(CDataExchange* pDX)
{
        CComboOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDDEffects)
        DDX_Control(pDX, IDC_SPINDROPPEDWIDTH, c_SpinDroppedWidth);
        DDX_Control(pDX, IDC_SHOWDROPDOWN, c_ShowDropDown);
        DDX_Control(pDX, IDC_SETEXTENDEDUI, c_SetExtendedUI);
        DDX_Control(pDX, IDC_SETDROPPEDWIDTH, c_SetDroppedWidth);
        DDX_Control(pDX, IDC_RECT, c_Rect);
        DDX_Control(pDX, IDC_GETEXTENDEDUI, c_GetExtendedUI);
        DDX_Control(pDX, IDC_GETDROPPEDWIDTH, c_GetDroppedWidth);
        DDX_Control(pDX, IDC_GETDROPPEDSTATE, c_GetDroppedState);
        DDX_Control(pDX, IDC_GETDROPPEDCONTROLRECT, c_GetDroppedControlRect);
        DDX_Control(pDX, IDC_EXTENDEDUI, c_ExtendedUI);
        DDX_Control(pDX, IDC_DROPPEDWIDTH, c_DroppedWidth);
        DDX_Control(pDX, IDC_DROPDOWN, c_DropDown);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDEffects, CComboOps)
        //{{AFX_MSG_MAP(CDDEffects)
        ON_BN_CLICKED(IDC_GETDROPPEDCONTROLRECT, OnGetdroppedcontrolrect)
        ON_BN_CLICKED(IDC_GETDROPPEDSTATE, OnGetdroppedstate)
        ON_BN_CLICKED(IDC_GETDROPPEDWIDTH, OnGetdroppedwidth)
        ON_BN_CLICKED(IDC_GETEXTENDEDUI, OnGetextendedui)
        ON_BN_CLICKED(IDC_SETDROPPEDWIDTH, OnSetdroppedwidth)
        ON_BN_CLICKED(IDC_SETEXTENDEDUI, OnSetextendedui)
        ON_BN_CLICKED(IDC_SHOWDROPDOWN, OnShowdropdown)
        ON_BN_CLICKED(IDC_DROPDOWN, OnDropdown)
        ON_BN_CLICKED(IDC_EXTENDEDUI, OnExtendedui)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDEffects message handlers

BOOL CDDEffects::OnInitDialog() 
{
        CComboOps::OnInitDialog();
        
        if(Is3())
           { /* API 3 */
            c_SpinDroppedWidth.EnableWindow(FALSE);
            c_SetDroppedWidth.EnableWindow(FALSE);
            c_GetDroppedWidth.EnableWindow(FALSE);
            c_DroppedWidth.EnableWindow(FALSE);
           } /* API 3 */
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CDDEffects::OnGetdroppedcontrolrect() 
{
 CRect r;
 int result = combo->SendMessage(CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&r);

 showResult_N_R_N(IDS_CB_GETDROPPEDCONTROLRECT, &r);
 CString s;
 s.Format(_T("[%d, %d, %d, %d]"), r.left, r.top, r.right, r.bottom);
 c_Rect.SetWindowText(s);
}

void CDDEffects::OnGetdroppedstate() 
{
 int result = combo->SendMessage(CB_GETDROPPEDSTATE);
 showResult_N_N_d(IDS_CB_GETDROPPEDSTATE, result);
        
}

void CDDEffects::OnGetdroppedwidth() 
{
 int width = combo->SendMessage(CB_GETDROPPEDWIDTH);
 showResult_N_N_d(IDS_CB_GETDROPPEDWIDTH, width);
 c_DroppedWidth.SetVal(width);
}

void CDDEffects::OnGetextendedui() 
{
 int result = combo->SendMessage(CB_GETEXTENDEDUI);
 showResult_N_N_d(IDS_CB_GETEXTENDEDUI, result);
 c_ExtendedUI.SetCheck(result);
}

void CDDEffects::OnSetdroppedwidth() 
{
 int width = c_DroppedWidth.GetVal();
 int result = combo->SendMessage(CB_SETDROPPEDWIDTH, width);
 showResult_d_N_CBERR(IDS_CB_SETDROPPEDWIDTH, width, result);
}

void CDDEffects::OnSetextendedui() 
{
 int val = c_ExtendedUI.GetCheck();
 int result = combo->SendMessage(CB_SETEXTENDEDUI, val);
 showResult_d_N_CBERR(IDS_CB_SETEXTENDEDUI, val, result);
}

void CDDEffects::OnShowdropdown() 
{
 int val = c_DropDown.GetCheck();
 combo->SendMessage(CB_SHOWDROPDOWN, val);
 showResult_d_N_N(IDS_CB_SHOWDROPDOWN, val);
}

void CDDEffects::OnDropdown() 
{
 // nothing needed here right now
}

void CDDEffects::OnExtendedui() 
{
 // nothing needed here right now
}

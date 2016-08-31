// DDState.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "indexpg.h"
#include "loggable.h"
#include "intedit.h"
#include "tinybutt.h"
#include "locales.h"
#include "comboOps.h"
#include "DDState.h"
#include "Is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDDState property page

IMPLEMENT_DYNCREATE(CDDState, CComboOps)

CDDState::CDDState() : CComboOps(CDDState::IDD)
{
        //{{AFX_DATA_INIT(CDDState)
        //}}AFX_DATA_INIT
}

CDDState::~CDDState()
{
}

void CDDState::DoDataExchange(CDataExchange* pDX)
{
        CComboOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDDState)
        DDX_Control(pDX, IDC_LOCALES, c_Locales);
        DDX_Control(pDX, IDC_TOPINDEX, c_TopIndex);
        DDX_Control(pDX, IDC_SPINTOPINDEX, c_SpinTopIndex);
        DDX_Control(pDX, IDC_SPINSELECTION, c_SpinSelection);
        DDX_Control(pDX, IDC_SPINLIMIT, c_SpinLimit);
        DDX_Control(pDX, IDC_SPINHORIZONTALEXTENT, c_SpinHorizontalExtent);
        DDX_Control(pDX, IDC_SETTOPINDEX, c_SetTopIndex);
        DDX_Control(pDX, IDC_SETLOCALE, c_SetLocale);
        DDX_Control(pDX, IDC_SETHORIZONTALEXTENT, c_SetHorizontalExtent);
        DDX_Control(pDX, IDC_SETCURSEL, c_SetCurSel);
        DDX_Control(pDX, IDC_SELECTION, c_Selection);
        DDX_Control(pDX, IDC_GETCOUNT, c_GetCount);
        DDX_Control(pDX, IDC_GETCURSEL, c_GetCurSel);
        DDX_Control(pDX, IDC_GETHORIZONTALEXTENT, c_GetHorizontalExtent);
        DDX_Control(pDX, IDC_GETLOCALE, c_GetLocale);
        DDX_Control(pDX, IDC_GETTOPINDEX, c_GetTopIndex);
        DDX_Control(pDX, IDC_HORIZONTALEXTENT, c_HorizontalExtent);
        DDX_Control(pDX, IDC_LIMITTEXT, c_LimitText);
        DDX_Control(pDX, IDC_LIMITVAL, c_LimitVal);
        DDX_Control(pDX, IDC_RESETCONTENT, c_ResetContent);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDState, CComboOps)
        //{{AFX_MSG_MAP(CDDState)
        ON_BN_CLICKED(IDC_GETCOUNT, OnGetcount)
        ON_BN_CLICKED(IDC_GETHORIZONTALEXTENT, OnGethorizontalextent)
        ON_BN_CLICKED(IDC_GETLOCALE, OnGetlocale)
        ON_BN_CLICKED(IDC_GETTOPINDEX, OnGettopindex)
        ON_BN_CLICKED(IDC_LIMITTEXT, OnLimittext)
        ON_BN_CLICKED(IDC_GETCURSEL, OnGetcursel)
        ON_BN_CLICKED(IDC_SETCURSEL, OnSetcursel)
        ON_BN_CLICKED(IDC_SETLOCALE, OnSetlocale)
        ON_BN_CLICKED(IDC_SETTOPINDEX, OnSettopindex)
        ON_BN_CLICKED(IDC_RESETCONTENT, OnResetcontent)
        ON_BN_CLICKED(IDC_SETHORIZONTALEXTENT, OnSethorizontalextent)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDState message handlers

void CDDState::OnGetcount() 
{
 int result = combo->SendMessage(CB_GETCOUNT);
 showResult_N_N_d(IDS_CB_GETCOUNT, result);
        
}

void CDDState::OnGethorizontalextent() 
{
 int result = combo->SendMessage(CB_GETHORIZONTALEXTENT);
 showResult_N_N_d(IDS_CB_GETHORIZONTALEXTENT, result);
 c_HorizontalExtent.SetVal(result);
}

void CDDState::OnGetlocale() 
{
 DWORD result = combo->SendMessage(CB_GETLOCALE);
 showResult_N_N_DW(IDS_CB_GETLOCALE, result);
}

void CDDState::OnGettopindex() 
{
 int result = combo->SendMessage(CB_GETTOPINDEX);
 showResult_N_N_d(IDS_CB_GETTOPINDEX, result);
 c_TopIndex.SetVal(result);
}

void CDDState::OnLimittext() 
{
 int limit = c_LimitVal.GetVal();
 combo->SendMessage(CB_LIMITTEXT, limit);
 showResult_d_N_N(IDS_CB_LIMITTEXT, limit);
}

void CDDState::OnGetcursel() 
{
 int result = combo->SendMessage(CB_GETCURSEL);
 showResult_N_N_CBERR(IDS_CB_GETCURSEL, result);
 c_Selection.SetVal(result);
}

void CDDState::OnSetcursel() 
{
 int index = c_Selection.GetVal();
 int result = combo->SendMessage(CB_SETCURSEL, index);
 showResult_d_N_CBERR(IDS_CB_SETCURSEL, index, result);
        
}

void CDDState::OnSetlocale() 
{
 int index = c_Locales.GetCurSel();
 if(index == CB_ERR)
    return;

 DWORD locale = c_Locales.GetItemData(index);
 DWORD result = combo->SendMessage(CB_SETLOCALE, locale);
 showResult_DW_N_DW(IDS_CB_SETLOCALE, locale, result, IDS_CB_ERR);
}

void CDDState::OnSettopindex() 
{
 int index = c_TopIndex.GetVal();
 int result = combo->SendMessage(CB_SETTOPINDEX, index);
 showResult_d_N_CBERR(IDS_CB_SETTOPINDEX, index, result);
        
}

void CDDState::OnResetcontent() 
{
 combo->SendMessage(CB_RESETCONTENT);
 showResult_N_N_N(IDS_CB_RESETCONTENT);
}

void CDDState::OnSethorizontalextent() 
{
 int val = c_HorizontalExtent.GetVal();
 
 combo->SendMessage(CB_SETHORIZONTALEXTENT, val);
 showResult_d_N_N(IDS_CB_SETHORIZONTALEXTENT, val);
        
}

/****************************************************************************
*                          CDDState::updateControls
* Result: void
*       
* Effect: 
*       Sets new values to the controls
****************************************************************************/

void CDDState::updateControls()
    {
     c_SpinSelection.SetRange(-1, combo->GetCount() - 1);     
     c_SpinTopIndex.SetRange(-1, combo->GetCount() - 1);
    }

BOOL CDDState::OnInitDialog() 
{
        CComboOps::OnInitDialog();

        if(Is3())
           { /* API 3 */
            c_SetTopIndex.EnableWindow(FALSE);
            c_GetTopIndex.EnableWindow(FALSE);
            c_SpinTopIndex.EnableWindow(FALSE);
            c_TopIndex.EnableWindow(FALSE);
            c_SetHorizontalExtent.EnableWindow(FALSE);
            c_GetHorizontalExtent.EnableWindow(FALSE);
            c_HorizontalExtent.EnableWindow(FALSE);
           } /* API 3 */
        
        
        c_SpinLimit.SetRange(0, 100);
        c_SpinHorizontalExtent.SetRange(0,1500);
        updateControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDDState::OnSetActive() 
{
        updateControls();
        
        return CComboOps::OnSetActive();
}

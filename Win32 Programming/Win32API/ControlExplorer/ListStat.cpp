// ListStat.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>  // for strtoul
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "listops.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "locales.h"
#include "ListStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListState property page

IMPLEMENT_DYNCREATE(CListState, CListOps)

CListState::CListState() : CListOps(CListState::IDD)
{
        //{{AFX_DATA_INIT(CListState)
        //}}AFX_DATA_INIT
}

CListState::~CListState()
{
}

void CListState::DoDataExchange(CDataExchange* pDX)
{
        CListOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListState)
        DDX_Control(pDX, IDC_HORIZONTALEXTENT, c_HorizontalExtent);
        DDX_Control(pDX, IDC_SELITEMRANGEFLAG, c_SelItemRangeFlag);
        DDX_Control(pDX, IDC_SPIN_RANGELOW, c_SpinRangeLow);
        DDX_Control(pDX, IDC_SPIN_RANGEHIGH, c_SpinRangeHigh);
        DDX_Control(pDX, IDC_SPIN_ITEMHEIGHT, c_SpinItemHeight);
        DDX_Control(pDX, IDC_SPIN_INDEX, c_SpinIndex);
        DDX_Control(pDX, IDC_SPIN_COLUMNWIDTH, c_SpinColumnWidth);
        DDX_Control(pDX, IDC_INDEXVAL, c_Index);
        DDX_Control(pDX, IDC_LOCALES, c_Locales);
        DDX_Control(pDX, IDC_ITEMHEIGHT, c_ItemHeight);
        DDX_Control(pDX, IDC_COLUMNWIDTH, c_ColumnWidth);
        DDX_Control(pDX, IDC_RESETCONTENT, c_ResetContent);
        DDX_Control(pDX, IDC_RANGELOW, c_RangeLow);
        DDX_Control(pDX, IDC_RANGEHIGH, c_RangeHigh);
        DDX_Control(pDX, IDC_REDRAWVAL, c_RedrawVal);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListState, CListOps)
        //{{AFX_MSG_MAP(CListState)
        ON_BN_CLICKED(IDC_RESETCONTENT, OnResetcontent)
        ON_BN_CLICKED(IDC_SELITEMRANGE, OnSelitemrange)
        ON_BN_CLICKED(IDC_SELITEMRANGEEX, OnSelitemrangeex)
        ON_BN_CLICKED(IDC_SETANCHORINDEX, OnSetanchorindex)
        ON_BN_CLICKED(IDC_SETCARETINDEX, OnSetcaretindex)
        ON_BN_CLICKED(IDC_SETCOLUMNWIDTH, OnSetcolumnwidth)
        ON_BN_CLICKED(IDC_SETITEMHEIGHT, OnSetitemheight)
        ON_BN_CLICKED(IDC_SETLOCALE, OnSetlocale)
        ON_BN_CLICKED(IDC_SETREDRAW, OnSetredraw)
        ON_BN_CLICKED(IDC_SETTOPINDEX, OnSettopindex)
        ON_BN_CLICKED(IDC_SETCURSEL, OnSetcursel)
        ON_BN_CLICKED(IDC_SETHORIZONTALEXTENT, OnSethorizontalextent)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListState message handlers

BOOL CListState::OnSetActive() 
{
        return CListOps::OnSetActive();
}


BOOL CListState::OnInitDialog() 
{
        CListOps::OnInitDialog();
        
        c_Locales.load();

        c_SpinRangeLow.SetRange(-1, 50);
        c_SpinRangeHigh.SetRange(-1, 50);
        c_SpinItemHeight.SetRange(0, 50);
        c_SpinItemHeight.SetPos(0);
        c_SpinIndex.SetRange(-1, 200);
        c_SpinIndex.SetPos(0);
        c_SpinColumnWidth.SetRange(0, 200);
        c_SpinColumnWidth.SetPos(20);  // random number
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CListState::OnResetcontent() 
{
 CListBox * box = getListBox();

 box->ResetContent();

 showResult_N_N_N(IDS_LB_RESETCONTENT);
        
}

void CListState::OnSelitemrange() 
{
 int first = c_RangeLow.GetVal();
 int last  = c_RangeHigh.GetVal();

 CListBox * box = getListBox();
 BOOL flag = c_SelItemRangeFlag.GetCheck();

 DWORD parms = MAKELONG(first, last);

 int result = box->SendMessage(LB_SELITEMRANGE, flag, parms);

 showResult_d_DW_LBERR(IDS_LB_SELITEMRANGE, flag, parms, result);
        
}

void CListState::OnSelitemrangeex() 
{
 int first = c_RangeLow.GetVal();
 int last  = c_RangeHigh.GetVal();

 CListBox * box = getListBox();

 int result = box->SendMessage(LB_SELITEMRANGEEX, first, last);

 showResult_d_d_LBERR(IDS_LB_SELITEMRANGEEX, first, last, result);
}

void CListState::OnSetanchorindex() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_SETANCHORINDEX, index);
 showResult_d_N_LBERR(IDS_LB_SETANCHORINDEX, index, result);
}

void CListState::OnSetcaretindex() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SetCaretIndex(index);
 showResult_d_N_LBERR(IDS_LB_SETCARETINDEX, index, result);
}

void CListState::OnSetcolumnwidth() 
{
 CListBox * box = getListBox();
 int width = c_ColumnWidth.GetVal();
 box->SetColumnWidth(width);
 showResult_d_N_N(IDS_LB_SETCOLUMNWIDTH, width);
}

void CListState::OnSetitemheight() 
{
 CListBox * box = getListBox();
 int height = c_ItemHeight.GetVal();
 int index = c_Index.GetVal();
 int result = box->SetItemHeight(index, height);
 showResult_d_d_LBERR(IDS_LB_SETITEMHEIGHT, index, height, result);

}

void CListState::OnSetlocale() 
{
 CListBox * box = getListBox();
 int index = c_Locales.GetCurSel();
 if(index == CB_ERR)
    return;

 DWORD locale = c_Locales.GetItemData(index);
 DWORD result = box->SendMessage(LB_SETLOCALE, locale);
 showResult_DW_N_DW(IDS_LB_SETLOCALE, locale, result, IDS_LB_ERR);
        
}

void CListState::OnSetredraw() 
{
 CListBox * box = getListBox();
 BOOL redraw = c_RedrawVal.GetCheck();
 box->SendMessage(WM_SETREDRAW, redraw);
 showResult_d_N_N(IDS_WM_SETREDRAW, redraw);
}

void CListState::OnSettopindex() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SetTopIndex(index);
 showResult_d_N_LBERR(IDS_LB_SETTOPINDEX, index, result);
        
}

void CListState::OnSetcursel() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SetCurSel(index);
 showResult_d_N_LBERR(IDS_LB_SETCURSEL, index, result);
        
}

void CListState::OnSethorizontalextent() 
{
 CListBox * box = getListBox();
 int val = c_HorizontalExtent.GetVal();

 box->SetHorizontalExtent(val);
 showResult_d_N_N(IDS_LB_SETHORIZONTALEXTENT, val);
        
}

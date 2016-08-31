// TrackPos.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "TrackOps.h"
#include "TrackPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackPos property page

IMPLEMENT_DYNCREATE(CTrackPos, CTrackOps)

CTrackPos::CTrackPos() : CTrackOps(CTrackPos::IDD)
{
        //{{AFX_DATA_INIT(CTrackPos)
        //}}AFX_DATA_INIT
}

CTrackPos::~CTrackPos()
{
}

void CTrackPos::DoDataExchange(CDataExchange* pDX)
{
        CTrackOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTrackPos)
        DDX_Control(pDX, IDC_SETRANGE, c_SetRange);
        DDX_Control(pDX, IDC_REDRAW, c_Redraw);
        DDX_Control(pDX, IDC_SETRANGEMIN, c_SetRangeMin);
        DDX_Control(pDX, IDC_SETRANGEMAX, c_SetRangeMax);
        DDX_Control(pDX, IDC_SETPOS, c_SetPos);
        DDX_Control(pDX, IDC_SETPAGESIZE, c_SetPageSize);
        DDX_Control(pDX, IDC_SETLINESIZE, c_SetLineSize);
        DDX_Control(pDX, IDC_MOVE, c_Move);
        DDX_Control(pDX, IDC_GETRANGEMIN, c_GetRangeMin);
        DDX_Control(pDX, IDC_GETRANGEMAX, c_GetRangeMax);
        DDX_Control(pDX, IDC_GETPOS, c_GetPos);
        DDX_Control(pDX, IDC_GETPAGESIZE, c_GetPageSize);
        DDX_Control(pDX, IDC_GETLINESIZE, c_GetLineSize);
        DDX_Control(pDX, IDC_SPINRANGEMIN, c_SpinRangeMin);
        DDX_Control(pDX, IDC_SPINRANGEMAX, c_SpinRangeMax);
        DDX_Control(pDX, IDC_SPINPOS, c_SpinPos);
        DDX_Control(pDX, IDC_SPINPAGESIZE, c_SpinPageSize);
        DDX_Control(pDX, IDC_SPINLINESIZE, c_SpinLineSize);
        DDX_Control(pDX, IDC_RANGEMIN, c_RangeMin);
        DDX_Control(pDX, IDC_RANGEMAX, c_RangeMax);
        DDX_Control(pDX, IDC_POS, c_Pos);
        DDX_Control(pDX, IDC_PAGESIZE, c_PageSize);
        DDX_Control(pDX, IDC_LINESIZE, c_LineSize);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackPos, CTrackOps)
        //{{AFX_MSG_MAP(CTrackPos)
        ON_BN_CLICKED(IDC_GETLINESIZE, OnGetlinesize)
        ON_BN_CLICKED(IDC_GETPAGESIZE, OnGetpagesize)
        ON_BN_CLICKED(IDC_GETPOS, OnGetpos)
        ON_BN_CLICKED(IDC_GETRANGEMAX, OnGetrangemax)
        ON_BN_CLICKED(IDC_GETRANGEMIN, OnGetrangemin)
        ON_BN_CLICKED(IDC_SETLINESIZE, OnSetlinesize)
        ON_BN_CLICKED(IDC_SETPAGESIZE, OnSetpagesize)
        ON_BN_CLICKED(IDC_SETPOS, OnSetpos)
        ON_BN_CLICKED(IDC_SETRANGEMAX, OnSetrangemax)
        ON_BN_CLICKED(IDC_SETRANGEMIN, OnSetrangemin)
        ON_BN_CLICKED(IDC_SETRANGE, OnSetrange)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackPos message handlers

void CTrackPos::OnGetlinesize() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETLINESIZE, 0, 0);
 showResult_N_N_d(IDS_TBM_GETLINESIZE, result);
 c_LineSize.SetWindowText(result);
}

void CTrackPos::OnGetpagesize() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETPAGESIZE, 0, 0);
 showResult_N_N_d(IDS_TBM_GETPAGESIZE, result);
 c_PageSize.SetWindowText(result);
}

void CTrackPos::OnGetpos() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETPOS, 0, 0);
 showResult_N_N_d(IDS_TBM_GETPOS, result);
 c_Pos.SetWindowText(result);
}

void CTrackPos::OnGetrangemax() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETRANGEMAX, 0, 0);
 showResult_N_N_d(IDS_TBM_GETRANGEMAX, result);
 c_RangeMax.SetWindowText(result);
}

void CTrackPos::OnGetrangemin() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETRANGEMIN, 0, 0);
 showResult_N_N_d(IDS_TBM_GETRANGEMIN, result);
 c_RangeMin.SetWindowText(result);
        
}

void CTrackPos::OnSetlinesize() 
{
 int val = c_LineSize.GetWindowInt();
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_SETLINESIZE, 0, val);
 showResult_d_d_d(IDS_TBM_SETLINESIZE, 0, val, result);
}

void CTrackPos::OnSetpagesize() 
{
 int val = c_PageSize.GetWindowInt();
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_SETPAGESIZE, 0, val);
 showResult_d_d_d(IDS_TBM_SETPAGESIZE, 0, val, result);
}

void CTrackPos::OnSetpos() 
{
 int pos = c_Pos.GetWindowInt();
 BOOL moveit = (BOOL)(c_Move.GetCheck() == BST_CHECKED);

 ::SendMessage(c_Slider->m_hWnd, TBM_SETPOS, moveit, pos);
 showResult_b_d_N(IDS_TBM_SETPOS, moveit, pos);
}

void CTrackPos::OnSetrangemax() 
{
 int val = c_RangeMax.GetWindowInt();
 BOOL redraw = c_Redraw.GetCheck();
 ::SendMessage(c_Slider->m_hWnd, TBM_SETRANGEMAX, redraw, val);
 showResult_b_d_N(IDS_TBM_SETRANGEMAX, redraw, val);
}

void CTrackPos::OnSetrangemin() 
{
 int val = c_RangeMin.GetWindowInt();
 BOOL redraw = c_Redraw.GetCheck();
 ::SendMessage(c_Slider->m_hWnd, TBM_SETRANGEMIN, redraw, val);
 showResult_b_d_N(IDS_TBM_SETRANGEMIN, redraw, val);
}

BOOL CTrackPos::OnInitDialog() 
{
        CTrackOps::OnInitDialog();
        
        c_Move.SetCheck(BST_CHECKED);
        c_Redraw.SetCheck(BST_CHECKED);

#define DEFAULT_LIMIT 1000
        c_SpinRangeMin.SetRange(0, DEFAULT_LIMIT);
        c_SpinRangeMax.SetRange(0, DEFAULT_LIMIT);
        c_SpinLineSize.SetRange(0, DEFAULT_LIMIT);
        c_SpinPageSize.SetRange(0, DEFAULT_LIMIT);
        c_SpinPos.SetRange(0, DEFAULT_LIMIT);

        // pre-load the controls in a loggable way

        OnGetlinesize();
        OnGetpagesize();
        OnGetpos();
        OnGetrangemax();
        OnGetrangemin();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrackPos::OnSetrange() 
{
  int lo = c_RangeMin.GetWindowInt();
  int hi = c_RangeMax.GetWindowInt();
  BOOL redraw = (BOOL)(c_Redraw.GetCheck() == BST_CHECKED);

  ::SendMessage(c_Slider->m_hWnd, TBM_SETRANGE, redraw, MAKELONG(lo, hi));
  showResult_b_DW_N(IDS_TBM_SETRANGE, redraw, MAKELONG(lo, hi));
}


void CTrackPos::enableSetRange()
{
  int lo = c_RangeMin.GetWindowInt();
  int hi = c_RangeMax.GetWindowInt();

  if(lo < -65536 || lo > 65535 ||
     hi < -65536 || hi > 65535)
        { /* 32 bits needed */
         c_SetRange.EnableWindow(FALSE);
        } /* 32 bits needed */
     else
        { /* 16 bits suffice */
         c_SetRange.EnableWindow(TRUE);
        } /* 16 bits suffice */

}
BOOL CTrackPos::OnSetActive() 
{
        enableSetRange();
        return CTrackOps::OnSetActive();
}

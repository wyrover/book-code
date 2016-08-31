// TrackSel.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "TrackOps.h"
#include "TrackSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackSelect property page

IMPLEMENT_DYNCREATE(CTrackSelect, CTrackOps)

CTrackSelect::CTrackSelect() : CTrackOps(CTrackSelect::IDD)
{
        //{{AFX_DATA_INIT(CTrackSelect)
        //}}AFX_DATA_INIT
}

CTrackSelect::~CTrackSelect()
{
}

void CTrackSelect::DoDataExchange(CDataExchange* pDX)
{
        CTrackOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTrackSelect)
        DDX_Control(pDX, IDC_SETTHUMBLENGTH, c_SetThumbLength);
        DDX_Control(pDX, IDC_SETSELSTART, c_SetSelStart);
        DDX_Control(pDX, IDC_SETSELEND, c_SetSelEnd);
        DDX_Control(pDX, IDC_SETSEL, c_SetSel);
        DDX_Control(pDX, IDC_GETTHUMBLENGTH, c_GetThumbLength);
        DDX_Control(pDX, IDC_GETSELSTART, c_GetSelStart);
        DDX_Control(pDX, IDC_GETSELEND, c_GetSelEnd);
        DDX_Control(pDX, IDC_CLEARSEL, c_ClearSel);
        DDX_Control(pDX, IDC_SELSTART, c_SelStart);
        DDX_Control(pDX, IDC_SELEND, c_SelEnd);
        DDX_Control(pDX, IDC_THUMBLENGTH, c_ThumbLength);
        DDX_Control(pDX, IDC_REDRAW, c_Redraw);
        DDX_Control(pDX, IDC_SPINSELEND, c_SpinSelEnd);
        DDX_Control(pDX, IDC_SPINSELSTART, c_SpinSelStart);
        DDX_Control(pDX, IDC_SPINTHUMBLENGTH, c_SpinThumbLength);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackSelect, CTrackOps)
        //{{AFX_MSG_MAP(CTrackSelect)
        ON_BN_CLICKED(IDC_CLEARSEL, OnClearsel)
        ON_BN_CLICKED(IDC_GETSELEND, OnGetselend)
        ON_BN_CLICKED(IDC_GETSELSTART, OnGetselstart)
        ON_BN_CLICKED(IDC_GETTHUMBLENGTH, OnGetthumblength)
        ON_BN_CLICKED(IDC_SETSEL, OnSetsel)
        ON_BN_CLICKED(IDC_SETSELEND, OnSetselend)
        ON_BN_CLICKED(IDC_SETSELSTART, OnSetselstart)
        ON_BN_CLICKED(IDC_SETTHUMBLENGTH, OnSetthumblength)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackSelect message handlers

BOOL CTrackSelect::OnInitDialog() 
{
        CTrackOps::OnInitDialog();
        
        BOOL enable;
        DWORD style = c_Slider->GetStyle();
        enable = style & TBS_ENABLESELRANGE ? TRUE : FALSE;

        c_ClearSel.EnableWindow(enable);
        c_GetSelEnd.EnableWindow(enable);
        c_GetSelStart.EnableWindow(enable);
        c_Redraw.EnableWindow(enable);
        c_SelEnd.EnableWindow(enable);
        c_SelStart.EnableWindow(enable);
        c_SetSel.EnableWindow(enable);
        c_SetSelEnd.EnableWindow(enable);
        c_SetSelStart.EnableWindow(enable);
        c_SetThumbLength.EnableWindow(enable);
        c_SpinSelEnd.EnableWindow(enable);
        c_SpinSelStart.EnableWindow(enable);

        c_Redraw.SetCheck(BST_CHECKED);
        
#define DEFAULT_SEL 100  // These should be recomputed on a SetActive event
        c_SpinSelStart.SetRange(0, DEFAULT_SEL);
        c_SpinSelEnd.SetRange(0, DEFAULT_SEL);
        c_SpinThumbLength.SetRange(0, DEFAULT_SEL);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrackSelect::OnClearsel() 
{
 ::SendMessage(c_Slider->m_hWnd, TBM_CLEARSEL, 
                        c_Redraw.GetCheck() ? TRUE : FALSE, 0);
 showResult_b_N_N(IDS_TBM_CLEARSEL, c_Redraw.GetCheck());
}

void CTrackSelect::OnGetselend() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETSELEND, 0, 0);
 showResult_N_N_d(IDS_TBM_GETSELEND, result);
 c_SelEnd.SetWindowText(result);
}

void CTrackSelect::OnGetselstart() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETSELSTART, 0, 0);
 showResult_N_N_d(IDS_TBM_GETSELSTART, result);
 c_SelStart.SetWindowText(result);
        
}

void CTrackSelect::OnGetthumblength() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETTHUMBLENGTH, 0, 0);
 showResult_N_N_d(IDS_TBM_GETTHUMBLENGTH, result);
 c_ThumbLength.SetWindowText(result);
}

void CTrackSelect::OnSetsel() 
{
 DWORD lParam = MAKELONG(c_SelStart.GetWindowInt(),
                                         c_SelEnd.GetWindowInt());

 ::SendMessage(c_Slider->m_hWnd, TBM_SETSEL, 
                                c_Redraw.GetCheck() ? TRUE : FALSE, lParam);
 showResult_b_DW_N(IDS_TBM_SETSEL, c_Redraw.GetCheck(), lParam);
}

void CTrackSelect::OnSetselend() 
{
 ::SendMessage(c_Slider->m_hWnd, TBM_SETSELEND, 
                                c_Redraw.GetCheck() ? TRUE : FALSE,
                                c_SelEnd.GetWindowInt());
 showResult_b_d_N(IDS_TBM_SETSELEND, 
                                c_Redraw.GetCheck() ? TRUE : FALSE,
                                c_SelEnd.GetWindowInt());
}

void CTrackSelect::OnSetselstart() 
{
 ::SendMessage(c_Slider->m_hWnd, TBM_SETSELSTART, 
                                c_Redraw.GetCheck() ? TRUE : FALSE,
                                c_SelStart.GetWindowInt());
 showResult_b_d_N(IDS_TBM_SETSELSTART, 
                                c_Redraw.GetCheck() ? TRUE : FALSE,
                                c_SelStart.GetWindowInt());
}

void CTrackSelect::OnSetthumblength() 
{
 ::SendMessage(c_Slider->m_hWnd, TBM_SETTHUMBLENGTH,
                                c_ThumbLength.GetWindowInt(), 0);

 showResult_d_d_N(IDS_TBM_SETTHUMBLENGTH, c_ThumbLength.GetWindowInt(), 0);
        
}

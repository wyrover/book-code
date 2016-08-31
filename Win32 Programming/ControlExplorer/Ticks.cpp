// Ticks.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "TrackOps.h"
#include "Ticks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicks property page

IMPLEMENT_DYNCREATE(CTicks, CTrackOps)

CTicks::CTicks() : CTrackOps(CTicks::IDD)
{
        //{{AFX_DATA_INIT(CTicks)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTicks::~CTicks()
{
}

void CTicks::DoDataExchange(CDataExchange* pDX)
{
        CTrackOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTicks)
        DDX_Control(pDX, IDC_TICKINDEX, c_TickIndex);
        DDX_Control(pDX, IDC_SPINTICKPOS, c_SpinTickPos);
        DDX_Control(pDX, IDC_SPINTICKINDEX, c_SpinTickIndex);
        DDX_Control(pDX, IDC_TICKPOS, c_Pos);
        DDX_Control(pDX, IDC_FREQ_CAPTION, c_c_Freq);
        DDX_Control(pDX, IDC_POS_CAPTION, c_c_Pos);
        DDX_Control(pDX, IDC_TICK, c_Tick);
        DDX_Control(pDX, IDC_TICKARRAY, c_TickArray);
        DDX_Control(pDX, IDC_SETTIC, c_SetTick);
        DDX_Control(pDX, IDC_GETTICPOS, c_GetTickPos);
        DDX_Control(pDX, IDC_GETTIC, c_GetTick);
        DDX_Control(pDX, IDC_GETPTICS, c_GetPTicks);
        DDX_Control(pDX, IDC_GETNUMTICS, c_GetNumTicks);
        DDX_Control(pDX, IDC_CLEARTICS, c_ClearTicks);
        DDX_Control(pDX, IDC_TICKS, c_Ticks);
        DDX_Control(pDX, IDC_TICKPOSY, c_TickPosY);
        DDX_Control(pDX, IDC_TICKPOSX, c_TickPosX);
        DDX_Control(pDX, IDC_TICKFREQ, c_TickFreq);
        DDX_Control(pDX, IDC_SPINTICKFREQ, c_SpinTickFreq);
        DDX_Control(pDX, IDC_SPINTICK, c_SpinTick);
        DDX_Control(pDX, IDC_SETTICFREQ, c_SetTickFreq);
        DDX_Control(pDX, IDC_REDRAW, c_Redraw);
        DDX_Control(pDX, IDC_NUMTICS, c_NumTicks);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTicks, CTrackOps)
        //{{AFX_MSG_MAP(CTicks)
        ON_BN_CLICKED(IDC_CLEARTICS, OnCleartics)
        ON_BN_CLICKED(IDC_GETNUMTICS, OnGetnumtics)
        ON_BN_CLICKED(IDC_GETPTICS, OnGetptics)
        ON_BN_CLICKED(IDC_GETTIC, OnGettic)
        ON_BN_CLICKED(IDC_GETTICPOS, OnGetticpos)
        ON_BN_CLICKED(IDC_SETTIC, OnSettic)
        ON_BN_CLICKED(IDC_SETTICFREQ, OnSetticfreq)
        ON_LBN_DBLCLK(IDC_TICKS, OnDblclkTicks)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicks message handlers

void CTicks::OnCleartics() 
{
 BOOL redraw = (c_Redraw.GetCheck() == BST_CHECKED);

 ::SendMessage(c_Slider->m_hWnd, TBM_CLEARTICS, redraw, 0);
 showResult_d_d_N(IDS_TBM_CLEARTICS, redraw, 0);
        
}

void CTicks::OnGetnumtics() 
{
 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETNUMTICS, 0, 0);
 c_NumTicks.SetWindowText(result);

 showResult_N_N_d(IDS_TBM_GETNUMTICS, result);
        
}

void CTicks::OnGetptics() 
{
 int n = ::SendMessage(c_Slider->m_hWnd, TBM_GETNUMTICS, 0, 0);
 LPINT p = (LPINT)::SendMessage(c_Slider->m_hWnd, TBM_GETPTICS, 0, 0);
 if(p == NULL)
    { /* NULL */
     CString s;
     s.LoadString(IDS_NULL);
     c_TickArray.SetWindowText(s);
    } /* NULL */
 else
    c_TickArray.SetWindowText((int)p, _T("0x%08x"));

 c_Ticks.ResetContent();

 if(p != NULL)
    { /* has array */
     // Adjust n for bizarre Microsoft bug...
     DWORD style = c_Slider->GetStyle();
     if( (style & TBS_AUTOTICKS) == 0)
        n -= 2;         // only for TBS_AUTOTICKS is n correct

     for(int i = 0; i < n; i++)
        { /* load array */
         CString s;
         s.Format(_T("%5d"), p[i]);
         int index = c_Ticks.AddString(s);
         c_Ticks.SetItemData(index, p[i]);
        } /* load array */
    } /* has array */
}

void CTicks::OnGettic() 
{
 int index = c_TickIndex.GetWindowInt();

 int result = ::SendMessage(c_Slider->m_hWnd, TBM_GETTIC, index, 0);

 c_Tick.SetWindowText(result);

 showResult_d_d_d(IDS_TBM_GETTIC, index, 0, result);
        
}

void CTicks::OnGetticpos() 
{
 int pos = c_TickIndex.GetWindowInt();
 DWORD result = (DWORD)::SendMessage(c_Slider->m_hWnd, TBM_GETTICPOS, pos, 0);

 c_TickPosX.SetWindowText((int)(short)LOWORD(result));
 c_TickPosY.SetWindowText((int)(short)HIWORD(result));

 showResult_d_d_pt(IDS_TBM_GETTICPOS, pos, 0, result);
}

void CTicks::OnSettic() 
{
 int pos = c_Tick.GetWindowInt();
 BOOL result = ::SendMessage(c_Slider->m_hWnd, TBM_SETTIC, 0, pos);
 showResult_d_d_b(IDS_TBM_SETTIC, 0, pos, result);
        
}

void CTicks::OnSetticfreq() 
{
 int pos = c_Pos.GetWindowInt();
 int freq = c_TickFreq.GetWindowInt();
 ::SendMessage(c_Slider->m_hWnd, TBM_SETTICFREQ, freq, pos);
 showResult_d_d_N(IDS_TBM_SETTICFREQ, freq, pos);
        
}

void CTicks::OnDblclkTicks() 
{
 c_TickIndex.SetWindowText(c_Ticks.GetCurSel());
        
}

BOOL CTicks::OnInitDialog() 
{
        CTrackOps::OnInitDialog();
        
        c_Redraw.SetCheck(BST_CHECKED);
        
        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTicks::OnSetActive() 
{
        enableControls();
        
        return CTrackOps::OnSetActive();
}

/****************************************************************************
*                           CTicks::enableControls
* Result: void
*       
* Effect: 
*       Enables the controls for tick marks based on the style
****************************************************************************/

void CTicks::enableControls()
    {
#if 0
     DWORD style = c_Slider->GetStyle();
     BOOL enable = (style & TBS_AUTOTICKS) ? TRUE : FALSE;

     if((style & TBS_NOTICKS))
        enable = FALSE;

     c_Pos.EnableWindow(enable);
     c_c_Pos.EnableWindow(enable);
     c_SpinTickPos.EnableWindow(enable);

     c_TickFreq.EnableWindow(enable);
     c_c_Freq.EnableWindow(enable);
     c_SpinTickFreq.EnableWindow(enable);

     c_SetTickFreq.EnableWindow(enable);

     if((style & TBS_NOTICKS))
        { /* no ticks */
         enable = FALSE;
        } /* no ticks */
#endif

     c_SpinTick.SetRange(0, 1000);
     c_SpinTickFreq.SetRange(0, 1000);
     c_SpinTickIndex.SetRange(0, 1000);
     c_SpinTickPos.SetRange(0, 1000);
    }

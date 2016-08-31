// TrackBar.cpp : implementation file
//

#include "stdafx.h"
#include <commctrl.h>
#include "ControlExplorer.h"
#include "msglog.h"
#include "indexpg.h"
#include "ixable.h"
#include "loggable.h"
#include "TinyButt.h"
#include "msgsheet.h"
#include "NumericEdit.h"
#include "TrackBar.h"
#include "animate.h"
#include "copy.h"

#include "TrackOps.h"
#include "trackpos.h"
#include "trackrc.h"
#include "tracksel.h" 
#include "trackndx.h"
#include "tracklog.h"
#include "ticks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MyTBWIDTH 50

/////////////////////////////////////////////////////////////////////////////
// CTrackBar property page

IMPLEMENT_DYNCREATE(CTrackBar, CPropertyPage)

CTrackBar::CTrackBar() : CPropertyPage(CTrackBar::IDD)
{
        //{{AFX_DATA_INIT(CTrackBar)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT

        initialized = FALSE;
}

CTrackBar::~CTrackBar()
{
}

void CTrackBar::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTrackBar)
        DDX_Control(pDX, IDC_INVALIDATERECT, c_InvalidateRect);
        DDX_Control(pDX, IDC_AUTOINVALIDATE, c_AutoInvalidate);
        DDX_Control(pDX, IDC_COPY, c_Copy);
        DDX_Control(pDX, IDC_WS_DISABLED, c_WS_Disabled);
        DDX_Control(pDX, IDC_WS_BORDER, c_WS_Border);
        DDX_Control(pDX, IDC_WH_CAPTION, c_c_WH);
        DDX_Control(pDX, IDC_WH, c_WH);
        DDX_Control(pDX, IDC_SPINWH, c_SpinWH);
        DDX_Control(pDX, IDC_SPINPOS, c_SpinPos);
        DDX_Control(pDX, IDC_SPINLOW, c_SpinLow);
        DDX_Control(pDX, IDC_SPINHIGH, c_SpinHigh);
        DDX_Control(pDX, IDC_POS, c_Pos);
        DDX_Control(pDX, IDC_HIGH, c_High);
        DDX_Control(pDX, IDC_LOW, c_Low);
        DDX_Control(pDX, IDC_WORKAREA, c_WorkArea);
        DDX_Control(pDX, IDC_TBS_VERT, c_TBS_Vert);
        DDX_Control(pDX, IDC_TBS_TOP, c_TBS_Top);
        DDX_Control(pDX, IDC_TBS_RIGHT, c_TBS_Right);
        DDX_Control(pDX, IDC_TBS_NOTICKS, c_TBS_NoTicks);
        DDX_Control(pDX, IDC_TBS_NOTHUMB, c_TBS_NoThumb);
        DDX_Control(pDX, IDC_TBS_LEFT, c_TBS_Left);
        DDX_Control(pDX, IDC_TBS_HORZ, c_TBS_Horz);
        DDX_Control(pDX, IDC_TBS_FIXEDLENGTH, c_TBS_FixedLength);
        DDX_Control(pDX, IDC_TBS_ENABLESELRANGE, c_TBS_EnableSelRange);
        DDX_Control(pDX, IDC_TBS_BOTTOM, c_TBS_Bottom);
        DDX_Control(pDX, IDC_TBS_BOTH, c_TBS_Both);
        DDX_Control(pDX, IDC_TBS_AUTOTICKS, c_TBS_AutoTicks);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackBar, CPropertyPage)
        //{{AFX_MSG_MAP(CTrackBar)
        ON_BN_CLICKED(IDC_INVALIDATERECT, OnInvalidaterect)
        ON_BN_CLICKED(IDC_NO_ANIMATION, OnNoAnimation)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_BN_CLICKED(IDC_TBS_AUTOTICKS, OnTbsAutoticks)
        ON_BN_CLICKED(IDC_TBS_BOTH, OnTbsBoth)
        ON_BN_CLICKED(IDC_TBS_BOTTOM, OnTbsBottom)
        ON_BN_CLICKED(IDC_TBS_ENABLESELRANGE, OnTbsEnableselrange)
        ON_BN_CLICKED(IDC_TBS_FIXEDLENGTH, OnTbsFixedlength)
        ON_BN_CLICKED(IDC_TBS_HORZ, OnTbsHorz)
        ON_BN_CLICKED(IDC_TBS_LEFT, OnTbsLeft)
        ON_BN_CLICKED(IDC_TBS_NOTHUMB, OnTbsNothumb)
        ON_BN_CLICKED(IDC_TBS_NOTICKS, OnTbsNoticks)
        ON_BN_CLICKED(IDC_TBS_RIGHT, OnTbsRight)
        ON_BN_CLICKED(IDC_TBS_TOP, OnTbsTop)
        ON_BN_CLICKED(IDC_TBS_VERT, OnTbsVert)
        ON_WM_TIMER()
        ON_WM_DESTROY()
        ON_EN_CHANGE(IDC_HIGH, OnChangeHigh)
        ON_EN_CHANGE(IDC_LOW, OnChangeLow)
        ON_EN_CHANGE(IDC_POS, OnChangePos)
        ON_EN_CHANGE(IDC_WH, OnChangeWh)
        ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
        ON_BN_CLICKED(IDC_WS_DISABLED, OnWsDisabled)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_BN_CLICKED(IDC_LOG, OnLog)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackBar message handlers

void CTrackBar::OnNoAnimation() 
{
 // Nothing to do...
}

void CTrackBar::OnSendmessages() 
{
 CTrackIndex ndx;
 CMessageSheet sheet(IDS_SEND_TRACKBAR_MESSAGES);
 CTrackPos pos;
 CTrackSelect sel;
 CTicks ticks;
 CTrackRect rc;

 sheet.AddPage(&ndx);

 pos.c_Messages = &c_Messages;
 pos.c_Slider = &c_Slider;
 sheet.AddPage(&pos);

 sel.c_Messages = &c_Messages;
 sel.c_Slider = &c_Slider;
 sheet.AddPage(&sel);

 ticks.c_Messages = &c_Messages;
 ticks.c_Slider = &c_Slider;
 sheet.AddPage(&ticks);

 rc.c_Messages = &c_Messages;
 rc.c_Slider = &c_Slider;
 sheet.AddPage(&rc);

 // Move us down to the bottom left corner
 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);

 sheet.DoModal();

}

void CTrackBar::OnTbsAutoticks() 
{
 setStyles();
}

void CTrackBar::OnTbsBoth() 
{
 BOOL both = c_TBS_Both.GetCheck();

 if(both)
    { /* both */
     c_TBS_Left.SetCheck(BST_UNCHECKED);
     c_TBS_Right.SetCheck(BST_UNCHECKED);
     c_TBS_Top.SetCheck(BST_UNCHECKED);
     c_TBS_Bottom.SetCheck(BST_UNCHECKED);
    } /* both */
 else
    { /* default */
     c_TBS_Bottom.SetCheck(BST_CHECKED);
     c_TBS_Right.SetCheck(BST_CHECKED);
    } /* default */

 enableControls();
 setStyles();
}

void CTrackBar::OnTbsBottom() 
{
 c_TBS_Right.SetCheck(c_TBS_Bottom.GetCheck());

 if(c_TBS_Bottom.GetCheck())
    { /* bottom */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Top.SetCheck(BST_UNCHECKED);
     c_TBS_Left.SetCheck(BST_UNCHECKED);
    } /* bottom */
 else
    { /* top */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Top.SetCheck(BST_CHECKED);
     c_TBS_Left.SetCheck(BST_CHECKED);
    } /* top */

 enableControls();
 setStyles();
}

void CTrackBar::OnTbsEnableselrange() 
{
 setStyles();
}

void CTrackBar::OnTbsFixedlength() 
{
 setStyles();
}

void CTrackBar::OnTbsHorz() 
{
 if(c_TBS_Horz.GetCheck())
    c_TBS_Vert.SetCheck(BST_UNCHECKED);
 else
    c_TBS_Vert.SetCheck(BST_CHECKED);
 enableControls();
 setStyles();
}

void CTrackBar::OnTbsLeft() 
{
 c_TBS_Top.SetCheck(c_TBS_Left.GetCheck());

 if(c_TBS_Left.GetCheck())
    { /* left */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Right.SetCheck(BST_UNCHECKED);
     c_TBS_Bottom.SetCheck(BST_UNCHECKED);
    } /* left */
 else
    { /* right */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Right.SetCheck(BST_CHECKED);
     c_TBS_Bottom.SetCheck(BST_CHECKED);
    } /* right */


 enableControls();
 setStyles();
}

void CTrackBar::OnTbsNothumb() 
{
 setStyles();
}

void CTrackBar::OnTbsNoticks() 
{
 setStyles();
}

void CTrackBar::OnTbsRight() 
{
 c_TBS_Bottom.SetCheck(c_TBS_Right.GetCheck());

 if(c_TBS_Right.GetCheck())
    { /* right */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Left.SetCheck(BST_UNCHECKED);
     c_TBS_Top.SetCheck(BST_UNCHECKED);
    } /* right */
 else
    { /* left */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Left.SetCheck(BST_CHECKED);
     c_TBS_Top.SetCheck(BST_CHECKED);
    } /* left */

 enableControls();
 setStyles();
}

void CTrackBar::OnTbsTop() 
{
 c_TBS_Left.SetCheck(c_TBS_Top.GetCheck());

 if(c_TBS_Top.GetCheck())
    { /* top */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Bottom.SetCheck(BST_UNCHECKED);
     c_TBS_Right.SetCheck(BST_UNCHECKED);
    } /* top */
 else
    { /* bottom */
     c_TBS_Both.SetCheck(BST_UNCHECKED);
     c_TBS_Bottom.SetCheck(BST_CHECKED);
     c_TBS_Right.SetCheck(BST_CHECKED);
    } /* bottom */

 enableControls();
 setStyles();
}

void CTrackBar::OnTbsVert() 
{
 if(c_TBS_Vert.GetCheck())
    c_TBS_Horz.SetCheck(BST_UNCHECKED);
 else
    c_TBS_Horz.SetCheck(BST_CHECKED);

 enableControls();

 setStyles();
}

/****************************************************************************
*                          CTrackBar::enableControls
* Result: void
*       
* Effect: 
*       Enables all controls and makes sure checkmarks are consistent
****************************************************************************/

void CTrackBar::enableControls()
    {
     c_TBS_Top.EnableWindow(c_TBS_Vert.GetCheck() ? FALSE : TRUE);
     c_TBS_Bottom.EnableWindow(c_TBS_Vert.GetCheck() ? FALSE : TRUE);
     c_TBS_Left.EnableWindow(c_TBS_Vert.GetCheck() ? TRUE : FALSE);
     c_TBS_Right.EnableWindow(c_TBS_Vert.GetCheck() ? TRUE : FALSE);
    }

BOOL CTrackBar::OnInitDialog() 
{
        initialized = FALSE;

        CPropertyPage::OnInitDialog();
        
        CString s;
        s.LoadString(IDS_TRACKBAR);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        c_TBS_Bottom.SetCheck(BST_CHECKED);
        c_TBS_Right.SetCheck(BST_CHECKED);
        c_TBS_Horz.SetCheck(BST_CHECKED);
        enableControls();
        
        low = ::SendMessage(c_Slider.m_hWnd, TBM_GETRANGEMIN, 0, 0);
        high = 100;
        pos = ::SendMessage(c_Slider.m_hWnd, TBM_GETPOS, 0, 0);
        page = ::SendMessage(c_Slider.m_hWnd, TBM_GETPAGESIZE, 0, 0);
        line = ::SendMessage(c_Slider.m_hWnd, TBM_GETLINESIZE, 0, 0);
        
        c_SpinLow.SetRange(-high * 10, high * 10);
        c_SpinLow.SetPos(low);

        c_SpinHigh.SetRange(-high * 10, high * 10);
        c_SpinHigh.SetPos(high);

        c_SpinPos.SetRange(-high * 10, high * 10);
        c_SpinPos.SetPos(pos);

        c_SpinWH.SetRange(0, 100);
        c_SpinWH.SetPos(MyTBWIDTH);
        c_WH.SetWindowText(MyTBWIDTH);

        createControl();
        c_Slider.ShowWindow(SW_SHOW);

        initialized = TRUE;

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                             CTrackBar::getStyle
* Result: DWORD
*       The style flags from the controls
****************************************************************************/

DWORD CTrackBar::getStyle()
    {
     DWORD style = 0;

     if(c_TBS_AutoTicks.GetCheck())
        style |= TBS_AUTOTICKS;
     if(c_TBS_Both.GetCheck())
        style |= TBS_BOTH;
     if(c_TBS_Bottom.GetCheck())
        style |= TBS_BOTTOM;
     if(c_TBS_EnableSelRange.GetCheck())
        style |= TBS_ENABLESELRANGE;
     if(c_TBS_FixedLength.GetCheck())
        style |= TBS_FIXEDLENGTH;
     if(c_TBS_Horz.GetCheck())
        style |= TBS_HORZ;
     if(c_TBS_Left.GetCheck())
        style |= TBS_LEFT;
     if(c_TBS_NoThumb.GetCheck())
        style |= TBS_NOTHUMB;
     if(c_TBS_NoTicks.GetCheck())
        style |= TBS_NOTICKS;
     if(c_TBS_Right.GetCheck())
        style |= TBS_RIGHT;
     if(c_TBS_Top.GetCheck())
        style |= TBS_TOP;
     if(c_TBS_Vert.GetCheck())
        style |= TBS_VERT;

     if(c_WS_Border.GetCheck())
        style |= WS_BORDER;

     return style;
    }

/****************************************************************************
*                          CTrackBar::createControl
* Result: void
*       
* Effect: 
*       Creates the control according to the style specs
****************************************************************************/


void CTrackBar::createControl()
    {
     int selstart = 0;
     int selend = 0;

     if(::IsWindow(c_Slider.m_hWnd))
        { /* kill old one */
         c_Slider.GetRange(low, high);
         pos = c_Slider.GetPos();
         page = c_Slider.GetPageSize();
         c_Slider.GetSelection(selstart, selend);
         HWND wnd = c_Slider.Detach();
         ::DestroyWindow(wnd);
        } /* kill old one */

     CRect r;
     c_WorkArea.GetClientRect(&r);

     DWORD style = getStyle();

     if(style & TBS_VERT)
        { /* vertical */
         r.left = r.right - c_WH.GetWindowInt();
        } /* vertical */
     else
        { /* horizontal */
         r.top = r.bottom - c_WH.GetWindowInt();
        } /* horizontal */

     // Note we create the control invisible so the animation works properly
#define IDC_MYTRACKBAR 4095
     HWND hSlider = ::CreateWindow(TRACKBAR_CLASS, NULL,
                                        WS_CHILD | style,
                                        r.left, r.top, r.Width(), r.Height(),
                                        m_hWnd,
                                        (HMENU)IDC_MYTRACKBAR, 
                                        AfxGetInstanceHandle(),
                                        NULL);
     c_Slider.Attach(hSlider);
     c_Slider.SetRange(low, high);
     c_Slider.SetPos(pos);
     c_Slider.SetPageSize(page);
     c_Slider.SetSelection(selstart, selend);
    }

/****************************************************************************
*                            CTrackBar::setStyles
* Result: void
*       
* Effect: 
*       Changes the style flags.  If a style flag requires re-creation
*       of the window, re-creates it
****************************************************************************/

void CTrackBar::setStyles()
    {
     createControl();

     if(!c_NoAnimation.GetCheck())
        beginAnimation(&c_Slider);
     else
        c_Slider.ShowWindow(SW_SHOW);
    }

void CTrackBar::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_Slider);
        
        CPropertyPage::OnTimer(nIDEvent);
}

void CTrackBar::OnDestroy() 
{
        // This is necessary because our creation of this window
        // outside the MFC framework would confuse MFC horribly at
        // this point if we left the window around
        if(::IsWindow(c_Slider.m_hWnd))
           { /* destroy it */
            HWND wnd = c_Slider.Detach();
            ::DestroyWindow(wnd);
           } /* destroy it */

        CPropertyPage::OnDestroy();
}

void CTrackBar::OnChangeHigh() 
{
 if(!initialized)
    return;
 
 int low;
 int high;
 c_Slider.GetRange(low, high);
 high = c_High.GetWindowInt();
 c_Slider.SetRange(low, high);
 c_Pos.SetWindowText(c_Slider.GetPos()); // in case it changed
 if(c_AutoInvalidate.GetCheck())
    OnInvalidaterect();
        
}

void CTrackBar::OnChangeLow() 
{
 if(!initialized)
    return;

 int low;
 int high;
 c_Slider.GetRange(low, high);
 low = c_Low.GetWindowInt();
 c_Slider.SetRange(low, high);
 c_Pos.SetWindowText(c_Slider.GetPos()); // in case it changed
 if(c_AutoInvalidate.GetCheck())
    OnInvalidaterect();
}

void CTrackBar::OnChangePos() 
{
 if(!initialized)
    return;
 if(recursive)
    return;
 c_Slider.SetPos(c_Pos.GetWindowInt());
 c_Pos.SetWindowText(c_Slider.GetPos()); // in case it changed
}

void CTrackBar::OnChangeWh() 
{
 if(!initialized)
    return;
 setStyles();
}

void CTrackBar::OnWsBorder() 
{
 setStyles();
        
}

/****************************************************************************
*                             CTrackBar::onScroll
* Inputs:
*       UINT nSBCode:
*       UINT nPos:
*       CScrollBar * pScrollBar:
*       int scrollid:
* Result: void
*       
* Effect: 
*       Handles scroll messages
****************************************************************************/

void CTrackBar::onScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar, int scrollid)
    {
     if(pScrollBar->m_hWnd != c_Slider.m_hWnd)
        return;

     // LOWORD(wParam) => nSBCode
     // HIWORD(wParam) => nPos
     // lParam => pScrollBar

     CString ctlName;
     ctlName.LoadString(IDS_TRACKBAR);

     CString fmt;
     fmt.LoadString(IDS_FORMAT_SCROLL);
     // %s(%08x=(%s, %d), %08x)

     BOOL vert = (c_Slider.GetStyle() & TBS_VERT) != 0;
     int id;
     switch(nSBCode)
        { /* nSBCode */
         case TB_BOTTOM:
                 id = IDS_TB_BOTTOM;
                 break;
         case TB_ENDTRACK:
                 id = IDS_TB_ENDTRACK;
                 break;
         case TB_LINEUP:
                 // id = (vert ? IDS_TB_LINELEFT : IDS_TB_LINEUP);
                 id = IDS_TB_LINEUP;
                 break;
         case TB_LINEDOWN:
                 // id = (vert ? IDS_TB_LINERIGHT : IDS_TB_LINEDOWN);
                 id = IDS_TB_LINEDOWN;
                 break;
         case TB_PAGEUP:
                 // id = (vert ? IDS_TB_PAGELEFT : IDS_TB_PAGEUP);
                 id = IDS_TB_PAGEUP;
                 break;
         case TB_PAGEDOWN:
                 // id = (vert ? IDS_TB_PAGERIGHT : IDS_TB_PAGEDOWN);
                 id = IDS_TB_PAGEDOWN;
                 break;
         case TB_THUMBPOSITION:
                 id = IDS_TB_THUMBPOSITION;
                 break;
         case TB_THUMBTRACK:
                 id = IDS_TB_THUMBTRACK;
                 break;
         case TB_TOP:
                 id = IDS_TB_TOP;
                 break;
        } /* nSBCode */

     CString sb;
     sb.LoadString(id);

     CString sop;
     sop.LoadString(scrollid);

     CString s;
     s.Format(fmt, sop, MAKELONG(nSBCode, nPos), sb, nPos, 
                                                (UINT)pScrollBar->m_hWnd);
     
     c_Messages.addMessage(m_hWnd, ctlName, s);

     recursive = TRUE;
     c_SpinPos.SetPos(c_Slider.GetPos());
     recursive = FALSE;
    }

void CTrackBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        onScroll(nSBCode, nPos,  pScrollBar, IDS_WM_HSCROLL);
        
        CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTrackBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        onScroll(nSBCode, nPos, pScrollBar, IDS_WM_VSCROLL);
        
        CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTrackBar::OnWsDisabled() 
{
 c_Slider.EnableWindow(c_WS_Disabled.GetCheck() ? FALSE : TRUE);
}

void CTrackBar::OnCopy() 
{
 copyToClipboard(&c_Slider);    
}

void CTrackBar::OnInvalidaterect()
{
 c_Slider.InvalidateRect(NULL);

 CString ctlName;
 ctlName.LoadString(IDS_TRACKBAR);

 CString s;
 s.LoadString(IDS_INVALIDATERECT);
 c_Messages.addMessage(m_hWnd, ctlName, s);
}

void CTrackBar::OnLog() 
{
 CLogarithmic dlg;
 dlg.DoModal();
        
}

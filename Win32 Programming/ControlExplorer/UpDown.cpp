// UpDown.cpp : implementation file
//

#include "stdafx.h"
#include <afxtempl.h>

#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "msglog.h"
#include "indexpg.h"
#include "ixable.h"
#include "loggable.h"
#include "msgsheet.h"
#include "animate.h"
#include "UDZ.h"
#include "copy.h"

#include "UpDown.h"
#include "udops.h"
#include "udaccel.h"
#include "udbuddy.h"
#include "udpos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpDown property page

IMPLEMENT_DYNCREATE(CUpDown, CPropertyPage)



CUpDown::CUpDown() : CPropertyPage(CUpDown::IDD)
{
        //{{AFX_DATA_INIT(CUpDown)
        //}}AFX_DATA_INIT
}

CUpDown::~CUpDown()
{
}

void CUpDown::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CUpDown)
        DDX_Control(pDX, IDC_WS_TABSTOP, c_WS_TabStop);
        DDX_Control(pDX, IDC_COPY3RECT, c_Copy3Rect);
        DDX_Control(pDX, IDC_WS_DISABLED, c_WS_Disabled);
        DDX_Control(pDX, IDC_COPY2_RECT, c_Copy2Rect);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_UDS_ALIGNLEFT, c_UDS_AlignLeft);
        DDX_Control(pDX, IDC_UDS_ALIGNRIGHT, c_UDS_AlignRight);
        DDX_Control(pDX, IDC_UDS_ARROWKEYS, c_UDS_ArrowKeys);
        DDX_Control(pDX, IDC_UDS_AUTOBUDDY, c_UDS_AutoBuddy);
        DDX_Control(pDX, IDC_UDS_HORZ, c_UDS_Horz);
        DDX_Control(pDX, IDC_UDS_NOTHOUSANDS, c_UDS_NoThousands);
        DDX_Control(pDX, IDC_UDS_SETBUDDYINT, c_UDS_SetBuddyInt);
        DDX_Control(pDX, IDC_UDS_WRAP, c_UDS_Wrap);
        DDX_Control(pDX, IDC_BUDDYD, c_BuddyD);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_BUDDYA, c_BuddyA);
        DDX_Control(pDX, IDC_BUDDYB, c_BuddyB);
        DDX_Control(pDX, IDC_HIGH, c_High);
        DDX_Control(pDX, IDC_LOW, c_Low);
        DDX_Control(pDX, IDC_VALUE, c_Value);
        DDX_Control(pDX, IDC_UPDOWN, c_UpDown);
        DDX_Control(pDX, IDC_BUDDYC, c_BuddyC);
        DDX_Control(pDX, IDC_BUDDYA_CAPTION, c_c_BuddyA);
        DDX_Control(pDX, IDC_BUDDYB_CAPTION, c_c_BuddyB);
        DDX_Control(pDX, IDC_BUDDYC_CAPTION, c_c_BuddyC);
        DDX_Control(pDX, IDC_BUDDYD_CAPTION, c_c_BuddyD);
        DDX_Control(pDX, IDC_Z1, c_Z1);
        DDX_Control(pDX, IDC_Z2, c_Z2);
        DDX_Control(pDX, IDC_Z3, c_Z3);
        DDX_Control(pDX, IDC_Z4, c_Z4);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpDown, CPropertyPage)
        //{{AFX_MSG_MAP(CUpDown)
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
        ON_EN_CHANGE(IDC_BUDDYA, OnChangeBuddyA)
        ON_EN_CHANGE(IDC_BUDDYB, OnChangeBuddyB)
        ON_EN_CHANGE(IDC_HIGH, OnChangeHigh)
        ON_EN_CHANGE(IDC_LOW, OnChangeLow)
        ON_BN_CLICKED(IDC_SWAP, OnSwap)
        ON_BN_CLICKED(IDC_UDS_ALIGNLEFT, OnUdsAlignleft)
        ON_BN_CLICKED(IDC_UDS_ALIGNRIGHT, OnUdsAlignright)
        ON_BN_CLICKED(IDC_UDS_ARROWKEYS, OnUdsArrowkeys)
        ON_BN_CLICKED(IDC_UDS_AUTOBUDDY, OnUdsAutobuddy)
        ON_BN_CLICKED(IDC_UDS_HORZ, OnUdsHorz)
        ON_BN_CLICKED(IDC_UDS_NOTHOUSANDS, OnUdsNothousands)
        ON_BN_CLICKED(IDC_UDS_SETBUDDYINT, OnUdsSetbuddyint)
        ON_BN_CLICKED(IDC_UDS_WRAP, OnUdsWrap)
        ON_NOTIFY(UDN_DELTAPOS, IDC_UPDOWN, OnDeltaposUpdown)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_WM_TIMER()
        ON_BN_CLICKED(IDC_ZSHIFT, OnZshift)
        ON_BN_CLICKED(IDC_COPY1, OnCopy1)
        ON_BN_CLICKED(IDC_COPY2, OnCopy2)
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_WS_DISABLED, OnWSDisabled)
        ON_BN_CLICKED(IDC_COPY3, OnCopy3)
        ON_BN_CLICKED(IDC_WS_TABSTOP, OnWsTabstop)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpDown message handlers


/****************************************************************************
*                              CUpDown::recreate
* Result: void
*       
* Effect: 
*       Re-creates the up-down control with the new styles.  Note that we
*       record the original rectangel value during WM_INITDIALOG so we don't
*       get an increasing-entropy situation
****************************************************************************/

void CUpDown::recreate()
    {
     int low = c_Low.GetWindowInt();
     int high = c_High.GetWindowInt();
     int pos = c_Value.GetWindowInt();
     HWND buddy = NULL;

     if(::IsWindow(c_UpDown.m_hWnd))
        { /* kill old one */
         c_UpDown.GetRange(low, high);
         pos = c_UpDown.GetPos();
         buddy = (HWND)::SendMessage(c_UpDown.m_hWnd, UDM_GETBUDDY, 0, 0);


         // See the note in the OnInitDialog handler as to why we have
         // to do this...
         HWND wnd =c_UpDown.Detach();
         ::DestroyWindow(wnd);
        } /* kill old one */

     // We also need to return all the buddy windows to their original
     // sizes.  This is because each time we re-create a window with one
     // of the alignment values, it chops another up/down-sized chunk out
     // of the *existing* window size, even if the window has already had
     // a chunk chopped out of it from a previous alignment

     for(int i = 0; i < czarray.GetSize(); i++)
        { /* resize */
         CZ * z = czarray.GetAt(i);
         z->wnd->MoveWindow(z->rect.left, z->rect.top,
                            z->rect.Width(), z->rect.Height());
        } /* resize */

     // in case there is an autobuddy style, we need to re-create the Z-order

     buildZOrder();
         

     DWORD style = getStyles();

     style |= WS_CHILD;  // must have child style!

     // Conspicuous by its absence: WS_VISIBLE...

     // If the new style has autobuddy, we want to set the buddy to NULL
     // because the AUTOBUDDY grabs a control, then the UDM_SETBUDDY
     // operation grabs it again.

     if(style & UDS_AUTOBUDDY)
        buddy = NULL;

     HWND wnd = ::CreateUpDownControl(style,            // style
                                      udrect.left,      // x 
                                      udrect.top,       // y
                                      udrect.Width(),   // cx
                                      udrect.Height(),  // cy
                                      m_hWnd,           // hParent
                                      IDC_UPDOWN,       // nID
                                      AfxGetInstanceHandle(), // hInstacne
                                      buddy,            // buddy
                                      high,             // upper limit
                                      low,              // lower limit
                                      pos);             // current position
     if(wnd == NULL)
        { /* failed */
         DWORD error = GetLastError();
         CString fmt;
         fmt.LoadString(IDS_UDCREATE_FAILED);
         CString s;
         s.Format(fmt, error);
         AfxMessageBox(s);
        } /* failed */

     c_UpDown.Attach(wnd);

     if(!c_NoAnimation.GetCheck())
        beginAnimation(&c_UpDown);
     else
        c_UpDown.ShowWindow(SW_SHOW);
         
     enableBuddies();
     c_WS_Disabled.SetCheck(c_UpDown.IsWindowEnabled() 
                                ? BST_UNCHECKED 
                                : BST_CHECKED);
    }

/****************************************************************************
*                             CUpDown::getStyles
* Result: DWORD
*       Style flags showing what is set in the controls
****************************************************************************/

DWORD CUpDown::getStyles()
    {
     DWORD result = 0;
     if(c_UDS_AlignLeft.GetCheck())
        result |= UDS_ALIGNLEFT;

     if(c_UDS_AlignRight.GetCheck())
        result |= UDS_ALIGNRIGHT;

     if(c_UDS_AutoBuddy.GetCheck())
        result |= UDS_AUTOBUDDY;

     if(c_UDS_ArrowKeys.GetCheck())
        result |= UDS_ARROWKEYS;

     if(c_UDS_Horz.GetCheck())
        result |= UDS_HORZ;

     if(c_UDS_NoThousands.GetCheck())
        result |= UDS_NOTHOUSANDS;

     if(c_UDS_SetBuddyInt.GetCheck())
        result |= UDS_SETBUDDYINT;

     if(c_UDS_Wrap.GetCheck())
        result |= UDS_WRAP;

     if(c_WS_Disabled.GetCheck())
        result |= WS_DISABLED;

     if(c_WS_TabStop.GetCheck())
        result |= WS_TABSTOP;

     return result;
    }

void CUpDown::setStyle(BOOL set, DWORD flag)
{
}

/****************************************************************************
*                            CUpDown::onScrollCtl
* Inputs:
*       UINT nSBCode:
*       UINT nPos:
*       CSpinCtrl * pScrollBar:
*       int scrollid:
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CUpDown::onScrollCtl(UINT nSBCode, UINT nPos, CSpinButtonCtrl * pScrollBar, int scrollid)
    {
     // LOWORD(wParam) => nSBCode
     // HIWORD(wParam) => nPos
     // lParam => pScrollBar

     CString ctlName;
     ctlName.LoadString(IDS_UPDOWN);

     CString fmt;
     fmt.LoadString(IDS_FORMAT_SCROLL);
     // %s(%08x=(%s, %d), %08x)

     BOOL vert = (c_UpDown.GetStyle() & UDS_HORZ) == 0;
     int id;
     switch(nSBCode)
        { /* nSBCode */
         case SB_BOTTOM:
                 id = IDS_SB_BOTTOM;
                 break;
         case SB_ENDSCROLL:
                 id = IDS_SB_ENDSCROLL;
                 break;
         case SB_LINEUP:
                 id = (vert ? IDS_SB_LINELEFT : IDS_SB_LINEUP);
                 break;
         case SB_LINEDOWN:
                 id = (vert ? IDS_SB_LINERIGHT : IDS_SB_LINEDOWN);
                 break;
         case SB_PAGEUP:
                 id = (vert ? IDS_SB_PAGELEFT : IDS_SB_PAGEUP);
                 break;
         case SB_PAGEDOWN:
                 id = (vert ? IDS_SB_PAGERIGHT : IDS_SB_PAGEDOWN);
                 break;
         case SB_THUMBPOSITION:
                 id = IDS_SB_THUMBPOSITION;
                 break;
         case SB_THUMBTRACK:
                 id = IDS_SB_THUMBTRACK;
                 break;
         case SB_TOP:
                 id = IDS_SB_TOP;
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
     
     c_Value.SetWindowText(c_UpDown.GetPos());
    }

/****************************************************************************
*                              CUpDown::onScroll
* Inputs:
*       UINT nSBCode: Scrollbar code
*       UINT nPos: position
*       CScrollBar * pScrollBar: control refererence
* Result: void
*       
* Effect: 
*       Handles the notifications
****************************************************************************/

void CUpDown::onScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar, int scrollid)
    {
     switch(pScrollBar->GetDlgCtrlID())
        { /* decode */
         case IDC_UPDOWN:
                 onScrollCtl(nSBCode, nPos, (CSpinButtonCtrl *)pScrollBar, scrollid);
                 break;
        } /* decode */
    }


void CUpDown::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        onScroll(nSBCode, nPos, pScrollBar, IDS_WM_HSCROLL);
        
        CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CUpDown::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        // Initialize the Z-order values
        
        c_Z1.setZ(&c_BuddyA, &c_c_BuddyA);
        c_Z2.setZ(&c_BuddyB, &c_c_BuddyB);
        c_Z3.setZ(&c_BuddyC, &c_c_BuddyC);
        c_Z4.setZ(&c_BuddyD, &c_c_BuddyD);

        // Initialize the Z-order array

        czarray.Add(&c_Z1);
        czarray.Add(&c_Z2);
        czarray.Add(&c_Z3);
        czarray.Add(&c_Z4);

        CString s;
        s.LoadString(IDS_UPDOWN);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        
        // Save the original size so we can use it over and over again
        c_UpDown.GetWindowRect(&udrect);
        ScreenToClient(&udrect);

        /*
           This is to get us around some bizarre problem in MFC.  MFC does not
           cooperate well with the peculiar things we do here.  Here's what
           we're trying to avoid: the window c_UpDown is created by the dialog
           creation, and the variable is initialized via the DDXControl
           function.  The first time we destroy the window must be via the
           DestroyWindow method; if we use ::DestroyWindow API function, MFC
           will assert-check like crazy when we finally exit the dialog.  But
           once we've re-created the window, MFC will assert-check like crazy
           if we try to use the DestroyWindow method; we have to use
           ::DestroyWindow.  The simplest solution is to destroy the window
           and re-create it; then we can consistently use ::DestroyWindow to
           get rid of our working window.
        */

        int low;
        int high;
        c_UpDown.GetRange(low, high);

        c_Low.SetWindowText(low);
        c_High.SetWindowText(high);
        c_UpDown.DestroyWindow();
        recreate();

        for(int i = IDS_FIRST_SAMPLE; i < IDS_LAST_SAMPLE; i++)
           { /* load list */
            CString s;
            if(s.LoadString(i) > 0)
               c_BuddyD.AddString(s);
           } /* load list */
                

        c_UpDown.GetRange(low, high);
        c_Low.SetWindowText(low);
        c_High.SetWindowText(high);
        c_Value.SetWindowText(c_UpDown.GetPos());

        enableBuddies();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpDown::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        onScroll(nSBCode, nPos, pScrollBar, IDS_WM_VSCROLL);
        
        CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CUpDown::OnChangeBuddyA() 
{
        // TODO: Add your control notification handler code here
        
}

void CUpDown::OnChangeBuddyB() 
{
        // TODO: Add your control notification handler code here
        
}

void CUpDown::OnChangeHigh() 
{
 int low;
 int high;
 c_UpDown.GetRange(low, high);
 high = c_High.GetWindowInt();
 c_UpDown.SetRange(low, high);
}

void CUpDown::OnChangeLow() 
{
 int low;
 int high;
 c_UpDown.GetRange(low, high);
 low = c_Low.GetWindowInt();
 c_UpDown.SetRange(low, high);
        
}

void CUpDown::OnSwap() 
{
 int t = c_Low.GetWindowInt();
 c_Low.SetWindowText(c_High.GetWindowInt());
 c_High.SetWindowText(t);
}

void CUpDown::OnUdsAlignleft() 
{
 if(c_UDS_AlignLeft.GetCheck())
    c_UDS_AlignRight.SetCheck(BST_UNCHECKED);
        
 recreate();
}

void CUpDown::OnUdsAlignright() 
{
 if(c_UDS_AlignRight.GetCheck())
    c_UDS_AlignLeft.SetCheck(BST_UNCHECKED);
        
 recreate();
}

void CUpDown::OnUdsArrowkeys() 
{
 recreate();
        
}

void CUpDown::OnUdsAutobuddy() 
{
 recreate();    
}

void CUpDown::OnUdsHorz() 
{
 recreate();
}

void CUpDown::OnUdsNothousands() 
{
 recreate();
}

void CUpDown::OnUdsSetbuddyint() 
{
 recreate();    
}

void CUpDown::OnUdsWrap() 
{
 recreate();
}

void CUpDown::OnDeltaposUpdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_UPDOWN* ph = (NM_UPDOWN*)pNMHDR;

        CString ctlName;
        ctlName.LoadString(IDS_UPDOWN);

        CString fmt;
        fmt.LoadString(IDS_NOTIFYUPDOWN);

        // WM_NOTIFY(%d, hdr {hwnd: %08x, id: %d, code: %d=%s [iPos: %d, iDelta: %d] })

        CString code;
        if(ph->hdr.code == UDN_DELTAPOS)
           code.LoadString(IDS_UDN_DELTAPOS);

        CString s;
        s.Format(fmt, ph->hdr.idFrom,
                        //-------------------
                        // NMHDR
                        (int)ph->hdr.hwndFrom,
                        ph->hdr.idFrom,
                        ph->hdr.code,
                        code,
                        //-------------------
                        // NM_UPDOWN
                        ph->iPos,
                        ph->iDelta
                        //-------------------
                        );
        
        c_Messages.addMessage(m_hWnd, ctlName, s);
        
        *pResult = 0;
}

void CUpDown::OnSendmessages() 
{
 CMessageSheet sheet(IDS_SEND_UPDOWN_MESSAGES);
 CUpDownPos pos;
 CUpDownBuddy bud;
 CUpDownAccelerators acc;

 pos.c_Messages = &c_Messages;
 pos.c_UpDown = &c_UpDown;
 sheet.AddPage(&pos);

 bud.c_Messages = &c_Messages;
 bud.c_UpDown = &c_UpDown;
 bud.czarray = &czarray;
 sheet.AddPage(&bud);

 acc.c_Messages = &c_Messages;
 acc.c_UpDown = &c_UpDown;
 sheet.AddPage(&acc);
 
 sheet.DoModal();

 enableBuddies();

 // We may have changed the range or position; reflect these to the
 // current controls
 { /* reflect range */
  int low;
  int high;
  int pos;
  c_UpDown.GetRange(low, high);
  pos = c_UpDown.GetPos();
  c_Low.SetWindowText(low);
  c_High.SetWindowText(high);
  c_Value.SetWindowText(pos);
 } /* reflect range */

}

/****************************************************************************
*                           CUpDown::enableBuddies
* Result: void
*       
* Effect: 
*       Enables whichever window is currently selected as a buddy.  If the
*       current buddy is NULL but the UDS_AUTOBUDDY style is set then enables
*       the predecessor edit control
****************************************************************************/

void CUpDown::enableBuddies()
    {
     HWND buddy = NULL;
     if(c_UpDown.m_hWnd != NULL)
        { /* has window */
         buddy = (HWND)::SendMessage(c_UpDown.m_hWnd, UDM_GETBUDDY, 0, 0);
         if(buddy == NULL)
            { /* no explicit buddy */
             DWORD style = c_UpDown.GetStyle();
             if(style & UDS_AUTOBUDDY)
                { /* auto buddy */
                 buddy = c_BuddyA.m_hWnd;
                } /* auto buddy */
            } /* no explicit buddy */
        } /* has window */

     for(int i = 0; i < czarray.GetSize(); i++)
        { /* enable each */
         czarray.GetAt(i)->enable(buddy);
        } /* enable each */
    }

void CUpDown::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_UpDown);
        
        CPropertyPage::OnTimer(nIDEvent);
}

void CUpDown::OnZshift() 
{
 // Shifts the controls around:
 //             Z4 Z3 Z2 Z1
 //          => Z1 Z4 Z3 Z2
 //          => Z2 Z1 Z4 Z3
        
 CZ t;

 t = *czarray.GetAt(0);

 for(int i = 0; i < czarray.GetSize() - 2; i++)
    *czarray.GetAt(i) = *czarray.GetAt(i+1);
  
 *czarray.GetAt(czarray.GetSize() - 1) = t;
}


/****************************************************************************
*                            CUpDown::buildZOrder
* Result: void
*       
* Effect: 
*       Looks at the c_Z1..cZn list and makes sure that the windows precede
*       the up/down control in the order selected.
****************************************************************************/

void CUpDown::buildZOrder()
    {
     // We do this by moving the windows to the end of the Z-order chain
     // starting with the last window and ending with the first, so that
     // the "first" window in the list is the last on the chain, making
     // it the "first" candidate for autobuddy of a newly-created window...

     // First, we have to move the last Zn window to the end of the chain...

     int i = czarray.GetSize() - 1;

     CZ * z = czarray.GetAt(i);  // 'z' is the window we want at the tail
     CWnd * last = z->GetWindow(GW_HWNDLAST); // 'last' is the current tail

     if(last->m_hWnd != z->m_hWnd)  // if not already last...
        { /* make it last */
         z->wnd->SetWindowPos(last, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        } /* make it last */

     // Now repeat in turn for each of the other windows
     last = z->wnd;
     for(i--; i >= 0; i--)
        { /* process each */
         z = czarray.GetAt(i);
         z->wnd->SetWindowPos(last, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
         last = z->wnd;
        } /* process each */

     // Just in case we are called with an active up/down window
     if(::IsWindow(c_UpDown.m_hWnd))
        c_UpDown.SetWindowPos (last, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

void CUpDown::OnCopy1() 
{
 copyToClipboard(&c_UpDown);
        
}

void CUpDown::OnCopy2() 
{
 copyToClipboard(&c_Copy2Rect);
        
}

void CUpDown::OnDestroy() 
{
        CPropertyPage::OnDestroy();
        
        if(::IsWindow(c_UpDown.m_hWnd))
           { /* kill it */
            HWND wnd = c_UpDown.Detach(); // See note in OnInitDialog handler
            ::DestroyWindow(wnd);
           } /* kill it */
        
}

void CUpDown::OnWSDisabled() 
{
 BOOL enable = c_WS_Disabled.GetCheck() == BST_UNCHECKED;
 c_UpDown.EnableWindow(enable);
        
}

void CUpDown::OnCopy3() 
{
 copyToClipboard(&c_Copy3Rect);
        
}

void CUpDown::OnWsTabstop() 
{
 DWORD style = c_UpDown.GetStyle();
 if(c_WS_TabStop.GetCheck())
    style |= WS_TABSTOP;
 else
    style &= ~WS_TABSTOP;

 ::SetWindowLong(c_UpDown.m_hWnd, GWL_STYLE, style);
        
}

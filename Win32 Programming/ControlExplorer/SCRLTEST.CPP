// scrltest.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "intedit.h"
#include "scparent.h"
#include "scrltest.h"
#include "is95.h"
#include "copy.h"
#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STOCK_SB_STYLES (WS_VISIBLE | WS_CHILD | WS_TABSTOP)

/////////////////////////////////////////////////////////////////////////////
// CScrollBarTest property page

IMPLEMENT_DYNCREATE(CScrollBarTest, CPropertyPage)

CScrollBarTest::CScrollBarTest() : CPropertyPage(CScrollBarTest::IDD)
{
        initialized = FALSE;
        inchange = FALSE;

        //{{AFX_DATA_INIT(CScrollBarTest)
        //}}AFX_DATA_INIT
}

 CScrollBarTest::~CScrollBarTest()
{
}

void CScrollBarTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CScrollBarTest)
        DDX_Control(pDX, IDC_PAGE, c_Page);
        DDX_Control(pDX, IDC_SPINPAGE, c_SpinPage);
        DDX_Control(pDX, IDC_SPINPOS, c_SpinPos);
        DDX_Control(pDX, IDC_SPINLOW, c_SpinLow);
        DDX_Control(pDX, IDC_SPINHIGH, c_SpinHigh);
        DDX_Control(pDX, IDC_POS, c_Pos);
        DDX_Control(pDX, IDC_LOW, c_Low);
        DDX_Control(pDX, IDC_HIGH, c_High);
        DDX_Control(pDX, IDC_FULLSIZE, c_FullSize);
        DDX_Control(pDX, IDC_SBS_SIZEBOXTOPLEFTALIGN, c_SBS_SizeBoxTopLeftAlign);
        DDX_Control(pDX, IDC_SBS_SIZEBOXBOTTOMRIGHTALIGN, c_SBS_SizeBoxBottomRightAlign);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_SBS_NONE, c_None);
        DDX_Control(pDX, IDC_SBS_VERT, c_SBS_Vert);
        DDX_Control(pDX, IDC_SBS_TOPALIGN, c_SBS_TopAlign);
        DDX_Control(pDX, IDC_SBS_SIZEBOX, c_SBS_SizeBox);
        DDX_Control(pDX, IDC_SBS_SIZEGRIP, c_SBS_SizeGrip);
        DDX_Control(pDX, IDC_SBS_RIGHTALIGN, c_SBS_RightAlign);
        DDX_Control(pDX, IDC_SBS_LEFTALIGN, c_SBS_LeftAlign);
        DDX_Control(pDX, IDC_SBS_HORZ, c_SBS_Horz);
        DDX_Control(pDX, IDC_SBS_BOTTOMALIGN, c_SBS_BottomAlign);
        DDX_Control(pDX, IDC_CREATE, c_Create);
        DDX_Control(pDX, IDC_SCROLLSPACE, c_ScrollSpace);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScrollBarTest, CPropertyPage)
        //{{AFX_MSG_MAP(CScrollBarTest)
        ON_BN_CLICKED(IDC_CREATE, OnCreate)
        ON_BN_CLICKED(IDC_SBS_BOTTOMALIGN, OnSbsBottomalign)
        ON_BN_CLICKED(IDC_SBS_HORZ, OnSbsHorz)
        ON_BN_CLICKED(IDC_SBS_NONE, OnSbsNone)
        ON_BN_CLICKED(IDC_SBS_LEFTALIGN, OnSbsLeftalign)
        ON_BN_CLICKED(IDC_SBS_RIGHTALIGN, OnSbsRightalign)
        ON_BN_CLICKED(IDC_SBS_SIZEBOX, OnSbsSizebox)
        ON_BN_CLICKED(IDC_SBS_SIZEGRIP, OnSbsSizegrip)
        ON_BN_CLICKED(IDC_SBS_TOPALIGN, OnSbsTopalign)
        ON_BN_CLICKED(IDC_SBS_VERT, OnSbsVert)
        ON_BN_CLICKED(IDC_SBS_SIZEBOXTOPLEFTALIGN, OnSbsSizeboxtopleftalign)
        ON_BN_CLICKED(IDC_SBS_SIZEBOXBOTTOMRIGHTALIGN, OnSbsSizeboxbottomrightalign)
        ON_EN_CHANGE(IDC_LOW, OnChangeLow)
        ON_EN_CHANGE(IDC_HIGH, OnChangeHigh)
        ON_EN_CHANGE(IDC_POS, OnChangePos)
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
        ON_MESSAGE(UWM_SIMULATE_WM_SIZE, OnSimulateSize)
        ON_EN_CHANGE(IDC_PAGE, OnChangePage)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollBarTest message handlers


/****************************************************************************
*                       CScrollBarTest::enableAlignment
* Inputs:
*       BOOL horz: Horizontal options
*       BOOL vert: Vertical options
*       BOOL size: Size options
* Result: void
*       
* Effect: 
*       Enables the various radio buttons
****************************************************************************/

void CScrollBarTest::enableAlignment(BOOL horz, BOOL vert, BOOL size)
    {
     c_SBS_BottomAlign.EnableWindow(horz);
     c_SBS_TopAlign.EnableWindow(horz);
     c_SBS_LeftAlign.EnableWindow(vert);
     c_SBS_RightAlign.EnableWindow(vert);
     c_SBS_SizeBoxBottomRightAlign.EnableWindow(size);
     c_SBS_SizeBoxTopLeftAlign.EnableWindow(size);
    }

/****************************************************************************
*                      CScrollBarTest::enableParameters
* Result: void
*       
* Effect: 
*       Enables the parameter-setting controls, which are valid only
*       for non-sizebox controls
****************************************************************************/

void CScrollBarTest::enableParameters(BOOL enable)
    {
     c_Page.EnableWindow(enable);
     c_SpinPage.EnableWindow(enable);

     c_Low.EnableWindow(enable);
     c_SpinLow.EnableWindow(enable);

     c_High.EnableWindow(enable);
     c_SpinHigh.EnableWindow(enable);

     c_Pos.EnableWindow(enable);
     c_SpinPos.EnableWindow(enable);
    }

/****************************************************************************
*                       CScrollBarTest::enableFullSize
* Result: void
*       
* Effect: 
*       Enables the 'full size' check box, which is only valid for the
*       alignment 'none'
****************************************************************************/

void CScrollBarTest::enableFullSize()
    {
     c_FullSize.EnableWindow(c_None.GetCheck());
    }

/****************************************************************************
*                       CScrollBarTest::enableControls
* Result: void
*       
* Effect: 
*       Based on the control settings, enable or disable other controls
****************************************************************************/

void CScrollBarTest::enableControls()
    {
     if(c_SBS_Horz.GetCheck())
        { /* horizontal */
         enableAlignment(TRUE, FALSE, FALSE);
        } /* horizontal */
     else
     if(c_SBS_Vert.GetCheck())
        { /* vertical */
         enableAlignment(FALSE, TRUE, FALSE);
        } /* vertical */
     else
     if(c_SBS_SizeBox.GetCheck() || c_SBS_SizeGrip.GetCheck())
        { /* size */
         enableAlignment(FALSE, FALSE, TRUE);
        } /* size */
     styleToAlignment();

     // Now take a look at the current scrollbar (if any) and enable
     // whatever controls make sense for it

     if(c_ScrollBar.m_hWnd != NULL)
        { /* has scrollbar */
         enableParameters(!(c_ScrollBar.GetStyle() & 
                                              (SBS_SIZEBOX | SBS_SIZEGRIP)));
        } /* has scrollbar */
     else
        { /* no scrollbar */
         enableParameters(TRUE);
        } /* no scrollbar */
     
     enableFullSize();
    }

/****************************************************************************
*                      CScrollBarTest::styleToAlignment
* Result: void
*       
* Effect: 
*       This transfers the alignment styles of the 'style' word to the
*       style controls.  It accounts for the fact that the style flags
*       are not mutually exclusive but depend upon the mode
****************************************************************************/

void CScrollBarTest::styleToAlignment()
    {
     int button = IDC_SBS_NONE;  // assume none until proven otherwise

     if(style & SBS_VERT)
        { /* vertical */
         if(style & SBS_LEFTALIGN)
            button = IDC_SBS_LEFTALIGN;
         else
         if(style & SBS_RIGHTALIGN)
            button = IDC_SBS_RIGHTALIGN;
        } /* vertical */
     else
     if(style & SBS_SIZEBOX || style & SBS_SIZEGRIP)
        { /* size */
         if(style & SBS_SIZEBOXTOPLEFTALIGN)
            button = IDC_SBS_SIZEBOXTOPLEFTALIGN;
         else
         if(style & SBS_SIZEBOXBOTTOMRIGHTALIGN)
            button = IDC_SBS_SIZEBOXBOTTOMRIGHTALIGN;
        } /* size */
     else
        { /* horz */
         if(style & SBS_BOTTOMALIGN)
            button = IDC_SBS_BOTTOMALIGN;
         else
         if(style & SBS_TOPALIGN)
            button = IDC_SBS_TOPALIGN;
        } /* horz */

     CheckRadioButton(IDC_SBS_BOTTOMALIGN, IDC_SBS_SIZEBOXTOPLEFTALIGN, 
                                                button);
    }

/****************************************************************************
*                       CScrollBarTest::controlsToStyle
* Result: void
*       
* Effect: 
*       Transfers bits to 'style' flag based on control settings
****************************************************************************/

void CScrollBarTest::controlsToStyle()
    {
     // First, clear all the style flags

     style = 0;

     if(c_SBS_Horz.GetCheck())
        style |= SBS_HORZ;
     else
     if(c_SBS_Vert.GetCheck())
        style |= SBS_VERT;
     else
     if(c_SBS_SizeBox.GetCheck())
        style |= SBS_SIZEBOX;
     if(c_SBS_SizeGrip.GetCheck())
        style |= SBS_SIZEGRIP;

     if(c_SBS_TopAlign.GetCheck())
        style |= SBS_TOPALIGN;
     else
     if(c_SBS_LeftAlign.GetCheck())
        style |= SBS_LEFTALIGN;
     else
     if(c_SBS_BottomAlign.GetCheck())
        style |= SBS_BOTTOMALIGN;
     else
     if(c_SBS_RightAlign.GetCheck())
        style |= SBS_RIGHTALIGN;
     else
     if(c_SBS_SizeBoxTopLeftAlign.GetCheck())
        style |= SBS_SIZEBOXTOPLEFTALIGN;
     else
     if(c_SBS_SizeBoxBottomRightAlign.GetCheck())
        style |= SBS_SIZEBOXBOTTOMRIGHTALIGN;

     style |= STOCK_SB_STYLES;
    }


void CScrollBarTest::OnCreate() 
{
 CScrollBar newscroll;

 CRect r;

 c_ScrollSpace.GetWindowRect(&r);
 c_ScrollSpace.ScreenToClient(&r);

 // If the "full size" box is set, we get a great big scroll bar.  If
 // the "full size" box is not set, and the alignment is none,
 // we get a scroll bar that is somewhat smaller

#define insetBy(n) (2 * ::GetSystemMetrics(n)) // random inset

 if(!c_FullSize.GetCheck())
    { /* small one? */
     if((style & (SBS_VERT | SBS_LEFTALIGN | SBS_RIGHTALIGN)) == SBS_VERT)
        { /* small vertical */
         int w = r.Width() / 3;
         int yinset = insetBy(SM_CYFRAME);
         r.left += w;
         r.right -= w;
         r.top += yinset;
         r.bottom -= yinset;
        } /* small vertical */
     else
     if((style & (SBS_HORZ | SBS_TOPALIGN | SBS_BOTTOMALIGN)) == SBS_HORZ)
        { /* small horizontal */
         int h = r.Height() / 3;
         int xinset = insetBy(SM_CXFRAME); // random inset
         r.top += h;
         r.bottom -= h;
         r.left += xinset;
         r.right -= xinset;
        } /* small horizontal */
    } /* small one? */

 if((style & (SBS_SIZEBOX | SBS_SIZEGRIP)) && Is3())
    { /* compensate for bug */
     // See the article "Scrollbar Controls in Win32" on the MSDN CD-ROM
     r.right += 3;
     r.bottom += 3;
    } /* compensate for bug */

 // The following sequence creates a new window, destroys the old one,
 // and then attaches the new window to the MFC variable.  This is rather
 // circuitous, but is the right protocol for this effect in MFC.  Note
 // that using 'Detach'/'Attach' is not sufficient because of how the
 // subclass mechanism works in MFC.

 newscroll.Create(style, r, &c_ScrollSpace, IDC_SCROLLBAR);

 if(c_ScrollBar.m_hWnd != NULL)
    { /* delete old one */
     HWND wnd = c_ScrollBar.UnsubclassWindow();
     ::DestroyWindow(wnd);
    } /* delete old one */

 HWND hnew = newscroll.UnsubclassWindow();

 c_ScrollBar.SubclassWindow(hnew);


 // Having now created a new control, set its values for scroll range and
 // position based on the control settings for creation.

 c_ScrollBar.SetScrollRange(c_Low.GetVal(), c_High.GetVal());
 c_ScrollBar.SetScrollPos(c_Pos.GetVal());
 c_ScrollBar.SetFocus();

 // Because of how we embed the scrollbar in the child window c_ScrollSpace,
 // we need to repaint our space whenever we make a change.  This is an
 // artifact of the implementation of the Control (ScrollBar) Explorer and
 // is, strictly speaking, not necessary because normally you don't 
 // dynamically create and destroy scrollbars.

 c_ScrollSpace.GetWindowRect(&r);
 ScreenToClient(&r);  // convert to our own space
 InvalidateRect(&r);  // redraw our own screen space inside the rectangle

 enableControls();
}

void CScrollBarTest::setScrollStyle()
{
 enableControls();
 controlsToStyle();
 styleToAlignment();
}

void CScrollBarTest::OnSbsHorz() 
{
 setScrollStyle();
}

void CScrollBarTest::OnSbsVert() 
{
 setScrollStyle();
}

void CScrollBarTest::OnSbsSizebox() 
{
 setScrollStyle();
}

void CScrollBarTest::OnSbsSizegrip() 
{
 setScrollStyle();
}


void CScrollBarTest::setAlignmentStyle()
{
 controlsToStyle();
 enableFullSize();
}

void CScrollBarTest::OnSbsBottomalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsTopalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsLeftalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsRightalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsSizeboxtopleftalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsSizeboxbottomrightalign() 
{
 setAlignmentStyle();
}

void CScrollBarTest::OnSbsNone() 
{
 setAlignmentStyle();
}

BOOL CScrollBarTest::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        int width = 0;
        CString s;

        c_Messages.setWrap(TRUE);

        s.LoadString(IDS_HSCROLL);
        width = max(width, c_Messages.GetStringWidth(s));
        s.LoadString(IDS_VSCROLL);
        width = max(width, c_Messages.GetStringWidth(s));
        s.LoadString(IDS_SIZEBOX);
        width = max(width, c_Messages.GetStringWidth(s));
        s.LoadString(IDS_SIZEGRIP);
        width = max(width, c_Messages.GetStringWidth(s));

        c_Messages.initialize(width);
        
#define MAX_RANGE 50

        c_SpinHigh.SetRange(0, MAX_RANGE);
        c_SpinLow.SetRange(0, MAX_RANGE);
        c_SpinHigh.SetPos(MAX_RANGE);
        c_SpinLow.SetPos(0);

        c_SpinPos.SetRange(0, MAX_RANGE);
        c_SpinPos.SetPos(0);

        style = SBS_HORZ | STOCK_SB_STYLES;

        CheckRadioButton(IDC_SBS_HORZ, IDC_SBS_SIZEGRIP, IDC_SBS_HORZ);

        styleToAlignment();

        c_SBS_SizeGrip.EnableWindow(!Is3());

        // The c_ScrollSpace window is a placeholder for the actual
        // control we are going to create.  Create that control now.
        // It is a generic CWnd.

        CWnd newspace;
        CRect r;
        c_ScrollSpace.GetWindowRect(&r);
        ScreenToClient(&r);

        newspace.Create(NULL,   // use default generic class
                        _T(""), // not used, window name
                        WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP, 
                        r,      // location and size
                        this,   // parent
                        IDC_SCROLLSPACE);
        HWND wnd = c_ScrollSpace.UnsubclassWindow();
        ::DestroyWindow(wnd);

        wnd = newspace.UnsubclassWindow();
        c_ScrollSpace.SubclassWindow(wnd);

        // Modify the simulated scrollbar parent window so it can't get
        // any larger than its initial size
        ::SetWindowLong(c_ScrollSpace.m_hWnd, GWL_USERDATA,
                        MAKELONG(r.Width(), r.Height()));


        c_SpinPage.SetRange(1, MAX_RANGE);
        c_SpinPage.SetPos(10);  // randomly-chosen initial value

        enableControls();

        initialized = TRUE;

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CScrollBarTest::OnChangeLow() 
{
 if(!initialized)
    return;

 if(c_ScrollBar.m_hWnd == NULL)
    return;

 int pos = c_ScrollBar.GetScrollPos();

 c_ScrollBar.SetScrollRange(c_Low.GetVal(), c_High.GetVal());
 if(pos < c_Low.GetVal())
    c_ScrollBar.SetScrollPos(c_Low.GetVal());

 CString s;
 CString t;

 t.LoadString(IDS_SETSCROLLRANGE_MSG);
 s.Format(t, c_Low.GetVal(), c_High.GetVal());
 c_Messages.addMessage(c_ScrollBar.m_hWnd, getCtlName(), s);

}

void CScrollBarTest::OnChangeHigh() 
{
 if(!initialized)
    return;

 if(c_ScrollBar.m_hWnd == NULL)
    return;

 int pos = c_ScrollBar.GetScrollPos();

 c_ScrollBar.SetScrollRange(c_Low.GetVal(), c_High.GetVal());
 if(pos > c_High.GetVal())
    c_ScrollBar.SetScrollPos(c_High.GetVal());

 CString s;
 CString t;

 t.LoadString(IDS_SETSCROLLRANGE_MSG);
 s.Format(t, c_Low.GetVal(), c_High.GetVal());
 c_Messages.addMessage(c_ScrollBar.m_hWnd, getCtlName(), s);

}

void CScrollBarTest::OnChangePos() 
{
 if(!initialized)
    return;

 if(c_ScrollBar.m_hWnd == NULL)
    return;

 // in case we are changing the value programmatically, rather than by the
 // user, we have a potential for an infinite recursion.  We short-circuit
 // this by setting the inchange flag before setting the value explicitly,
 // and bypassing any action that would result in an infinite recursion

 if(inchange)
    return;

 c_ScrollBar.SetScrollPos(c_Pos.GetVal());

 CString s;
 CString t;
 t.LoadString(IDS_SETSCROLLPOS_MSG);
 s.Format(t, c_Pos.GetVal());
 c_Messages.addMessage(c_ScrollBar.m_hWnd, getCtlName(), s);
}

/****************************************************************************
*                         CScrollBarTest::getCtlName
* Result: CString
*       Name of control
****************************************************************************/

CString CScrollBarTest::getCtlName()
    {
     CString s;
     DWORD style = c_ScrollBar.GetStyle();

     if(style & SBS_VERT)
        s.LoadString(IDS_VSCROLL);
     else
     if(style & SBS_SIZEBOX)
        s.LoadString(IDS_SIZEBOX);
     else
     if(style & SBS_SIZEGRIP)
        s.LoadString(IDS_SIZEGRIP);
     else
        s.LoadString(IDS_HSCROLL);

     return s;
    }

/****************************************************************************
*                        CScrollBarTest::moveScrollBar
* Inputs:
*       UINT nSBCode:
*       UINT nPos:
*       CScrollBar * pScrollBar:
* Result: void
*       
* Effect: 
*       Actually moves the scrollbar by adjusting the position.  This
*       works equally well for both horizontal and vertical scrollbars
*       so is called by both the hScroll handler and the VScroll handler
****************************************************************************/

void CScrollBarTest::moveScrollBar(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
    {
     CString s;
     CString t;

     BOOL horz;
     int pos = pScrollBar->GetScrollPos();
     int low;
     int high;

     pScrollBar->GetScrollRange(&low, &high);

     horz = !(pScrollBar->GetStyle() & SBS_VERT);

     switch(nSBCode)
        { /* nSBCode */
         case SB_LINEUP: // a.k.a. SB_LINELEFT
                 s.LoadString(horz ? IDS_SB_LINELEFT : IDS_SB_LINEUP);
                 if(pos > low)
                    pos--;
                 break;
         case SB_LINEDOWN: // a.k.a. SB_LINERIGHT
                 s.LoadString(horz ? IDS_SB_LINERIGHT : IDS_SB_LINEDOWN);
                 if(pos < high)
                    pos++;
                 break;
         case SB_PAGEUP:   // a.k.a. SB_PAGELEFT
                 s.LoadString(horz ? IDS_SB_PAGELEFT : IDS_SB_PAGEUP);
                 pos -= 10;
                 if(pos < low)
                    pos = low;
                 break;
         case SB_PAGEDOWN: // a.k.a. SB_PAGERIGHT
                 s.LoadString(horz ? IDS_SB_PAGERIGHT : IDS_SB_PAGEDOWN);
                 pos+= 10;
                 if(pos > high)
                    pos = high;
                 break;
         case SB_THUMBPOSITION:
                 pos = nPos;
                 t.LoadString(IDS_SB_THUMBPOSITION);
                 s.Format(_T("%s: %d"), t, nPos);
                 break;
         case SB_THUMBTRACK:
                 pos = nPos;
                 t.LoadString(IDS_SB_THUMBTRACK);
                 s.Format(_T("%s: %d"), t, nPos);
                 break;
         case SB_TOP:      // a.k.a. SB_LEFT
                 pos = low;
                 s.LoadString(horz ? IDS_SB_LEFT : IDS_SB_TOP);
                 break;
         case SB_BOTTOM:   // a.k.a. SB_RIGHT
                 pos = high;
                 s.LoadString(horz ? IDS_SB_RIGHT : IDS_SB_BOTTOM);
                 break;
         case SB_ENDSCROLL:
                 s.LoadString(IDS_SB_ENDSCROLL);
                 break;
        } /* nSBCode */

     c_Messages.addMessage(pScrollBar->m_hWnd, getCtlName(), s);

     c_ScrollBar.SetScrollPos(pos);

     t.LoadString(IDS_SETSCROLLPOS_MSG);
     s.Format(t, pos);
     c_Messages.addMessage(pScrollBar->m_hWnd, getCtlName(), s);
     
     // We want to show the position in the position edit control, but  
     // this will generate an EN_CHANGE notification that will, if we
     // are not careful, get us into a potential infinite loop.
     // We protect ourselves by setting a "global" flag which limits
     // any recursion by short-circuiting the recusion in the
     // EN_CHANGE handler

     inchange = TRUE;
     c_Pos.SetVal(pos);
     inchange = FALSE;
    }

/****************************************************************************
*                          CScrollBarTest::OnHScroll
* Inputs:
*       UINT nSBCode: SB_ code
*       UINT nPos: Current position when doing thumb tracking
*       CScrollBar * pScrollBar: Scrollbar object
* Result: void
*       
* Effect: 
*       If the scrollbar is our scrollbar object, move it.  Otherwise,
*       we may be getting scroll messages from some other control, and
*       we do whatever would normally be done.
****************************************************************************/

void CScrollBarTest::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_ScrollBar.m_hWnd)
    moveScrollBar(nSBCode, nPos, pScrollBar);
 else
    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScrollBarTest::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_ScrollBar.m_hWnd)
    moveScrollBar(nSBCode, nPos, pScrollBar);
 else
    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

/****************************************************************************
*                       CScrollBarTest::OnSimulateSize
* Inputs:
*       WPARAM wParam: nType: type of resize
*       LPARAM lParam: MAKELONG(cx, cy);
* Result: LRESULT
*       void, logically.  0.  Ignored
* Effect: 
*       Handles WM_SIZE messages from the sizebox or sizegrip controls
* Notes:
*       In other than MFC, we would have to deal with the initial WM_SIZE
*       message coming in.  Here it is easy; we basically are transparent
*       to WM_SIZE until the 'initialized' flag is set.  After it is set,
*       we will never see a WM_SIZE because we are not a resizable dialog.
****************************************************************************/

LRESULT CScrollBarTest::OnSimulateSize(WPARAM wParam, LPARAM lParam)
{
 if(!initialized || c_ScrollBar.m_hWnd == NULL ||
                  !(c_ScrollBar.GetStyle() & SBS_SIZEBOX | SBS_SIZEGRIP))
    return 0;
 else
    { /* sizing */
     CString t;
     t.LoadString(IDS_WM_SIZE_MSG);
     
     CString s;
     s.Format(t, LOWORD(lParam), HIWORD(lParam));
     c_Messages.addMessage(c_ScrollBar.m_hWnd, getCtlName(), s);
     
     // Force repainting because the resizing may have left trash in our
     // space...
     CRect r;
     c_ScrollSpace.GetWindowRect(&r);
     ScreenToClient(&r);
     InvalidateRect(&r);
    } /* sizing */

 return 0;
}

void CScrollBarTest::OnChangePage() 
{
 if(!initialized)
    return;

 if(c_ScrollBar.m_hWnd == NULL)
    return;
        
 SCROLLINFO info;

 info.fMask = SIF_PAGE;  // we are setting only the page info
 info.nPage = c_Page.GetVal();

 CString s;
 CString t;
 c_ScrollBar.SetScrollInfo(&info);
 t.LoadString(IDS_SETPAGE_MSG);
 s.Format(t, info.nPage);
 c_Messages.addMessage(c_ScrollBar.m_hWnd, getCtlName(), s);
 
}

void CScrollBarTest::OnCopy() 
{
 copyToClipboard(&c_ScrollBar); 
}

// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "afxpriv.h" // for WM_QUERY3DCONTROLS

#include "ControlExplorer.h"
#include "MyButton.h"
#include "dlgcodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
        //{{AFX_MSG_MAP(CMyButton)
        ON_MESSAGE(WM_QUERY3DCONTROLS, OnQuery3DControls)
        ON_WM_GETDLGCODE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

/****************************************************************************
*                             CMyButton::DrawItem
* Inputs:
*       LPDRAWITEMSTRUCT dis: draw structure
* Result: void
*       
* Effect: 
*       Draws the button.  
* Notes:
*       This is written in very-nearly-pure-C for the parts that are copied
*       into the book, so that we know that it is really working code and
*       not some substitute.  I've left the Explorer support and other
*       non-book features in C++.
****************************************************************************/

// The following definition is required so that it looks, after we paste
// this example in the book, as if windowsx.h has been used...

#define GetWindowStyle(hwnd) ::GetWindowLong(hwnd, GWL_STYLE)

// Our standard DIM macro:
#define DIM(x) (sizeof(x) / sizeof(x[0]))

//================

void CMyButton::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 //============================================================================
 // For purposes of the Explorer, log the event to the parent window

 {
  CString trace;
  CString t;
  t.LoadString(IDS_WM_DRAWITEM);

  static int line = 0; // helps make trace readable by numbering events
  trace.Format(_T("%03d: %s ODA: %s%s%s ODS: %s%s%s"), line, t,
                dis->itemAction & ODA_DRAWENTIRE ? _T("Drw") : _T(""),
                dis->itemAction & ODA_FOCUS      ? _T("Foc") : _T(""),
                dis->itemAction & ODA_SELECT     ? _T("Sel") : _T(""),
                dis->itemState  & ODS_CHECKED    ? _T("Chk") : _T(""),
                dis->itemState  & ODS_DISABLED   ? _T("Dis") : _T(""),
                dis->itemState  & ODS_FOCUS      ? _T("Foc") : _T(""),
                dis->itemState  & ODS_GRAYED     ? _T("Gry") : _T(""));
  line++;
  GetParent()->SendMessage(UWM_DRAWITEM, (WPARAM)m_hWnd, (LPARAM)(LPCTSTR)trace);
 }

 //============================================================================

 RECT r = dis->rcItem;
 COLORREF txcolor;
 HICON icon;
 SIZE sz;
 TCHAR s[256];
 POINT pos = {0,0};
 POINT ipos = {0, 0};
 SIZE rsize;
 int saved = ::SaveDC(dis->hDC);
 int space;  // space between icon and text
 DWORD style;
 RECT focus;
 SIZE offset = {0, 0};

 // Make the rectangle a little smaller

 InflateRect(&r, -::GetSystemMetrics(SM_CXBORDER),
                 -::GetSystemMetrics(SM_CYBORDER));

 if(dis->itemState & ODS_SELECTED)
    { /* selected */
     txcolor = GetSysColor(COLOR_WINDOWTEXT);
     icon = (HICON)::LoadImage(AfxGetInstanceHandle(), 
                        MAKEINTRESOURCE(IDI_OWNERDRAW1_DOWN), 
                        IMAGE_ICON, 
                        ::GetSystemMetrics(SM_CXSMICON), 
                        ::GetSystemMetrics(SM_CYSMICON),
                        LR_DEFAULTCOLOR);
    } /* selected */
 else
    { /* unselected */
     if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
        { /* disabled */
         txcolor = GetSysColor(COLOR_GRAYTEXT);
         icon = (HICON)::LoadImage(AfxGetInstanceHandle(), 
                        MAKEINTRESOURCE(IDI_OWNERDRAW1_DISABLED), 
                        IMAGE_ICON, 
                        ::GetSystemMetrics(SM_CXSMICON), 
                        ::GetSystemMetrics(SM_CYSMICON),
                        LR_DEFAULTCOLOR);
        } /* disabled */
     else
        { /* enabled */
         txcolor = GetSysColor(COLOR_WINDOWTEXT);
         icon = (HICON)::LoadImage(AfxGetInstanceHandle(), 
                        MAKEINTRESOURCE(IDI_OWNERDRAW1_UP), 
                        IMAGE_ICON, 
                        ::GetSystemMetrics(SM_CXSMICON), 
                        ::GetSystemMetrics(SM_CYSMICON),
                        LR_DEFAULTCOLOR);
        } /* enabled */
    } /* unselected */
     
 ::SetTextColor(dis->hDC, txcolor);
        
 ::GetWindowText(dis->hwndItem, s, DIM(s));

 ::GetTextExtentPoint32(dis->hDC, s, lstrlen(s), &sz);

 // deal with all the possible style flags and options

 // Compute the width and height for later convenience
 rsize.cx = (r.right - r.left);
 rsize.cy = (r.bottom - r.top);

 // compute a default position of icon
 ipos.x =  rsize.cx - ::GetSystemMetrics(SM_CXSMICON);

 space = ::GetSystemMetrics(SM_CXSMICON) / 2;

 // Now determine the alignment based on the style flags.
 // Note that since we are an owner-draw control we can use the
 // API4-level flags even on Windows/NT 3.x

 style = GetWindowStyle(dis->hwndItem);
 style &= (BS_LEFT | BS_RIGHT | BS_CENTER);
     
 switch(style)
    { /* hstyle */
     case BS_LEFT:
             pos.x = 0;
             ipos.x = pos.x + sz.cx + space;
             break;
     case BS_RIGHT:
             pos.x = rsize.cx - sz.cx;
             ipos.x = pos.x - space - ::GetSystemMetrics(SM_CXSMICON);
             break;
     case BS_CENTER:
             pos.x = (rsize.cx - sz.cx) / 2;
             ipos.x = pos.x + sz.cx + space;
             break;
    } /* hstyle */

 style = GetWindowStyle(dis->hwndItem);
 style &=  ( BS_TOP | BS_BOTTOM | BS_VCENTER);

 switch(style)
    { /* vstyle */
      
     case BS_TOP:
             pos.y = 0;
             ipos.y = 0;
             break;
     case BS_BOTTOM:
             pos.y = rsize.cy - sz.cy;
             ipos.y = rsize.cy - ::GetSystemMetrics(SM_CYSMICON);
             break;
     case BS_VCENTER:
             pos.y = (rsize.cy - sz.cy) / 2;
             ipos.y = (rsize.cy - ::GetSystemMetrics(SM_CYSMICON)) / 2;
             break;
    } /* vstyle */

 style = GetWindowStyle(dis->hwndItem);

 // Note: we do not support multiline style for this control

 // compute the rectangle for drawing the focus

 focus.left = pos.x;
 focus.top  = pos.y;
 focus.right = pos.x + sz.cx;
 focus.bottom = pos.y + sz.cy;
 ::InflateRect(&r, 1, 1); // allow a little more space around the text

 // For visual effect, if the button is down, shift the text over and down
 // to make it look like it is being "pushed"

 if(dis->itemState & ODS_SELECTED)
    { /* button down */
     offset.cx = ::GetSystemMetrics(SM_CXBORDER);
     offset.cy = ::GetSystemMetrics(SM_CYBORDER);
    } /* button down */

 if(dis->itemState & ODS_SELECTED)
    { /* button down */
     ::DrawEdge(dis->hDC, &r, EDGE_SUNKEN, BF_RECT | BF_MIDDLE | BF_SOFT);
    } /* button down */
 else
    { /* button up */
     ::DrawEdge(dis->hDC, &r, EDGE_RAISED, BF_RECT | BF_MIDDLE | BF_SOFT);
    } /* button up */

 ::TextOut(dis->hDC, pos.x + offset.cx, pos.y + offset.cy, s, lstrlen(s));

 ::DrawIconEx(dis->hDC, ipos.x + offset.cx, ipos.y + offset.cy, 
                        icon,
                        ::GetSystemMetrics(SM_CXSMICON),
                        ::GetSystemMetrics(SM_CYSMICON),
                        0, NULL,
                        DI_NORMAL);

 focus.left += offset.cx; // compute new position
 focus.right += offset.cx;
 focus.top += offset.cy;
 focus.bottom += offset.cy;

 if(dis->itemState & ODS_FOCUS)
    ::DrawFocusRect(dis->hDC, &focus);

 ::RestoreDC(dis->hDC, saved);
}

/****************************************************************************
*                        CMyButton::OnQuery3DControls
* Inputs:
*       WPARAM wParam: ignored but passed to superclass
*       LPARAM lParam: ignored but passed to superclass
* Result: LRESULT
*       TRUE, always
* Notes: 
*       Signals MFC that it should subclass the control thru CTL3D
****************************************************************************/

LRESULT CMyButton::OnQuery3DControls(WPARAM wParam, LPARAM lParam)
    {
     return CButton::OnQuery3dControls(wParam, lParam);
    }

/****************************************************************************
*                         CMyButton::OnGetDlgCode
* Result: UINT
*       DLG code flags
* Notes: 
*       This allows us to subclass a control and return specific flags for
*       the WM_GETDLGCODE.  These flags are kept in the low-order 16 bits.
*
*       In the high-order word, which we never return, we keep two flags:
*               DLGCX_VALID - the values in the flag word are valid
*                             If this flag is not set, we pass the message
*                             on to the superclass.
*               DLGCX_ACTIVE - the values in the flag word are active.
*                             If this flag is not set, we pass the message
*                             on to the superclass.
*       Note that both flags must be set to have this function override the
*       parent.
*
*       For now, we keep the value in the GWL_USERDATA word; in the future
*       we might attach it with AddProp and use GetProp
*
* 	This code duplicates similar code.  We could avoid this if we could
*	use multiple inheritance under MFC, but we can't use it, hence the
*	duplication.
****************************************************************************/

#define DLGCX_BOTH (DLGCX_VALID | DLGCX_ACTIVE)

UINT CMyButton::OnGetDlgCode() 
{
 if((getDlgCode(m_hWnd) & DLGCX_BOTH) == DLGCX_BOTH)
    return getDlgCode(m_hWnd) & ~DLGCX_BOTH;
        
 return CButton::OnGetDlgCode();
}

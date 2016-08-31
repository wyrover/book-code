#include "stdSDK.h"
#include "scroll.h"
#include "resource.h"
#include "initialization.h"

static void
scroll_OnSetFocus(HWND hwnd, HWND hwndOld)
{
 FORWARD_WM_NCACTIVATE(hwnd, TRUE, FALSE, FALSE, SendMessage);
 FORWARD_WM_SETFOCUS(hwnd, hwndOld, DefWindowProc);
}

static void
scroll_OnKillFocus(HWND hwnd, HWND hwndNew)
{
 FORWARD_WM_NCACTIVATE(hwnd, FALSE, FALSE, FALSE, SendMessage);
 FORWARD_WM_SETFOCUS(hwnd, hwndNew, DefWindowProc);
}

static int 
scroll_OnMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT hittest, UINT msg)
{
    if(GetFocus() == hwnd)
       return MA_ACTIVATE;

    BringWindowToTop(hwnd);
    SetFocus(hwnd);

    if(IsIconic(hwnd))
       return MA_ACTIVATE;  // required for API4
    else
       return (hittest == HTCLIENT ? MA_ACTIVATEANDEAT : MA_ACTIVATE);
}

static BOOL
scroll_OnCreate(HWND hwnd, LPCREATESTRUCT cs)
{
  // ... if there were initialization to do, we would do it here...

  return (-1 != FORWARD_WM_CREATE(hwnd, cs, DefWindowProc));
}

static LRESULT
scroll_OnGetMargins(HWND hwnd)
{
    HDC     hdc ;
    LONG    result;

   hdc = GetDC (hwnd) ;
   result = MAKELONG ((GetDeviceCaps (hdc, LOGPIXELSX) / 8),
                 (GetDeviceCaps (hdc, LOGPIXELSY) / 8)) ;
   ReleaseDC (hwnd, hdc) ;
   return result ;
}

/****************************************************************************
*                            scroll_OnGetScrollPos
* Inputs:
*       HWND hwnd: Window whose scroll control values are needed 
* Result: LRESULT
*       MAKELONG(horizontal_in_pixels, vertical_in_lines)
****************************************************************************/

static LRESULT
scroll_OnGetScrollPos(HWND hwnd)
{
  SCROLLINFO si;
  LONG h;
  LONG v;

  si.cbSize = sizeof(SCROLLINFO) ;
  si.fMask = SIF_POS;
  if(!GetScrollInfo(hwnd, SB_VERT, &si))
     v = 0;
  else
     v = si.nPos;

  if(!GetScrollInfo(hwnd, SB_HORZ, &si))
     h = 0;
  else
     h = si.nPos;

  return MAKELONG (LOWORD (h), LOWORD (v)) ;
}

/****************************************************************************
*                             scroll_OnGetDocSize
* Inputs:
*       HWND hwnd: Window handle
* Result: LRESULT
*       LOWORD: width of document, in pixels
*       HIWORD: height of document, in lines
****************************************************************************/

static LRESULT
scroll_OnGetDocSize(HWND hwnd)
{
   return getDocSize(hwnd);
}

/****************************************************************************
*                           scroll_RecomputeHScroll
* Inputs:
*       HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Recomputes the need for a horizontal scroll bar.
****************************************************************************/

static void
scroll_RecomputeHScroll(HWND hwnd)
    {
     SCROLLINFO si;
     RECT r;
     int MaxHPos;
     int CurrHPos;

     {
      LONG val = SendMessage (hwnd, SM_GETDOCSIZE, 0, 0L) ;
      MaxHPos    = LOWORD (val) ; // horizontal size, in pixels
     }

     si.cbSize = sizeof(SCROLLINFO) ;
     si.fMask = SIF_POS;

     if(!GetScrollInfo(hwnd, SB_HORZ, &si))
        CurrHPos = 0;
     else
        CurrHPos   = si.nPos;

     // Compute the horizontal distance visible on the screen
     GetClientRect(hwnd, &r);

     if(r.right - r.left > MaxHPos)  // if all is visible,
        CurrHPos = MaxHPos = 0;  // no horizontal scroll bar needed

     si.nMin = 0 ;
     si.nMax = MaxHPos ;
     si.nPos = CurrHPos;
     si.fMask = SIF_POS | SIF_RANGE  ;
     if(si.nMin != si.nMax)
        { /* needs page size */
         si.fMask |= SIF_PAGE;   
         si.nPage = r.right - r.left;  // scroll box size
        } /* needs page size */

     SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
     
    }

/****************************************************************************
*                           scroll_RecomputeVScroll
* Inputs:
*       HWND hwnd: Window handle 
* Result: void
*       
* Effect: 
*       Recomputes the visibility of the vertical scroll bar
****************************************************************************/

static void
scroll_RecomputeVScroll(HWND hwnd)
    {
     SCROLLINFO si;
     int  MaxVPos ;
     int  CurrVPos ;
     int  cyChar;
     int  cyMargin ;
     int  LinesVisible;
     int  cy;

     // Get the size of a character in the current font (in pixels).
     {
      HDC  hdc;
      TEXTMETRIC tm;
      hdc = GetDC (hwnd) ;
      GetTextMetrics (hdc, &tm) ;
      ReleaseDC (hwnd, hdc);
      cyChar      = tm.tmHeight + tm.tmExternalLeading ;
     }

     // Get the present postion of the scroll bars.
     si.cbSize = sizeof(SCROLLINFO) ;
     si.fMask = SIF_POS;

     GetScrollInfo(hwnd, SB_VERT, &si);
     CurrVPos   = si.nPos;

     // Get the size of the document text.
     {
      LONG tmp = SendMessage (hwnd, SM_GETDOCSIZE, 0, 0L) ;
      MaxVPos    = HIWORD (tmp) ; // vertical size, in lines

      // Get the size of the margins (in pixels).
      tmp = SendMessage (hwnd, SM_GETMARGINS, 0, 0L) ;
      cyMargin   = HIWORD (tmp) ;
     }

     // Get the size of the screen
     {
      RECT r;
      GetClientRect(hwnd, &r);
      cy = r.bottom;
     }

     // Compute the number of lines visible on the screen

     LinesVisible = (cy - 2 * cyMargin) / cyChar;
     if(LinesVisible > MaxVPos) // Unlikely...
        CurrVPos = MaxVPos = 0;  // but if it happens, suppress the scroll bar

     // Set the parameters of min, max, and page size
     si.nMin = 0 ;
     si.nMax = MaxVPos ;
     si.nPos = CurrVPos;
     
     si.nPage = LinesVisible;
     si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE ;
     SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
    }

/****************************************************************************
*                             scroll_OnSetDocSize
* Inputs:
*       HWND hwnd: Window to set this for
*       int width: Width of document, in pixels
*       int height: Height of document, in lines
* Result: void
*       
* Effect: 
*       Sets the document size for scroll_OnSetDocSize
****************************************************************************/

static void
scroll_OnSetDocSize(HWND hwnd, int width, int height)
{
 ASSERT( (short)height >= 0);

 setDocSize(hwnd, width, height);
 scroll_RecomputeHScroll(hwnd); 
 scroll_RecomputeVScroll(hwnd);
}

/****************************************************************************
*                                scroll_OnSize
* Inputs:
*       HWND hwnd:
*       UINT state:
*       int cx: New window width
*       int cy: New window height
* Result: void
*       
* Effect: 
*       Changes the scrollbar information so the thumb correctly reflects
*       the amount of information actually shown in the window
*       If the window has been enlarged so the scroll bars are no longer
*       required, the unnecessary ones will disappear
****************************************************************************/

static void
scroll_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
  scroll_RecomputeHScroll(hwnd);
  scroll_RecomputeVScroll(hwnd);
}

/****************************************************************************
*                              scroll_OnHScroll
* Inputs:
*       HWND hwnd: Window handle
*       HWND hctl: 0, since we only deal with standard scroll bars
*       UINT sbcode: SB_ indication of scrolling type
*       int  pos: For THUMBTRACK, THUMBPOSITION handling
* Result: void
*       
* Effect: 
*       Scrolls the window and updates the scroll control
****************************************************************************/

static void
scroll_OnHScroll(HWND hwnd, HWND hctl, UINT sbcode, int pos)
{
  int         CurrHPos ;
  int         MaxHPos ;
  int         cxChar ;
  int         delta ;
  SCROLLINFO  si;
  int         pagesize;

  { /* get char height */
   TEXTMETRIC tm;
   HDC dc;

   dc = GetDC (hwnd) ;
   GetTextMetrics (dc, &tm) ;
   ReleaseDC (hwnd, dc);
   cxChar = tm.tmAveCharWidth;
  } /* get char height */

  { /* get page size */
   RECT r;
   LONG l = SendMessage (hwnd, SM_GETMARGINS, 0, 0L) ;
   int cxMargin    = LOWORD (l) ;
   GetClientRect(hwnd, &r);
   pagesize = (r.right - r.left) ;
  } /* get page size */

  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_POS | SIF_RANGE;
  GetScrollInfo(hwnd, SB_HORZ, &si);

  CurrHPos = si.nPos;
  MaxHPos = si.nMax;

  switch (sbcode) 
     { /* sbcode */
      case SB_RIGHT:
              delta = MaxHPos - CurrHPos;
              break ;
      case SB_LINELEFT:
              delta = -cxChar;
              break ;
      case SB_LINERIGHT:
              delta = cxChar;
              break ;
      case SB_PAGELEFT:
              delta = -si.nMax / 10;
              break ;
      case SB_PAGERIGHT:
              delta = si.nMax / 10;
              break ;
      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
              delta = pos - CurrHPos ;
              break ;
      case SB_LEFT:
              delta = - CurrHPos;
              break ;
      default:
              delta = 0 ;
     } /* sbcode */

  if (delta != 0) 
     {
      int newpos;

      si.cbSize = sizeof(SCROLLINFO) ;
      si.fMask = SIF_POS ;

      // Make sure we haven't exceeded limits
      newpos = CurrHPos + delta ;
      newpos = max(0, min(newpos, MaxHPos));

      // Now set the new scroll position
      si.nPos = newpos ;
      SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;

#ifdef _DEBUG
      // Checking out why value doesn't take...
      si.fMask = SIF_POS | SIF_RANGE;
      GetScrollInfo (hwnd, SB_HORZ, &si);
      newpos = si.nPos;  // avoid gratuitous scrolling
#endif

      // Now scroll the window by the delta amount
      ScrollWindow (hwnd, -(newpos - CurrHPos), 0, NULL, NULL) ;
      UpdateWindow (hwnd) ;
     }
}

/****************************************************************************
*                              scroll_OnVScroll
* Inputs:
*       HWND hwnd: Window containing scroll bar
*       HWND hctl: 0, always (vertical standard scroll bar)
*       UINT sbcode: SB_ code
*       int pos: For THUMBPOSITION, THUMBTRACK
* Result: void
*       
* Effect: 
*       Scrolls vertically
****************************************************************************/

static void
scroll_OnVScroll(HWND hwnd, HWND hctl, UINT sbcode, int pos)
{
  int         CurrVPos ;
  int         MaxVPos ;
  int         cyChar ;
  int         delta ;
  SCROLLINFO  si;
  int         pagesize;

  { /* get char height */
   TEXTMETRIC tm;
   HDC dc;

   dc = GetDC (hwnd) ;
   GetTextMetrics (dc, &tm) ;
   ReleaseDC (hwnd, dc);
   cyChar = tm.tmHeight + tm.tmExternalLeading ;
  } /* get char height */

  { /* get page size */
   RECT r;
   LONG l = SendMessage (hwnd, SM_GETMARGINS, 0, 0L) ;
   int cyMargin    = HIWORD (l) ;
   GetClientRect(hwnd, &r);
   pagesize = (r.bottom - r.top) / cyChar;
  } /* get page size */

  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_POS | SIF_RANGE;
  GetScrollInfo(hwnd, SB_VERT, &si);
  CurrVPos = si.nPos;
  MaxVPos = si.nMax;

  switch (sbcode) 
     { /* sbcode */
      case SB_BOTTOM:
              delta = MaxVPos - CurrVPos ;
              break ;
      case SB_LINEUP:
              delta = -1 ;
              break ;
      case SB_LINEDOWN:
              delta = 1 ;
              break ;
      case SB_PAGEUP:
              delta = -pagesize;
              break ;
      case SB_PAGEDOWN:
              delta = pagesize;
              break ;
      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
              delta = pos - CurrVPos ;
              break ;
      case SB_TOP:
              delta = -CurrVPos ;
              break ;
      default:
              delta = 0 ;
     } /* sbcode */

  if (delta != 0) 
     {
      int newpos;

      si.cbSize = sizeof(SCROLLINFO) ;
      si.fMask = SIF_POS ;

      // Make sure we haven't exceeded limits
      newpos = CurrVPos + delta ;      
      newpos = max(0, min(newpos, MaxVPos));

      // Now set the new scroll position
      si.nPos = newpos ;
      SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

#ifdef _DEBUG
      // Checking out why value doesn't take...
      si.fMask = SIF_POS | SIF_RANGE;
      GetScrollInfo (hwnd, SB_VERT, &si);
      newpos = si.nPos;  // avoid gratuitous scrolling
#endif

      // Scroll the window.  Convert line distance to pixel distance
      ScrollWindow (hwnd, 0, -cyChar * (newpos - CurrVPos), NULL, NULL) ;
      UpdateWindow (hwnd) ;
     }
}

/****************************************************************************
*                           scroll_OnGetMinMaxInfo
* Inputs:
*       HWND hwnd: Window handle
*       LPGETMINMAXINFO mmi: Address of MINMAXINFO structure
* Result: void
*       
* Effect: 
*       Limits the size to be that which can fit in the client area, less
*       the space required for the toolbar
****************************************************************************/

static void
scroll_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO mmi)
    {
     HWND hwndToolBar;
     RECT r;
     RECT tr;

     hwndToolBar = GetDlgItem(GetParent(hwnd), IDC_TOOLBAR);

     if(hwndToolBar == NULL)
        return;  // no limits, because no toolbar

     GetWindowRect(hwndToolBar, &tr);

     GetClientRect(GetParent(hwnd), &r);
     mmi->ptMaxSize.y = r.bottom - r.top - (tr.bottom - tr.top);
     mmi->ptMaxPosition.y = tr.bottom - tr.top;
    }

/****************************************************************************
*                         scroll_OnWindowPosChanging
* Inputs:
*       HWND hwnd: Window whose position or size is changing 
*       LPWINDOWPOS wpos: Window position information
* Result: BOOL
*       TRUE if
*       FALSE if
* Effect: 
*       Limits the window size and position so it cannot cover the toolbar
****************************************************************************/

static BOOL
scroll_OnWindowPosChanging(HWND hwnd, LPWINDOWPOS wpos)
    {
     RECT tr;
     RECT r;
     RECT pr;
     HWND hwndToolBar = GetDlgItem(GetParent(hwnd), IDC_TOOLBAR);
     
     if(hwndToolBar == NULL)
        return TRUE;  // no toolbar, no restrictions

     GetWindowRect(hwndToolBar, &tr);
     GetClientRect(hwnd, &r);
     GetClientRect(GetParent(hwnd), &pr);

     wpos->y = max(wpos->y, tr.bottom - tr.top);
     wpos->cy = min(wpos->cy, pr.bottom - pr.top - (tr.bottom - tr.top));
     wpos->cx = min(wpos->cx, pr.right - pr.left);

     return TRUE;
    }

/****************************************************************************
*                              ScrollingWndProc
* Inputs:
*       HWND hwnd:
*       UINT msg:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       As per message sent
* Effect: 
*       Dispatches messages for all scrolling windows.  
* Notes:
*       ScrollingWndProc represents the abstract scrolling class.  There
*       is not actually a registered class that uses this handler; instead,
*       "derived" classes use this as their common handler.
****************************************************************************/

LRESULT CALLBACK 
ScrollingWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
       { /* msg */
        HANDLE_MSG(hwnd, WM_CREATE, scroll_OnCreate);
        HANDLE_MSG(hwnd, WM_SIZE, scroll_OnSize);
        HANDLE_MSG(hwnd, WM_HSCROLL, scroll_OnHScroll);
        HANDLE_MSG(hwnd, WM_VSCROLL, scroll_OnVScroll);
        HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, scroll_OnMouseActivate);
        HANDLE_MSG(hwnd, WM_SETFOCUS, scroll_OnSetFocus);
        HANDLE_MSG(hwnd, WM_KILLFOCUS, scroll_OnKillFocus);
        HANDLE_MSG(hwnd, WM_GETMINMAXINFO, scroll_OnGetMinMaxInfo);
        HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, scroll_OnWindowPosChanging);
        case SM_GETMARGINS:
            return scroll_OnGetMargins(hwnd);
        case SM_GETDOCSIZE:
            return scroll_OnGetDocSize(hwnd);
        case SM_GETSCROLLPOS:
                  return scroll_OnGetScrollPos(hwnd);
        case SM_SETDOCSIZE:
                scroll_OnSetDocSize(hwnd, wParam, lParam);
                return 0;
    } /* msg */
    return DefWindowProc (hwnd, msg, wParam, lParam) ;
}

/****************************************************************************
*                              createChildWindow
* Inputs:
*       HINSTANCE hinst: Instance handle
*       HWND hwnd: Parent window
*       UINT classid: ID of class name string
*       UINT titleid: ID of title string
* Result: HWND
*       Handle of created window, or NULL
* Effect: 
*       Creates a window of the specific child class
****************************************************************************/

HWND createChildWindow(HINSTANCE hinst, HWND hwnd, UINT classid, UINT titleid)
    {
     TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
     TCHAR Title [MAX_RESOURCESTRING + 1] ;
     HWND  hwndChild;
     RECT  r;
     HWND  hwndToolbar = GetDlgItem(hwnd, IDC_TOOLBAR);
     RECT  t;

     hwndChild = GetDlgItem(hwnd, classid);
     if(hwndChild != NULL)
        { /* already exists */
         if(IsIconic(hwndChild))
            OpenIcon(hwndChild);
         SetFocus(hwndChild);
         BringWindowToTop(hwndChild);
         return hwndChild;  // already exists
        } /* already exists */

     // The window does not already exist.  

     // Create the child window.  Make sure it doesn't exceed the
     // size left after we've included the toolbar

     GetWindowRect(hwndToolbar, &t);
     GetClientRect(hwnd, &r);
     r.top += t.bottom - t.top;

     VERIFY( LoadString(hinst, classid, ClassName, DIM(ClassName)));
     VERIFY( LoadString(hinst, titleid, Title, DIM(Title)));

     hwndChild = CreateWindowEx(0, // Extended styles
                                  ClassName, // class name
                                  Title,     // class title
                                  WS_CHILD | WS_VISIBLE | 
                                      WS_CAPTION | WS_MINIMIZEBOX | 
                                      WS_MAXIMIZEBOX | WS_SYSMENU |
                                      WS_THICKFRAME | WS_CLIPSIBLINGS,
                                  r.left, 
                                  r.top,
                                  r.right - r.left,
                                  r.bottom - r.top,
                                  hwnd,  // parent window
                                  (HMENU)classid, // child ID
                                  hinst,
                                  NULL);


     if(hwndChild == NULL)
        { /* error */
         DWORD error = GetLastError(); // examine with debugger
         return NULL;
        } /* error */

     // We cheat here.  Knowing the window has been created, we call
     // recursively to get the initial setup to occur.  
     return createChildWindow(hinst, hwnd, classid, titleid);
     
    }

/****************************************************************************
*                             registerChildClass
* Inputs:
*       HINSTANCE hinst: Instance handle
*       UINT classid: ID of class name string
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the child class whose ID is given.
****************************************************************************/

BOOL registerChildClass(HINSTANCE hinst, UINT classid, UINT icon, WNDPROC proc)
    {
     TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
     WNDCLASSEX wcex ;

     VERIFY (LoadString (hinst, classid, ClassName, DIM(ClassName))) ;

     wcex.cbSize        = sizeof (WNDCLASSEX) ;
     wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
     wcex.lpfnWndProc   = proc ;
     wcex.cbClsExtra    = 0 ;
     wcex.cbWndExtra    = UGW_SWMAXUSED ;
     wcex.hInstance     = hinst ;
     wcex.hIcon         = LoadIcon (hinst, MAKEINTRESOURCE (icon)) ;
     wcex.hCursor       = LoadCursor (NULL, IDC_IBEAM) ;
     wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1) ;

     wcex.lpszMenuName  = NULL;
     wcex.lpszClassName = ClassName ;
     wcex.hIconSm       = LoadImage (hinst,
                                     MAKEINTRESOURCE (icon),
                                     IMAGE_ICON,
                                     GetSystemMetrics (SM_CXSMICON),
                                     GetSystemMetrics (SM_CYSMICON),
                                     LR_SHARED) ;


     if(!internalRegisterClass (&wcex))
        return FALSE;

     return TRUE;
     
    }

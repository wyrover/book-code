#include "stdSDK.h"
#include "resource.h"
#include "scroll.h"
#include "systype.h"
#include <tchar.h>

static void metrics_CleanString(HWND hwnd, int value, LPSTR BUffer, int size);
static void metrics_Arrange(HWND hwnd, int value, LPSTR BUffer, int size);

static struct tagSMTbl {
    int         Index ;
    WORD        StringId ;
    WORD        SystemType ;
    void (*format)(HWND hwnd, int value, LPSTR Buffer, int size);
} SMtbl [] = {
    { SM_ARRANGE,           IDS_SM_ARRANGE,          SYSTYPE_API4, metrics_Arrange },
    { SM_CLEANBOOT,         IDS_SM_CLEANBOOT,        SYSTYPE_API4, metrics_CleanString },
    { SM_CMETRICS,          IDS_SM_CMETRICS,         SYSTYPE_API4, NULL },
    { SM_CMOUSEBUTTONS,     IDS_SM_CMOUSEBUTTONS,    SYSTYPE_API4, NULL },
    { SM_CXBORDER,          IDS_SM_CXBORDER,         SYSTYPE_ALL,  NULL },
    { SM_CYBORDER,          IDS_SM_CYBORDER,         SYSTYPE_ALL,  NULL },
    { SM_CYCAPTION,         IDS_SM_CYCAPTION,        SYSTYPE_ALL,  NULL },
    { SM_CXCURSOR,          IDS_SM_CXCURSOR,         SYSTYPE_ALL,  NULL },
    { SM_CYCURSOR,          IDS_SM_CYCURSOR,         SYSTYPE_ALL,  NULL },
    { SM_CXDLGFRAME,        IDS_SM_CXDLGFRAME, SYSTYPE_W32s | SYSTYPE_API3 | SYSTYPE_OBSOLETE4, 
                                                                   NULL },
    { SM_CYDLGFRAME,        IDS_SM_CYDLGFRAME, SYSTYPE_W32s | SYSTYPE_API3 | SYSTYPE_OBSOLETE4,
                                                                   NULL },
    { SM_CXDOUBLECLK,       IDS_SM_CXDOUBLECLK,      SYSTYPE_ALL,  NULL }, 
    { SM_CYDOUBLECLK,       IDS_SM_CYDOUBLECLK,      SYSTYPE_ALL,  NULL }, 
    { SM_CXDRAG,            IDS_SM_CXDRAG,           SYSTYPE_API4, NULL },
    { SM_CYDRAG,            IDS_SM_CYDRAG,           SYSTYPE_API4, NULL },
    { SM_CXEDGE,            IDS_SM_CXEDGE,           SYSTYPE_API4, NULL },
    { SM_CYEDGE,            IDS_SM_CYEDGE,           SYSTYPE_API4, NULL },
    { SM_CXFIXEDFRAME,      IDS_SM_CXFIXEDFRAME,     SYSTYPE_API4, NULL },
    { SM_CYFIXEDFRAME,      IDS_SM_CYFIXEDFRAME,     SYSTYPE_API4, NULL },
    { SM_CXFRAME,           IDS_SM_CXFRAME,          SYSTYPE_ALL | SYSTYPE_OBSOLETE4,
                                                                   NULL },
    { SM_CYFRAME,           IDS_SM_CYFRAME,          SYSTYPE_ALL | SYSTYPE_OBSOLETE4,
                                                                   NULL },
    { SM_CXFULLSCREEN,      IDS_SM_CXFULLSCREEN,     SYSTYPE_ALL,  NULL },
    { SM_CYFULLSCREEN,      IDS_SM_CYFULLSCREEN,     SYSTYPE_ALL,  NULL },
    { SM_CXHSCROLL,         IDS_SM_CXHSCROLL,        SYSTYPE_ALL,  NULL },
    { SM_CYHSCROLL,         IDS_SM_CYHSCROLL,        SYSTYPE_ALL,  NULL },
    { SM_CXHTHUMB,          IDS_SM_CXHTHUMB,         SYSTYPE_ALL,  NULL },
    { SM_CXICON,            IDS_SM_CXICON,           SYSTYPE_ALL,  NULL },
    { SM_CYICON,            IDS_SM_CYICON,           SYSTYPE_ALL,  NULL },
    { SM_CXICONSPACING,     IDS_SM_CXICONSPACING,    SYSTYPE_ALL,  NULL }, 
    { SM_CYICONSPACING,     IDS_SM_CYICONSPACING,    SYSTYPE_ALL,  NULL },
    { SM_CYKANJIWINDOW,     IDS_SM_CYKANJIWINDOW,    SYSTYPE_ALL,  NULL },
    { SM_CXMAXIMIZED,       IDS_SM_CXMAXIMIZED,      SYSTYPE_API4, NULL },
    { SM_CYMAXIMIZED,       IDS_SM_CYMAXIMIZED,      SYSTYPE_API4, NULL },
    { SM_CXMAXTRACK,        IDS_SM_CXMAXTRACK,       SYSTYPE_API4, NULL },
    { SM_CYMAXTRACK,        IDS_SM_CYMAXTRACK,       SYSTYPE_API4, NULL },
    { SM_CYMENU,            IDS_SM_CYMENU,           SYSTYPE_ALL,  NULL },
    { SM_CXMENUCHECK,       IDS_SM_CXMENUCHECK,      SYSTYPE_API4, NULL },
    { SM_CYMENUCHECK,       IDS_SM_CYMENUCHECK,      SYSTYPE_API4, NULL },
    { SM_CXMENUSIZE,        IDS_SM_CXMENUSIZE,       SYSTYPE_API4, NULL },
    { SM_CYMENUSIZE,        IDS_SM_CYMENUSIZE,       SYSTYPE_API4, NULL },
    { SM_CXMIN,             IDS_SM_CXMIN,            SYSTYPE_ALL,  NULL },
    { SM_CYMIN,             IDS_SM_CYMIN,            SYSTYPE_ALL,  NULL },
    { SM_CXMINIMIZED,       IDS_SM_CXMINIMIZED,      SYSTYPE_API4, NULL },
    { SM_CYMINIMIZED,       IDS_SM_CYMINIMIZED,      SYSTYPE_API4, NULL },
    { SM_CXMINSPACING,      IDS_SM_CXMINSPACING,     SYSTYPE_API4, NULL },
    { SM_CYMINSPACING,      IDS_SM_CYMINSPACING,     SYSTYPE_API4, NULL },
    { SM_CXMINTRACK,        IDS_SM_CXMINTRACK,       SYSTYPE_ALL,  NULL },
    { SM_CYMINTRACK,        IDS_SM_CYMINTRACK,       SYSTYPE_ALL,  NULL },
    { SM_CYSMCAPTION,       IDS_SM_CYSMCAPTION,      SYSTYPE_API4, NULL },
    { SM_CXSMICON,          IDS_SM_CXSMICON,         SYSTYPE_API4, NULL },
    { SM_CYSMICON,          IDS_SM_CYSMICON,         SYSTYPE_API4, NULL },
    { SM_CXSMSIZE,          IDS_SM_CXSMSIZE,         SYSTYPE_API4, NULL },
    { SM_CYSMSIZE,          IDS_SM_CYSMSIZE,         SYSTYPE_API4, NULL },
    { SM_CXSCREEN,          IDS_SM_CXSCREEN,         SYSTYPE_ALL,  NULL },
    { SM_CYSCREEN,          IDS_SM_CYSCREEN,         SYSTYPE_ALL,  NULL },
    { SM_CXSIZE,            IDS_SM_CXSIZE,           SYSTYPE_ALL,  NULL },
    { SM_CYSIZE,            IDS_SM_CYSIZE,           SYSTYPE_ALL,  NULL },
    { SM_CXSIZEFRAME,       IDS_SM_CXSIZEFRAME,      SYSTYPE_API4, NULL },
    { SM_CYSIZEFRAME,       IDS_SM_CYSIZEFRAME,      SYSTYPE_API4, NULL },
    { SM_CXVSCROLL,         IDS_SM_CXVSCROLL,        SYSTYPE_ALL,  NULL },
    { SM_CYVSCROLL,         IDS_SM_CYVSCROLL,        SYSTYPE_ALL,  NULL },
    { SM_CYVTHUMB,          IDS_SM_CYVTHUMB,         SYSTYPE_ALL,  NULL },
    { SM_DBCSENABLED,       IDS_SM_DBCSENABLED,      SYSTYPE_ALL,  NULL }, 
    { SM_DEBUG,             IDS_SM_DEBUG,            SYSTYPE_ALL,  NULL },
    { SM_MENUDROPALIGNMENT, IDS_SM_MENUDROPALIGNMENT,SYSTYPE_ALL,  NULL },
    { SM_MIDEASTENABLED,    IDS_SM_MIDEASTENABLED,   SYSTYPE_API4, NULL },
    { SM_MOUSEPRESENT,      IDS_SM_MOUSEPRESENT,     SYSTYPE_ALL,  NULL },
    { SM_NETWORK,           IDS_SM_NETWORK,          SYSTYPE_API4, NULL },
    { SM_PENWINDOWS,        IDS_SM_PENWINDOWS,       SYSTYPE_ALL,  NULL },
    { SM_SECURE,            IDS_SM_SECURE,           SYSTYPE_API4, NULL },
    { SM_SHOWSOUNDS,        IDS_SM_SHOWSOUNDS,       SYSTYPE_W32,  NULL },
    { SM_SWAPBUTTON,        IDS_SM_SWAPBUTTON,       SYSTYPE_ALL,  NULL }
} ;

int displays[] = {
        IDS_CLEAN_BOOT,
        IDS_FAILSAFE_BOOT,
        IDS_FAILSAFE_NETWORK_BOOT,
        0        }; // EOT


/****************************************************************************
*                             metrics_CleanString
* Inputs:
*       HWND hwnd: Window handle
*       int value: Value returned by SM_CLEANBOOT
*       LPSTR Buffer: Place to put result string
*       int size: Size of Buffer
* Result: void
*       
* Effect: 
*       Loads the string for the type of boot found in value
****************************************************************************/

static void
metrics_CleanString(HWND hwnd, int value, LPSTR Buffer, int size)
{
 // 0: normal boot
 // 1: fail-safe boot
 // 2: fail-safe with network boot
 switch(value)
    { /* value */
     case 0: // normal boot
             LoadString(GetWindowInstance(hwnd), IDS_CLEAN_BOOT, Buffer, size);
             return;
     case 1: // fail-safe boot
             LoadString(GetWindowInstance(hwnd), IDS_FAILSAFE_BOOT, Buffer, size);
             return;
     case 2: // fail-safe w/Netword boot
             LoadString(GetWindowInstance(hwnd), IDS_FAILSAFE_NETWORK_BOOT, Buffer, size);
             return;
    } /* value */
}

/****************************************************************************
*                             metrics_Arrange
* Inputs:
*       HWND hwnd: Window handle
*       int value: Value returned by SM_ARRANGE
*       LPSTR Buffer: Place to put result string
*       int size: Size of Buffer
* Result: void
*       
* Effect: 
*       Loads the string for the type of arrangement found in value
****************************************************************************/

static void
metrics_Arrange(HWND hwnd, int value, LPSTR Buffer, int size)
{
 // ARW_BOTTOMLEFT:    BL
 // ARW_BOTTOMRIGHT:   BR
 // ARW_TOPLEFT:       TL
 // ARW_TOPRIGHT:      TR
 // 
 // ARW_LEFT:            <= note that left and right are same code
 // ARW_RIGHT:           <= and we leave it blank
 // ARW_UP:            U
 // ARW_DOWN:          D
 //
 // ARW_HIDE:          H 
 int len;

 switch(value & ARW_STARTMASK)
    { /* value */
     case ARW_BOTTOMLEFT: 
             LoadString(GetWindowInstance(hwnd), IDS_ARW_BOTTOMLEFT, Buffer, size);
             break;
     case ARW_BOTTOMRIGHT:
             LoadString(GetWindowInstance(hwnd), IDS_ARW_BOTTOMRIGHT, Buffer, size);
             break;
     case ARW_TOPLEFT:
             LoadString(GetWindowInstance(hwnd), IDS_ARW_TOPLEFT, Buffer, size);
             break;
     case ARW_TOPRIGHT:
             LoadString(GetWindowInstance(hwnd), IDS_ARW_TOPRIGHT, Buffer, size);
             break;
     default:
             *Buffer = _T('\0');
    } /* value */

 len = lstrlen(Buffer);

 switch(value & ~ARW_STARTMASK & ~ARW_HIDE)
    { /* direction */
     case ARW_LEFT: // aka ARW_RIGHT
             LoadString(GetWindowInstance(hwnd), IDS_ARW_LR, Buffer+len, size - len);
             break;
     case ARW_UP:  // aka ARW_DOWN
             LoadString(GetWindowInstance(hwnd), IDS_ARW_UD, Buffer+len, size - len);
             break;
    } /* direction */
 
 len = lstrlen(Buffer);

 if(value & ARW_HIDE)
    LoadString(GetWindowInstance(hwnd), IDS_ARW_HIDE, Buffer+len, size - len);
}

/****************************************************************************
*                               metrics_OnPaint
* Inputs:
*       HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Paints the lines of text.
* Notes:
*       This is a three-pass algorithm.  The first pass computes the widths
*       off all the first-column strings so that we know the maximum
*       width of the first column.  Only the lines that are actually visible
*       are drawn.
*
*       The second pass does exactly the same thing for the second column
*       except that it computes the maximum width of the second column
*
*       The third column is right-justified, so we run a computation based
*       on the widest possible string before we draw anything.
*
*       This is a picture of the abstract document.  The window fits
*       over this.  All values but line count are in pixels.
*
*       |<---------document width---------->|
*       +-----------------------------------+ <---
*       |                                   |     top margin
*       |  +-----------------------------+  | <---
*       |  |xxxxxx         xxxxxx     xxx|  |   ^
*       |  |xxxxxxxx       xxxx      xxxx|  |   |
*       |  |xxxxxxxxx      xxxx       xxx|  |   | line count
*       |  |xxxxxxxxxxxxxx xxxxx      xxx|  |   |
*       |  |xxxxxx         xxxxxxxx  xxxx|  |   |
*       |  |xxxxx          xxxx     xxxxx|  |   v
*       |  +-----------------------------+  | <---
*       |                                   |
*       +-----------------------------------+
*       ^  ^
*       |  | Left margin
*
*       The window keeps its scrollbars in pixels horizontally and
*       lines vertically.
*
*       We scroll vertically in integral number of lines, so the
*       coordinate of an actual line i is 
*               hMargin + ((i - getScrollPos()) * cyChar)
****************************************************************************/

static void
metrics_OnPaint(HWND hwnd)
{
 HINSTANCE   hInstance;
 SIZE        TextExtent ;
 int         i;
 int         Count;
 int         value;
 int         LineFirst;
 int         LineLast ;
 LONG        lval ;
 PAINTSTRUCT ps ;
 RECT        rectText;
 RECT        rectExplain;
 RECT        rect ;
 HDC         hdc;
 TCHAR       Buffer [MAX_RESOURCESTRING] ;
 int         cxLeftMargin ;
 int         cyTopMargin ;
 int         CurrHPos;
 int         CurrVPos ;
 WORD        SystemType ;
 int         SymExtent;
 int         DescExtent ;
 TEXTMETRIC  tm ;
 int         cxChar;
 int         cyChar;
 int         maxlen;

 // Initialization

 hdc = BeginPaint (hwnd, &ps) ;

 GetTextMetrics (hdc, &tm) ;
 cxChar = tm.tmMaxCharWidth ;
 cyChar = tm.tmHeight + tm.tmExternalLeading ;

 SystemType = getSystemType() ;
 hInstance = GetWindowInstance (hwnd) ;
 DescExtent = 0 ;
 SymExtent = 0 ;

 // Get the current margins
 lval = SendMessage (hwnd, SM_GETMARGINS, 0, 0) ;
 cxLeftMargin = LOWORD (lval) ;
 cyTopMargin  = HIWORD (lval) ;

 // Get the current position of the scroll bars. 
 // This uses our own defined message SM_GETSCROLLPOS 
 lval = SendMessage (hwnd, SM_GETSCROLLPOS, 0, 0L) ;
 CurrHPos = LOWORD (lval) ;  // in pixels
 CurrVPos = HIWORD (lval) ;  // in lines

 /* Display the symbolic names */
 rectText.left = cxLeftMargin - CurrHPos ;

 // Compute the first and last lines that will be displayed

 LineFirst = max (0, CurrVPos + ps.rcPaint.top / cyChar - 1) ;
 LineLast  = min (DIM(SMtbl), CurrVPos + (ps.rcPaint.bottom + cyChar - 1) / cyChar) ;

 for (i = 0; i < DIM(SMtbl); i++) 
    { /* SMtbl names */
     // Show options not available in a dimmed color
     if ((SystemType & SMtbl[i].SystemType) == 0)
        SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
     else
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

     // Get the SystemMetric's symbolic name. 
     // Compute its maximum width so we know the overall max string size

     Count = LoadString (hInstance, SMtbl [i]. StringId,
                                                   Buffer, DIM(Buffer)) ;
     GetTextExtentPoint32 (hdc, Buffer, Count, &TextExtent) ;
     SymExtent = max (SymExtent, TextExtent.cx) ;

     // If the line is off the screen, don't bother showing it.
     // But we've accounted for its width
     if (i < LineFirst || i >= LineLast)
         continue ;

     rectText.top = cyTopMargin + (i - CurrVPos) * cyChar ;
     rectText.right = rectText.left + TextExtent.cx;
     rectText.bottom = rectText.top + cyChar ;

     // Demonstrate optimization: only paint it if it is in the
     // invalidated rectangle
     if (IntersectRect (&rect, &rectText, &ps.rcPaint))
        TextOut (hdc, rectText.left, rectText.top,
                 Buffer, Count) ;

    } /* SMtbl names */

 // Now that we've put out the first column, we know the widest string
 // that has been displayed in that column

 /* Display the descriptions */
 rectText.left += (int) SymExtent + cxChar ;
 for (i = 0; i < DIM(SMtbl); i++) 
    { /* SMtbl descriptions */
     // Load the description and display it
     Count = LoadString (hInstance, SMtbl [i].StringId + 1,
                          Buffer, DIM(Buffer)) ;
     GetTextExtentPoint32 (hdc, Buffer, Count, &TextExtent) ;
     DescExtent = max (DescExtent, TextExtent.cx) ;

     if (i < LineFirst || i >= LineLast)
         continue ;

     if ((SystemType & SMtbl[i].SystemType) == 0)
        SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
     else
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

     rectText.top = cyTopMargin + (i - CurrVPos) * cyChar ;
     rectText.right = rectText.left + TextExtent.cx;
     rectText.bottom = rectText.top + cyChar ;
     if (IntersectRect (&rect, &rectText, &ps.rcPaint))
             TextOut (hdc, rectText.left, rectText.top,
                             Buffer, Count) ;
    } /* SMtbl descriptions */

 /* Display the values */
 // We have to first compute the widest string.  To do this, we have a 
 // list of all the strings we use.  We compute the text extent based on this

 LoadString(hInstance, IDS_VALUE, Buffer, DIM(Buffer));
 GetTextExtentPoint32(hdc, Buffer, lstrlen(Buffer), &TextExtent);

 rectText.left += (int) DescExtent + cxChar ;
 rectText.right = rectText.left + TextExtent.cx;
 for (i = LineFirst; i < LineLast; i++) 
    { /* value */
     /* Get the SystemMetric's value. */
     if ((SystemType & SMtbl[i].SystemType) == 0)
        { /* not defined */
         SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
         lstrcpy(Buffer, _T("----"));
        } /* not defined */
     else
        { /* defined */
         SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
         value = GetSystemMetrics (SMtbl [i].Index) ;
         wsprintf (Buffer, _T("%4d"), value) ;
        } /* defined */

     rectText.top = cyTopMargin + (i - CurrVPos) * cyChar ;
     rectText.bottom = rectText.top + cyChar ;

     SetTextAlign (hdc, TA_RIGHT) ;
     if (IntersectRect (&rect, &rectText, &ps.rcPaint))
          TextOut (hdc, rectText.right - 1, rectText.top,
                                        Buffer, lstrlen(Buffer)) ;

     rectExplain = rectText;
     rectExplain.left = rectText.right + cxChar;
     // We will compute rectExplain.right as needed

     maxlen = rectText.right; // assume no explanations needed yet

     if(SMtbl[i].format != NULL && (SystemType & SMtbl[i].SystemType) != 0)
        { /* has explanation */
         SIZE tx;
         value = GetSystemMetrics (SMtbl [i].Index) ;
         SetTextAlign (hdc, TA_LEFT) ;
         SMtbl[i].format(hwnd, value, Buffer, DIM(Buffer));
         GetTextExtentPoint32(hdc, Buffer, lstrlen(Buffer), &tx);
         maxlen = max(maxlen, rectExplain.left + tx.cx);
         rectExplain.right = rectExplain.left + tx.cx;
         if(IntersectRect(&rect, &rectExplain, &ps.rcPaint))
              TextOut(hdc, rectExplain.left - 1, rectExplain.top, 
                                        Buffer, lstrlen(Buffer));
        } /* has explanation */
    } /* value */

 // Now set the horizontal and vertical scrolling limits
  
 SendMessage(hwnd, SM_SETDOCSIZE, maxlen, DIM(SMtbl));
 EndPaint (hwnd, &ps) ;
}

/****************************************************************************
*                            SystemMetricsWndProc
* Inputs:
*       HWND hwnd:
*       UINT msg: Message ID
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: LRESULT
*       as per message type
* Effect: 
*       Dispatcher for system metrics windows
* Notes:
*       System metrics windows are a subclass of the abstract class of
*       scrolling windows, so any message that is not specific to this
*       window class is handled by the scrolling class
****************************************************************************/

static LRESULT CALLBACK
SystemMetricsWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
       { /* msg */
        HANDLE_MSG(hwnd, WM_PAINT, metrics_OnPaint);
       } /* msg */

    // Call the superclass directly 

    return
        CallWindowProc (ScrollingWndProc, hwnd, msg, wParam, lParam) ;
}

/****************************************************************************
*                             createSystemMetrics
* Inputs:
*       HINSTANCE hinst: Instance handle
*       HWND hwnd: Parent window of System Metrics window
* Result: HWND
*       Window handle of child window.  If a  child window already exists,
*       its handle is returned.
* Effect: 
*       The result is a visible, active, non-iconic window.  If a child
*       window of the correct type already exists, it is activated.  
* Notes:
*       The window fills the client area, except for the area occupied by
*       the toolbar
****************************************************************************/

HWND createSystemMetrics(HINSTANCE hinst, HWND hwnd)
{
 return createChildWindow(hinst, hwnd, IDS_SYSTEMMETRICSCLASS, 
                                       IDS_SYSTEMMETRICSTITLE);
}

/****************************************************************************
*                            registerSystemMetrics
* Inputs:
*       HINSTANCE hinst: Instance handle
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the System Metrics window class
****************************************************************************/

BOOL registerSystemMetrics(HINSTANCE hinst)
{
 return registerChildClass(hinst, IDS_SYSTEMMETRICSCLASS, IDI_SYSMETRICS,
                                        (WNDPROC)SystemMetricsWndProc);
}

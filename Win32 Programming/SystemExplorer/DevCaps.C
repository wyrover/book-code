#include "stdSDK.h"
#include "resource.h"
#include "scroll.h"
#include "systype.h"


static struct tagDCTbl {
    int         Index ;
    WORD        StringId ;
    WORD        hex ;
} DCtbl [] = {
    { DRIVERVERSION,       IDS_DRIVERVERSION,   TRUE },
    { TECHNOLOGY,          IDS_TECHNOLOGY,      FALSE },
    { HORZSIZE,            IDS_HORZSIZE,        FALSE },
    { VERTSIZE,            IDS_VERTSIZE,        FALSE },
    { HORZRES,             IDS_HORZRES,         FALSE },
    { VERTRES,             IDS_VERTRES,         FALSE },
    { LOGPIXELSX,          IDS_LOGPIXELSX,      FALSE },
    { LOGPIXELSY,          IDS_LOGPIXELSY,      FALSE },
    { BITSPIXEL,           IDS_BITSPIXEL,       FALSE },
    { PLANES,              IDS_PLANES,          FALSE },
    { NUMBRUSHES,          IDS_NUMBRUSHES,      FALSE },
    { NUMPENS,             IDS_NUMPENS,         FALSE },
    { NUMFONTS,            IDS_NUMFONTS,        FALSE },
    { NUMCOLORS,           IDS_NUMCOLORS,       FALSE },
    { ASPECTX,             IDS_ASPECTX,         FALSE },
    { ASPECTY,             IDS_ASPECTY,         FALSE },
    { ASPECTXY,            IDS_ASPECTXY,        FALSE },
    { PDEVICESIZE,         IDS_PDEVICESIZE,     FALSE },
    { CLIPCAPS,            IDS_CLIPCAPS,        FALSE },
    { SIZEPALETTE,         IDS_SIZEPALETTE,     FALSE },
    { NUMRESERVED,         IDS_NUMRESERVED,     FALSE },
    { COLORRES,            IDS_COLORRES,        FALSE },
    { RASTERCAPS,          IDS_RASTERCAPS,      TRUE  },
    { CURVECAPS,           IDS_CURVECAPS,       TRUE  },
    { LINECAPS,            IDS_LINECAPS,        TRUE  },
    { POLYGONALCAPS,       IDS_POLYGONALCAPS,   TRUE  },
    { TEXTCAPS,            IDS_TEXTCAPS,        TRUE  }
} ;

/****************************************************************************
*                               devcaps_OnPaint
* Inputs:
*       HWND hwnd: Window handle
* Result: void
*       
* Effect: 
*       Paints the lines of text.
* Notes:
*       This is a two-pass algorithm.  The first pass computes the widths
*       off all the first-column strings so that we know the maximum
*       width of the first column.  Only the lines that are actually visible
*       are drawn.
*
*       The second pass does exactly the same thing for the second column
*       except that it computes the maximum width of the second column
*
*       This is a picture of the abstract document.  The window fits
*       over this.  All values but line count are in pixels.
*
*       |<-----document width------>|
*       +---------------------------+ <---
*       |                           |     top margin
*       |  +---------------------+  | <---
*       |  |xxxxxx            xxx|  |   ^
*       |  |xxxxxxxx         xxxx|  |   |
*       |  |xxxxxxxxx         xxx|  |   | line count
*       |  |xxxxxxxxxxxxxx    xxx|  |   |
*       |  |xxxxxx           xxxx|  |   |
*       |  |xxxxx           xxxxx|  |   v
*       |  +---------------------+  | <---
*       |                           |
*       +---------------------------+
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
devcaps_OnPaint(HWND hwnd)
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
 RECT        rect ;
 HDC         hdc;
 TCHAR       Buffer [MAX_RESOURCESTRING] ;
 int         cxLeftMargin ;
 int         cyTopMargin ;
 int         CurrHPos;
 int         CurrVPos ;
 WORD        SystemType ;
 int         SymExtent;
 TEXTMETRIC  tm ;
 int         cxChar;
 int         cyChar;

 // Initialization

 hdc = BeginPaint (hwnd, &ps) ;

 GetTextMetrics (hdc, &tm) ;
 cxChar = tm.tmMaxCharWidth ;
 cyChar = tm.tmHeight + tm.tmExternalLeading ;

 SystemType = getSystemType() ;
 hInstance = GetWindowInstance (hwnd) ;
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
 LineLast  = min (DIM(DCtbl), (CurrVPos + ps.rcPaint.bottom + cyChar - 1) / cyChar) ;

 for (i = 0; i < DIM(DCtbl); i++) 
    { /* DCtbl names */
     // Get the DevCap's symbolic name. 
     // Compute its maximum width so we know the overall max string size

     Count = LoadString (hInstance, DCtbl [i]. StringId,
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

    } /* DCtbl names */

 // Now that we've put out the first column, we know the widest string
 // that has been displayed in that column

 /* Display the values */
 // We have to first compute the widest string.  To do this, we have a 
 // list of all the strings we use.  We compute the text extent based on this

 LoadString(hInstance, IDS_DEV_VALUE, Buffer, DIM(Buffer));
 GetTextExtentPoint32(hdc, Buffer, lstrlen(Buffer), &TextExtent);

 rectText.left += (int) SymExtent + cxChar ;
 rectText.right = rectText.left + TextExtent.cx;
 for (i = LineFirst; i < LineLast; i++) 
    { /* value */
     /* Get the Devcap's value. */
     value = GetDeviceCaps (hdc, DCtbl [i].Index) ;
     if(DCtbl[i].hex)
        wsprintf (Buffer, _T("0x%08x"), value);
     else
        wsprintf (Buffer, _T("%4d"), value) ;

     rectText.top = cyTopMargin + (i - CurrVPos) * cyChar ;
     rectText.bottom = rectText.top + cyChar ;

     SetTextAlign (hdc, TA_RIGHT) ;
     if (IntersectRect (&rect, &rectText, &ps.rcPaint))
          TextOut (hdc, rectText.right - 1, rectText.top,
                                        Buffer, lstrlen(Buffer)) ;
    } /* value */

 // Now set the horizontal and vertical scrolling limits

 SendMessage(hwnd, SM_SETDOCSIZE, CurrHPos + rectText.right, DIM(DCtbl));
 EndPaint (hwnd, &ps) ;
}

/****************************************************************************
*                            DeviceCapsWndProc
* Inputs:
*       HWND hwnd:
*       UINT msg: Message ID
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: LRESULT
*       as per message type
* Effect: 
*       Dispatcher for device capabilities windows
* Notes:
*       Device capabilities windows are a subclass of the abstract class of
*       scrolling windows, so any message that is not specific to this
*       window class is handled by the scrolling class
****************************************************************************/

static LRESULT CALLBACK
DeviceCapsWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) 
       { /* msg */
        HANDLE_MSG(hwnd, WM_PAINT, devcaps_OnPaint);
       } /* msg */

    // Call the superclass directly 

    return
        CallWindowProc (ScrollingWndProc, hwnd, msg, wParam, lParam) ;
}

/****************************************************************************
*                             createDevCaps
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

HWND createDevCaps(HINSTANCE hinst, HWND hwnd)
{
 return createChildWindow(hinst, hwnd, IDS_DEVICECAPSCLASS, 
                                       IDS_DEVICECAPSTITLE);
}

/****************************************************************************
*                            registerDevCaps
* Inputs:
*       HINSTANCE hinst: Instance handle
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the Device capabilities window class
****************************************************************************/

BOOL registerDevCaps(HINSTANCE hinst)
{
 return registerChildClass(hinst, IDS_DEVICECAPSCLASS, IDI_DEVCAPS, 
                                                 (WNDPROC)DeviceCapsWndProc);
}

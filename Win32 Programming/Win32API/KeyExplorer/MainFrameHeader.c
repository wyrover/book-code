#include "StdSDK.h"             // Standard application includes
#include "MainFrameHeader.h"   // For non-static function prototypes
#include "resource.h"           // For resource identifiers

#include "columns.h"

//
// Function prototypes for static functions
//

static BOOL header_AddItem (HWND hwndHeader, LPCTSTR pszText, int nWidth) ;

//
// Function prototypes for callback functions
//


//
//  HWND
//  header_CreateHeader (HWND hwndParent)
//
//  hwndParent      Handle of parent window of header control
//
//  PURPOSE:        Create a Header for the main frame window
//
//  COMMENTS::
//
//

HWND
header_CreateHeader (HWND hwndParent)
{
    HINSTANCE           hinst ;
    HWND                hwndHeader ;

    // Get the application's instance handle
    hinst = GetWindowInstance (hwndParent) ;

    // Create the header control
    hwndHeader =
        CreateWindowEx (0,                      // extended window style
                        WC_HEADER,              // header control class name
                        NULL,                   // window name 
                        WS_CHILD |              // window style
                        WS_BORDER |
                        HDS_HORZ,
                        0, 0, 0, 0,             // horiz, vert, width, height
                        hwndParent,             // parent window
                        (HMENU)IDC_HEADER,      // child window identifier
                        hinst,                  // application instance
                        NULL) ;                 // creation parameters

    return hwndHeader ;
}


//
//  BOOL Header_Initialize (HWND hwndHeader)
//
//  hwndHeader      Handle of header control
//
//  PURPOSE:        (Re)Initialize the size of the header control
//
//  COMMENTS:
//


BOOL header_Initialize (HWND hwndHeader)
{
    HWND                hwndParent ;
    RECT                rectParent ;
    HD_LAYOUT           hdl ;
    WINDOWPOS           wp ;

    // Determine the parent window
    hwndParent = GetParent (hwndHeader) ;

    // Get the client rectangle of the parent window
    GetClientRect (hwndParent, &rectParent) ;

    // Constrain the header control to its parent window's bounding rectangle
    hdl.prc   = &rectParent ;
    hdl.pwpos = &wp ;

    // Retrieves the size and position
    // of the header control within its parent's client area.
    if (!Header_Layout (hwndHeader, &hdl))
        return FALSE ;

    // Set the size, position, and visibility of the header control.

    SetWindowPos (hwndHeader, 
                  wp.hwndInsertAfter, 
                  wp.x, 
                  wp.y,
                  wp.cx, 
                  wp.cy,
                  wp.flags | SWP_SHOWWINDOW) ;

    return TRUE ;
}

//
//  void header_AddItems (HWND hwndHeader)
//
//  hwndHeader      Handle of header control
//
//  PURPOSE:        Insert all items into the header control
//
//  COMMENTS:
//


BOOL header_AddItems (HWND hwndHeader)
{
    HINSTANCE           hInst ;
    int                 nResult ;
    TCHAR               achBuffer [64] ;
    int i;

    hInst = GetWindowInstance (hwndHeader) ;

    for(i = 0; i < DIM(column_widths); i++)
       { /* load columns */
	VERIFY(nResult = LoadString(hInst, column_widths[i].caption,
				achBuffer, DIM(achBuffer)));
        header_AddItem(hwndHeader, achBuffer, column_widths[i].width);
       } /* load columns */

    return TRUE ;
}

//
//  BOOL header_AddItem (HWND hwndHeader, int nWidth)
//
//  hwndHeader      Handle of header control
//  nWidth          Width of string, in thousandths of an inch 
//
//  PURPOSE:        Insert an item into the header control
//                  at the rightmost end
//
//  COMMENTS:
//


static BOOL header_AddItem (HWND hwndHeader, LPCTSTR pszText, int nWidth)
{
    int        nItem ;
    HD_ITEM    hdi;
    HDC        dc = GetDC(hwndHeader);
    POINT      pt;

    SetMapMode(dc, MM_HIENGLISH);

    hdi.mask = HDI_TEXT |       // pszText and cchTextMax member are valid
               HDI_FORMAT |     // fmt member is valid
               HDI_WIDTH ;      // cxy member is valid

    // Specify text of header item
    hdi.pszText    = (LPTSTR) pszText ;
    hdi.cchTextMax = _tcslen (pszText) ;

    // Specify the item is a left-justified string
    hdi.fmt = HDF_LEFT | HDF_STRING ;

    // Specify the width of the item, which must be in pixels
    hdi.cxy = nWidth;

    pt.x = nWidth;
    pt.y = 0;
    LPtoDP(dc, &pt, 1);
    hdi.cxy = pt.x;

    ReleaseDC(hwndHeader, dc);

    // Insert after last item in header
    nItem = Header_GetItemCount (hwndHeader) ;
    if (-1 == Header_InsertItem (hwndHeader, nItem, &hdi))
        return FALSE ;

    return TRUE;
}


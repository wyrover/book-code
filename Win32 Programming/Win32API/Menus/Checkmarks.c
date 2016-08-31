#include "StdSDK.h"             // Standard application includes
#include "Checkmarks.h"         // For non-static function prototypes
#include "About.h"              // For non-static function prototypes
#include "resource.h"           // For resource identifiers

//
// Function prototypes for static functions
//

//
// Global data
//

HBITMAP hbitmapChecked   = NULL ;
HBITMAP hbitmapUnchecked = NULL ;


void
createCheckmarkBitmaps (HWND hwnd)
{
    DWORD               dw ;
    HBITMAP             hbitmapOrig ;
    HBRUSH              hbrush, hbrushOrig ;
    HDC                 hdc, hMemoryDC ;
    HPEN                hpenCross, hpenFrame, hpenOrig ;
    WORD                wWidth, wHeight ;

    hdc = GetDC (hwnd) ;

    // Create a compatible memory device context.
    hMemoryDC = CreateCompatibleDC (hdc) ;

    // Get the proper size of a checkmark.
    dw = GetMenuCheckMarkDimensions () ;
    wWidth = LOWORD (dw) ;
    wHeight = HIWORD (dw) ;

    // Create two uninitialized bitmaps.
    // One for the checked bitmap and one for the unchecked bitmap.
    hbitmapChecked = CreateCompatibleBitmap (hdc, wWidth, wHeight) ;
    hbitmapUnchecked = CreateCompatibleBitmap (hdc, wWidth, wHeight) ;

    ReleaseDC (hwnd, hdc) ;

    /********************************************************/
    /* Draw a rectangle in the system menu text color.      */
    /* The inside of the rectangle is filled with the       */
    /* system menu text color.                              */
    /* The checked bitmap has an X drawn in this rectangle. */
    /********************************************************/

    hbrush = CreateSolidBrush (GetSysColor (COLOR_MENU)) ;
    hpenFrame = CreatePen (PS_INSIDEFRAME, 0, GetSysColor (COLOR_MENUTEXT)) ;
    hpenCross = CreatePen (PS_SOLID, 2, GetSysColor (COLOR_MENUTEXT)) ;

    hbitmapOrig = SelectBitmap (hMemoryDC, hbitmapUnchecked) ;
    hbrushOrig  = SelectBrush (hMemoryDC, hbrush) ;
    hpenOrig    = SelectPen (hMemoryDC, hpenFrame) ;

    // Initialize the unchecked bitmap.
    PatBlt (hMemoryDC, 0, 0, wWidth, wHeight, PATCOPY) ;
 
    // Create the unchecked image.
    Rectangle (hMemoryDC, 1, 1, wWidth - 1, wHeight - 1) ;

    // Initialize the unchecked bitmap.
    SelectBitmap (hMemoryDC, hbitmapChecked) ;
    PatBlt (hMemoryDC, 0, 0, wWidth, wHeight, PATCOPY) ;
 
    // Create the checked image.
    Rectangle (hMemoryDC, 1, 1, wWidth - 1, wHeight - 1) ;
    SelectPen (hMemoryDC, hpenCross) ;
    MoveToEx (hMemoryDC, 2, 2, NULL) ;
    LineTo (hMemoryDC, wWidth - 2, wHeight - 2) ;
    MoveToEx (hMemoryDC, wWidth - 2, 2, NULL) ;
    LineTo (hMemoryDC, 2, wHeight - 2) ;

    // Free the resources.
    SelectBitmap (hMemoryDC, hbitmapOrig) ;
    SelectBrush (hMemoryDC, hbrushOrig) ;
    SelectPen (hMemoryDC, hpenOrig) ;
    DeleteDC (hMemoryDC) ;

    DeleteBrush (hbrush) ;
    DeletePen (hpenFrame) ;
    DeletePen (hpenCross) ;
}


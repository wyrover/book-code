#include "StdSDK.h"             // Standard application includes
#include "DisplayMessage.h"     // For non-static function prototypes
#include "resource.h"           // For resource identifiers

#include <stdlib.h>
#include "columns.h"

//
// Function prototypes for static functions and arrays
//

static int VkMap [] = {
    IDS_VK_UNDEFINED,   IDS_VK_LBUTTON,    IDS_VK_RBUTTON,    IDS_VK_CANCEL,
    IDS_VK_MBUTTON,     IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_BACK,        IDS_VK_TAB,        IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_CLEAR,       IDS_VK_RETURN,     IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_SHIFT,       IDS_VK_CONTROL,    IDS_VK_MENU,       IDS_VK_PAUSE,
    IDS_VK_CAPITAL,     IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_ESCAPE,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_SPACE,       IDS_VK_PRIOR,      IDS_VK_NEXT,       IDS_VK_END,
    IDS_VK_HOME,        IDS_VK_LEFT,       IDS_VK_UP,         IDS_VK_RIGHT,
    IDS_VK_DOWN,        IDS_VK_SELECT,     IDS_VK_PRINT,      IDS_VK_EXECUTE,
    IDS_VK_SNAPSHOT,    IDS_VK_INSERT,     IDS_VK_DELETE,     IDS_VK_HELP,
    IDS_VK_0,           IDS_VK_1,          IDS_VK_2,          IDS_VK_3,
    IDS_VK_4,           IDS_VK_5,          IDS_VK_6,          IDS_VK_7,
    IDS_VK_8,           IDS_VK_9,          IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_A,          IDS_VK_B,          IDS_VK_C,
    IDS_VK_D,           IDS_VK_E,          IDS_VK_F,          IDS_VK_G,
    IDS_VK_H,           IDS_VK_I,          IDS_VK_J,          IDS_VK_K,
    IDS_VK_L,           IDS_VK_M,          IDS_VK_N,          IDS_VK_O,
    IDS_VK_P,           IDS_VK_Q,          IDS_VK_R,          IDS_VK_S,
    IDS_VK_T,           IDS_VK_U,          IDS_VK_V,          IDS_VK_W,
    IDS_VK_X,           IDS_VK_Y,          IDS_VK_Z,          IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_NUMPAD0,     IDS_VK_NUMPAD1,    IDS_VK_NUMPAD2,    IDS_VK_NUMPAD3,
    IDS_VK_NUMPAD4,     IDS_VK_NUMPAD5,    IDS_VK_NUMPAD6,    IDS_VK_NUMPAD7,
    IDS_VK_NUMPAD8,     IDS_VK_NUMPAD9,    IDS_VK_MULTIPLY,   IDS_VK_ADD,
    IDS_VK_SEPARATOR,   IDS_VK_SUBTRACT,   IDS_VK_DECIMAL,    IDS_VK_DIVIDE,
    IDS_VK_F1,          IDS_VK_F2,         IDS_VK_F3,         IDS_VK_F4,
    IDS_VK_F5,          IDS_VK_F6,         IDS_VK_F7,         IDS_VK_F8,
    IDS_VK_F9,          IDS_VK_F10,        IDS_VK_F11,        IDS_VK_F12,
    IDS_VK_F13,         IDS_VK_F14,        IDS_VK_F15,        IDS_VK_F16,
    IDS_VK_F17,         IDS_VK_F18,        IDS_VK_F19,        IDS_VK_F20,
    IDS_VK_F21,         IDS_VK_F22,        IDS_VK_F23,        IDS_VK_F24,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_NUMLOCK,     IDS_VK_OEM_SCROLL, IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_LSHIFT,      IDS_VK_RSHIFT,     IDS_VK_LCONTROL,   IDS_VK_RCONTROL,
    IDS_VK_LMENU,       IDS_VK_RMENU,      IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_OEM_1,      IDS_VK_OEM_PLUS,
    IDS_VK_OEM_COMMA,   IDS_VK_OEM_MINUS,  IDS_VK_OEM_PERIOD, IDS_VK_OEM_2,
    IDS_VK_OEM_3,       IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_OEM_4,
    IDS_VK_OEM_5,       IDS_VK_OEM_6,      IDS_VK_OEM_7,      IDS_VK_OEM_8,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_OEM_102,    IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_PROCESSKEY, IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,  IDS_VK_UNDEFINED,
    IDS_VK_UNDEFINED,   IDS_VK_UNDEFINED,  IDS_VK_ATTN,       IDS_VK_CRSEL,
    IDS_VK_EXSEL,       IDS_VK_EREOF,      IDS_VK_PLAY,       IDS_VK_ZOOM,
    IDS_VK_NONAME,      IDS_VK_PA1,        IDS_VK_OEM_CLEAR,  IDS_VK_UNDEFINED,
    0 // end of table for message width scan
} ;


static int msgMap[] = {IDS_WM_CHAR, IDS_WM_DEADCHAR, IDS_WM_KEYDOWN,
                       IDS_WM_KEYUP, IDS_WM_SYSCHAR, IDS_WM_SYSDEADCHAR,
		       IDS_WM_SYSKEYDOWN, IDS_WM_SYSKEYUP, 0};

static int digits[] = {IDS_DIGITS, 0};


static int ext[] = {IDS_EXT, IDS_NOTEXT, 0};

static int context[] = {IDS_ALT, IDS_NOALT, 0};

static int updown[] = {IDS_UP, IDS_DOWN, 0};

static int trans[] = {IDS_RELEASE, IDS_PRESS, 0};

widthtable column_widths[8] = {
	{msgMap,  IDS_MESSAGETITLE,     0, 0},  // WM_CHAR, etc.
	{VkMap, IDS_KEYCODETITLE,     0, 0},  // VK_WHATEVER, etc.
	{digits,  IDS_REPEATCOUNTTITLE, 0, 0},  // repeat count: 1
	{digits,  IDS_SCANCODETITLE,    0, 0},  // scan code: 99
	{ext,     IDS_EXTFLAGTITLE,     0, 0},  // Extended/Not extended
	{context, IDS_CONTEXTTITLE,     0, 0},  // context code: ALT, Not ALT
	{updown,  IDS_UPDOWNTITLE,      0, 0},  // up/down code
        {trans,   IDS_TRANSITIONTITLE,  0, 0},  // transition code
			      };

//
// Function prototypes for callback functions
//


//
// Function prototypes for message handlers
//


//
// Typedefs
//

//
// Function prototypes for callback functions
//

VOID
mainFrame_DisplayMessage (HWND hwnd, UINT wId, WPARAM wParam, LPARAM lParam)
{
    TCHAR       Buffer [64] ;
    HINSTANCE   hInst ;
    HDC         hdc ;
    HWND        hwndHeader ;
    int         nResult, xPos, nLabelId, length ;
    RECT        rect, rectHeader, rectScrolled ;
    SIZE	sz;
    SIZE        space;

    // Calculate area to be scrolled - omit header control
    GetClientRect (hwnd, &rectScrolled) ;

    // Exclude size of header
    hwndHeader = GetDlgItem (hwnd, IDC_HEADER) ;
    if (NULL != hwndHeader) {
        GetClientRect (hwndHeader, &rectHeader) ;
        rectScrolled.top += rectHeader.bottom ;    // Omit header
    }


    // Make room for the new line.
    ScrollWindowEx (hwnd, 0, -mainFrame_cyPixels,
                    &rectScrolled, &rectScrolled,
                    NULL, NULL, SW_ERASE | SW_INVALIDATE) ;
    
    // Erase the bottom line.
    UpdateWindow (hwnd) ;

    // Get the symbolic name of the message.
    hInst = GetWindowInstance (hwnd) ;
    VERIFY (nResult = LoadString (hInst, wId, Buffer, DIM (Buffer))) ;

    // Switch to 0.001 inch units.
    hdc = GetDC (hwnd) ;
    SetMapMode (hdc, MM_HIENGLISH) ;

    GetClientRect (hwnd, &rect) ;
    SetViewportOrgEx (hdc, rect.left, rect.bottom, NULL) ;

    // Display name of message starting 0.125 inch from left.
    TextOut (hdc, column_widths[COLUMN_MESSAGE].start, 
    				     mainFrame_cyChar, Buffer, nResult) ;

    length = 3 ;
    switch (wId) {

        case IDS_WM_CHAR:
        case IDS_WM_DEADCHAR:
        case IDS_WM_SYSCHAR:
        case IDS_WM_SYSDEADCHAR:

            // Display the ANSI/UNICODE character.
            Buffer [0] = TEXT('\'') ;
            Buffer [1] = (TCHAR) wParam ;
            Buffer [2] = TEXT('\'') ;

            switch (Buffer [1]) {
            case TEXT('\a'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('a') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\b'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('b') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\f'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('f') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\n'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('n') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\r'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('r') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\t'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('t') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\v'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('v') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\''):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('\'') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\"'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('\"') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;

            case TEXT('\\'):
                Buffer [1] = TEXT('\\') ;
                Buffer [2] = TEXT('\\') ;
                Buffer [3] = TEXT('\'') ;
                length = 4 ;
                break ;
            }

            // Display ANSI/UNICODE character 2.00 logical inches from left.
            TextOut (hdc, column_widths[COLUMN_KEYCODE].start, 
	    			       mainFrame_cyChar, Buffer, length) ;
            break ;

        case IDS_WM_KEYDOWN:
        case IDS_WM_KEYUP:
        case IDS_WM_SYSKEYDOWN:
        case IDS_WM_SYSKEYUP:

            // Display the virtual-key code.
            VERIFY (LoadString (hInst, VkMap [wParam], Buffer, DIM (Buffer))) ;

            // Display virtual-key code name 2.00 logical inches from left.
            TextOut (hdc, column_widths[COLUMN_KEYCODE].start, 
	    		   mainFrame_cyChar, Buffer, _tcslen (Buffer)) ;
            break ;
    }

    // Display repeat count 3.375 logical inches from left.
    // Right justify the numerals.
     
    GetTextExtentPoint32(hdc, _T(" "), 1, &space);


    // Display repeat count 3.375 logical inches from left.
    // Right justify the numerals.
    _itot ((int)(short)LOWORD (lParam), Buffer, 10) ;

    GetTextExtentPoint32(hdc, Buffer, _tcslen(Buffer), &sz);

    xPos = column_widths[COLUMN_REPEATCOUNT].start + 
           column_widths[COLUMN_REPEATCOUNT].width - (sz.cx + space.cx/2);

    TextOut (hdc, xPos, mainFrame_cyChar, Buffer, _tcslen (Buffer)) ;

    // Display scan code 4.00 logical inches from left.
    // Right justify the numerals.
    _itot (LOBYTE (HIWORD (lParam)), Buffer, 10) ;

    GetTextExtentPoint32(hdc, Buffer, _tcslen(Buffer), &sz);

    xPos = column_widths[COLUMN_SCANCODE].start + 
    	   column_widths[COLUMN_SCANCODE].width - (sz.cx + space.cx/2);

    TextOut (hdc, xPos, mainFrame_cyChar, Buffer, _tcslen (Buffer)) ;

    // Display extended key flag 4.50 logical inches from left.
    nLabelId = (lParam & 0x01000000) ? IDS_EXT : IDS_NOTEXT ;
    VERIFY (nResult = LoadString (hInst, nLabelId, Buffer, DIM (Buffer))) ;
    TextOut (hdc, column_widths[COLUMN_EXTENDED].start, 
    				       mainFrame_cyChar, Buffer, nResult) ;

    // Display context code 5.25 logical inches from left.
    nLabelId = (lParam & 0x20000000) ? IDS_ALT : IDS_NOALT ;
    VERIFY (nResult = LoadString (hInst, nLabelId, Buffer, DIM (Buffer))) ;
    TextOut (hdc, column_widths[COLUMN_CONTEXT].start, 
    				       mainFrame_cyChar, Buffer, nResult) ;

    // Display up-down code 5.875 logical inches from left.
    nLabelId = (lParam & 0x40000000) ? IDS_DOWN : IDS_UP ;
    VERIFY (nResult = LoadString (hInst, nLabelId, Buffer, DIM (Buffer))) ;
    TextOut (hdc, column_widths[COLUMN_UPDOWN].start, 
    				       mainFrame_cyChar, Buffer, nResult) ;

    // Display transition state 6.5 logical inches from left.
    nLabelId = (lParam & 0x80000000) ? IDS_RELEASE : IDS_PRESS ;
    VERIFY (nResult = LoadString (hInst, nLabelId, Buffer, DIM (Buffer))) ;
    TextOut (hdc, column_widths[COLUMN_TRANSITION].start, 
    				       mainFrame_cyChar, Buffer, nResult) ;

    ValidateRect (hwnd, NULL) ;
    ReleaseDC (hwnd, hdc) ;
}


/****************************************************************************
*                                compute_width
* Inputs:
*       HWND hwnd: Window to use for DC
*	int * table: Pointer to array of IDS_ symbols terminated with a 0
* Result: int
*       Width of widest string in the table, in pixels
****************************************************************************/

int compute_width(HWND hwnd, widthtable * cwt)
{
 int i;
 int width = 0;
 SIZE space;
 HDC hdc = GetDC(hwnd);
 SIZE sz;
 TCHAR text[64];

 SetMapMode(hdc, MM_HIENGLISH);

 // allow one space as margin

 GetTextExtentPoint32(hdc, _T(" "), 1, &space);

 for(i = 0; cwt->table[i] != 0; i++)
     { /* compute each width */
      LoadString(GetWindowInstance(hwnd), cwt->table[i], text, DIM(text));
      GetTextExtentPoint32(hdc, text, _tcslen(text), &sz);
      width = max(width, sz.cx + space.cx);
     } /* compute each width */

  // Compute the width of the caption

  LoadString(GetWindowInstance(hwnd), cwt->caption, text, DIM(text));
  GetTextExtentPoint32(hdc, text, _tcslen(text), &sz);
  width = max(width, sz.cx + 3 * space.cx);

  ReleaseDC(hwnd, hdc);
  return width;
}

/****************************************************************************
*                        mainFrame_ComputMessageWidths
* Inputs:
*       HWND hwnd: Window to use for DC required
* Result: void
*       
* Effect: 
*       Initializes the table of column widths
* Notes:
*	Must be called before the header is constructed
****************************************************************************/

void mainFrame_ComputeMessageWidths(HWND hwnd)
{
 int i;

 for(i = 0; i < DIM(column_widths); i++)
    { /* initialize each row */
     column_widths[i].width = compute_width(hwnd, &column_widths[i]);
    } /* initialize each row */

 // now compute the starting position

 column_widths[0].start = 0;
 for(i = 1; i < DIM(column_widths); i++)
    { /* compute start */
     column_widths[i].start = column_widths[i-1].start + 
     						column_widths[i-1].width;
    } /* compute start */

 // and adjust the start position of the first column

 column_widths[0].start = COLUMN_MESSAGE_INDENT;
}

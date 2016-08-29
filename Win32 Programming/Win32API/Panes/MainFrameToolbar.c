#include "StdSDK.h"             // Standard application includes
#include "MainFrameToolbar.h"   // For non-static function prototypes
#include "resource.h"           // For resource identifiers

#include <tchar.h>              // For _tcschar

//
// Function prototypes for static functions
//


//
// Function prototypes for callback functions
//


//
// Toolbar button map entry
//

typedef struct _tagButtonMapEntry {
    UINT  nID ;               // Command identifier
    UINT  nIndex ;            // Bitmap index
} BUTTONMAPENTRY ;

//
// Buttons appearing initially on the toolbar
//

static BUTTONMAPENTRY ButtonMap [] =
{
  { ID_FILE_NEW,            STD_FILENEW    },
  { ID_FILE_OPEN,           STD_FILEOPEN   },
  { ID_FILE_SAVE,           STD_FILESAVE   },
  { 0,                      0              },
  { ID_EDIT_CUT,            STD_CUT        },
  { ID_EDIT_COPY,           STD_COPY       },
  { ID_EDIT_PASTE,          STD_PASTE      },
  { 0,                      0              },
  { ID_FILE_PRINT,          STD_PRINT      },
  { ID_APP_ABOUT,           STD_HELP       }
} ;

//
// Buttons optionally appearing on the toolbar
//

static BUTTONMAPENTRY ButtonMapExtras [] =
{
  { ID_EDIT_UNDO,           STD_UNDO       },
  { ID_EDIT_REDO,           STD_REDOW       },
  { ID_EDIT_CLEAR,          STD_DELETE     },
  { ID_FILE_PRINT_PREVIEW,  STD_PRINTPRE   },
  { ID_EDIT_PROPERTIES,     STD_PROPERTIES },
  { ID_EDIT_FIND,           STD_FIND       },
  { ID_EDIT_REPLACE,        STD_REPLACE    }
} ;


//
//  HWND
//  mainFrame_CreateToolbar (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Create a toolbar for the main frame window
//
//  COMMENTS::
//
//

HWND
mainFrame_CreateToolbar (HWND hwnd)
{
    HMENU     hmenu ;
    HWND      hwndToolbar ;
    HINSTANCE hinst ;
    int       i;
    int       nButtons;
    int       nImages ;
    TBBUTTON  tbb [DIM (ButtonMap)] ;

    // Get the application's instance handle
    hinst = GetWindowInstance (hwnd) ;

    // Get menu for main frame window
    hmenu = GetMenu (hwnd) ;

    // Set number of buttons to display
    nButtons = DIM (ButtonMap) ;

    // Set number of images in the bitmap
    nImages  = STD_PRINT + 1 ;

    // Initialize the toolbar button array
    ZeroMemory (tbb, sizeof (tbb)) ;

    for (i = 0; i < nButtons; i++) {
        // When the command identifer is zero,
        //  place a separator in this position of the toolbar
        if (0 == ButtonMap [i].nID) {
            tbb [i].fsStyle = TBSTYLE_SEP ;
            // tbb [i].iBitmap is the separator width on
            // TBSTYLE_SEP entries. It is already zero
            // which means "use a reasonable width."
            continue ;
        }

        tbb [i].idCommand = ButtonMap [i].nID ;     // Command identifier
        tbb [i].iBitmap   = ButtonMap [i].nIndex ;  // Bitmap index
        tbb [i].fsState   = TBSTATE_ENABLED ;
        tbb [i].fsStyle   = TBSTYLE_BUTTON ;
    }

    // Create the toolbar control
    hwndToolbar = CreateToolbarEx (
        hwnd,                  // Parent window handle
        TBSTYLE_TOOLTIPS |     // Toolbar window styles
        TBSTYLE_WRAPABLE |
        CCS_ADJUSTABLE,
        IDC_TOOLBAR,           // Control ID
        nImages,               // Number of images in the bitmap
        HINST_COMMCTRL,        // Module containing bitmap
        IDB_STD_LARGE_COLOR,   // Resource ID of bitmap
        tbb,                   // TBBUTTON structs about buttons
        nButtons,              // # of buttons to add
        0,                     // Width of button (default is 24)
        0,                     // Height of button (default is 22)
        0,                     // Width of bitmap (default is 16)
        0,                     // Height of bitmap (default is 15)
        sizeof (TBBUTTON));    // Size of TBBUTTON struct

    toolbar_UpdateUI (hwndToolbar) ;    // Sync toolbar with menu state
    ShowWindow (hwndToolbar, SW_SHOW) ;

    return hwndToolbar ;
}

//
//  void toolbar_OnNotify (HWND hwnd, int idCtrl, NMHDR FAR* pnmhdr)
//
//  hwnd            Handle of window to which this message applies
//  idCtrl          Identifier of the control sending the message
//  pnmhdr          Pointer to structure that contains the notification
//                  code and additional information
//
//  PURPOSE:        Handle all notifications associated with the toolbar
//
//  COMMENTS:
//

BOOL
toolbar_OnNotify (HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    switch (pnmhdr->code) {
        case TTN_NEEDTEXT:
            // Tool tip control needs the tip text
            toolbar_OnNeedText (hwnd, idFrom, (LPTOOLTIPTEXT) pnmhdr) ;
            return TRUE ;

        case TBN_QUERYINSERT:
        case TBN_QUERYDELETE:
            // Returns TRUE to allow the button to be inserted or deleted
            return TRUE ;

        case TBN_GETBUTTONINFO:
            // The user double-clicked on the toolbar
            // which displayed the toolbar customization dialog box.

            // Provide the dialog box with button information
            // about buttons which can be added
            
            {
            LPTBNOTIFY pTbNotify = (LPTBNOTIFY) pnmhdr ;

            // As long as we have new buttons to describe...
            if (pTbNotify->iItem < DIM (ButtonMapExtras)) {
                // Describe another button...
                ZeroMemory (&pTbNotify->tbButton, sizeof (pTbNotify->tbButton)) ;
                pTbNotify->tbButton.iBitmap   = ButtonMapExtras [pTbNotify->iItem].nIndex ;
                pTbNotify->tbButton.idCommand = ButtonMapExtras [pTbNotify->iItem].nID ;
                pTbNotify->tbButton.fsState   = TBSTATE_ENABLED ;
                pTbNotify->tbButton.fsStyle   = TBSTYLE_BUTTON ;
                pTbNotify->cchText            = 0 ;
                return TRUE;
            }
            }
            return FALSE ;
    }

    // Unrecognized toolbar notification
    // Let some subsequent handle process the notification
    return FALSE ;
}


//
//  void
//  toolbar_OnNeedText (HWND hwnd, int idCtrl, LPTOOLTIPTEXT pttt)
//
//  hwnd            Handle of frame window owning toolbar
//  idCtrl          Control identifier of toolbar window
//  pttt            Pointer to tool tip text structure
//
//  PURPOSE:        Return the requested tool tip text
//
//  COMMENTS::
//
//

void toolbar_OnNeedText (HWND hwnd, int idCtrl, LPTOOLTIPTEXT pttt)
{
    HINSTANCE hinst ;
    TCHAR     Prompt [256] ;

    ASSERT (0 == (pttt->uFlags & TTF_IDISHWND)) ;

    // Get instance handle for string resources
    hinst = GetWindowInstance (hwnd) ;

    // Load the string resource with the button ID
    LoadString (hinst, pttt->hdr.idFrom, Prompt, DIM (Prompt)) ;

    // The tool tip is the part after the newline character
    pttt->lpszText = _tcschr (Prompt, _T('\n')) ;

    // If tool tip text found, skip over the newline character
    if (pttt->lpszText)
        pttt->lpszText++ ;

    pttt->hinst    = NULL ;
}


//
//  void
//  toolbar_UpdateUI (HWND hwnd)
//
//  hwnd            Handle of toolbar window
//
//  PURPOSE:        For each button on the toolbar,
//                  enable the button if there is an enabled menu item
//                  with the same command identifier as the button.
//                  Disable the button if there is a disabled menu item
//                  with the same command identifier as the button.
//                  Otherwise, leave the button state unmodified.
//
//  COMMENTS::
//
//

void
toolbar_UpdateUI (HWND hwnd)
{
    HWND  hwndFrame ;
    HMENU hmenu ;
    int   Count ;

    // Get frame window (parent of toolbar)
    hwndFrame = GetParent (hwnd) ;
    ASSERT (NULL != hwndFrame) ;

    // Get the main frame window's menu
    hmenu = GetMenu (hwndFrame) ;
    // This line has to be commented out for the Panes example because the
    // second overlapped window has no menu
    // ASSERT (NULL != hmenu) ;

    if (NULL == hmenu)
        return ;

    // Get count of buttons on the toolbar
    Count = Toolbar_ButtonCount (hwnd) ;

    // For each button on the toolbar...
    while (Count > 0) {
        BOOL     MenuEnabled, ButtonEnabled ;
        TBBUTTON tbb ;
        UINT     uiState ;

        // Get information about the button
        VERIFY (Toolbar_GetButton (hwnd, --Count, &tbb)) ;

        // If the button is a separator, ignore it
        if (tbb.fsStyle & TBSTYLE_SEP)
            continue ;

        // Get information about the corresponding menu item, if any
        uiState = GetMenuState (hmenu, tbb.idCommand, MF_BYCOMMAND) ;
        if (0xFFFFFFFF == uiState)
            continue ;                      // No menu item with this ID

        MenuEnabled   = 0 == (uiState & (MF_DISABLED | MF_GRAYED)) ;
        ButtonEnabled = 0 != (tbb.fsState & TBSTATE_ENABLED) ;

        // If button and menu are in the same state,
        //  we need do nothing for this button
        if (MenuEnabled == ButtonEnabled)
            continue ;

        // Enable/Disable this toolbar button
        VERIFY (Toolbar_EnableButton (hwnd, tbb.idCommand, MenuEnabled)) ;
    }
}


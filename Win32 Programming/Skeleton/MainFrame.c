#include "StdSDK.h"             // Standard application includes
#include "About.h"              // For non-static function prototypes
#include "resource.h"           // For resource identifiers

//
// Function prototypes for static functions
//


//
// Function prototypes for callback functions
//

LRESULT CALLBACK mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;


//
// Function prototypes for message handlers
//

static void mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;
static void mainFrame_OnDestroy (HWND hwnd) ;


//
// Typedefs
//


//
//  LRESULT CALLBACK
//  mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  hwnd            Handle of window to which this message applies
//  message         Message number
//  wParam          Message parameter
//  lParam          Message parameter
//
//  PURPOSE:  Processes messages for the main window.
//
//  MESSAGES:
//
//      WM_COMMAND          - notification from the menu or controls
//      WM_DESTROY          - window is being destroyed
//

LRESULT CALLBACK
mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
        case WM_COMMAND:                    // Notification from menu or control
            return HANDLE_WM_COMMAND (hwnd, wParam, lParam, mainFrame_OnCommand) ;

        case WM_DESTROY:                    // Window is being destroyed
            return HANDLE_WM_DESTROY (hwnd, wParam, lParam, mainFrame_OnDestroy) ;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam) ;
        }
}


//
//  void mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
//
//  hwnd            Handle of window to which this message applies
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.                  
//
//  PURPOSE:        
//
//  COMMENTS:
//

static void
mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {

        case ID_APP_ABOUT:
            doAbout(hwnd);
            return ;

        case ID_APP_EXIT:
            DestroyWindow (hwnd) ;
            return ;

        default:
            FORWARD_WM_COMMAND (hwnd, id, hwndCtl, codeNotify, DefWindowProc) ;
        }
}


//
//  void mainFrame_OnDestroy (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Notification that the specified window is being destroyed.
//                  The window is no longer visible to the user.
//
//  COMMENTS:
//

static void
mainFrame_OnDestroy (HWND hwnd)
{
    // Tell WinHelp we don't need it any more...
    WinHelp (hwnd, getHelpFileName (), HELP_QUIT, (DWORD) 0) ;
    PostQuitMessage (0) ;
}

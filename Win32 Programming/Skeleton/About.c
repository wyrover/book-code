#include "StdSDK.h"         // Standard application includes
#include "About.h"          // For non-static function prototypes
#include "resource.h"       // For resource identifiers

//
// Function prototypes for static functions
//

//
// Function prototypes for callback functions
//

//
// Function prototypes for message handlers
//

static void aboutDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;
static BOOL aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam) ;

//
// Typedefs
//

//
//  BOOL CALLBACK
//  aboutDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  PURPOSE:  
// 		Dialog function for the About dialog box.
// 		Display version information for the application,
//      the operating system, and the processor.
//
//  MESSAGES:
//
//
//

BOOL CALLBACK
aboutDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
        case WM_COMMAND:                    // Notification from a control
			return HANDLE_WM_COMMAND (hwnd, wParam, lParam, aboutDlg_OnCommand) ;

        case WM_INITDIALOG:                 // Initialization of controls complete
            return HANDLE_WM_INITDIALOG (hwnd, wParam, lParam, aboutDlg_OnInitDialog) ;
	}
    return FALSE ;
}


//
//  void AboutDlg_OnCommand (HWND hwnd, int id, hwnd hwndCtl, UINT codeNotify)
//
//  hwnd            Window handle for the dialog box window
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.
//                  
//
//  PURPOSE:        
//                  Handle the keyboard and control notifications.
//                  An OK button press, or Enter/Esc keypress
//                  all dismiss the About dialog box.
//                  
//
//  COMMENTS:
//

static
void aboutDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
        case IDOK:                          // OK pushbutton/Enter keypress
        case IDCANCEL:                      // Esc keypress
            EndDialog (hwnd, TRUE) ;        // Dismiss the about dialog box
			break ;
	}
}

//
//  void aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
//
//  hwnd            Window handle for the dialog box window
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.
//                  
//
//  PURPOSE:        
//                  
//                  
//
//  COMMENTS:
//

static BOOL
aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE ;
}

void doAbout(HWND hwnd)
{
 HINSTANCE hinst = GetWindowInstance (hwnd) ;
 DialogBox (hinst, MAKEINTRESOURCE (IDD_ABOUTBOX), hwnd, 
                                                     (DLGPROC) aboutDlgProc) ;
}

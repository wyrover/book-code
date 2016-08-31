#include "stdSDK.h"
#include "resource.h"
#include "options.h"

static int lastWindow = IDC_GRAPHIC;


/****************************************************************************
*                               enableControls
* Inputs:
*       HWND hdlg: Dialog reference
* Result: void
*       
* Effect: 
*       Enables controls based on other controls
****************************************************************************/

static void
enableControls(HWND hdlg)
    {
     EnableWindow(GetDlgItem(hdlg, IDC_CUSTOMOPTIONS), 
                        IsDlgButtonChecked(hdlg, IDC_CUSTOM));
    }

/****************************************************************************
*                              new_OnInitDialog
* Inputs:
*       HWND hdlg: Dialog reference
*       HWND hwndFocus: Focus window
*       LPARAM lParam: value passed in as parameter to DialogBoxParam, which
*                       is the id of the last window selected
* Result: BOOL
*       TRUE, always (doesn't mess with focus...)
* Effect: 
*       Initializes controls
****************************************************************************/

static BOOL
new_OnInitDialog(HWND hdlg, HWND hwndFocus, LPARAM lParam)
    {
     SetWindowLong(hdlg, GWL_USERDATA, (LPARAM)&CustomOptions );
     CheckRadioButton(hdlg, IDC_GRAPHIC, IDC_CUSTOM, lParam);
     enableControls(hdlg);
     return TRUE;
    }

/****************************************************************************
*                                new_OnCommand
* Inputs:
*       HWND hdlg: Dialog reference
*       int id: Control ID
*       HWND hctl: Control handle
*       UINT codeNotify: Notification code
* Result: void
*       
* Effect: 
*       Processes command messages
****************************************************************************/

static void
new_OnCommand(HWND hdlg, int id, HWND hctl, UINT codeNotify)
    {
     int type;

     switch(id)
        { /* id */
         case IDCANCEL:
                 EndDialog(hdlg, IDCANCEL);
                 return;
         case IDOK:
                 type = IDCANCEL; // just for safety...

                 if(IsDlgButtonChecked(hdlg, IDC_GRAPHIC))
                    type = IDC_GRAPHIC;
                 if(IsDlgButtonChecked(hdlg, IDC_TEXT))
                    type = IDC_TEXT;
                 if(IsDlgButtonChecked(hdlg, IDC_CUSTOM))
                    type = IDC_CUSTOM;

                 EndDialog(hdlg, type);
                 return;
         case IDC_GRAPHIC:
                 enableControls(hdlg);
                 return;
         case IDC_TEXT:
                 enableControls(hdlg);
                 return;
         case IDC_CUSTOM:
                 enableControls(hdlg);
                 return;
         case IDC_CUSTOMOPTIONS:
                 setOptions(hdlg, (LPoptions)GetWindowLong(hdlg, GWL_USERDATA));
                 return;
        } /* id */
    }

/****************************************************************************
*                                 NewDlgProc
* Inputs:
*       HWND hdlg:
*       int message:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: BOOL
*       TRUE if processed message
*       FALSE if ignored it
* Effect: 
*       Handles 'New' dialog
****************************************************************************/

BOOL CALLBACK NewDlgProc(HWND hdlg, int message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
         case WM_INITDIALOG:
                 return (BOOL)HANDLE_WM_INITDIALOG(hdlg, wParam, lParam,
                                new_OnInitDialog);
         case WM_COMMAND:
                 HANDLE_WM_COMMAND(hdlg, wParam, lParam, new_OnCommand);
                 return TRUE ;
        } /* message */
     return FALSE;
    }

/****************************************************************************
*                                  whichNew
* Inputs:
*       HWND hwnd: Parent window for dialog
* Result: int
*       IDCANCEL: no window creation
*       IDC_GRAPHIC: create graphic window
*       IDC_TEXT: create text window
*       IDC_CUSTOM: create custom window
* Effect: 
*       Prompts user for window type.  If confirmed, sets that as the
*       default for the next time it prompts
****************************************************************************/

int whichNew(HWND hwnd)
    {
     int result = DialogBoxParam(GetWindowInstance(hwnd),
                                 MAKEINTRESOURCE(IDD_NEW),
                                 hwnd,
                                 (DLGPROC)NewDlgProc,
                                 lastWindow);
     if(result != IDCANCEL)
        lastWindow = result;

     return result;
    }

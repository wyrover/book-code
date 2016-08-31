#include "stdSDK.h"
#include "resource.h"
#include "options.h"
#include <tchar.h>

options CustomOptions;

typedef struct { int id; DWORD styles; int abbrev; } stylemap;

static stylemap map[] = {
        {IDC_WS_HSCROLL,        WS_HSCROLL,     IDS_HS},
        {IDC_WS_VSCROLL,        WS_VSCROLL,     IDS_VS},
        {IDC_ES_AUTOHSCROLL,    ES_AUTOHSCROLL, IDS_AH},
        {IDC_ES_AUTOVSCROLL,    ES_AUTOVSCROLL, IDS_AV},
        {IDC_ES_CENTER,         ES_CENTER,      IDS_TC},
        {IDC_ES_LEFT,           ES_LEFT,        IDS_TL},
        {IDC_ES_LOWERCASE,      ES_LOWERCASE,   IDS_LC},
        {IDC_ES_MULTILINE,      ES_MULTILINE,   IDS_ML},
        {IDC_ES_NOHIDESEL,      ES_NOHIDESEL,   IDS_NH},
        {IDC_ES_NUMBER,         ES_NUMBER,      IDS_NM},
        {IDC_ES_OEMCONVERT,     ES_OEMCONVERT,  IDS_OEM},
        {IDC_ES_PASSWORD,       ES_PASSWORD,    IDS_PW},
        {IDC_ES_READONLY,       ES_READONLY,    IDS_RO},
        {IDC_ES_RIGHT,          ES_RIGHT,       IDS_TR},
        {IDC_ES_UPPERCASE,      ES_UPPERCASE,   IDS_UC},
        {IDC_ES_WANTRETURN,     ES_WANTRETURN,  IDS_WR},
        {0,                     0 } // EOT
                        };


static void options_OnMultiline(HWND hdlg);

/****************************************************************************
*                                styleToTitle
* Inputs:
*       HWND hwnd: Window handle
*       DWORD styles: Style flags
*       LPCTSTR suffix: Suffix string to add, or NULL
* Result: void
*       
* Effect: 
*       Appends the style codes to the window title
****************************************************************************/

void styleToTitle(HWND hwnd, DWORD styles, LPCTSTR suffix)
    {
     int i;
     TCHAR title[256];
     TCHAR text[256];
     LPTSTR sep = _T("");

     lstrcpy(text,_T(""));
     for(i = 0; map[i].id != 0; i++)
        { /* scan styles */
         if(styles & map[i].styles)
            { /* has style */
             TCHAR t[20];
             LoadString(GetWindowInstance(hwnd), map[i].abbrev, t, DIM(t));
             lstrcat(text, sep);
             lstrcat(text, t);
             sep = _T(", ");
            } /* has style */
        } /* scan styles */
     GetWindowText(hwnd, title, DIM(title));
     lstrcat(title, text);
     if(suffix != NULL && lstrlen(suffix) > 0)
        { /* add suffix */
         lstrcat(title, sep);
         lstrcat(title, suffix);
        } /* add suffix */
     SetWindowText(hwnd, title);
    }

/****************************************************************************
*                               styleToControls
* Inputs:
*       HWND hdlg: Dialog reference
* Result: void
*       
* Effect: 
*       Using the options data stored in the dialog, sets the controls to
*       reflect the style bits
****************************************************************************/

void styleToControls(HWND hdlg)
    {
     LPoptions opts = (LPoptions)GetWindowLong(hdlg, GWL_USERDATA);
     int i;

     for(i = 0; map[i].id != 0; i++)
        { /* scan table */
         CheckDlgButton(hdlg, map[i].id, 
                        (map[i].styles & opts->styles ? BST_CHECKED
                                                      : BST_UNCHECKED));
        } /* scan table */
    }

/****************************************************************************
*                              controlsToStyles
* Inputs:
*       HWND hdlg: Dialog handle
* Result: void
*       
* Effect: 
*       Modifies the styles bits to reflect the selections in the controls
****************************************************************************/

void controlsToStyles(HWND hdlg)
    {
     LPoptions opts = (LPoptions)GetWindowLong(hdlg, GWL_USERDATA);
     int i;

     for(i = 0; map[i].id != 0; i++)
        { /* scan table */
         if(IsDlgButtonChecked(hdlg, map[i].id))
            opts->styles |= map[i].styles;
         else
            opts->styles &= ~map[i].styles;
        } /* scan table */
    }

/****************************************************************************
*                            options_OnInitDialog
* Inputs:
*       HWND hdlg: Dialog reference
* Result: BOOL
*       TRUE, always (doesn't change focus)
* Effect: 
*       Initializes the controls
****************************************************************************/

BOOL options_OnInitDialog(HWND hdlg, HWND hfocus, LPARAM lParam)
    {
     LPoptions opts = (LPoptions)lParam;
     OSVERSIONINFO info;

     info.dwOSVersionInfoSize = sizeof(info);
     GetVersionEx(&info);
     EnableWindow(GetDlgItem(hdlg, IDC_ES_NUMBER), 
                (info.dwMajorVersion >= 4));

     SetWindowLong(hdlg, GWL_USERDATA, lParam);

     styleToControls(hdlg);
     options_OnMultiline(hdlg); // make sure correct controls are enabled

     CheckDlgButton(hdlg, IDC_SUBCLASS, opts->subclass);
     CheckDlgButton(hdlg, IDC_CTLCOLOR, opts->docolor);

     return TRUE;
    }

/****************************************************************************
*                              options_OnCancel
* Inputs:
*       HWND hdlg: Dialog handle
* Result: void
*       
* Effect: 
*       Terminates dialog with IDCANCEL
****************************************************************************/

void options_OnCancel(HWND hdlg)
    {
     EndDialog(hdlg, IDCANCEL);
    }

/****************************************************************************
*                                options_OnOK
* Inputs:
*       HWND hdlg: Dialog handle
* Result: void
*       
* Effect: 
*       Terminates dialog, storing control values in options block
****************************************************************************/

void options_OnOK(HWND hdlg)
    {
     LPoptions opts = (LPoptions)GetWindowLong(hdlg, GWL_USERDATA);

     controlsToStyles(hdlg);

     opts->subclass = IsDlgButtonChecked(hdlg, IDC_SUBCLASS);
     opts->docolor  = IsDlgButtonChecked(hdlg, IDC_CTLCOLOR);

     EndDialog(hdlg, IDOK);
     
    }

/****************************************************************************
*                               options_OnLeft
* Inputs:
*       HWND hdlg: Dialog reference
* Result: void
*       
* Effect: 
*       Turns off center and right buttons
****************************************************************************/

void options_OnLeft(HWND hdlg)
    {
     if(IsDlgButtonChecked(hdlg, IDC_ES_LEFT))
        { /* left */
         CheckDlgButton(hdlg, IDC_ES_CENTER, BST_UNCHECKED);
         CheckDlgButton(hdlg, IDC_ES_RIGHT,  BST_UNCHECKED);
        } /* left */
    }

/****************************************************************************
*                               options_OnRight
* Inputs:
*       HWND hdlg: Dialgo reference
* Result: void
*       
* Effect: 
*       Turns off center and left options
****************************************************************************/

void options_OnRight(HWND hdlg)
    {
     if(IsDlgButtonChecked(hdlg, IDC_ES_RIGHT))
        { /* right */
         CheckDlgButton(hdlg, IDC_ES_CENTER, BST_UNCHECKED);
         CheckDlgButton(hdlg, IDC_ES_LEFT,  BST_UNCHECKED);
        } /* right */
    }

/****************************************************************************
*                              options_OnCenter
* Inputs:
*       HWND hdlg: Dialog reference
* Result: void
*       
* Effect: 
*       Turns off left and center otions
****************************************************************************/

void options_OnCenter(HWND hdlg)
    {
     if(IsDlgButtonChecked(hdlg, IDC_ES_CENTER))
        { /* left */
         CheckDlgButton(hdlg, IDC_ES_LEFT, BST_UNCHECKED);
         CheckDlgButton(hdlg, IDC_ES_RIGHT,  BST_UNCHECKED);
        } /* left */
    }

/****************************************************************************
*                             options_OnMultiline
* Inputs:
*       HWND hdlg: Dialog reference
* Result: void
*       
* Effect: 
*       Enables/disables the justification options, which only apply to
*       multiline controls
****************************************************************************/

static void options_OnMultiline(HWND hdlg)
    {
     BOOL multi = IsDlgButtonChecked(hdlg, IDC_ES_MULTILINE);

     EnableWindow(GetDlgItem(hdlg, IDC_ES_CENTER), multi);
     EnableWindow(GetDlgItem(hdlg, IDC_ES_RIGHT), multi);
     EnableWindow(GetDlgItem(hdlg, IDC_ES_LEFT), multi);

    }

/****************************************************************************
*                              options_OnCommand
* Inputs:
*       HWND hdlg: Dialog handle
*       int id: Control ID
*       HWND hctl: Control handle
*       UINT codeNotify: notification code
* Result: void
*       
* Effect: 
*       Command dispatcher
****************************************************************************/

void options_OnCommand(HWND hdlg, int id, HWND hctl, UINT codeNotify)
    {
     switch(id)
        { /* id */
         case IDOK:
                 options_OnOK(hdlg);
                 return;
         case IDCANCEL:
                 options_OnCancel(hdlg);
                 return;
         case IDC_ES_LEFT:
                 options_OnLeft(hdlg);
                 return;
         case IDC_ES_RIGHT:
                 options_OnRight(hdlg);
                 return;
         case IDC_ES_CENTER:
                 options_OnCenter(hdlg);
                 return;
         case IDC_ES_MULTILINE:
                 options_OnMultiline(hdlg);
                 return;
        } /* id */
    }

/****************************************************************************
*                               optionsDlgProc
* Inputs:
*       HWND hdlg: Dialog handle
*       int message: message id
*       WPARAM wParam:
*       LPARAM lParam:
* Result: BOOL
*       TRUE if message handled
*       FALSE if message not handled
* Effect: 
*       Dialog box handler
****************************************************************************/

static BOOL CALLBACK 
optionsDlgProc(HWND hdlg, int message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
         case WM_INITDIALOG:
                 return (BOOL)HANDLE_WM_INITDIALOG(hdlg, wParam, lParam, 
                                                   options_OnInitDialog);
         case WM_COMMAND:
                 HANDLE_WM_COMMAND(hdlg, wParam, lParam, options_OnCommand);
                 return TRUE;
        } /* message */
     return FALSE;
    }

/****************************************************************************
*                                 setOptions
* Inputs:
*       HWND hwnd: Parent window
*       LPoptions opts: Current option set
* Result: int
*       IDOK, IDCANCEL
* Effect: 
*       Pops up options dialog.  If result is IDOK, 'opts' will be modified
****************************************************************************/

int setOptions(HWND hwnd, LPoptions opts)
    {
     return DialogBoxParam(GetWindowInstance(hwnd),
                                 MAKEINTRESOURCE(IDD_OPTIONS),
                                 hwnd,
                                 (DLGPROC)optionsDlgProc,
                                 (LPARAM)opts);
    }

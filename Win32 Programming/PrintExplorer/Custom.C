#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "printit.h"
#include "resource.h"
#include "custom.h"


/****************************************************************************
*                              custom_OnCommand
* Inputs:
*       HWND hdlg:
*	int id:
*	HWND hctl:
*	UINT codeNotify:
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

static void custom_OnCommand(HWND hdlg, int id, HWND hctl, UINT codeNotify)
    {
     LPPrintIt prt = (LPPrintIt)GetWindowLong(hdlg, GWL_USERDATA);
     if(id == IDCANCEL)
        { /* aborted */
	 if(prt != NULL)
	    prt->Aborted = TRUE;
	} /* aborted */
    }

/****************************************************************************
*                              void centerWindow
* Inputs:
*       HWND hwnd: Window to be centered on screen
* Result: void
*       
* Effect: 
*       Centers the window on the screen
****************************************************************************/

static void centerWindow(HWND hwnd)
    {
     RECT r;
     POINT pt;
     GetWindowRect(hwnd, &r);
     pt.x = (GetSystemMetrics(SM_CXSCREEN) - (r.right - r.left)) / 2;
     pt.y = (GetSystemMetrics(SM_CYSCREEN) - (r.bottom - r.top)) / 2;
     SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

/****************************************************************************
*                             custom_OnInitDialog
* Inputs:
*       HWND hdlg: Dialog handle
*	HWND hfocus: Window that has the focus (ignored)
*	LPARAM lParam: LPPrintit reference
* Result: BOOL
*       TRUE, always
* Effect: 
*       Initializes the custom dialog box and saves the structure reference
*	in the GWL_USERDATA field
****************************************************************************/

static BOOL custom_OnInitDialog(HWND hdlg, HWND hfocus, LPARAM lParam)
    {
     LPDEVNAMES dn;
     LPPrintIt prt = (LPPrintIt)lParam;

     SetWindowLong(hdlg, GWL_USERDATA, lParam);

     prt->Aborted = FALSE;
     centerWindow(hdlg);

     dn = (LPDEVNAMES)GlobalLock(prt->prdata.pd->hDevNames);
     if(dn != NULL)
        { /* locked */
	 SetDlgItemText(hdlg, IDC_PRINTER, 
     		(LPCTSTR)&((LPCSTR)dn)[dn->wDeviceOffset]);

	 GlobalUnlock(prt->prdata.pd->hDevNames);
	} /* locked */

     return TRUE;
    }

/****************************************************************************
*                            custom_OnPageNotify
* Inputs:
*       HWND hdlg: Dialog reference
*	int page: Page number
* Result: void
*       
* Effect: 
*       Updates the page number in the dialog display
****************************************************************************/

static void custom_OnPageNotify(HWND hdlg, int page)
    {
     TCHAR fmt[256];
     TCHAR msg[256];
     if(LoadString(GetWindowInstance(hdlg), IDS_PRINTING_PAGE,
			fmt, sizeof(fmt) / sizeof(TCHAR)) == 0)
         return;
     wsprintf(msg, fmt, page);
     SetDlgItemText(hdlg, IDC_PAGENO, msg);
    }

/****************************************************************************
*                                 CustomHook
* Inputs:
*       HWND hdlg: Dialog reference
*	
* Result: BOOL
*       TRUE if message handled
*	FALSE if not handled
* Effect: 
*       ²
****************************************************************************/

BOOL CALLBACK CustomHook(HWND hdlg, int msg, WPARAM wParam, LPARAM lParam)
    {
     static int NotificationMessage = 0;
     if(NotificationMessage == 0)
	NotificationMessage = RegisterWindowMessage(PRINTIT_PAGE_NOTIFICATION_MSG);
     if(msg == NotificationMessage)
        { /* page notification */
	 custom_OnPageNotify(hdlg, lParam);
	 return TRUE;
	} /* page notification */

     switch(msg)
        { /* msg */
	 case WM_COMMAND:
		 HANDLE_WM_COMMAND(hdlg, wParam, lParam, custom_OnCommand);
		 return TRUE;
	 case WM_INITDIALOG:
		 return (BOOL)HANDLE_WM_INITDIALOG(hdlg, wParam, lParam,
		 		custom_OnInitDialog);
	} /* msg */
     return FALSE;
    }

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <dlgs.h>
#include <tchar.h>
#include <commctrl.h>

#include "resource.h"


static BOOL handled;

/****************************************************************************
*                                 encodeYYMM
* Inputs:
*       WORD year: Year
*	WORD month: Month, 1..12
* Result: int
*       Encoded year/month
****************************************************************************/

int encodeYYMM(WORD year, WORD month)
    {
     return (year * 12) + (month - 1);
    }

/****************************************************************************
*                                  decodeYY
* Inputs:
*       int yymm: Encoded year and month
* Result: int
*       Year
****************************************************************************/

int decodeYY(int yymm)
    {
     return (yymm / 12);
    }

/****************************************************************************
*                                  decodeMM
* Inputs:
*       int yymm: Encoded yymm
* Result: int
*       Month, 1..12
****************************************************************************/

int decodeMM(int yymm)
    {
     return (yymm % 12) + 1;
    }

/****************************************************************************
*                                 setDateData
* Inputs:
*       HWND hwnd: Window handle
*	int ctl: ID of control to get data
*	int date: Date to convert
* Result: void
*       
* Effect: 
*       Converts the date, stored as YY * 12 + mm, to a printable date
* Notes:
*	Fully localized version would use fancier techniques
****************************************************************************/

static void setDateData(HWND hwnd, int ctl, int date)
    {
     TCHAR strdate[256];
     if(date == 0)
	lstrcpy(strdate,_T(""));
     else
	wsprintf(strdate, _T("%d/%02d"), decodeYY(date), decodeMM(date));

     SetDlgItemText(hwnd, ctl, strdate);
    }

/****************************************************************************
*                                getSpinValue
* Inputs:
*       HWND hwnd: Window handle
*	UINT id: Control id of spin control
* Result: int
*       Current value of control
****************************************************************************/

static int getSpinValue(HWND hwnd, UINT id)
    {
     return (int)LOWORD(SendDlgItemMessage(hwnd,id, UDM_GETPOS, 0, 0));
    }

/****************************************************************************
*                                 setDateSpin
* Inputs:
*       HWND hwnd: Window handle
*	int ctl: Spin control ID
*	int minval: Minimum value
*	int maxval: Maximum value
*	int val: Current value
* Result: void
*       
* Effect: 
*       Sets the limits for the date spin control
****************************************************************************/

static void setDateSpin(HWND hwnd, int ctl, int minval, int maxval, int val)
    {
     SendDlgItemMessage(hwnd, ctl, UDM_SETRANGE, 0, MAKELONG(maxval, minval));
     SendDlgItemMessage(hwnd, ctl, UDM_SETPOS, 0, val);
    }

/****************************************************************************
*                               printhook_OnOK
* Inputs:
*       HWND hwnd: Window handle for dialog
* Result: void
*       
* Notes: 
*       The common dialog must process the OK button, so we must indicate
*	by setting the 'handled' flag FALSE that we want the normal processing
*	to continue.  However, one of the effects of the normal OK processing
* 	is to copy the integer values out of the "From" and "To" page controls
*	(if page selection is chosen) and place them in the nFromPage and 
*	nToPage fields of the PRINTDLG structure.  Although we have not 
*	actually used these controls (they are invisible and disabled), the
*	PrintDlg handler insists that it is *their* values that will be stored
*	in the PRINTDLG structure.  We therefore have to copy the values from
*	our controls to the standard controls so that the OK handler in the
*	common dialogs will actually work correctly.
****************************************************************************/

static void printhook_OnOK(HWND hwnd)
    {
     SetDlgItemInt(hwnd, edt1, getSpinValue(hwnd, IDC_SPINSTART), FALSE);
     SetDlgItemInt(hwnd, edt2, getSpinValue(hwnd, IDC_SPINEND), FALSE);
     handled = FALSE;  // we want normal handling
     
    }

/****************************************************************************
*                               printhook_OnCommand
* Inputs:
*       HWND hwnd: Window handle for dialog
*	int id: Control ID
*	HWND hctl: Control handle
*	UINT codeNotify
* Result: void
*       Sets 'handled' Boolean to indicate state
* Effect: 
*       Handles the changes to the edit controls
****************************************************************************/

static void printhook_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     LPPRINTDLG pd = (LPPRINTDLG)GetWindowLong(hwnd, GWL_USERDATA);
     handled = FALSE;
     if(ctlFirst <= id && id <= ctlLast)
        { /* normal control */
	 return;
	} /* normal control */
     
     switch(id)
        { /* id */
	 case IDOK:
		 printhook_OnOK(hwnd);
		 break;
	} /* id */
    }

/****************************************************************************
*                               printhook_OnVScroll
* Inputs:
*       HWND hwnd: Parent window handle
*	HWND hctl: Control handle 
*	UINT code: SB_ codes
*	int pos: Current position
* Result: void
*       
* Effect: 
*       Sets the text control associated with the
****************************************************************************/

static void printhook_OnVScroll(HWND hwnd, HWND hctl, UINT code, int pos)
    {
     HWND buddy;
     int val;

     handled = FALSE;

     switch(GetDlgCtrlID(hctl))
        { /* which */
	 case IDC_SPINSTART:
	 case IDC_SPINEND:
		 buddy = (HWND)GetWindowLong(hctl, GWL_USERDATA);
		 break;
	 default:
		 return;
	} /* which */

     handled = TRUE;
     val = (int)LOWORD(SendMessage(hctl, UDM_GETPOS, 0, 0));

     setDateData(hwnd, GetDlgCtrlID(buddy), val);
    }

/****************************************************************************
*                                  setBuddy
* Inputs:
*       HWND spin: Window of spin control
*	HWND buddy: Window which is its buddy
* Result: void
*       
* Effect: 
*       Sets the buddy control
****************************************************************************/

static void setBuddy(HWND spin, HWND buddy)
    {
     SetWindowLong(spin, GWL_USERDATA, (LPARAM) buddy);
    }

/****************************************************************************
*                             printhook_OnInitDialog
* Inputs:
*       HWND hwnd: WIndow handle
*	HWND hfocus: Control with focus
*	LPARAM lParam: LPPRINTDLG
* Result: BOOL
*       TRUE, always (no change in focus)
* Effect: 
*       Loads up custom edit controls with date info
****************************************************************************/

static BOOL printhook_OnInitDialog(HWND hwnd, HWND hfocus, LPARAM lParam)
    {
     LPPRINTDLG pd = (LPPRINTDLG)lParam;

     SetWindowLong(hwnd, GWL_USERDATA, lParam);

     setBuddy(GetDlgItem(hwnd, IDC_SPINSTART), GetDlgItem(hwnd, IDC_STARTMONTH));
     setBuddy(GetDlgItem(hwnd, IDC_SPINEND), GetDlgItem(hwnd, IDC_ENDMONTH));

     setDateData(hwnd, IDC_STARTMONTH, pd->nFromPage);
     setDateData(hwnd, IDC_ENDMONTH,   pd->nToPage);
     setDateSpin(hwnd, IDC_SPINSTART, pd->nMinPage, pd->nMaxPage, pd->nFromPage);
     setDateSpin(hwnd, IDC_SPINEND,   pd->nMinPage, pd->nMaxPage, pd->nToPage);

     return TRUE;
    }


/****************************************************************************
*                                  printHook
* Inputs:
*       ²
* Result: UINT (BOOL, actually)
*       TRUE if we have processed message
*	FALSE if we have not processed message
* Effect: 
*       Intercepts the messages sent to the print dialog
****************************************************************************/

UINT CALLBACK printHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* msg */
	 case WM_INITDIALOG:
		 return (BOOL)HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, printhook_OnInitDialog);
	 case WM_COMMAND:
		 HANDLE_WM_COMMAND(hwnd, wParam, lParam, printhook_OnCommand);
		 return handled;
	 case WM_VSCROLL:
		 HANDLE_WM_VSCROLL(hwnd, wParam, lParam, printhook_OnVScroll);
		 return handled;
	} /* msg */
     return 0;
    }

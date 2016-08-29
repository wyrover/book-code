/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 12-Jan-96 | Created
* 15-Jan-96 | Fixed computation of centered heading to account for left margin
*  6-Feb-96 | Fixed bug where all conversions to metric used top.margin
*           | (cut-and-pasto)
*           | If the top position initially overlaps with the bottom margin,
*           | throw a PRINTERR_MARGINS exception.
*****************************************************************************/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "center.h"
#include "printit.h"
#include "printmc.h" // error codes

#define PRINTERR_STOP_REQUESTED 0x100 // randomly chosen value

// All static storage is process-local storage
// We could generalize this to thread-local if needed

static HINSTANCE hInst;

HWND hAbort;
BOOL Aborted;
BOOL PrinterError;
BOOL pageopen;
DWORD lasterror;
int PageNotificationMessage;

#define PH_LEFT   0
#define PH_CENTER 1
#define PH_RIGHT  2
#define PH_BOTTOM 0x10

#define throw(e) RaiseException( (e), EXCEPTION_NONCONTINUABLE, 0, 0)

/****************************************************************************
*                                   DLLMain
* Inputs:
*       HINSTANCE hinstDLL: Instance handle for this DLL
*	DWORD fdwReason: ignored
*	LPVOID lpReserved: not used
* Result: BOOL
*       TRUE, always
* Effect: 
*       Saves the instance handle so LoadString and CreateDialog can get the
*	necessary resources
****************************************************************************/

BOOL WINAPI DLLMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
    {
     hInst = hinstDLL; // save for future activities

     return TRUE;
    }


/****************************************************************************
*                                  abortFunc
* Inputs:
*       HDC hdc: The printer DC
*	int Code: An error code
* Result: BOOL
*       TRUE to continue printing
*	FALSE to abort printing
* Effect: 
*       Handles interactions with the modeless dialog boxes.
****************************************************************************/

BOOL CALLBACK abortFunc(HDC hdc, int Code)
    {
     MSG msg;

     while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        { /* peeked */
	 if(!hAbort || !IsDialogMessage(hAbort, &msg))
	    { /* dispatch */
	     TranslateMessage(&msg);
	     DispatchMessage(&msg);
	    } /* dispatch */
	} /* peeked */
     return !Aborted;
    }

/****************************************************************************
*                            abortdlg_OnInitDialog
* Inputs:
*       HWND hDlg: Dialog handle
*	HWND hwndctl: Control which gets first focus
*	LPARAM lParam: dialog creation data
* Result: BOOL
*       TRUE, always (no focus change)
* Effect: 
*       Initializes the dialog by putting the device and driver parameters
*	in the dialog box
****************************************************************************/

BOOL abortdlg_OnInitDialog(HWND hdlg, HWND hwndctl, LPARAM lParam)
    {
     LPDEVNAMES dn;
     LPPrintIt pr = (LPPrintIt)lParam;

     SetWindowLong(hdlg, GWL_USERDATA, lParam);

     Aborted = FALSE;
     PrinterError = FALSE;

     centerWindow(hdlg);

     dn = (LPDEVNAMES)GlobalLock(pr->prdata.pd->hDevNames);
     if(dn == NULL)

	 throw (PRINTERR_MEMLOCK_FAILED);

     if(IsWindow(GetDlgItem(hdlg, IDC_PRINTER)))
         SetDlgItemText(hdlg, IDC_PRINTER,
     		&((LPCTSTR)dn)[dn->wDeviceOffset]);
     if(IsWindow(GetDlgItem(hdlg, IDC_PORT)))
         SetDlgItemText(hdlg, IDC_PORT,
     		&((LPCTSTR)dn)[dn->wOutputOffset]);
     GlobalUnlock(pr->prdata.pd->hDevNames);
     return TRUE;
    }

/****************************************************************************
*                             abortdlg_OnCommand
* Inputs:
*       HWND hdlg: Dialog reference
*	int id: Control ID
*	HWND hctl: Control handle
*	UINT codeNotify: Notification code
* Result: void
*       
* Effect: 
*       Handles Cancel button
****************************************************************************/

static void abortdlg_OnCommand(HWND hdlg, int id, HWND hctl, UINT codeNotify)
    {
     switch(id)
        { /* id */
	 case IDCANCEL:
		 Aborted = TRUE;
		 return;
	} /* id */
    }

/****************************************************************************
*                           abortdlg_OnCurrentPage
* Inputs:
*       HWND hdlg: Dialog reference
*	int page: Page number
* Result: void
*       
* Effect: 
*       Sets the current page number in the dialog
****************************************************************************/

static void abortdlg_OnCurrentPage(HWND hdlg, int page)
    {
     TCHAR fmt[256];
     TCHAR msg[256];

     if(!IsWindow(GetDlgItem(hdlg, IDC_PAGE)))
	return;  // control is missing in custom dialog


     if(LoadString(GetWindowInstance(hdlg), IDS_PRINTING_PAGE, 
				fmt, sizeof(fmt) / sizeof(TCHAR)) == 0)
	return;
     wsprintf(msg, fmt, page);
     SetDlgItemText(hdlg, IDC_PAGE, msg);
    }

/****************************************************************************
*                                abortDlgFunc
* Inputs:
*       HWND hdlg: Dialog reference
*	int msg: Message ID
*	WPARAM wParam:
*	LPARAM lParam:
* Result: BOOL
*       TRUE if handled
*	FALSE if not handled
* Effect: 
*       The abort dialog handler.
****************************************************************************/

BOOL CALLBACK abortDlgFunc(HWND hdlg, int msg, WPARAM wParam, LPARAM lParam)
    {
     LPPrintIt prt = (LPPrintIt)GetWindowLong(hdlg, GWL_USERDATA);

     // Note that we won't handle any messages until after the 
     // WM_INITDIALOG is handled, which is why we do a special
     // test for it later

     if(prt != NULL && (prt->Flags & PrintIt_ENABLEHOOK) && prt->Hook != NULL)
        { /* call hook */
	 BOOL result;

	 // This is a bit tricky; if we are already aborted, we simply want
	 // to retain the value; if it changes to TRUE, we want the new
	 // value to be true; but we can never set it back to false via this
	 // call.  

	 prt->Aborted = Aborted; // make sure it is correct going in
	 result = CallWindowProc(prt->Hook, hdlg, msg, wParam, lParam);
	 Aborted |= prt->Aborted; // in case it is changed
	 if( result )
	    return TRUE;
	} /* call hook */

     if(msg == PageNotificationMessage)
        { /* current page */
	 abortdlg_OnCurrentPage(hdlg, lParam);
	 return TRUE;
	} /* current page */

     switch(msg)
        { /* msg */
	 case WM_COMMAND:
		 HANDLE_WM_COMMAND(hdlg, wParam, lParam, abortdlg_OnCommand);
		 return TRUE;
	 case WM_INITDIALOG:
		 {
		  LPPrintIt prt = (LPPrintIt)lParam;

		  // We call our initializer first.  We know that it
		  // always returns TRUE so we can ignore its result.
		  // We want to return the result of the user-defined hook
		  HANDLE_WM_INITDIALOG(hdlg, wParam, lParam,
		 			abortdlg_OnInitDialog);

		  if(prt != NULL && 
		     (prt->Flags & PrintIt_ENABLEHOOK) && 
		      prt->Hook != NULL)
		        { /* has hook */
			 return (BOOL)CallWindowProc(prt->Hook, hdlg, msg, wParam, lParam);
			} /* has hook */
		  return TRUE; // no hook, return TRUE
		 }
	} /* msg */
     return FALSE;
    }

/****************************************************************************
*                                 startPrint
* Inputs:
*       LPPrintIt pr: Print object
* Result: void
*       
* Effect: 
*       Initializes printing.  If any problem, throws an exception.
****************************************************************************/

static void startPrint(LPPrintIt pr)
    {
     DOCINFO di;
     HINSTANCE dlginst = hInst;
     LPCTSTR dlgname = MAKEINTRESOURCE(IDD_ABORT);

     di.cbSize = sizeof(DOCINFO);
     di.lpszDocName = pr->prdata.DocName;
     di.lpszOutput = NULL;

     if(pr->Flags & PrintIt_ENABLETEMPLATE)
        { /* use template */
	 dlgname = pr->Template;
	 dlginst = pr->hInst;
	} /* use template */

     hAbort = CreateDialogParam(dlginst, dlgname,
				  pr->prdata.Owner,
				  (DLGPROC)abortDlgFunc,
				  (LPARAM)pr);

     if(hAbort == NULL)
	throw (PRINTERR_ABORTDLG_FAILED); // printer error

     if(SetAbortProc(pr->prdata.pd->hDC, abortFunc) < 0)
	throw (PRINTERR_SETABORT_FAILED);

     ShowWindow(hAbort, SW_NORMAL);
     UpdateWindow(hAbort);

     if(pr->prdata.Owner != NULL)
	EnableWindow(pr->prdata.Owner, FALSE);

     if(StartDoc(pr->prdata.pd->hDC, &di) < 0)
	throw (PRINTERR_STARTDOC_FAILED); // printer error

     pageopen = FALSE;
    }

/****************************************************************************
*                                  doHeading
* Inputs:
*       LPPrintIt pr: PrintIt structure 
*	int ypos: Vertical position for heading
*	BOOL (*fn)(HDC hdc, int pageno, LPVOID data, LPTSTR buffer, int len):
*					 Function to call for string
*	UINT pos: PH_ code for positioning
* Result: int
*       Height of the heading printed, or 0 if no heading printed, expressed
*	in device units
* Effect: 
*       Positions the heading as indicated.  If the function is NULL, no
*	heading is printed.  If the function returns NULL, no heading
*	is printed.
****************************************************************************/

static int doHeading(LPPrintIt pr, int ypos, LPPrintitHeaderFunc fn, UINT which)
    {
     TCHAR line[256];
     HDC hdc = pr->prdata.pd->hDC;
     int savedDC;

     if(fn == NULL)
	return 0;

     savedDC = SaveDC(hdc);
     _try
        {
	 POINT pos;
	 SIZE sz;
	 int adjust;

	 sz.cx = 0;  // not needed, but why confuse LPtoDP
	 sz.cy = fn(&pr->prdata, line, sizeof(line) / sizeof(TCHAR));

         switch(sz.cy)
	    { /* fn val */
	     case PH_Stop:
		     throw (PRINTERR_STOP_REQUESTED);
	     case PH_Continue:
		     break;
	     default:
		     LPtoDP(hdc, (LPPOINT)&sz, 1);
		     return sz.cy;
	    } /* fn val */

	 // Get the text size in logical cooordinates

	 GetTextExtentPoint(hdc, line, lstrlen(line), &sz);
         
	 switch(which & ~PH_BOTTOM)
	    { /* which */
	     case PH_CENTER:
		     pos.x = pr->prdata.page.left + 
		            (pr->prdata.page.right - pr->prdata.page.left) / 2;
		     adjust = sz.cx / 2;
		     break;
	     case PH_RIGHT:
		     pos.x = pr->prdata.page.right;
		     adjust = sz.cx;
		     break;
	     default:
		     pos.x = pr->prdata.page.left;
		     adjust = 0;
		     break;
	    } /* which */

	 DPtoLP(hdc, &pos, 1);
	 pos.x -= adjust;
	 pos.y = ypos;	// vertical physical coordinate

	 LPtoDP(hdc, (LPPOINT)&sz, 1);
	 
	 // Handle the case of being on the bottom by computing the
	 // top of the text box (ypos will be the bottom of the text box
	 // for bottom line output)
	 if(which & PH_BOTTOM)
	    ypos -= sz.cy;

	 TextOut(hdc, pos.x, pos.y, line, lstrlen(line));
	 return sz.cy;
	}
     _finally
        { 
	 RestoreDC(hdc, savedDC);
	}
    }

/****************************************************************************
*                                 checkFilter
* Inputs:
*       int code: Exception code
* Result: int
*       EXCEPTION_EXECUTE_HANDLER if it is one of our printer exceptions
*	EXCEPTION_CONTINUE_SEARCH if it is not a recognized exception
****************************************************************************/

int checkFilter(int code)
    {
#define errmask ~0xE0000000
     if((code & errmask) > (PRINTERR_FIRST & errmask) &&
	(code & errmask) < (PRINTERR_LAST & errmask))
	return EXCEPTION_EXECUTE_HANDLER;

     return EXCEPTION_CONTINUE_SEARCH;
    }

/****************************************************************************
*                                   printLoop
* Inputs:
*       LPPrintIt pr: Pointer to a PrintIt data structure
* Result: void
*       
* Effect: 
*       Prints the sample text
****************************************************************************/

static void printLoop(LPPrintIt pr)
    {
     HDC hdc = pr->prdata.pd->hDC;
     TEXTMETRIC tm;
     int savedDC;
     RECT basemargins;  // margins we reset to on each new page
     BOOL needFinish = FALSE;
     int bottommargin;

     PageNotificationMessage = RegisterWindowMessage(PRINTIT_PAGE_NOTIFICATION_MSG);
     basemargins.right = GetDeviceCaps(hdc, HORZRES);
     basemargins.left = 0;
     basemargins.top = 0;
     basemargins.bottom = GetDeviceCaps(hdc, VERTRES);

     if(pr->psu != NULL)
        { /* has page setup */
	 RECT margin;
	 int hres = GetDeviceCaps(hdc, LOGPIXELSX);
	 int vres = GetDeviceCaps(hdc, LOGPIXELSY);

	 margin = pr->psu->rtMargin;

	 if(pr->psu->Flags & PSD_INHUNDREDTHSOFMILLIMETERS)
	    { /* mm */
	     margin.top = (margin.top * 1000) / 2540;
	     margin.bottom = (margin.bottom * 1000) / 2540;
	     margin.left = (margin.left * 1000) / 2540;
	     margin.right = (margin.right * 1000) / 2540;
	    } /* mm */

	 // Now normalize to device resolution
	 margin.top = (margin.top * vres) / 1000;
	 margin.bottom = (margin.bottom * vres) / 1000;
	 margin.left = (margin.left * hres) / 1000;
	 margin.right = (margin.right * hres) / 1000;

	 basemargins.top += margin.top;
	 basemargins.left += margin.left;
	 basemargins.right -= margin.right;
	 basemargins.bottom -= margin.bottom;
	} /* has page setup */

     GetTextMetrics(hdc, &tm);
     pr->prdata.dy = tm.tmHeight + tm.tmExternalLeading;
     pr->prdata.pageno = 0;

     _try 
       { /* try around loop */
        while(TRUE)
	   { /* main loop */
	    if(Aborted)
	       break;

	    if(!pageopen)
	       { /* open page */
		int hh; // heading height, in device units
		int fh; // footing height, in device units
		POINT pt;

		pr->prdata.page = basemargins;

		if(pr->Query != NULL)
		   { /* ask about new page */
		    switch(pr->Query(&pr->prdata))
		       { /* query */
			case PQ_Continue:
				break;
			case PQ_Stop:
				return;
			case PQ_NewPage:
				break;    // since we're already planning a 
					  // new page, nothing special needed
		       } /* query */
		   } /* ask about new page */

		if(StartPage(hdc) < 0)
		   throw (PRINTERR_STARTPAGE_FAILED); // printer error

		// If the programmer has specified a DC setup function,
		// call it here
		if(pr->PrepareDC != NULL)
		   { /* DC changed */
		    (pr->PrepareDC)(&pr->prdata);
		   } /* DC changed */

		GetTextMetrics(hdc, &tm);
		pt.x = 0;
		pt.y = tm.tmHeight + tm.tmExternalLeading;
		LPtoDP(hdc, &pt, 1);
		pr->prdata.dy = pt.y;
		pageopen = TRUE;
		needFinish = TRUE;
		pr->prdata.pageno++;

		SendMessage(hAbort, PageNotificationMessage, 0, pr->prdata.pageno);

		// Set the page heading and footing
		hh = 0;
		fh = 0;
		_try 
		   { /* try headings */
		    hh = doHeading(pr, pr->prdata.page.top, 
						   pr->TopLeft, PH_LEFT);
		    hh = max(hh, doHeading(pr, pr->prdata.page.top, 
						   pr->TopCenter, PH_CENTER));
		    hh = max(hh, doHeading(pr, pr->prdata.page.top, 
						   pr->TopRight, PH_RIGHT));
		    fh = doHeading(pr, pr->prdata.page.bottom, 
						   pr->BottomLeft, 
						   PH_BOTTOM | PH_LEFT);
		    fh = max(fh, doHeading(pr, pr->prdata.page.bottom, 
						   pr->BottomCenter, 
						   PH_BOTTOM | PH_CENTER));
		    fh = max(fh, doHeading(pr, pr->prdata.page.bottom, 
						   pr->BottomRight, 
						   PH_BOTTOM | PH_RIGHT));
		    pr->prdata.page.top += hh;
		    pr->prdata.page.bottom -= fh;
		    pr->prdata.ypos = pr->prdata.page.top;
		    bottommargin = pr->prdata.page.bottom - pr->prdata.dy;
		    if(pr->prdata.ypos > bottommargin)
		       { /* impossible margins */
			throw(PRINTERR_MARGINS);
		       } /* impossible margins */
		   } /* try headings */
		_except(checkFilter(GetExceptionCode()))
		   {
		    throw(GetExceptionCode());
		   }
	       } /* open page */
	    else
	       { /* already open */
		if(pr->Query != NULL)
		   { /* query need new page */
		    switch(pr->Query(&pr->prdata))
		       { /* query */
			case PQ_Continue:
				break;             // yes, print it
			case PQ_Stop:
				return;            // stop printing
			case PQ_NewPage:
						   // force new page
				if(pr->FinishPage != NULL)
				   switch(pr->FinishPage(&pr->prdata))
				      { /* finish */
				       case PF_Stop:
					       needFinish = FALSE;
					       return;
				       case PF_Continue:
					       break;
				      } /* finish */
				if(EndPage(hdc) < 0)
				   throw(PRINTERR_ENDPAGE_FAILED);
				pageopen = FALSE;
				continue;
		       } /* query */
		   } /* query need new page */
	       } /* already open */

	    // Ask the caller to format the line
	    _try{
		 savedDC = SaveDC(hdc);
		 switch(pr->FormatLine(&pr->prdata))
		    { /* fmt codes */
		     case PI_Continue:
			     break;
		     case PI_Stop: // stop printing, e.g., EOF
			     return;
		     case PI_NewPage:
			     pr->prdata.ypos = bottommargin + 1; // force page break
			     break;
		    } /* fmt codes */
		}
	     _finally
		{
		 RestoreDC(hdc, savedDC);
		}

	    if(pr->prdata.ypos > bottommargin)
	       { /* end of page */
		if(pr->FinishPage != NULL)
		   switch(pr->FinishPage(&pr->prdata))
		      { /* finish */
		       case PF_Stop:
			       needFinish = FALSE;
			       return;
		       case PF_Continue:
			       break;
		      } /* finish */
		if(EndPage(hdc) < 0)
		   throw (PRINTERR_ENDPAGE_FAILED); // printer error
		pageopen = FALSE;
	       } /* end of page */
	   } /* main loop */
       } /* try around loop */
     _finally
	{ /* close page */
	 if(pageopen)
	    { /* end page */
	     if(needFinish && pr->FinishPage != NULL)
		pr->FinishPage(&pr->prdata);
	     if(EndPage(hdc) < 0)
		throw (PRINTERR_ENDPAGE_FAILED); // printer error
	     pageopen = FALSE;
	    } /* end page */
	} /* close page */
    }

/****************************************************************************
*                                  endPrint
* Inputs:
*       HDC hdc: Device context
* Result: void
*       
* Effect: 
*       Performs an EndDoc operation.  If any problem, throws an exception
****************************************************************************/

static void endPrint(HDC hdc)
    {
     if(PrinterError)
	return;

     if(Aborted)
	AbortDoc(hdc);
     else
	EndDoc(hdc);
    }

/****************************************************************************
*                                   doPrintIt
* Inputs:
*       LPPrintIt pr: Print structure
* Result: DWORD
*       0 if successful
*	nonzero error code if any problem
* Effect: 
*       Initiates printing, using the pd->hDC value.
*	Upon completion, successful or unsuccessful, the DC is released
*	and the pd->hDC field is set to NULL.
*	If an error occurs, a system error is indicated by setting pr->error
*	to a nonzero value, which is the GetLastError value.  If it is a
*	pure doPrintIt error, the last error code is set to 0, as is the
*	pr->error value, but a nonzero value is returned.
****************************************************************************/

_declspec(dllexport) 
DWORD APIENTRY doPrintIt(LPPrintIt pr)
    {
     DWORD result = 0;
     
     if(pr->lStructSize != sizeof(PrintIt))
        { /* wrong version */
	 return PRINTERR_STRUCTSIZE;
	} /* wrong version */

     pr->error = 0;  // assume no error until proven otherwise

     if(pr->prdata.pd->hDC == NULL)
        { /* no DC */
	 SetLastError(0);
	 return PRINTERR_NO_DC;
	} /* no DC */

     if(pr->FormatLine == NULL)
        { /* no fmt line */
	 SetLastError(0);
	 return PRINTERR_NO_FMTLINE;
	} /* no fmt line */

     _try
         {
	  _try
	      {
	       startPrint(pr);
	       printLoop(pr);
	      }
	  _except(checkFilter(GetExceptionCode()))
	      {
	       result = GetExceptionCode();
	       pr->error = GetLastError();

	       // Was it the EndPage that failed?
	       if(result == PRINTERR_ENDPAGE_FAILED)
		  PrinterError = TRUE;
	      }

	 }
      _finally {
		endPrint(pr->prdata.pd->hDC);

		DeleteDC(pr->prdata.pd->hDC);
		pr->prdata.pd->hDC = NULL;

		if(hAbort != NULL)
		   DestroyWindow(hAbort);

		if(pr->prdata.Owner != NULL)
		   EnableWindow(pr->prdata.Owner, TRUE);
	       };
     return result;
    }

/****************************************************************************
*                               getPrintItError
* Inputs:
*       DWORD error: Error code
*	LPTSTR Buffer: Place to put message
*	int length: Length of buffer
* Result: DWORD
*       FormatMessage result
* Effect: 
*       Calls FormatMessage with the error code
****************************************************************************/

_declspec(dllexport) 
DWORD APIENTRY getPrintItError(DWORD error, LANGID langid, LPTSTR Buffer, int length)
    {
     return FormatMessage(FORMAT_MESSAGE_FROM_HMODULE, (LPVOID)hInst, error,
     		    langid, Buffer, length, NULL);
    }

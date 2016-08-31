#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <winerror.h>

#include <tchar.h>

__declspec(dllexport) int __cdecl xdummyEntryPoint(HWND hWnd, HINSTANCE hInst, 
					   LPCTSTR body, 
					   LPCTSTR caption, 
					   DWORD flags, 
					   ...) 
{
    return MessageBox(hWnd, _T("Wrong entry point called"), 
                   _T("DLL Change Error"),
                   MB_ICONERROR | MB_OK );
}

/****************************************************************************
*                              formatMessageBox
* Inputs:
*       HWND hWnd: Parent window handle; may be NULL
*	HINSTANCE hInst: Instance handle for string resources, or NULL
*			 if no resources used, or hWnd provides instance
*	LPCTSTR body: Body string, or MAKEINTRESOURCE(id) for string ID
*	LPCTSTR caption: Caption string, or MAKEINTRESOURCE(id) for string ID
*	DWORD flags: MessageBox flags
*	...: Additional parameters used for formatting the message
* Result: int
*       Result of MessageBox call, or 0 if any failure
* Effect: 
*       Uses the body string as a formatting string for the variable argument
*	list; formats the arguments and pops up a messagebox
*	If return value is 0, GetLastError will give the reason
* Notes:
*	If the instance handle is NULL and a MAKEINTRESOURCE parameter is
*	passed in, the instance associated with the parent window will be
*	used to locate the string resource.  If the parent handle is NULL,
*	the function will return 0 with GetLastError() == ERROR_INVALID_HANDLE.
*
*	Uses FormatMessage to format the message, so the message string
*	must be in terms of FormatMessage formatting escapes
****************************************************************************/

__declspec(dllexport) int __cdecl formatMessageBox(HWND hWnd, HINSTANCE hInst, 
					   LPCTSTR body, 
					   LPCTSTR caption, 
					   DWORD flags, 
					   ...)
    {
     TCHAR fmt[256];
     TCHAR cap[256];
     LPCTSTR pfmt;
     LPCTSTR pcap;
     HINSTANCE rinst = NULL;
     LPCTSTR pbody = NULL;
     int result;
     va_list args;

     _try
        { /* try */
	 va_start(args, flags);

	 if (hInst == NULL && hWnd != NULL)
	    rinst = GetWindowInstance(hWnd);
	 else
	    rinst = hInst;

	 if(HIWORD(caption) == 0)
	    { /* load caption */
	     if(rinst == NULL)
		{ /* bad handle */
		 SetLastError(ERROR_INVALID_HANDLE);
		 return 0;
		} /* bad handle */

	     LoadString(rinst, LOWORD(caption), cap, sizeof(cap) / sizeof(TCHAR));
	     pcap = cap;
	    } /* load caption */
	 else
	    { /* string pointer */
	     pcap = caption;
	    } /* string pointer */

	 if(HIWORD(body) == 0)
	    { /* load body */
	     if(rinst == NULL)
		{ /* bad handle */
		 SetLastError(ERROR_INVALID_HANDLE);
		 return 0;
		} /* bad handle */

	     LoadString(rinst, LOWORD(body), fmt, sizeof(fmt) / sizeof(TCHAR));
	     pfmt = fmt;
	    } /* load body */
	 else
	    { /* fmt pointer */
	     pfmt = body;
	    } /* fmt pointer */

	 if(FormatMessage(FORMAT_MESSAGE_FROM_STRING | 
			  FORMAT_MESSAGE_ALLOCATE_BUFFER, 
			  pfmt,
			  0,    // no message ID when FROM_STRING
			  0,    // no language ID when FROM_STRING
			  (LPTSTR)&pbody,
			  0,
			  &args) == 0)
	     { /* FormatMessage failed */
	      // Note: GetLastError already set by FormatMessage
	      return 0;
	     } /* FormatMessage failed */

	  result = MessageBox(hWnd, pbody, pcap, flags);
	  return result;
	} /* try */
     _finally
        { /* finally */
	 va_end(args);
	 if(pbody != NULL)
	    LocalFree((LPVOID)pbody);
	} /* finally */
    }

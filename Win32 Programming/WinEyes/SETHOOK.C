#define STRICT
#define NOCOMM
#include <windows.h>
#include <windowsx.h>
#include "wineyes.h"

#pragma data_seg(".SHARED")
static HWND      hwndWineyes = NULL ;
#pragma data_seg()
static HHOOK     hhook = NULL ;
static HINSTANCE hModule = NULL ;

LRESULT CALLBACK WineyesMsgHook (int nCode, WPARAM wParam, LPARAM lParam) ;

/****************************************************************************
*                                DllEntryPoint
* Inputs:
*       HINSTANCE hInstDll: DLL instance
*	DWORD reason: Code for event
*	LPVOID reserved: ignored
* Result: BOOL
*       TRUE for process attach
*	FALSE for all other cases
* Effect: 
*       Process attach: Stores the instance handle.
*	Process detach: Unhooks the function
****************************************************************************/

BOOL WINAPI DllEntryPoint(HINSTANCE hInstDll, DWORD reason, LPVOID reserved)
    {
     switch(reason)
         { /* reason */
	  case DLL_PROCESS_ATTACH:
		  hModule = hInstDll;
		  return TRUE;
	  case DLL_PROCESS_DETACH:
		 if(hwndWineyes != NULL)
		    unsetWineyesHook(hwndWineyes);
	 } /* reason */
      return FALSE;
    }

/****************************************************************************
*                               setWinEyesHook
* Inputs:
*       HWND hwnd: Window that will receive notification messages
* Result: BOOL
*       TRUE if successful
*	FALSE if error, or already hooked
* Effect: 
*       Sets a hook to the WineyesMsgHook function for all GetMessage
*	events.
****************************************************************************/

__declspec(dllexport) BOOL WINAPI setWineyesHook (HWND hwnd)
    {
     /* If already hooked, don't do it again. */
     if (hwndWineyes != NULL)
	return FALSE ;
     hhook =
        SetWindowsHookEx (WH_GETMESSAGE,
                       (HOOKPROC) WineyesMsgHook,
                       hModule, 
		       0L) ;
     if (hhook) 
	{ /* hook set */
	 hwndWineyes = hwnd ;
	 return TRUE ;
	} /* hook set */
     return FALSE ;
    }

/****************************************************************************
*                              unsetWineyesHook
* Inputs:
*       HWND hwnd: Window reference
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Unhooks the hook function
****************************************************************************/

__declspec(dllexport) BOOL WINAPI unsetWineyesHook (HWND hwnd)
    {
     BOOL Unhooked ;
     if (hwnd != hwndWineyes)
	return FALSE ;
     Unhooked  = FALSE ;
     Unhooked = UnhookWindowsHookEx (hhook) ;
     if (Unhooked)
	hwndWineyes = NULL ;
     return Unhooked ;
    }

/****************************************************************************
*                               WineyesMsgHook
* Inputs:
*       int nCode: 
*	WPARAM wParam: not used except as passed on
*	LPARAM lParam: a reference to the MSG structure
* Result: LRESULT
*       0, always
* Effect: 
*       Whenever a mouse move message is received, posts a message to the
*	winEyes window showing the current mouse position.  This lets the
*	eyes track the mouse.
****************************************************************************/

LRESULT CALLBACK WineyesMsgHook (int nCode, WPARAM wParam, LPARAM lParam)
{
    LPMSG lpmsg ;
    if (nCode < 0) 
       { /* just pass it on */
	CallNextHookEx (hhook, nCode, wParam, lParam) ;
        return 0 ;
       } /* just pass it on */

    lpmsg = (LPMSG) lParam ;
    if (lpmsg->message == WM_MOUSEMOVE ||
        lpmsg->message == WM_NCMOUSEMOVE)
        PostMessage (hwndWineyes, UWM_WINEYESHOOK, 0, 0L) ;

    CallNextHookEx (hhook, nCode, wParam, lParam) ;
    return 0 ;
}

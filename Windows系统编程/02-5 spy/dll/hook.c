
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*****************************************************************************\
* hook.c - Windows message spy application dll
*
* Functions:
*
* DllMain()
* FindSpyWindow()
* HookProc()
* SpyGetMsgProc()
* SpyCallWndProc()
* DbgPrintf()
*
* Comments:
*
\*****************************************************************************/

#include <windows.h>
#include "..\hook.h"


PRIVATE HWND ghwndSpyHook = NULL;   // the handle back to the spy executable
PRIVATE SPYMSGDATA gsmd;
PRIVATE COPYDATASTRUCT gcds = { 0, sizeof(SPYMSGDATA), &gsmd };


PRIVATE VOID FindSpyWindow(VOID);

#ifdef DBG
VOID DbgPrintf(LPTSTR fmt, ...);
#endif



/*****************************************************************************\
* DllMain (hModule,cbHeap,lpchCmdLine)
*
* Called when the libary is loaded
*
* Arguments:
*    PVOID hModule - Module handle for the libary.
*    ULONG ulReason - DLL purpose
*    PCONTEXT pctx - not used
*
* Returns:
*    TRUE - Everything is ok
*    FALSE- Error.
\*****************************************************************************/

BOOL
APIENTRY DllMain(
    PVOID hModule,
    ULONG ulReason,
    PCONTEXT pctx
    )
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(pctx);

    //
    // This function is called for every instance of the DLL. We must find
    // and store the handle to the spy window every time an instance of the
    // DLL is instantiated.
    //
    if ( ulReason == DLL_PROCESS_ATTACH ) {
        FindSpyWindow();
    }

    return TRUE;
}



/*****************************************************************************\
* FindSpyWindow
*
* Finds the spy window and store a local copy in this instances data.
* This must be called everytime that a new instance of the DLL is
* created.
*
* Arguments:
*    none
*
* Returns:
*    VOID
\*****************************************************************************/

PRIVATE VOID
FindSpyWindow(
    VOID
    )
{
    ghwndSpyHook = FindWindow(HOOKWINDOWCLASS, HOOKWINDOWNAME);
}


/*****************************************************************************\
* HookProc( hWnd, uiMessage, wParam, lParam )
*
* The hook proc for the windows hook being spied on
*
* Arguments:
*    HWND hWnd - window handle for the parent window
*    UINT uiMessage - message number
*    WPARAM wParam - message-dependent
*    LPARAM lParam - message-dependent
*
* Returns:
*    0 if processed, nonzero if ignored
\*****************************************************************************/

BOOL WINAPI
HookProc(
    HWND hwnd,
    UINT uiMessage,
    WPARAM wParam,
    LPARAM lParam
    )
{
    HWND hwndSpyingOn;
    HWND hwndSpyApp;

    if (ghwndSpyHook == NULL || !IsWindow(ghwndSpyHook))
    {
        //
        // Spy has terminated. Find the new window.
        //
        FindSpyWindow();
    }

    if (ghwndSpyHook != NULL && hwnd != ghwndSpyHook)
    {
        hwndSpyingOn = (HWND)GetWindowLong(ghwndSpyHook, 0);
        hwndSpyApp = (HWND)GetWindowLong(ghwndSpyHook, sizeof(HWND));
//DbgPrintf("H ghwndSpyHook:%8.8x", ghwndSpyHook);
//DbgPrintf("H hwndSpyingOn:%8.8x", hwndSpyingOn);
//DbgPrintf("H hwndSpyApp:%8.8x", hwndSpyApp);

        //
        // Send the message on asynchronously for Spy to deal with if
        // it is the appropriate hwndSpyingOn window to spy on.
        //

        if (hwndSpyingOn == hwnd
            || (hwndSpyingOn == HWND_ALL && hwnd != hwndSpyApp
            && !IsChild(hwndSpyApp, hwnd)))
        {
            gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = uiMessage;

//DbgPrintf("H Sending Message hwnd:%8.8x msg:%d", hwnd, uiMessage);
            SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
//DbgPrintf("H Sent Message hwnd:%8.8x msg:%d", hwnd, uiMessage);

//DbgPrintf("");
            return TRUE;
        }
//DbgPrintf("");
    }

    return FALSE;
}



/*****************************************************************************\
* SpyGetMsgProc
*
* The Get Message hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyGetMsgProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PMSG pmsg;

    pmsg = (PMSG)lParam;

    if (hc >= 0 && pmsg && pmsg->hwnd)
    {
        return HookProc(pmsg->hwnd, pmsg->message, pmsg->wParam, pmsg->lParam);
    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



/*****************************************************************************\
* SpyCallWndProc
*
* The Call Window Proc (Send Message) hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyCallWndProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PCWPSTRUCT pcwps;

    pcwps = (PCWPSTRUCT)lParam;

    if (hc >= 0 && pcwps && pcwps->hwnd)
    {
        return HookProc(pcwps->hwnd, pcwps->message, pcwps->wParam, pcwps->lParam);
    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



#ifdef DBG
/****************************************************************************
* DBGprintf
*
* This debugging function prints out a string to the debug output.
* An optional set of substitutional parameters can be specified,
* and the final output will be the processed result of these combined
* with the format string, just like printf.  A newline is always
* output after every call to this function.
*
* Arguments:
*   LPTSTR fmt - Format string (printf style).
*   ...        - Variable number of arguments.
* Returns:
*    VOID
\****************************************************************************/

VOID DbgPrintf(
    LPTSTR fmt,
    ...
    )
{
    va_list marker;
    TCHAR szBuf[256];

    va_start(marker, fmt);
    wvsprintf(szBuf, fmt, marker);
    va_end(marker);

    OutputDebugString(szBuf);
    OutputDebugString(TEXT("\r\n"));
}
#endif



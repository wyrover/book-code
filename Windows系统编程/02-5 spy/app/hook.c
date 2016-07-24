
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
*
* Module: hook.c
*
*   Contains the message hooking functions for the Windows debugging Spy
*   SDK applet.
*
* Functions:
*
*   CreateHookThread()
*   HookMain()
*   HookWndProc()
*   SetSpyHook()
*   SetWindowToSpyOn()
*   DbgPrintf()
*
* Comments:
*
\*****************************************************************************/

#include "spy.h"


PRIVATE BOOL gfProcessHooks = TRUE;


/*****************************************************************************\
* CreateHookThread
*
*    Creates the hook thread.
*
* Arguments: 
*    none
*
* Returns:
*    BOOL - Whether or not hook create succeeeded.
\*****************************************************************************/

BOOL
CreateHookThread(
    VOID
    )
{
    WNDCLASS wc;
    DWORD Id;

    //
    // Register a class for the hook stuff to forward its messages to.
    //
    wc.hCursor        = NULL;    // this window never shown, so no
    wc.hIcon          = NULL;    // cursor or icon are necessary
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = HOOKWINDOWCLASS;
    wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance      = ghInst;
    wc.style          = 0;
    wc.lpfnWndProc    = HookWndProc;
    wc.cbWndExtra     = sizeof(HWND) + sizeof(HWND);
    wc.cbClsExtra     = 0;

    if (!RegisterClass(&wc))
        return FALSE;

    //
    // Now create another thread to handle the new queue
    //
    if (!(ghHookThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HookMain,
        0L, STANDARD_RIGHTS_REQUIRED, &Id)))
        return FALSE;

    return TRUE;
}



/*****************************************************************************\
* HookMain
*
*    Main window procedure for the Hook window
*
\*****************************************************************************/

DWORD
HookMain(
    LPVOID lpv
    )
{
    MSG msg;

    //
    // Create a hidden window for all to find, but not to see
    //
    ghwndSpyHook = CreateWindow(HOOKWINDOWCLASS, HOOKWINDOWNAME,
        WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0,
        (HWND) NULL,        /* no parent */
        (HMENU) NULL,       /* use class menu */
        (HANDLE) ghInst,    /* handle to window instance */
        (LPSTR) NULL        /* no params to pass on */
        );

    if (!ghwndSpyHook)
    {
        ExitThread(0);
    }

    SetWindowToSpyOn(HWND_ALL);

    //
    // Polling forwarded messages from hook's event queue
    //
    while (IsWindow(ghwndSpyHook) && GetMessage(&msg, ghwndSpyHook, 0, 0))
    {
        if (gfProcessHooks)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    ghwndSpyHook = NULL;

    return 0; // not reached
}



/*****************************************************************************\
* HookWndProc
*
* Window procedure for the spy hook.
*
* Arguments:
*   HWND hwnd - handle to the hook window.   
*   UINT msg - message sent to hook window.
*   WPARAM wParam - message parameter.
*   LPARAM lParam - message parameter.
*
* Returns:
*   The value that the proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

LRESULT CALLBACK
HookWndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg)
    {
        //
        // New message for Win32 - allows the application to pass data to another application.
        //      
        case WM_COPYDATA:
            {
                MSG msgT;

                msgT.hwnd = (HWND)wParam;
                msgT.message = ((PCOPYDATASTRUCT)lParam)->dwData;
                msgT.wParam = ((PSPYMSGDATA)((PCOPYDATASTRUCT)lParam)->lpData)->wParam;
                msgT.lParam = ((PSPYMSGDATA)((PCOPYDATASTRUCT)lParam)->lpData)->lParam;
//DbgPrintf("S Received Message hwnd:%8.8x msg:%d", msgT.hwnd, msgT.message);
                PrintMsg(&msgT);
//DbgPrintf("S Printed Message hwnd:%8.8x msg:%d", msgT.hwnd, msgT.message);
            }

            return TRUE;

        case WM_CREATE:
            //
            // Initialize the second HWND in the window words to be the
            // window handle of the spy app.  This will be queried by
            // the hook DLL.
            //
            SetWindowLong(hwnd, sizeof(HWND), (LONG)ghwndSpyApp);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_NCDESTROY:
            gfProcessHooks = FALSE;
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}



/*****************************************************************************\
* SetSpyHook
*
* Sets the windows hooks used to trap the messages.  After this
* is called with a TRUE for fSet, the messages will start flowing
* through the hook DLL.
*
* Arguments:
*   BOOL fSet - TRUE to hook, FALSE to unhook.
*
* Returns:
*   TRUE if successful.
*
\*****************************************************************************/

BOOL
SetSpyHook(
    BOOL fSet
    )
{
    static HHOOK hhkGetMessage = NULL;
    static HHOOK hhkCallWndProc = NULL;
    static HANDLE hmodHook;

    if (fSet)
    {
        if (!hmodHook)
        {
            if (!(hmodHook = LoadLibrary("hook")))
            {
                Message(MB_OK | MB_ICONEXCLAMATION, 
                        LoadResourceString(IDS_ERROR_CANT_LOAD_DLL));
                return FALSE;
            }
        }

        if (!hhkGetMessage)
        {
            if (!(hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE,
                (HOOKPROC)GetProcAddress(hmodHook, "SpyGetMsgProc"), hmodHook, 0)))
            {
                return FALSE;
            }
        }

        if (!hhkCallWndProc)
        {
            if (!(hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC,
                (HOOKPROC)GetProcAddress(hmodHook, "SpyCallWndProc"), hmodHook, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                return FALSE;
            }
        }
    }
    else
    {
        if (hhkGetMessage)
        {
            UnhookWindowsHookEx(hhkGetMessage);
            hhkGetMessage = NULL;
        }

        if (hhkCallWndProc)
        {
            UnhookWindowsHookEx(hhkCallWndProc);
            hhkCallWndProc = NULL;
        }
    }

    return TRUE;
}



/*****************************************************************************\
* SetWindowToSpyOn
*
* Sets the current window to spy on to the specified hwnd.  This hwnd can
* also be the special value HWND_ALL to specify that all windows should
* be spy'd upon.
*
* Arguments:
*   HWND hwndSpyingOn - Window handle to spy on, or HWND_ALL for all windows.
*
* Returns:
*   VOID
\*****************************************************************************/

VOID
SetWindowToSpyOn(
    HWND hwndSpyingOn
    )
{
    ghwndSpyingOn = hwndSpyingOn;
    gfSpyAll = (ghwndSpyingOn == HWND_ALL) ? TRUE : FALSE;
    SetWindowLong(ghwndSpyHook, 0, (LONG)ghwndSpyingOn);
    SetSpyCaption();
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
*
* Returns:
*    VOID
****************************************************************************/

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

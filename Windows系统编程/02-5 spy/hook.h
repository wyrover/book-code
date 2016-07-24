
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*----------------------------------------------------------------------------
*|   hook.h    - Windows message spy application                               |
|                                                                              |
\*----------------------------------------------------------------------------*/

#define PUBLIC
#if !DBG
#define PRIVATE static
#else
#define PRIVATE
#endif

// special hwnds for the hooks to recognize
#define HWND_ALL  ((HWND)0xffffffff)
#define HWND_NULL ((HWND)0L)

#define SPYAPPNAME      "Spy"
#define SPYCLASSNAME    "Spy Class"
#define HOOKWINDOWNAME  "SpyHookWindow"
#define HOOKWINDOWCLASS "SpyHookClass"


//
// Structure that contains the spy message data sent from the hook
// to the spy app to display.  The hwnd is sent using wParam of
// the WM_COPYDATA message, and the message number is in the dwData
// field of the COPYDATASTRUCT.  This structure allows the wParam,
// lParam and any optional extra data to be passed across.
//
typedef struct
{
    WPARAM wParam;
    LPARAM lParam;
    BYTE ExtraData[64];
} SPYMSGDATA, *PSPYMSGDATA;


LRESULT CALLBACK SpyGetMsgProc(INT hc, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SpyCallWndProc(INT hc, WPARAM wParam, LPARAM lParam);



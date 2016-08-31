#include "StdSDK.h"     // Standard application includes
#include "Initialization.h"       // For non-static function prototypes
#include "Frame.h"      // For non-static function prototypes
#include "resource.h"   // For resource identifiers
#include "mdi.h"

//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//
//  Copyright 1995  Brent Rector and Joseph M. Newcomer.  All Rights Reserved.
//
//    PROGRAM:    Skeleton.exe
//
//    PURPOSE:    Provides a skeletal well-behaved Win32 application template
//
//    PLATFORMS:    Windows 95, Windows NT, Win32s
//


//
//  int WinMain (HINSTANCE, HINSTANCE, LPSTR, int)
//
//  PURPOSE: Entry point for the application.
//
//  COMMENTS:
//
//    This function initializes the application and processes the
//    message loop.
//

int
WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG     msg ;

    // Application initialization

    // Note: hPrevInstance is always NULL for Win32 applications
    // Instance initialization
    if (!initInstance (hInstance, IDR_MAINFRAME, nCmdShow)) {
        return FALSE ;
    }

    // Main message loop:
    while (GetMessage (&msg, NULL, 0, 0)) 
       {
    if (!TranslateMDISysAccel(MDIClientWnd, &msg))
           if (!TranslateAccelerator (msg.hwnd, haccel, &msg)) {
              TranslateMessage (&msg) ;
              DispatchMessage (&msg) ;
        }
    }

    return exitInstance (&msg) ;

    // These prevent 'unused formal parameter' warnings
    hPrevInstance ;
    lpCmdLine ;
}

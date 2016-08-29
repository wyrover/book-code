#include "StdSDK.h"         // Standard application includes
#include "Initialization.h" // For non-static function prototypes
#include "MainFrame.h"      // For non-static function prototypes
#include "resource.h"       // For resource identifiers

//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//
//  Copyright 1996  Brent Rector.  All Rights Reserved.
//
//  PROGRAM:    Skeleton.exe
//
//  PURPOSE:    Provides a skeletal well-behaved Win32 application template
//
//  PLATFORMS:  Windows 95, Windows NT, Win32s
//


//
//  int WinMain (HINSTANCE, HINSTANCE, LPSTR, int)
//
//  PURPOSE: Entry point for the application.
//
//  COMMENTS:
//
//  This function initializes the application and processes the
//  message loop.
//

int
WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG    msg ;
    HACCEL haccel ;

    // Instance initialization
    if (!initInstance (hInstance, IDR_MAINFRAME, nCmdShow)) {
        return FALSE ;
    }

    // Load accelerator table for the main frame window's menu
    haccel =
        LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_MAINFRAME)) ;
    ASSERT (NULL != haccel) ;

    // Main message loop:
    while (GetMessage (&msg, NULL, 0, 0)) {
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

#define STRICT
#include "stdafx.h"

/****************************************************************************
*                              void centerWindow
* Inputs:
*       HWND hwnd: Window to be centered on screen
* Result: void
*       
* Effect: 
*       Centers the window on the screen
****************************************************************************/

void centerWindow(HWND hwnd)
    {
     RECT r;
     POINT pt;
     GetWindowRect(hwnd, &r);
     pt.x = (GetSystemMetrics(SM_CXSCREEN) - (r.right - r.left)) / 2;
     pt.y = (GetSystemMetrics(SM_CYSCREEN) - (r.bottom - r.top)) / 2;
     SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

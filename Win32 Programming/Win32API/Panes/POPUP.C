#include "StdSDK.h"
#include "resource.h"
#include "MainFrame.h"
#include <tchar.h>

void popup_OnDestroy(HWND hwnd);

LRESULT CALLBACK
popupWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch(message)
    { /* message */
     HANDLE_MSG(hwnd, WM_DESTROY, popup_OnDestroy);
    } /* message */
 return DefWindowProc(hwnd, message, wParam, lParam);
}

/****************************************************************************
*                               popup_OnDestroy
* Inputs:
*       HWND hwnd: Handle of popup window
* Result: void
*       
* Effect: 
*       Indicates that the unowned popup has been destroyed by setting its
*	handle to NULL
****************************************************************************/

void popup_OnDestroy(HWND hwnd)
{
 if(GetWindowOwner(hwnd) == (HWND)NULL)
    UnownedPopup = (HWND)NULL;
}
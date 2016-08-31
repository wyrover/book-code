#include "StdSDK.h"
#include "resource.h"
#include "MainFrame.h"
#include <tchar.h>

static BOOL child_OnContextMenu(HWND hwnd, HWND hwndctl, int x, int y);
static void child_OnCommand(HWND hwnd, int id, HWND hctl, UINT codenotify);
static void child_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL sysmenu);

LRESULT CALLBACK
childWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch(message)
    { /* message */
     HANDLE_MSG(hwnd, WM_CONTEXTMENU, child_OnContextMenu);
     HANDLE_MSG(hwnd, WM_COMMAND, child_OnCommand);
     HANDLE_MSG(hwnd, WM_INITMENUPOPUP, child_OnInitMenuPopup);
    } /* message */
 return DefWindowProc(hwnd, message, wParam, lParam);
}

static void child_OnCommand(HWND hwnd, int id, HWND hctl, UINT codenotify)
{
 // We could have commands that are unique to this window, or general
 // commands.  If we recognize the command, do it, otherwise pass to
 // our parent

 switch(id)
    { /* id */
     case IDM_RESTORE:
             ShowWindow(hwnd, SW_RESTORE);
             return;
     case IDM_MINIMIZE:
             ShowWindow(hwnd, SW_MINIMIZE);
             return;
     case IDM_MAXIMIZE:
             ShowWindow(hwnd, SW_MAXIMIZE);
             return;
    } /* id */
 FORWARD_WM_COMMAND(GetParent(hwnd), id, hctl, codenotify, SendMessage);
}


BOOL child_DisplayContextMenu (HWND hwnd, POINT pt)
{
    HMENU hmenu, hmenuPopup ;
    BOOL result;

    // Determine the appropriate context menu to display.
    // generally using the application state and mouse click coordinates.

    hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_CHILDCONTEXT));

    hmenuPopup = GetSubMenu (hmenu, 0) ;
    ASSERT (NULL != hmenuPopup) ;

    // Convert click location to screen coordinates
    ClientToScreen (hwnd, &pt) ;

    // Display the floating popup menu at the mouse click location
    // Track the right mouse as this function is called during
    // WM_CONTEXTMENU message processing.
    result = TrackPopupMenu (hmenuPopup,
                           TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           pt.x, pt.y, 0, hwnd, NULL) ;
    DestroyMenu(hmenu);
    return result;
}

/****************************************************************************
*                               child_OnContextMenu
* Inputs:
*       HWND hwnd: Handle of popup window
* Result: BOOL
*       
* Effect: 
*       Pops up a menu that allows the child to be minimized, maximized, or
*       restored
****************************************************************************/

static BOOL child_OnContextMenu(HWND hwnd, HWND hwndctl, int x, int y)
{
    POINT pt = { x, y } ;   // location of mouse click 
    RECT  rc ;                    // client area of window 
 
    // Get the bounding rectangle of the client area. 
    GetClientRect (hwnd, &rc) ;
 
    // Convert the mouse position to client coordinates. 
    ScreenToClient (hwnd, &pt) ;
 
    // If the mouse click was in the client area,
    // display the appropriate floating popup menu.
    if (PtInRect (&rc, pt))
        if (child_DisplayContextMenu (hwnd, pt))
            return TRUE ;
 
    // Otherwise forward the message for default processing
    return FORWARD_WM_CONTEXTMENU (hwnd, hwndctl, x, y, DefWindowProc) ;
}

static void child_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu)
{
 EnableMenuItem(hmenu, IDM_RESTORE, IsZoomed(hwnd) || IsIconic(hwnd) 
                                ? MF_BYCOMMAND | MF_ENABLED 
                                : MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hmenu, IDM_MINIMIZE, !IsIconic(hwnd)
                                ? MF_BYCOMMAND | MF_ENABLED 
                                : MF_BYCOMMAND | MF_GRAYED);
 EnableMenuItem(hmenu, IDM_MAXIMIZE, !IsZoomed(hwnd)
                                ? MF_BYCOMMAND | MF_ENABLED 
                                : MF_BYCOMMAND | MF_GRAYED);
}

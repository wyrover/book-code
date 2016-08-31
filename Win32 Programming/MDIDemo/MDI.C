#include "stdSDK.h"

#include "resource.h"
#include "Frame.h"
#include "mdi.h"
#include "error.h"
// If you are going to run under Win32s, you must enable the line below.
// CreateMDIWindow does not exist in Win32s.
//#define USE_WM_MDICREATE

/****************************************************************************
*                           mdi_DisplayContextMenu
* Inputs:
*       HWND hwnd: window handle of child window
*       POINT pt: Mouse point
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Pops up a text context menu
****************************************************************************/

BOOL mdi_DisplayContextMenu(HWND hwnd, POINT pt, HMENU context)
    {
     HMENU popup;

     if(context == NULL)
        return FALSE;
     
     popup = GetSubMenu(context, 0);

     ClientToScreen(hwnd, &pt);
     mdi_ConformMenus(GetMenu(hMainFrame), popup);

     TrackPopupMenu(popup, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                pt.x, pt.y, 0, 
                                hMainFrame, NULL);
     return TRUE;
    }

/****************************************************************************
*                             mdi_OnContextMenu
* Inputs:
*       HWND hwnd: Window handle
*       HWND hwndCtl: 
*       int xPos: Mouse-x in screen coordinates
*       int yPos: Mouse-y in screen coordinates
*       HMENU context: Context menu handle
* Result: BOOL
*       TRUE if we handle the message
*       FALSE if it is not ours
* Effect: 
*       Pops up a context-specific menu
****************************************************************************/

BOOL 
mdi_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos, HMENU context)
    {
     POINT pt = { xPos, yPos};
     RECT  rc;

     // Get the bounding rectangle of the client area of the child
     GetClientRect(hwnd, &rc);

     // Convert the mouse position to client coordinates
     ScreenToClient(hwnd, &pt);

     // If the mouse click was in the client area of this child window,
     // display the appropriate popup menu.
     if(PtInRect(&rc, pt))
        if(mdi_DisplayContextMenu(hwnd, pt, context))
           return TRUE;

     // Otherwise, tell our caller that we don't handle it
     return FALSE;
    }

/****************************************************************************
*                              mdi_ConformMenus4
* Inputs:
*       HMENU hMaster: Menu controlling what should be set in popup
*       HMENU hPopup: Menu which will be popped up
* Result: void
*       
* Effect: 
*       For each menu item in hPopup, its state is set to be the same state
*       as found in hMaster, unless there is no such state in which case it
*       is left alone.
* Notes:
*       This can only work with API level 4 and above.
****************************************************************************/

static void mdi_ConformMenus4(HMENU hMaster, HMENU hPopup)
    {
     int count = GetMenuItemCount(hPopup);
     int i;
     MENUITEMINFO getpopupinfo;
     MENUITEMINFO setpopupinfo;
     MENUITEMINFO masterinfo;
     
     getpopupinfo.cbSize = sizeof(MENUITEMINFO);
     getpopupinfo.fMask  = MIIM_SUBMENU | MIIM_ID;

     setpopupinfo.cbSize = sizeof(MENUITEMINFO);
     setpopupinfo.fMask  = MIIM_STATE;

     masterinfo.cbSize   = sizeof(MENUITEMINFO);
     masterinfo.fMask    = MIIM_STATE;

     for(i = 0; i < count; i++)
        { /* process each */
         VERIFY(GetMenuItemInfo(hPopup, i, TRUE, &getpopupinfo));
         if(getpopupinfo.hSubMenu != NULL)
            { /* submenu */
             mdi_ConformMenus4(hMaster, getpopupinfo.hSubMenu);
            } /* submenu */
         else
            { /* menu item */
             GetMenuItemInfo(hMaster, getpopupinfo.wID, FALSE, &masterinfo);
             setpopupinfo.fState = masterinfo.fState;
             SetMenuItemInfo(hPopup, i, TRUE, &setpopupinfo);
            } /* menu item */
        } /* process each */
    }

/****************************************************************************
*                              mdi_ConformMenus3
* Inputs:
*       HMENU hMaster: Menu controlling what should be set in popup
*       HMENU hPopup: Menu which will be popped up
* Result: void
*       
* Effect: 
*       For each menu item in hPopup, its state is set to be the same state
*       as found in hMaster, unless there is no such state in which case it
*       is left alone.
* Notes:
*       This somewhat clunky version is required for API level 3.
****************************************************************************/

static void mdi_ConformMenus3(HMENU hMaster, HMENU hPopup)
    {
     int count = GetMenuItemCount(hPopup);
     int i;
     UINT popupstate;
     HMENU submenu;

     for(i = 0; i < count; i++)
        { /* process each */
         popupstate = GetMenuState(hPopup, i, MF_BYPOSITION);
         if( (popupstate & ~0xFF) != 0)
            { /* submenu */
             submenu = GetSubMenu(hPopup, i);
             mdi_ConformMenus3(hMaster, submenu);
            } /* submenu */
         else
            { /* menu item */
             UINT id = GetMenuItemID(hPopup, i);
             UINT masterstate = GetMenuState(hMaster, id, MF_BYCOMMAND);

             EnableMenuItem(hPopup, i, MF_BYPOSITION |
                        (masterstate & (MF_ENABLED | MF_DISABLED | MF_GRAYED)));
             CheckMenuItem(hPopup, i, MF_BYPOSITION |
                        (masterstate & (MF_CHECKED | MF_UNCHECKED)));
            } /* menu item */
        } /* process each */
    }

/****************************************************************************
*                              mdi_ConformMenus
* Inputs:
*       HMENU hMaster: Menu controlling what should be set in popup
*       HMENU hPopup: Menu which will be popped up
* Result: void
*       
* Effect: 
*       For each menu item in hPopup, its state is set to be the same state
*       as found in hMaster, unless there is no such state in which case it
*       is left alone.
****************************************************************************/

void mdi_ConformMenus(HMENU hMaster, HMENU hPopup)
    {
     OSVERSIONINFO info;
     
     info.dwOSVersionInfoSize = sizeof(info);
     GetVersionEx(&info);
     if(info.dwMajorVersion < 4)
        mdi_ConformMenus3(hMaster, hPopup);
     else
        mdi_ConformMenus4(hMaster, hPopup);
    }

/****************************************************************************
*                                 mdi_Create
* Inputs:
*       HWND hwnd: Parent window, that is, the MDI client window
*	DWORD styles: Style flags
*	int classid: Class ID from resource file
*	int titleid: Title ID from resource file
* Result: HWND
*       The result of 
* Effect: 
*       Creates an MDI child window
* Notes:
*	CreateMDIWindow only exists in Windows/NT and Windows/96.  To use
*	in Win32s, you must define USE_WM_MDICREATE.
****************************************************************************/

HWND mdi_Create(HWND hwnd, DWORD styles, int classid, int titleid)
    {
     HINSTANCE hinst = GetWindowInstance(hwnd);
     HWND hwndChild ;
     TCHAR Class[80] ;
     TCHAR Title[80] ;
#ifdef USE_WM_MDICREATE
     MDICREATESTRUCT mcs;
#endif
     
     LoadString(hinst, classid, Class, DIM(Class));
     LoadString(hinst, titleid, Title, DIM(Title));

#ifdef USE_WM_MDICREATE
     mcs.szClass = Class;
     mcs.szTitle = Title;
     mcs.hOwner  = hinst; 
     mcs.x       = 0;
     mcs.y	 = 0;
     mcs.cx      = CW_USEDEFAULT; 
     mcs.cy      = CW_USEDEFAULT;    
     mcs.style   = styles; 
     mcs.lParam  = 0;

     hwndChild = FORWARD_WM_MDICREATE(hwnd, &mcs, SendMessage);

#else
     hwndChild = CreateMDIWindow(Class, Title, 0, 
                                 CW_USEDEFAULT, 0,
                                 CW_USEDEFAULT, 0,
                                 hwnd, 
                                 hinst,
                                 0);
#endif
     
     if(hwndChild == NULL)
        { /* failed */
         reportError(GetLastError());
        } /* failed */

     return hwndChild;
    }

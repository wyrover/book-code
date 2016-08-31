#define STRICT

#include <windows.h>
#include "resource.h"

/****************************************************************************
*                               findWindowMenu
* Inputs:
*       HMENU hMenu: Menu which contains a "Window" menu
* Result: HMENU
*       Handle of "Window" popup, or NULL if not found
* Notes: 
*       The "Window" menu is the menu which contains the ID_WINDOW_CASCADE
*       menu item.  If you have decided to disallow this capability by
*       removing the "Cascade" menu item, this will have to be rewritten
****************************************************************************/

HMENU findWindowMenu(HMENU hMenu)
    {
     int count = GetMenuItemCount(hMenu);
     int i;

     for(i = 0; i < count; i++)
        { /* scan menu */
         HMENU tmenu = GetSubMenu(hMenu, i);
         if( GetMenuState(tmenu, ID_WINDOW_CASCADE, MF_BYCOMMAND) != (UINT)-1)
            { /* found it */
             return tmenu;
            } /* found it */
        } /* scan menu */
         
     return NULL;
    }

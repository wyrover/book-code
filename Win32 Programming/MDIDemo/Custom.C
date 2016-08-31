#include "StdSDK.h"     // Standard application includes
#include <tchar.h>
#include "Initialization.h"       // For non-static function prototypes
#include "Frame.h"      // For non-static function prototypes
#include "resource.h"   // For resource identifiers
#include "mdi.h"
#include "error.h"
#include "findwnd.h"
#include "options.h"
#include "edit.h"


static void custom_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify);
static BOOL custom_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos);
static HBRUSH custom_OnCtlColorEdit(HWND hwnd, HDC hdc, HWND hchild, int type);
static void custom_OnDestroy(HWND hwnd);
static void custom_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, HWND hDeactivate);
static void custom_OnSize(HWND hwnd, UINT state, int cx, int cy);
static void custom_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu);
static void custom_OnSetFocus(HWND hwnd, HWND oldfocus);
static void custom_OnUpdateMenu(HWND hwnd, HMENU hmenu);

//=============================================================================
// These values are initialized once in InitInstance and destroyed in
// ExitInstance
static HMENU customMenu = NULL;
static HACCEL customAccel = NULL;
static HMENU customWindowMenu = NULL;
static HMENU customContextMenu = NULL;
//=============================================================================

//=============================================================================
// These macros are used to access the WindowExtra components of our class
#define EXTRA_BYTES (sizeof(LPARAM))
#define EXTRA_CTLCOLOR 0

#define getUseCtlColor(hwnd) (BOOL)GetWindowLong(hwnd, EXTRA_CTLCOLOR)
#define setUseCtlColor(hwnd, val) (BOOL)SetWindowLong(hwnd, EXTRA_CTLCOLOR, (LPARAM)val)


/****************************************************************************
*                                customWndProc
* Inputs:
*       HWND hwnd: Window handle
*       UINT message: Message ID
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: LRESULT
*       As per message spec
* Effect: 
*       Dispatcher
****************************************************************************/

LRESULT CALLBACK 
customWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
         HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, custom_OnCtlColorEdit);
         HANDLE_MSG(hwnd, WM_MDIACTIVATE, custom_OnMDIActivate);
         HANDLE_MSG(hwnd, WM_DESTROY, custom_OnDestroy);
         HANDLE_MSG(hwnd, WM_COMMAND, custom_OnCommand);
         HANDLE_MSG(hwnd, WM_SIZE, custom_OnSize);
         HANDLE_MSG(hwnd, WM_INITMENUPOPUP, custom_OnInitMenuPopup);
         HANDLE_MSG(hwnd, WM_SETFOCUS, custom_OnSetFocus);
	 HANDLE_MSG(hwnd, WM_CONTEXTMENU, custom_OnContextMenu);
         case UWM_UPDATE_MENU:
                 custom_OnUpdateMenu(hwnd, GetMenu(hMainFrame));
                 return 0;
         case UWM_CONTEXTMENU:
                 return HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, 
                                                  custom_OnContextMenu);
        } /* message */
     return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

/****************************************************************************
*                             custom_InitInstance
* Inputs:
*       HINSTANCE hinst: Instance handle
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the custom window class, and loads its menu to a local
*       static variable.
****************************************************************************/

BOOL custom_InitInstance(HINSTANCE hinst)
   {
    CustomOptions.subclass = TRUE;
    CustomOptions.docolor = TRUE;
    CustomOptions.menu = NULL;
    CustomOptions.styles = WS_CHILD | WS_VISIBLE | 
                           ES_MULTILINE | 
                           ES_WANTRETURN |
                           ES_AUTOVSCROLL |
                           WS_VSCROLL;

    if(!registerMDIChild(hinst, IDR_CUSTOM, customWndProc, EXTRA_BYTES))
       return FALSE;

    customMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_CUSTOM));
    customContextMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_CUSTOM_CONTEXT));

    if(customMenu == NULL)
       return FALSE;

    customAccel = LoadAccelerators(hinst, 
                                MAKEINTRESOURCE(IDR_CUSTOM));
    customWindowMenu = findWindowMenu(customMenu);

    return TRUE;
   }


/****************************************************************************
*                             custom_ExitInstance
* Result: void
*       
* Effect: 
*       Does whatever cleanup is necessary.  In this case, destroys the
*       menu handle.
****************************************************************************/

void custom_ExitInstance()
    {
     DestroyMenu(customMenu);
     DestroyMenu(customContextMenu);
    }

/****************************************************************************
*                                custom_Create
* Inputs:
*       HWND hwnd: Parent window
*       DWORD editstyles: Edit window style flags
*       BOOL subclassing: TRUE to subclass edit control, FALSE to not subclass
*       BOOL ctlColor: TRUE to use CtlColor, FALSE to not use it
* Result: BOOL
*       TRUE if success, FALSE if failure
* Effect: 
*       Creates an MDI child window of the correct class, using the
*       editstyles flags to set the styles of the edit window.
****************************************************************************/

BOOL custom_Create(HWND hwnd, DWORD editstyles, BOOL subclassing, BOOL ctlColor)
    {
     HWND hwndChild;

     hwndChild = mdi_Create(MDIClientWnd, 0, IDR_CUSTOM, IDS_CUSTOM);

     if(hwndChild == NULL)
        { /* failed */
         reportError(GetLastError());
        } /* failed */
     else
        { /* success */
         HWND hedit;

         setUseCtlColor(hwndChild, ctlColor);

	 hedit = edit_New(hwndChild, editstyles, TRUE); 

         if(hedit != NULL)
            { /* have edit control */
             TCHAR suffix[20];
             LPTSTR sep = _T("");

             lstrcpy(suffix, _T(""));

             if(!subclassing)
                { /* no subclassing */
                 LoadString(GetWindowInstance(hwndChild), 
                                IDS_NO_SUBCLASS, suffix, DIM(suffix));
                 sep = _T(", ");
                } /* no subclassing */

             if(!getUseCtlColor(hwndChild))
                { /* no color */
                 lstrcat(suffix, sep);
                 LoadString(GetWindowInstance(hwndChild),
                                IDS_NO_COLOR, &suffix[lstrlen(suffix)],
                                                DIM(suffix) - lstrlen(suffix));
                } /* no color */

             styleToTitle(hwndChild, editstyles, suffix);
            } /* have edit control */
	 else
	    { /* no edit */
	     DestroyWindow(hwndChild);
	     return FALSE;
	    } /* no edit */
        } /* success */

     return (hwndChild != NULL);
     
    }

/****************************************************************************
*                                  custom_New
* Inputs:
*       HWND hwnd: Window handle
* Result: BOOL
*       TRUE if successful
*       FALSE if failure
* Effect: 
*       Creates a new custom window
****************************************************************************/

BOOL custom_New(HWND hwnd)
    {
     if(!custom_Create(hwnd, CustomOptions.styles, CustomOptions.subclass,
                                CustomOptions.docolor))
        return FALSE;
     PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);
     return TRUE;
    }

/****************************************************************************
*                              custom_OnSetFocus
* Inputs:
*       HWND hwnd: Window receiving focus
*       HWND oldfocus: Window losing focus
* Result: void
*       
* Effect: 
*       Posts the custom menu if that is not the current menu
*       If the window is being deactivated, unchecks the color
****************************************************************************/

static void 
custom_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, 
					HWND hDeactivate)
    {
     CheckMenuItem(customMenu, GetWindowWord(hwnd, 0), 
                        (active ? MF_CHECKED : MF_UNCHECKED));
     if(active)
        { /* activate */
         FORWARD_WM_MDISETMENU(MDIClientWnd, TRUE, customMenu, 
	 				customWindowMenu, SendMessage);
         SendMessage(hMainFrame, UWM_SET_ACCELERATOR, 0, (LPARAM)customAccel);
         DrawMenuBar(hMainFrame);
         PostMessage(hMainFrame, UWM_UPDATE_TOOLBAR, 0, 0);
        } /* activate */
    }

/****************************************************************************
*                             custom_OnDestroy
* Inputs:
*       HWND hwnd:
*       HWND destroy:
* Result: void
*       
* Effect: 
*       After forwarding the message to the default procedure, posts a
*       message to the main window suggesting that it should check for
*       the proper menu being posted
****************************************************************************/

static void custom_OnDestroy(HWND hwnd)
    {
     FORWARD_WM_DESTROY(hwnd, DefMDIChildProc);
     PostMessage(hMainFrame, UWM_MDI_DESTROY, 0, 0);
    }

/****************************************************************************
*                                 custom_OnFont
* Inputs:
*       HWND  hwnd: Window handle
* Result: void
*       
* Effect: 
*       Changes the font of the window
****************************************************************************/

static void 
custom_OnFont(HWND hwnd)
    {
     edit_ChooseFont(GetDlgItem(hwnd, ID_EDIT_CONTROL));
    }

/****************************************************************************
*                                 custom_OnEdit
* Inputs:
*       HWND hwnd: Window handle
*       int id: Child window ID (ignored, we only have one edit control)
*       HWND hctl: Child window handle
*       UINT codeNotify: which event happened?
* Result: void
*       
* Effect: 
*       When changes occur that affect possible menu/toolbar settings, cause
*       the objects to update
****************************************************************************/

static void 
custom_OnEdit(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     switch(codeNotify)
        { /* codeNotify */
         case EN_CHANGE:
                 PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);
                 return;
        } /* codeNotify */
    }

/****************************************************************************
*                               custom_OnCommand
* Inputs:
*       HWND hwnd: Window handle
*       int id: Command ID
*       HWND hctl: Handle of control
*       UINT codeNotify: Notification code
* Result: void
*       
* Effect: 
*       Processes menu items for this window
****************************************************************************/

static void
custom_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     switch (id) 
         { /* id */
         case IDM_FONT:
                 custom_OnFont(hwnd);
                 return ;
         case ID_EDIT_UNDO:
                 FORWARD_WM_UNDO(GetDlgItem(hwnd, ID_EDIT_CONTROL), SendMessage);
                 return;
         case ID_EDIT_REDO:
                 return;
         case ID_EDIT_CUT:
                 FORWARD_WM_CUT(GetDlgItem(hwnd, ID_EDIT_CONTROL), SendMessage);
                 return;
         case ID_EDIT_COPY:
                 FORWARD_WM_COPY(GetDlgItem(hwnd, ID_EDIT_CONTROL), SendMessage);
                 return;
         case ID_EDIT_PASTE:
                 FORWARD_WM_PASTE(GetDlgItem(hwnd, ID_EDIT_CONTROL), SendMessage);
                 return;
         case ID_EDIT_CLEAR:
                 FORWARD_WM_CLEAR(GetDlgItem(hwnd, ID_EDIT_CONTROL), SendMessage);
                 return;
         case ID_EDIT_SELECT_ALL:
                 Edit_SetSel(GetDlgItem(hwnd, ID_EDIT_CONTROL), 0, -1);
                 return;
         case ID_EDIT_PROPERTIES:
                 return;
         case ID_EDIT_CONTROL:
                 custom_OnEdit(hwnd, id, hctl, codeNotify);
                 return;
         } /* id */ /* id */
     FORWARD_WM_COMMAND(hwnd, id, hctl, codeNotify, DefMDIChildProc);
    }

/****************************************************************************
*                                 custom_OnSize
* Inputs:
*       HWND hwnd: Window reference
*       UINT state: SIZEICONIC, SIZENORMAL, etc.
*       int cx: width of client area
*       int cy: height of client area
* Result: void
*       
* Effect: 
*       Resizes the edit control to fit in the client area
****************************************************************************/

static void 
custom_OnSize(HWND hwnd, UINT state, int cx, int cy)
    {
     if(state != SIZEICONIC)
        MoveWindow(GetDlgItem(hwnd, ID_EDIT_CONTROL), 0, 0, cx, cy, TRUE);
     FORWARD_WM_SIZE(hwnd, state, cx, cy, DefMDIChildProc);
    }

/****************************************************************************
*                            custom_OnInitMenuPopup
* Inputs:
*       HWND hwnd:
*       HMENU hmenu:
*       UINT item:
*       BOOL sysmenu:
* Result: void
*       
* Effect: 
*       Enables cut, copy, paste, etc.
****************************************************************************/

static void 
custom_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu)
    {
     PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);
    }

/****************************************************************************
*                              custom_OnUpdateMenu
* Inputs:
*       HWND hwnd: Current window handle
*       HMENU hmenu: Menu handle
* Result: void
*       
* Effect: 
*       Updates the menu (for InitMenuPopup) and updates the toolbar as
*       well.  
****************************************************************************/

static void 
custom_OnUpdateMenu(HWND hwnd, HMENU hmenu)
    {
     HWND hedit = GetDlgItem(hwnd, ID_EDIT_CONTROL);

     edit_UpdateMenu(hedit, hmenu);

     PostMessage(hMainFrame, UWM_UPDATE_TOOLBAR, 0, 0);
     
    }

/****************************************************************************
*                               custom_OnSetFocus
* Inputs:
*       HWND hwnd: Window receiving focus
*       HWND oldfocus: Window losing focus
* Result: void
*       
* Effect: 
*       Sets focus to edit control which is contained inside the MDI child
****************************************************************************/

static void 
custom_OnSetFocus(HWND hwnd, HWND oldfocus)
    {
     SetFocus(GetDlgItem(hwnd, ID_EDIT_CONTROL));
    }

/****************************************************************************
*			     custom_OnCtlColorEdit
* Inputs:
*       HWND hwnd: Window handle of parent window
*       HDC hdc: Display context for child window
*       HWND hchild: Window handle of child window
*       int type: CTLCOLOR_EDIT is the only value we care about
* Result: HBRUSH
*       Background brush
*       NULL, always -- use default brush for class
* Effect: 
*       Sets the text color
****************************************************************************/

static HBRUSH 
custom_OnCtlColorEdit(HWND hwnd, HDC hdc, HWND hchild, int type)
    {
     return FORWARD_WM_CTLCOLOREDIT(hchild, hdc, hchild, SendMessage);
    }

/****************************************************************************
*                             custom_OnContextMenu
* Inputs:
*       HWND hwnd: Window handle
*       HWND hwndCtl: 
*       int xPos: Mouse-x in screen coordinates
*       int yPos: Mouse-y in screen coordinates
* Result: BOOL
*       TRUE if we handle the message
*       FALSE if it is not ours
* Effect: 
*       Pops up a context-specific menu
****************************************************************************/

static BOOL 
custom_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos)
    {
     return mdi_OnContextMenu(hwnd, hwndCtl, xPos, yPos, customContextMenu);
    }

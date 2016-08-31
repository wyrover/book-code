#include "StdSDK.h"     // Standard application includes
#include <tchar.h>
#include "Initialization.h"       // For non-static function prototypes
#include "Frame.h"      // For non-static function prototypes
#include "resource.h"   // For resource identifiers
#include "mdi.h"
#include "error.h"
#include "findwnd.h"
#include "edit.h"


static void text_OnClose(HWND hwnd);
static void text_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify);
static BOOL text_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos);
static HBRUSH text_OnCtlColorEdit(HWND hwnd, HDC hdc, HWND hchild, int type);
static void text_OnDestroy(HWND hwnd);
static void text_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, HWND hDeactivate);
static BOOL text_OnQueryEndSession(HWND hwnd);
static void text_OnSize(HWND hwnd, UINT state, int cx, int cy);
static void text_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu);
static void text_OnSetFocus(HWND hwnd, HWND oldfocus);
static void text_OnUpdateMenu(HWND hwnd, HMENU hmenu);
static BOOL text_QueryClose(HWND hwnd);

//=============================================================================
// These values are initialized once in InitInstance and destroyed in
// ExitInstance
static HMENU textMenu = NULL;
static HACCEL textAccel = NULL;
static HMENU textWindowMenu = NULL;
static HMENU textContextMenu = NULL;
//=============================================================================

//=============================================================================
// These macros are used to access the WindowExtra components of our class

#define EXTRA_BYTES 0

/****************************************************************************
*                                textWndProc
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

static LRESULT CALLBACK 
textWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
	 HANDLE_MSG(hwnd, WM_CLOSE, text_OnClose);
         HANDLE_MSG(hwnd, WM_COMMAND, text_OnCommand);
	 HANDLE_MSG(hwnd, WM_CONTEXTMENU, text_OnContextMenu);
         HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, text_OnCtlColorEdit);
         HANDLE_MSG(hwnd, WM_DESTROY, text_OnDestroy);
         HANDLE_MSG(hwnd, WM_INITMENUPOPUP, text_OnInitMenuPopup);
         HANDLE_MSG(hwnd, WM_MDIACTIVATE, text_OnMDIActivate);
	 HANDLE_MSG(hwnd, WM_QUERYENDSESSION, text_OnQueryEndSession);
         HANDLE_MSG(hwnd, WM_SETFOCUS, text_OnSetFocus);
         HANDLE_MSG(hwnd, WM_SIZE, text_OnSize);
         case UWM_UPDATE_MENU:
                 text_OnUpdateMenu(hwnd, GetMenu(hMainFrame));
                 return 0;
         case UWM_CONTEXTMENU:
                 return HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, 
		 					text_OnContextMenu);
        } /* message */
     return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

/****************************************************************************
*                             text_InitInstance
* Inputs:
*       HINSTANCE hinst: Instance handle
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the text window class, and loads its menu to a local
*       static variable.
****************************************************************************/

BOOL text_InitInstance(HINSTANCE hinst)
   {
    if(!registerMDIChild(hinst, IDR_TEXT, textWndProc, EXTRA_BYTES))
       return FALSE;

    textMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_TEXT));

    if(textMenu == NULL)
       return FALSE;

    textContextMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_TEXT_CONTEXT));
    // this could be NULL, that's OK

    textAccel = LoadAccelerators(hinst, 
                                MAKEINTRESOURCE(IDR_TEXT));
    textWindowMenu = findWindowMenu(textMenu);

    return TRUE;
   }


/****************************************************************************
*                             text_ExitInstance
* Result: void
*       
* Effect: 
*       Does whatever cleanup is necessary.  In this case, destroys the
*       menu handle.
****************************************************************************/

void text_ExitInstance()
    {
     DestroyMenu(textMenu);
     DestroyMenu(textContextMenu);
    }

/****************************************************************************
*                                  text_New
* Inputs:
*       HWND hwnd: Parent window
* Result: BOOL
*       TRUE if successful
*       FALSE if failure
* Effect: 
*       Creates a new text window
****************************************************************************/

BOOL text_New(HWND hwnd)
    {
     HWND hwndChild ;
     

     hwndChild = mdi_Create(MDIClientWnd, 0, IDR_TEXT, IDS_TEXT);

     if(hwndChild != NULL)
        { /* success */
         HWND hedit;

	 hedit = edit_New(hwndChild, WS_CHILD | 
	 			     WS_VISIBLE | 
                                     ES_MULTILINE | 
                                     ES_WANTRETURN |
                                     ES_AUTOVSCROLL |
                                     WS_VSCROLL, TRUE);
         if(hedit == NULL)
	    { /* no edit window */
	     DestroyWindow(hwndChild);
	     return FALSE;
	    } /* no edit window */
	 PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);

        } /* success */

     return (hwndChild != NULL);
    }

/****************************************************************************
*                              text_OnMDIActivate
* Inputs:
*       HWND hwnd: Window receiving focus
*       HWND oldfocus: Window losing focus
* Result: void
*       
* Effect: 
*       Posts the text menu if that is not the current menu
*       If the window is being deactivated, unchecks the color
****************************************************************************/

static void 
text_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, HWND hDeactivate)
    {
     CheckMenuItem(textMenu, GetWindowWord(hwnd, 0), 
                        (active ? MF_CHECKED : MF_UNCHECKED));
     if(active)
        { /* activating */
         FORWARD_WM_MDISETMENU(MDIClientWnd, TRUE, textMenu, textWindowMenu, 
	 						         SendMessage);
         SendMessage(hMainFrame, UWM_SET_ACCELERATOR, 0, (LPARAM)textAccel);
         DrawMenuBar(hMainFrame);
         PostMessage(hMainFrame, UWM_UPDATE_TOOLBAR, 0, 0);
        } /* activating */
    }

/****************************************************************************
*                                text_OnClose
* Inputs:
*       HWND hwnd: Window to close
* Result: void
*       
* Effect: 
*       If the contents have changed, prompt the user for confirmation;
*	otherwise just close the window
****************************************************************************/

static void 
text_OnClose(HWND hwnd)
    {
     if(!text_QueryClose(hwnd))
	return;
     FORWARD_WM_CLOSE(hwnd, DefMDIChildProc);
    }

/****************************************************************************
*                             text_OnDestroy
* Inputs:
*       HWND hwnd:
*       HWND destroy:
* Result: void
*       
* Effect: 
*       After forwarding the message to the default procedure, posts a
*       message to the main window suggesting that it should check for
*       the proper menu being posted.  
****************************************************************************/

static void 
text_OnDestroy(HWND hwnd)
    {
     FORWARD_WM_DESTROY(hwnd, DefMDIChildProc);
     PostMessage(hMainFrame, UWM_MDI_DESTROY, 0, 0);
    }

/****************************************************************************
*                                 text_OnFont
* Inputs:
*       HWND  hwnd: Window handle
* Result: void
*       
* Effect: 
*       Changes the font of the window
****************************************************************************/

static void 
text_OnFont(HWND hwnd)
    {
     edit_ChooseFont(GetDlgItem(hwnd, ID_EDIT_CONTROL));
    }

/****************************************************************************
*                                 text_OnEdit
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
text_OnEdit(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     switch(codeNotify)
        { /* codeNotify */
         case EN_CHANGE:
                 PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);
                 return;
        } /* codeNotify */
    }

/****************************************************************************
*                               text_OnCommand
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
text_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     switch (id) 
	{ /* id */
         case IDM_FONT:
                 text_OnFont(hwnd);
                 return ;
         case ID_EDIT_UNDO:
                 FORWARD_WM_UNDO(GetDlgItem(hwnd, ID_EDIT_CONTROL), 
		 						SendMessage);
                 return;
         case ID_EDIT_REDO:
                 return;
         case ID_EDIT_CUT:
                 FORWARD_WM_CUT(GetDlgItem(hwnd, ID_EDIT_CONTROL), 
		 						SendMessage);
                 return;
         case ID_EDIT_COPY:
                 FORWARD_WM_COPY(GetDlgItem(hwnd, ID_EDIT_CONTROL), 
		 						SendMessage);
                 return;
         case ID_EDIT_PASTE:
                 FORWARD_WM_PASTE(GetDlgItem(hwnd, ID_EDIT_CONTROL), 
		 						SendMessage);
                 return;
         case ID_EDIT_CLEAR:
                 FORWARD_WM_CLEAR(GetDlgItem(hwnd, ID_EDIT_CONTROL), 
		 						SendMessage);
                 return;
         case ID_EDIT_SELECT_ALL:
                 Edit_SetSel(GetDlgItem(hwnd, ID_EDIT_CONTROL), 0, -1);
                 return;
         case ID_EDIT_PROPERTIES:
                 return;
         case ID_EDIT_CONTROL:
                 text_OnEdit(hwnd, id, hctl, codeNotify);
                 return;
	} /* id */
     FORWARD_WM_COMMAND(hwnd, id, hctl, codeNotify, DefMDIChildProc);
    }

/****************************************************************************
*                                 text_OnSize
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
text_OnSize(HWND hwnd, UINT state, int cx, int cy)
    {
     if(state != SIZEICONIC)
        MoveWindow(GetDlgItem(hwnd, ID_EDIT_CONTROL), 0, 0, cx, cy, TRUE);
     FORWARD_WM_SIZE(hwnd, state, cx, cy, DefMDIChildProc);
    }

/****************************************************************************
*                            text_OnInitMenuPopup
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
text_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu)
    {
     PostMessage(hwnd, UWM_UPDATE_MENU, 0, 0);
    }

/****************************************************************************
*                              text_OnUpdateMenu
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
text_OnUpdateMenu(HWND hwnd, HMENU hmenu)
    {
     HWND hedit = GetDlgItem(hwnd, ID_EDIT_CONTROL);

     edit_UpdateMenu(hedit, hmenu);
     
     PostMessage(hMainFrame, UWM_UPDATE_TOOLBAR, 0, 0);
    }

/****************************************************************************
*                               text_OnSetFocus
* Inputs:
*       HWND hwnd: Window receiving focus
*       HWND oldfocus: Window losing focus
* Result: void
*       
* Effect: 
*       Sets focus to edit control which is contained inside the MDI child
****************************************************************************/

static void 
text_OnSetFocus(HWND hwnd, HWND oldfocus)
    {
     SetFocus(GetDlgItem(hwnd, ID_EDIT_CONTROL));
    }

/****************************************************************************
*			      text_OnCtlColorEdit
* Inputs:
*       HWND hwnd: Window handle of parent window
*       HDC hdc: Display context for child window
*       HWND hchild: Window handle of child window
*       int type: CTLCOLOR_EDIT is the only value we care about
* Result: HBRUSH
*       Background brush, COLOR_BACKGROUND, always
*       NULL, always -- use default brush for class
* Effect: 
*       Sets the text color
****************************************************************************/

static HBRUSH 
text_OnCtlColorEdit(HWND hwnd, HDC hdc, HWND hchild, int type)
    {
     return FORWARD_WM_CTLCOLOREDIT(hchild, hdc, hchild, SendMessage);
    }

/****************************************************************************
*                             text_OnContextMenu
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
text_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos)
    {
     return mdi_OnContextMenu(hwnd, hwndCtl, xPos, yPos, textContextMenu);
    }

/****************************************************************************
*                               text_QueryClose
* Inputs:
*       HWND hwnd: Window handle
* Result: BOOL
*       TRUE if window can close
*	FALSE if it can't
* Effect: 
*       If there is a change in the edit control, pops up a message box
*	asking for confirmation.
****************************************************************************/

static BOOL
text_QueryClose(HWND hwnd)
    {
     BOOL modified = Edit_GetModify(GetDlgItem(hwnd, ID_EDIT_CONTROL));
     TCHAR caption[256];
     TCHAR changed[256];

     if(!modified)
	return TRUE;
     VERIFY(LoadString(GetWindowInstance(hwnd), IDS_SAVE_CHANGES_CAPTION,
     				caption, DIM(caption)));
     VERIFY(LoadString(GetWindowInstance(hwnd), IDS_SAVE_CHANGES,
     				changed, DIM(changed)));

     // We use a more general structure here so that in the future, we
     // could issue a box that said "Save changes [Yes][No][Cancel]" and
     // do a multiway branch based on the three possible return values.

     switch(MessageBox(hwnd, changed, caption, MB_ICONSTOP | MB_YESNO))
        { /* MessageBox */
	 case IDYES:
		 return TRUE;
	 case IDNO:
		 return FALSE;
	} /* MessageBox */

     return FALSE; // should never get here, but just in case...
     
    }

/****************************************************************************
*                           text_OnQueryEndSession
* Inputs:
*       HWND hwnd: Window handle
* Result: BOOL
*       TRUE if session can be terminated
*	FALSE if session must not terminate
* Effect: 
*       If necessary, pops up a MessageBox indicating text window has changed
****************************************************************************/

static BOOL 
text_OnQueryEndSession(HWND hwnd)
    {
     return text_QueryClose(hwnd);
    }

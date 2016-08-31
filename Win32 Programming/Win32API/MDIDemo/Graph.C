#include "StdSDK.h"     // Standard application includes
#include <math.h>
#include "Initialization.h"       // For non-static function prototypes
#include "Frame.h"      // For non-static function prototypes
#include "resource.h"   // For resource identifiers
#include "mdi.h"
#include "error.h"
#include "findwnd.h"


static void graph_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify);
static int  graph_OnCreate(HWND hwnd, LPCREATESTRUCT cs);
static BOOL graph_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos);
static void graph_OnDestroy(HWND hwnd);
static void graph_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu);
static void graph_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, HWND hDeactivate);
static void graph_OnPaint(HWND hwnd);


static HMENU graphMenu = NULL;
static HMENU graphContextMenu = NULL;
static HACCEL graphAccel;
static HMENU graphWindowMenu = NULL;

#define COLOR_OFFSET 0

__inline WORD getCurrentColor(HWND hwnd) 
                            { return GetWindowWord(hwnd, COLOR_OFFSET); }
__inline WORD setCurrentColor(HWND hwnd, WORD id) 
			    { return SetWindowWord(hwnd, COLOR_OFFSET, id); }

/****************************************************************************
*                                graphWndProc
* Inputs:
*       HWND hwnd: Window handle
*       UINT message: Message ID
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: LRESULT
*       As per message spec
* Effect: 
*       ²
****************************************************************************/

LRESULT CALLBACK graphWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
     switch(message)
        { /* message */
         HANDLE_MSG(hwnd, WM_MDIACTIVATE, graph_OnMDIActivate);
         HANDLE_MSG(hwnd, WM_DESTROY, graph_OnDestroy);
         HANDLE_MSG(hwnd, WM_PAINT, graph_OnPaint);
         HANDLE_MSG(hwnd, WM_COMMAND, graph_OnCommand);
	 HANDLE_MSG(hwnd, WM_CREATE, graph_OnCreate);
	 HANDLE_MSG(hwnd, WM_INITMENUPOPUP, graph_OnInitMenuPopup);
         case UWM_CONTEXTMENU:
                 return HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, graph_OnContextMenu);
        } /* message */
     return DefMDIChildProc(hwnd, message, wParam, lParam);
    }

/****************************************************************************
*                             graph_InitInstance
* Inputs:
*       HINSTANCE hinst: Instance handle
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Registers the graphical window class, and loads its menu to a local
*       static variable.
****************************************************************************/

BOOL graph_InitInstance(HINSTANCE hinst)
   {
    if(!registerMDIChild(hinst, IDR_GRAPHIC, graphWndProc, sizeof(WORD)))
       return FALSE;

    graphMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_GRAPHIC));
    graphContextMenu = LoadMenu(hinst, MAKEINTRESOURCE(IDR_GRAPHIC_CONTEXT));

    if(graphMenu == NULL)
       return FALSE;

    graphAccel = LoadAccelerators(hinst, MAKEINTRESOURCE(IDR_GRAPHIC));
    graphWindowMenu = findWindowMenu(graphMenu);

    return TRUE;
   }


/****************************************************************************
*                             graph_ExitInstance
* Result: void
*       
* Effect: 
*       Does whatever cleanup is necessary.  In this case, destroys the
*       menu handle.
****************************************************************************/

void graph_ExitInstance()
    {
     DestroyMenu(graphMenu);
     DestroyMenu(graphContextMenu);
    }

/****************************************************************************
*                                  graph_New
* Inputs:
*       HWND hwnd: Window handle
* Result: BOOL
*       TRUE if successful
*       FALSE if failure
* Effect: 
*       Creates a new graphic window
****************************************************************************/

BOOL graph_New(HWND hwnd)
    {
     HWND hwndChild ;
     
     hwndChild = mdi_Create(MDIClientWnd, 0, IDR_GRAPHIC, IDS_GRAPHIC);

     return (hwndChild != NULL);
    }

/****************************************************************************
*                              graph_OnSetFocus
* Inputs:
*       HWND hwnd: Window receiving focus
*       HWND oldfocus: Window losing focus (ignored)
* Result: void
*       
* Effect: 
*       Posts the graph menu if that is not the current menu
****************************************************************************/

static void graph_OnMDIActivate(HWND hwnd, BOOL active, HWND hActivate, 
							HWND hDeactivate)
    {
     if(active)
        { /* activate */
         FORWARD_WM_MDISETMENU(MDIClientWnd, TRUE, graphMenu, 
	 				graphWindowMenu, SendMessage);
         SendMessage(hMainFrame, UWM_SET_ACCELERATOR, 0, (LPARAM)graphAccel);
         DrawMenuBar(hMainFrame);
         PostMessage(hMainFrame, UWM_UPDATE_TOOLBAR, 0, 0);
        } /* activate */
        
    }

/****************************************************************************
*                               graph_OnCreate
* Inputs:
*       HWND hwnd: Window reference
*	LPCREATESTRUCT cs: Creation structure
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Initializes the window-extra value to start with a red graph
****************************************************************************/

static BOOL graph_OnCreate(HWND hwnd, LPCREATESTRUCT cs)
    {
     SetWindowWord (hwnd, 0, (WORD)IDM_RED);
     return FORWARD_WM_CREATE(hwnd, cs, DefMDIChildProc) == 0;
    }

/****************************************************************************
*                             graph_OnDestroy
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

static void graph_OnDestroy(HWND hwnd)
    {
     FORWARD_WM_DESTROY(hwnd, DefMDIChildProc);
     PostMessage(hMainFrame, UWM_MDI_DESTROY, 0, 0);
    }

/****************************************************************************
*                                graph_OnPaint
* Inputs:
*       HWND hwnd: Handle to window
* Result: void
*       
* Effect: 
*       Paints the graphic window
****************************************************************************/

static void graph_OnPaint(HWND hwnd)
    {
     COLORREF    Color ;
     double      PI ;
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ;
     WORD        ColorID ;
     HPEN        hpen ;
     HPEN        hpenPrev ;
     int         xPos ;
     int         yPos ;

     /* Get PI to the maximum precision supported by the RTL. */
     PI = 4.0 * atan (1.0) ;
     hdc = BeginPaint (hwnd, &ps) ;
     GetClientRect (hwnd, &rect) ;
     SetMapMode (hdc, MM_ANISOTROPIC) ;
     SetWindowOrgEx (hdc, 0, 0, NULL) ;
     SetViewportOrgEx (hdc, 0, rect.bottom / 2, NULL) ;
     SetWindowExtEx (hdc, 100, 100, NULL) ;
     SetViewportExtEx (hdc, rect.right, -rect.bottom, NULL) ;
     MoveToEx (hdc, 0, 0, NULL) ;
     LineTo (hdc, 100, 0) ;
     MoveToEx (hdc, 0, 0, NULL) ;
     /* Create a pen of the appropriate color. */
     ColorID = getCurrentColor(hwnd) ;
     switch (ColorID) 
        { /* ColorID */
         case IDM_RED:
             Color = RGB (255, 0, 0) ;
             break ;
         case IDM_GREEN:
             Color = RGB (0, 255, 0) ;
             break ;
         case IDM_BLUE:
             Color = RGB (0, 0, 255) ;
             break ;
         case IDM_YELLOW:
             Color = RGB (255, 255, 0) ;
             break ;
         default:
             Color = RGB (0, 0, 0) ;
             break ;
        } /* ColorID */ /* wColor */
     hpen = CreatePen (PS_SOLID, 10, Color) ;
     hpenPrev = SelectPen (hdc, hpen) ;
     for (xPos = 0; xPos < 100; xPos++) 
        { /* loop */
         yPos = (int) (50.0 * sin (((double) xPos * PI) / 25.0)) ;
         LineTo (hdc, xPos, yPos) ;
        } /* loop */ /* loop */
     SelectPen (hdc, hpenPrev) ;
     DeletePen (hpen) ;
     EndPaint (hwnd, &ps) ;

    }

/****************************************************************************
*                               graph_OnCommand
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
graph_OnCommand(HWND hwnd, int id, HWND hctl, UINT codeNotify)
    {
     switch (id) 
         { /* id */
          case IDM_RED:
          case IDM_GREEN:
          case IDM_BLUE:
          case IDM_YELLOW:
		  setCurrentColor(hwnd, (WORD)id);
		  InvalidateRect (hwnd, NULL, TRUE) ;
		  return ;
         } /* id */ /* id */
    }

/****************************************************************************
*                             graph_OnContextMenu
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
graph_OnContextMenu(HWND hwnd, HWND hwndCtl, int xPos, int yPos)
    {
     return mdi_OnContextMenu(hwnd, hwndCtl, xPos, yPos, graphContextMenu);
    }

/****************************************************************************
*                                setColorMenu
* Inputs:
*	HWND hwnd: window handle of graphic window
*	HMENU hmenu: Menu handle of popup menu (may be context menu!)
*       int id: ID of menu item to set
* Result: void
*       
* Effect: 
*       If the current color is the id, check the menu item, else uncheck it
****************************************************************************/

static void setColorMenu(HWND hwnd, HMENU hmenu, int id)
    {
     CheckMenuItem(hmenu, id, 
     		   (getCurrentColor(hwnd) == id ? MF_CHECKED : MF_UNCHECKED));
    }

/****************************************************************************
*                            graph_OnInitMenuPopup
* Inputs:
*       HWND hwnd: Window handle of graph window
*	HMENU hmenu: Menu handle of menu about to be popped up
*	UINT item: ignored
*	BOOL sysmenu: TRUE if system menu, FALSE if not
* Result: void
*       
* Effect: 
*       Checks the appropriate options in the menu
****************************************************************************/

static void graph_OnInitMenuPopup(HWND hwnd, HMENU hmenu, UINT item, BOOL sysmenu)
    {
     if(sysmenu)
	return;  // don't need to do anything to system menu

     setColorMenu(hwnd, hmenu, IDM_RED);
     setColorMenu(hwnd, hmenu, IDM_GREEN);
     setColorMenu(hwnd, hmenu, IDM_BLUE);
     setColorMenu(hwnd, hmenu, IDM_YELLOW);
    }

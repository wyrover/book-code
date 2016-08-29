#include "StdSDK.h"             // Standard application includes
#include "About.h"              // For non-static function prototypes
#include "Initialization.h"     // For non-static function prototypes
#include "DiskHandling.h"       // For non-static function prototypes
#include "resource.h"           // For resource identifiers

//
// Function prototypes for static functions
//

static BOOL mainFrame_DisplayContextMenu (HWND hwnd, POINT pt) ;
static BOOL mainFrame_IntersectRect (LPRECT rect1, LPRECT rect2) ;
static void mainFrame_ResetDisks (HWND hwnd) ;
static void mainFrame_SetHanoiMappingMode (HWND hwnd, int cxClient, int cyClient) ;
static void mainFrame_SolveProblem (HWND hwnd) ;


//
// Function prototypes for callback functions
//

BOOL    CALLBACK mainFrame_SysColorChangeEnumeration (HWND hwnd, LPARAM lParam) ;
LRESULT CALLBACK mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;


//
// Function prototypes for message handlers
//

static void mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;
static BOOL mainFrame_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos) ;
static BOOL mainFrame_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct) ;
static void mainFrame_OnDestroy (HWND hwnd) ;
static void mainFrame_OnDisplayChange (HWND hwnd, UINT cBitsPerPixel, UINT cxScreen, UINT cyScreen) ;
static void mainFrame_OnNCRButtonUp (HWND hwnd, int x, int y, UINT codeHitTest) ;
static BOOL mainFrame_OnNotify (HWND hwnd, int idFrom, NMHDR FAR* pnmhdr) ;
static void mainFrame_OnPaint (HWND hwnd) ;
static void mainFrame_OnPrintClient (HWND hwnd, HDC hdc, UINT uFlags) ;
static void mainFrame_OnRButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) ;
static void mainFrame_OnSettingChange (HWND hwnd, UINT uiFlag, LPCTSTR pszMetrics) ;
static void mainFrame_OnSize (HWND hwnd, UINT state, int cx, int cy) ;
static void mainFrame_OnSysColorChange(HWND hwnd) ;
static void mainFrame_OnUserChanged (HWND hwnd) ;
static void mainFrame_OnWinIniChange (HWND hwnd, LPCTSTR pszSection) ;


//
// Typedefs
//


//
// Function prototypes for callback functions
//

BOOL    CALLBACK mainFrame_SysColorChangeNotification (HWND hwnd, LPARAM lParam) ;
LRESULT CALLBACK mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;

//
//  LRESULT CALLBACK
//  mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  hwnd            Handle of window to which this message applies
//  message         Message number
//  wParam          Message parameter
//  lParam          Message parameter
//
//  PURPOSE:  Processes messages for the main window.
//
//  MESSAGES:
//
//  WM_COMMAND          - notification from the menu or controls
//  WM_CONTEXTMENU      - request to display a context menu
//  WM_CREATE           - notification that a window is being created
//  WM_DESTROY          - window is being destroyed
//  WM_DISPLAYCHANGE    - display resolution change notification
//  WM_NCRBUTTONUP      - right button release in non-client area
//  WM_NOTIFY           - notifcation from a common control
//  WM_PAINT            - redraw all or part of the client area
//  WM_PRINTCLIENT      - request to draw all of client area into provided DC
//  WM_RBUTTONDOWN      - right button click in client area
//  WM_SETTINGCHANGE    - system parameter change notification
//  WM_SIZE             - window size has changed
//  WM_SYSCOLORCHANGE   - system color setting change notification
//  WM_USERCHANGED      - user log in/out notification
//

LRESULT CALLBACK
mainFrameWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
        case WM_COMMAND:                    // Notification from menu or control
            return HANDLE_WM_COMMAND (hwnd, wParam, lParam, mainFrame_OnCommand) ;

        case WM_CONTEXTMENU:                // Request to display a context menu
            return HANDLE_WM_CONTEXTMENU (hwnd, wParam, lParam, mainFrame_OnContextMenu) ;

        case WM_CREATE:                     // Notification that a window is being created
            return HANDLE_WM_CREATE (hwnd, wParam, lParam, mainFrame_OnCreate) ;

        case WM_DESTROY:                    // Window is being destroyed
            return HANDLE_WM_DESTROY (hwnd, wParam, lParam, mainFrame_OnDestroy) ;

        case WM_DISPLAYCHANGE:              // Only comes through on plug'n'play systems
            return HANDLE_WM_DISPLAYCHANGE (hwnd, wParam, lParam, mainFrame_OnDisplayChange) ;

        case WM_NCRBUTTONUP:                // Right click on windows non-client area...
            return HANDLE_WM_NCRBUTTONUP (hwnd, wParam, lParam, mainFrame_OnNCRButtonUp) ;

        case WM_NOTIFY:                     // Notification from a Common Control
            return HANDLE_WM_NOTIFY (hwnd, wParam, lParam, mainFrame_OnNotify) ;

        case WM_PAINT:                      // Draw all or part of client area
            return HANDLE_WM_PAINT (hwnd, wParam, lParam, mainFrame_OnPaint) ;

        case WM_PRINTCLIENT:                // Draw all of client area into provided DC
            return HANDLE_WM_PRINTCLIENT (hwnd, wParam, lParam, mainFrame_OnPrintClient) ;

        case WM_RBUTTONDOWN:                // Right click in windows client area...
            return HANDLE_WM_RBUTTONDOWN (hwnd, wParam, lParam, mainFrame_OnRButtonDown) ;

        case WM_SETTINGCHANGE:              // An application changed a systemwide setting
//      case WM_WININICHANGE:               // A WIN.INI setting has changed
            return HANDLE_WM_SETTINGCHANGE (hwnd, wParam, lParam, mainFrame_OnSettingChange) ;

        case WM_SIZE:                       // Window size has changed
            return HANDLE_WM_SIZE (hwnd, wParam, lParam, mainFrame_OnSize) ;

        case WM_SYSCOLORCHANGE:             // A change has been made to a system color setting
            return HANDLE_WM_SYSCOLORCHANGE (hwnd, wParam, lParam, mainFrame_OnSysColorChange) ;

        case WM_USERCHANGED:                // User logged in or out
            return HANDLE_WM_USERCHANGED (hwnd, wParam, lParam, mainFrame_OnUserChanged) ;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam) ;
    }
}


//
//  void mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
//
//  hwnd            Handle of window to which this message applies
//  id              Specifies the identifier of the menu item, control, or accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a control.
//                  This parameter is 1 when the message is from an accelerator.
//                  This parameter is 0 when the message is from a menu.                  
//
//  PURPOSE:        
//
//  COMMENTS:
//

static void
mainFrame_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {

        case ID_APP_ABOUT:
                doAbout(hwnd);
                return ;

        case ID_APP_EXIT:
                DestroyWindow (hwnd) ;
                return ;

        case ID_HELP_HELP_TOPICS:
            {

             BOOL bGotHelp = WinHelp (hwnd, getHelpFileName (), HELP_FINDER, (DWORD) 0) ;
             if (!bGotHelp)
                 MessageBox (GetFocus(), TEXT("Unable to activate help"), getAppName (), MB_OK|MB_ICONHAND) ;
            }
            return ;

        case IDM_HELP_HOWTO:
		doHow(hwnd);
		return ;

        case IDM_RESET_DISKS:
            mainFrame_ResetDisks (hwnd) ;
            return ;

        case IDM_SOLVE_PROBLEM:
            {
            HMENU hmenu = GetMenu (hwnd) ;
            EnableMenuItem (hmenu, IDM_RESET_DISKS,   MF_DISABLED | MF_GRAYED) ;
            EnableMenuItem (hmenu, IDM_SOLVE_PROBLEM, MF_DISABLED | MF_GRAYED) ;
            DrawMenuBar (hwnd) ;
            mainFrame_SolveProblem (hwnd) ;
            EnableMenuItem (hmenu, IDM_RESET_DISKS,   MF_ENABLED) ;
            EnableMenuItem (hmenu, IDM_SOLVE_PROBLEM, MF_ENABLED) ;
            DrawMenuBar (hwnd) ;
            }
            return ;

        default:
            FORWARD_WM_COMMAND (hwnd, id, hwndCtl, codeNotify, DefWindowProc) ;
        }
}


//
//  BOOL mainFrame_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos)
//
//  hwnd            Handle of window to which this message applies
//  hwndCtl         Handle of the window in which the user right clicked the mouse
//                  This may be the frame window itself or a control.
//  xPos            Horizontal position of the cursor, in screen coordinates
//  yPos            Vertical position of the cursor, in screen coordinates
//
//  PURPOSE:        Notification that the user clicked the right
//                  mouse button in the window.
//
//  COMMENTS:       Normally a window processes this message by
//                  displaying a context menu using the TrackPopupMenu
//                  or TrackPopupMenuEx functions. If a window does not
//                  display a context menu it should pass this message
//                  to the DefWindowProc function. 
//

static BOOL
mainFrame_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos)
{
    POINT               pt = { xPos, yPos } ;   // location of mouse click 
    RECT                rc ;                    // client area of window 
 
    // Get the bounding rectangle of the client area. 
    GetClientRect (hwnd, &rc) ;
 
    // Convert the mouse position to client coordinates. 
    ScreenToClient (hwnd, &pt) ;
 
    // If the mouse click was in the client area,
    // display the appropriate floating popup menu.
    if (PtInRect (&rc, pt))
        if (mainFrame_DisplayContextMenu (hwnd, pt))
            return TRUE ;
 
    // Otherwise forward the message for default processing
    return FORWARD_WM_CONTEXTMENU (hwnd, hwndCtl, xPos, yPos, DefWindowProc) ;
}


//
//  BOOL mainFrame_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct)
//
//  hwnd            Handle of window to which this message applies
//  lpCreateStruct  Points to a CREATESTRUCT structure that contains
//                  information about the window being created
//
//  PURPOSE:        Perform any per-window initialization in response
//                  to this message, such as creating any desired
//                  child windows such as toolbars and status bars.
//
//  COMMENTS:       Windows sends this message after the window is
//                  created, but before the window becomes visible.
//                  Return TRUE to continue creation of the window; 
//                  otherwise return FALSE to fail the window creation.
//

static BOOL
mainFrame_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    /***************************************************/
    /* The main application window is being created.   */
    /* Create the five windows representing the disks. */
    /***************************************************/

    mainFrame_CreateDiskWindows (hwnd) ;

    return TRUE ;
}


//
//  void mainFrame_OnDestroy (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Notification that the specified window is being destroyed.
//                  The window is no longer visible to the user.
//
//  COMMENTS:
//

static void
mainFrame_OnDestroy (HWND hwnd)
{
    // Tell WinHelp we don't need it any more...
    WinHelp (hwnd, getHelpFileName (), HELP_QUIT, (DWORD) 0) ;
    PostQuitMessage (0) ;
}


//
//  void mainFrame_OnDisplayChange (HWND hwnd, UINT cBitsPerPixel,
//                                  UINT cxScreen, UINT cyScreen)
//
//  hwnd            Handle of window to which this message applies
//  cBitsPerPixel   Specifies the new image depth of the display
//                  in bits per pixel.
//  cxScreen        Specifies the new horizontal resolution of the screen.
//  cyScreen        Specifies the new vertical resolution of the screen.
//
//  PURPOSE:        Windows calls this handler when the display
//                  resolution has changed.
//
//  COMMENTS:       Beta versions of Windows 95 sent this message twice,
//                  once in anticipation of a change to the display
//                  resolution and once after the display resolution
//                  changed. The cBitsPerPixel parameter was a Boolean
//                  value which indicated which notification applied.
//
//                  Windows 95 now only sends this message after the
//                  display resolution has changed and the cBitsPerPixel
//                  parameter has the semantics described above.
//
//  VERSION NOTES:  The WM_DISPLAYCHANGE message is implemented on Windows 95
//                  but not currently on the Windows NT.
//

static void
mainFrame_OnDisplayChange (HWND hwnd, UINT cBitsPerPixel, UINT cxScreen, UINT cyScreen)
{
    FORWARD_WM_DISPLAYCHANGE (hwnd, cBitsPerPixel, cxScreen, cyScreen, DefWindowProc) ;
}


//
//  void mainFrame_OnNCRButtonUp (HWND hwnd, int x, int y, UINT codeHitTest)
//
//  hwnd            Handle of window to which this message applies
//  x               Specifies the horizontal client coordinate of the mouse click.
//  y               Specifies the vertical client coordinate of the mouse click.
//  codeHitTest     Specifies the hit-test value returned by the DefWindowProc
//                  function as a result of processing the WM_NCHITTEST message. 
//
//  PURPOSE:        Windows calls this handler when the user releases the right
//                  mouse button while the cursor is in the non-client area of
//                  a window.
//
//  COMMENTS:       Windows 95 user interface guildines state that you should
//                  display a window popup menu in certain circumstances in
//                  response to this message. The window pop-up menu is
//                  the popup menu associated with a window, in this case,
//                  the main frame window. Do not mistake the window popup
//                  menu for the "Window" drop-down menu found in MDI
//                  applications. The window popup menu replaces the
//                  Windows 3.x Control menu, also referred to as the System
//                  menu.
//
//                  Note: DefWindowProc on Windows 95 processes this message
//                  by sending a WM_CONTEXTMENU message. Therefore all context
//                  menu processing under Windows 95 is normally performed
//                  in response to the WM_CONTEXTMENU message.

//                  The user displays a window’s popup menu by clicking the
//                  right mouse button anywhere in the title bar area, excluding
//                  the title bar icon. Clicking on the title bar icon with
//                  the right button displays the pop-up menu for the object
//                  represented by the icon.
//

static void
mainFrame_OnNCRButtonUp (HWND hwnd, int x, int y, UINT codeHitTest)
{
    switch (codeHitTest) {
        case HTSYSMENU:
            // Windows NT note:

            // The user just clicked the right mouse button on the application's
            // title bar icon. Normally you would now alter the default system menu
            // however your application requires. See the Explorer for an example.

            return ;

        case HTCAPTION:
        case HTREDUCE:
        case HTZOOM:
            // Windows NT note:

            // The user just clicked the right mouse button on the application's
            // title bar , excluding the title bar icon. Normally you would now
            // display the window popup menu.

            return ;
    }

    // Allow default message processing regardless
    FORWARD_WM_NCRBUTTONUP (hwnd, x, y, codeHitTest, DefWindowProc) ;
}


//
//  BOOL mainFrame_OnNotify (HWND hwnd, int idCtrl, NMHDR FAR* pnmhdr)
//
//  hwnd            Handle of window to which this message applies
//  idCtrl          Identifier of the control sending the message
//  pnmhdr          Pointer to structure that contains the notification
//                  code and additional information
//
//  PURPOSE:        An event has occurred in one of thie window's child
//                  controls or a control requires some kind of information
//
//  COMMENTS:
//

static BOOL
mainFrame_OnNotify (HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
        return TRUE ;
}


//
//  void mainFrame_OnPaint (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Windows calls this handler when the window needs repainting.
//
//  COMMENTS:
//

static void
mainFrame_OnPaint (HWND hwnd)
{
    HBRUSH              hbrush ;
    HDC                 hdc ;
    PAINTSTRUCT         ps ;
    RECT                rect ;

    /**********************************************************/
    /* The window needs to be redrawn.  Draw the three posts. */
    /**********************************************************/

    hdc = BeginPaint (hwnd, &ps) ;

    hbrush = GetStockObject (BLACK_BRUSH) ;

    SetRect (&rect,
        CENTERPOSTPOS - POSTWIDTH / 2, POSTHEIGHT,
        CENTERPOSTPOS + POSTWIDTH / 2, 0) ;

    if (mainFrame_IntersectRect (&rect, &ps.rcPaint))
        FillRect (hdc, &rect, hbrush) ;

    SetRect (&rect,
        LEFTPOSTPOS - POSTWIDTH / 2, POSTHEIGHT,
        LEFTPOSTPOS + POSTWIDTH / 2, 0) ;

    if (mainFrame_IntersectRect (&rect, &ps.rcPaint))
        FillRect (hdc, &rect, hbrush) ;

    SetRect (&rect,
        RIGHTPOSTPOS - POSTWIDTH / 2, POSTHEIGHT,
        RIGHTPOSTPOS + POSTWIDTH / 2, 0) ;

    if (mainFrame_IntersectRect (&rect, &ps.rcPaint))
        FillRect (hdc, &rect, hbrush) ;

    EndPaint (hwnd, &ps) ;
}


//
//  void mainFrame_OnPrintClient (HWND hwnd, HDC hdc, UINT uFlags)
//
//  hwnd            Handle of window to which this message applies
//  hdc             Handle of device context in which to draw client area
//  uFlags          Drawing flags from WM_PRINT message
//
//  PURPOSE:        Draw the client area into the provided device context
//                  which is typically a printer device context. You should
//                  drawing the entire client area, unlike WM_PRINT processing
//                  where you draw only the invalid area.
//
//  COMMENTS:
//
//  VERSION NOTES:  The WM_PRINTCLIENT message is implemented on Windows 95
//                  but not currently on the Windows NT.
//

static void
mainFrame_OnPrintClient (HWND hwnd, HDC hdc, UINT uFlags)
{
    // Your drawing code goes here
}


//
//  void mainFrame_OnRButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
//
//  hwnd            Handle of window to which this message applies
//  fDoubleClick    TRUE when this is a double-click notification,
//                  FALSE when this is a single-click notification.
//  x               Specifies the horizontal client coordinate of the mouse click
//  y               Specifies the vertical client coordinate of the mouse click
//  keyFlags        Flags indicating the state of some virtual keys
//
//  PURPOSE:        Windows calls this handler when the user presses the right
//                  mouse button while the cursor is in the client area of a window.
//
//  COMMENTS:       Windows 95 user interface guidelines recommend you display
//                  the context menu for the item in the client area upon which
//                  the user just clicked.
//                  
//

static void
mainFrame_OnRButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
        POINT               pt = { x, y } ;

    mainFrame_DisplayContextMenu (hwnd, pt) ;
}


//
//  void mainFrame_OnSettingChange (HWND hwnd, UINT uiFlag, LPCTSTR pszMetrics)
//  void mainFrame_OnWinIniChange  (HWND hwnd, UINT unused, LPCTSTR pszSection)
//
//                  WM_SETTINGCHANGE:
//  hwnd            Handle of window to which this message applies
//  uiFlag          Specifies the systemwide parameter that has changed.
//  pszMetrics      Points to the string "WindowMetrics" when certain
//                  systemwide parameters have changed
//
//                  WM_WININICHANGE:
//  hwnd            Handle of window to which this message applies
//  unused          Unused, must be zero.
//  pszSection      Points to a string containing the name
//                  of the section that has changed
//
//  PURPOSE:        This is the appropriate place to reload any system
//                  metrics that the application has cached.
//
//  COMMENTS:       Windows 95 sends the WM_SETTINGCHANGE message to all
//                  windows after an application changes a system-wide
//                  setting by calling the SystemParametersInfo function.
//
//                  Windows NT sends the WM_WININICHANGE message to all
//                  windows after an application changes a system-wide
//                  setting by calling the SystemParametersInfo function.
//
//                  On all operating systems, an application sends the
//                  WM_WININICHANGE message to all top-level windows
//                  after making a change to the WIN.INI file
//
//  VERSION NOTES:  Microsoft assigned the same message code (26) to both
//                  the WM_SETTINGCHANGE and the WM_WININICHANGE messages.
//                  The only way to distinguish the two messages is to
//                  test the uiFlag parameter. A zero value means this is
//                  a WM_WININICHANGE message and a non-zero value means
//                  this is a WM_SETTINGCHANGE message.
//

static void
mainFrame_OnSettingChange (HWND hwnd, UINT uiFlag, LPCTSTR pszMetrics)
{
}

//
//  void mainFrame_OnSize (HWND hwnd, UINT state, int cx, int cy)
//
//  hwnd            Handle of window to which this message applies
//  state
//  cx              New horizontal extent of window
//  cy              New vertical extent of window
//
//  PURPOSE:        Notify child windows that parent changed size.
//
//  COMMENTS:       
//

static void mainFrame_OnSize (HWND hwnd, UINT state, int cx, int cy)
{
    HWND                hwndDisk ;

    /***************************************/
    /* The size of the window has changed. */
    /* Map the logical coordinate system   */
    /* to the new extent of the window and */
    /* move the disks to their appropriate */
    /* position relative to the new size.  */
    /***************************************/
    
    /****************************/
    /* Set new scaling factors. */
    /****************************/

    mainFrame_SetHanoiMappingMode (hwnd, cx, cy) ;

    /**********************************/
    /* (Re)position each disk window. */
    /**********************************/

    hwndDisk = GetFirstChild (hwnd) ;
    while (hwndDisk) {
        disk_PositionDiskWindow (hwndDisk) ;
        hwndDisk = GetNextSibling (hwndDisk) ;
    }
}


//
//  void mainFrame_OnSysColorChange (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Forward the WM_SYSCOLORCHANGE message to all child
//                  windows of this top-level window, as well as to all
//                  children of the child windows, if any.
//
//  COMMENTS:       Windows sends the WM_SYSCOLORCHANGE message to all
//                  top-level windows after a change has been made to
//                  the system color settings. Your top level window
//                  must forward the WM_SYSCOLORCHANGE message to its
//                  common controls; otherwise the controls will not be
//                  notified of the color change.
//

static void
mainFrame_OnSysColorChange (HWND hwnd)
{
    EnumChildWindows (hwnd, mainFrame_SysColorChangeNotification, (LPARAM) NULL) ;
}

//
//  void mainFrame_SysColorChangeNotification (HWND hwnd, LPARAM lParam)
//
//  hwnd            Handle of child window
//  lParam          Application-defined value specified as the last
//                  parameter on the EnumChildWindows function call.
//
//  PURPOSE:        Forward a WM_SYSCOLORCHANGE message to the
//                  specified child window.
//
//  COMMENTS:
//

BOOL CALLBACK
mainFrame_SysColorChangeNotification (HWND hwnd, LPARAM lParam)
{
    // Forward the message to a child window
    FORWARD_WM_SYSCOLORCHANGE (hwnd, SendMessage) ;

    // Keep on enumerating...
    return TRUE ;

    lParam ;        // Parameter not referenced
}

//
//  void mainFrame_OnUserChanged (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        This is the appropriate place to reload any cached
//                  user-specific information.
//                  
//  COMMENTS:       Windows sends the WM_USERCHANGED message to all windows
//                  after a user has logged on or off. When a user logs on
//                  or off, the system updates the user-specific settings. 
//                  Windows sends the WM_USERCHANGED message immediately 
//                  after updating the user-specific settings. 
//
//  VERSION NOTES:  The WM_USERCHANGED message is implemented on Windows 95
//                  but not currently on the Windows NT.
//

static void
mainFrame_OnUserChanged (HWND hwnd)
{
}


//
//  BOOL mainFrame_DisplayContextMenu (HWND hwnd, POINT pt)
//
//  hwnd            Handle of window to which this message applies
//  pt              Location of mouse click in client coordinates
//
//  PURPOSE:        Display the appropriate context menu for the object
//                  located at 'pt' in the main frame window.
//
//  COMMENTS:
//

BOOL mainFrame_DisplayContextMenu (HWND hwnd, POINT pt)
{
    HMENU               hmenuBar, hmenuPopup ;
    int                 nItems ;

    // Determine the appropriate context menu to display.
    // generally using the application state and mouse click coordinates.

    // Bring up the 'Help' popup menu for lack of a better solution

    // Get main menu handle
    hmenuBar = GetMenu (hwnd) ;
    ASSERT (NULL != hmenuBar) ;

    // Get the count of items on the main menu
    nItems = GetMenuItemCount (hmenuBar) ;
    ASSERT (-1 != nItems) ;

    // ASSUMPTION: Help menu is the rightmost on the menu bar
    hmenuPopup = GetSubMenu (hmenuBar, nItems - 1) ;
    ASSERT (NULL != hmenuPopup) ;

    // Convert click location to screen coordinates
    ClientToScreen (hwnd, &pt) ;

    // Display the floating popup menu at the mouse click location
    // Track the right mouse as this function is called during
    // WM_CONTEXTMENU message processing.
    return TrackPopupMenu (hmenuPopup,
                           TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           pt.x, pt.y, 0, hwnd, NULL) ;
}

/*************************************************************/
/*                                                           */
/*                    SetHanoiMappingMode                    */
/*                                                           */
/*      Map a logical coordinate system of the form:         */
/*                                                           */
/*                           +2048 y                         */
/*            +------------------------------------|         */
/*            |                 ^                  |         */
/*            |                 |                  |         */
/*            |          W i n  |  d o w           |         */
/*            |                 |                  |         */
/*            |                 |                  |         */
/*            |<----------------|----------------->|         */
/*         -1024 x            (0,0)             +1023 x      */
/*                                                           */
/*      to the current physical size of the client area.     */
/*                                                           */
/*************************************************************/

static void
mainFrame_SetHanoiMappingMode (HWND hwnd, int cxClient, int cyClient)
{
    HDC                         hdc ;

    /***************************************************/
    /* (Re)initialize the device context mapping mode. */
    /***************************************************/

    hdc = GetDC (hwnd) ;

    // Use the anisotropic mapping mode.
    // Arbitrarily scaled x- and y-axes.

    SetMapMode (hdc, MM_ANISOTROPIC) ;

    // The origin of the window is (still) at point (0,0).

    SetWindowOrgEx (hdc, 0, 0, NULL) ;

    /************************************************************/
    /* Map the window origin to the point that is on the bottom */
    /* of the window vertically and in the middle of the window */
    /* horizontally.  Once the y-axis is inverted, the window   */
    /* displays quadrant one and two of a cartesian coordinate  */
    /* system.                                                  */
    /************************************************************/

    SetViewportOrgEx (hdc, cxClient / 2, cyClient, NULL) ;

    /********************************************************/
    /* Set the logical extent of the window to 2,048 units, */
    /* both vertically and horizontally.                    */
    /********************************************************/

    SetWindowExtEx (hdc, 2048, 2048, NULL) ;

    /******************************************************************/
    /* Map the 2,048 logical units to the current size of the window. */
    /* The negative y-extent parameter inverts the y-axis.            */
    /******************************************************************/

    SetViewportExtEx (hdc, cxClient, -cyClient, NULL) ;
    ReleaseDC (hwnd, hdc) ;
}

/*******************************************************************/
/*                  mainFrame_IntersectRect                        */
/* This performs the same function as the Windows IntersectRect()  */
/* except that this function expects the vertical axis to increase */
/* upwardly.  Windows' function expects it to increase downwardly  */
/* and doesn't correctly calculate the intersection.               */
/*******************************************************************/

static BOOL
mainFrame_IntersectRect (LPRECT rect1, LPRECT rect2)
{
    int                 nHighestLeft, nLowestRight ;
    int                 nHighestBottom, nLowestTop ;

    nHighestLeft = max (rect1->left, rect2->left) ;
    nLowestRight = min (rect1->right, rect2->right) ;

    if (nHighestLeft >= nLowestRight)
        return FALSE ;

    nHighestBottom = max (rect1->bottom, rect1->bottom) ;
    nLowestTop     = min (rect1->top, rect2->top) ;

    return (nHighestBottom >= nLowestTop ? FALSE : TRUE) ;
}

/*****************************************/
/*        mainFrame_ResetDisks           */
/* Place all disks back on the left peg. */
/*****************************************/

static void
mainFrame_ResetDisks (HWND hwnd)
{
    HWND                hwndDisk ;
    int                 i, nPostPos ;

    HowMany [FIRST]  = NUMDISKS ;
    HowMany [SECOND] = 0 ;
    HowMany [THIRD]  = 0 ;

    // Move all disks to the left peg.

    hwndDisk = GetFirstChild (hwnd) ;
    while (hwndDisk) {
        disk_SetPostNum (hwndDisk, FIRST) ;
        nPostPos = NUMDISKS - disk_GetDiskSize (hwndDisk) ;
        WhosOn [FIRST][nPostPos] = hwndDisk ;
        hwndDisk = GetNextSibling (hwndDisk) ;
    }
    for (i = 0; i < NUMDISKS; i++)
        disk_PositionDiskWindow (WhosOn [FIRST][i]) ;
}

/*****************************************/
/*        mainFrame_SolveProblem         */
/* Move the disks in the correct order.  */
/*****************************************/

static void
mainFrame_SolveProblem (HWND hwnd)
{
    /***************************************************************/
    /* Protect the current execution environment.  We raise an     */
    /* exception when the user closes the window while in the nd   */
    /* second level message loop.  We then terminate the program.  */
    /***************************************************************/

    __try {
        mainFrame_ResetDisks (hwnd) ;
        disk_MoveTower (NUMDISKS, FIRST, SECOND, THIRD) ;
    }
    __except (GetExceptionCode() == STATUS_TERMINATION_REQUEST) {
        PostQuitMessage (0) ;
    }
}

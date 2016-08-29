#include "StdSDK.h"             // Standard application includes
#include "Towers.h"             // Application specific declarations
#include "About.h"              // For non-static function prototypes
#include "DiskHandling.h"       // For non-static function prototypes
#include "resource.h"           // For resource identifiers

//
// Function prototypes for static functions
//

static BOOL disk_IsTopDisk (HWND hwndDisk) ;
static BOOL disk_AddDiskToPost (HWND hwnd, UINT PostNum) ;
static void disk_MoveOneDisk (UINT FromPeg, UINT ToPeg) ;
static BOOL disk_RemoveDiskFromPost (HWND hwnd) ;
static void disk_TwiddleThumbs (HWND hwnd, UINT Delay) ;


//
// Static data definitions
//

static POINT                    ptOrigPos ;
static RECT                     rectOrigPos ;

//
// Global data definitions
//

HWND WhosOn [THIRD + 1][NUMDISKS] ;
WORD HowMany [THIRD + 1] ;


//
// Function prototypes for message handlers
//

static BOOL disk_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct) ;
static void disk_OnLButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) ;
static void disk_OnLButtonUp (HWND hwnd, int x, int y, UINT keyFlags) ;
static void disk_OnMouseMove (HWND hwnd, int x, int y, UINT keyFlags) ;
static void disk_OnPaint (HWND hwnd) ;

//
// Typedefs
//


//
//  LRESULT CALLBACK
//  diskWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
//  WM_CREATE           - notification that a window is being created
//  WM_LBUTTONDOWN      - left button click in client area
//  WM_LBUTTONUP        - left button release in client area
//  WM_MOUSEMOVE        - mouse move in client area
//  WM_PAINT            - redraw all or part of the client area
//

LRESULT CALLBACK
diskWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
        case WM_CREATE:        // Notification that a window is being created
            return HANDLE_WM_CREATE (hwnd, wParam, lParam, disk_OnCreate) ;

        case WM_LBUTTONDOWN:   // Left click in windows client area...
            return HANDLE_WM_LBUTTONDOWN (hwnd, wParam, lParam, disk_OnLButtonDown) ;

        case WM_LBUTTONUP:    // Left button up in windows client area...
            return HANDLE_WM_LBUTTONUP (hwnd, wParam, lParam, disk_OnLButtonUp) ;

        case WM_MOUSEMOVE:    // Mouse move
            return HANDLE_WM_MOUSEMOVE (hwnd, wParam, lParam, disk_OnMouseMove) ;

        case WM_PAINT:        // Draw all or part of client area
            return HANDLE_WM_PAINT (hwnd, wParam, lParam, disk_OnPaint) ;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam) ;
    }
}


//
//  BOOL disk_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct)
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
disk_OnCreate (HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    /****************************************************/
    /* A disk is being created.  Place it on left post. */
    /****************************************************/

    disk_AddDiskToPost (hwnd, FIRST) ;

    return TRUE ;
}
//
//  void disk_OnLButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
//
//  hwnd            Handle of window to which this message applies
//  fDoubleClick    TRUE when a double click, FALSE when a single click
//  x               x coordinate of cursor
//  y               y coordinate of cursor
//  keyFlags        state of keys
//
//  PURPOSE:        The mouse cursor has moved.
//
//  COMMENTS:       
//

static void
disk_OnLButtonDown (HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    HWND hwndParent ;
    RECT rect ;

    /*******************************************************/
    /* The left mouse button is pressed while over a disk. */
    /* Prepare to drag the disk as long as it's depressed. */
    /*******************************************************/

    /*************************************************/
    /* Only allow the topmost disk(s) to be dragged. */
    /*************************************************/

    if (!disk_IsTopDisk (hwnd))
        return ;

    /************************************************************/
    /* Start tracking the cursor...                             */
    /* This entails remembering from where it all started...    */
    /************************************************************/

    ptOrigPos.x = x ;
    ptOrigPos.y = y ;
    ClientToScreen (hwnd, &ptOrigPos) ;
    GetWindowRect (hwnd, &rectOrigPos) ;

    /****************************************/
    /* Capturing the mouse so all future    */
    /* movements are sent to this window... */
    /****************************************/
    
    SetCapture (hwnd) ;

    /************************************************/
    /* And restricting its movement to              */
    /* within the client area of our parent window. */
    /************************************************/

    hwndParent = GetParent (hwnd) ;
    GetClientRect (hwndParent, &rect) ;
    ClientToScreen (hwndParent, (LPPOINT) &rect.left) ;
    ClientToScreen (hwndParent, (LPPOINT) &rect.right) ;
    ClipCursor (&rect) ;
}

//
//  void disk_OnLButtonUp (HWND hwnd, int x, int y, UINT keyFlags)
//
//  hwnd            Handle of window to which this message applies
//  x               x coordinate of cursor
//  y               y coordinate of cursor
//  keyFlags        state of keys
//
//  PURPOSE:        The mouse cursor has moved.
//
//  COMMENTS:       
//

static void
disk_OnLButtonUp (HWND hwnd, int x, int y, UINT keyFlags)
{
    HDC   hdc ;
    HWND  hwndParent ;
    POINT cursor ;
    RECT  rect ;
    UINT  Nearest ;

    /******************************************************/
    /* The left mouse button was released.  Stop dragging */
    /* the disk and place it on the nearest post.  When   */
    /* the nearest post is an illegal move, place the     */
    /* back in its original position.                     */
    /******************************************************/

    ReleaseCapture () ;
    ClipCursor (NULL) ;

    /********************************************************/
    /* Determine the coordinates of the center of the disk. */
    /********************************************************/

    GetWindowRect (hwnd, &rect) ;
    cursor.x = (rect.left + rect.right) / 2 ;
    cursor.y = (rect.top + rect.bottom) / 2 ;

    /* Convert screen coordinates to client coordinates. */

    hwndParent = GetParent (hwnd) ;
    ScreenToClient (hwndParent, &cursor) ;

    /* Convert client coordinates to logical coordinates. */

    hdc = GetDC (hwndParent) ;
    DPtoLP (hdc, &cursor, 1) ;
    ReleaseDC (hwndParent, hdc) ;

    /**************************/
    /* Find the nearest post. */
    /**************************/

    if (cursor.x < LEFTPOSTPOS / 2)
        Nearest = FIRST ;
    else if (cursor.x > RIGHTPOSTPOS / 2)
        Nearest = THIRD ;
    else
        Nearest = SECOND ;

    /***************************************************************/
    /* Remove the disk from its current post, if possible.         */
    /* If successful, place it on the nearest post.                */
    /* If the add fails, then put the disk back on the orig. post. */
    /***************************************************************/

    if (disk_RemoveDiskFromPost (hwnd) &&
        !disk_AddDiskToPost (hwnd, Nearest))
        disk_AddDiskToPost (hwnd, disk_GetPostNum (hwnd)) ;

    disk_PositionDiskWindow (hwnd) ;
}


//
//  void disk_OnMouseMove (HWND hwnd, int x, int y, UINT keyFlags)
//
//  hwnd            Handle of window to which this message applies
//  x               x coordinate of cursor
//  y               y coordinate of cursor
//  keyFlags        state of keys
//
//  PURPOSE:        The mouse cursor has moved.
//
//  COMMENTS:       
//

static void
disk_OnMouseMove (HWND hwnd, int x, int y, UINT keyFlags)
{
    HWND  hwndParent ;
    POINT cursor ;
    RECT  rect ;

    /*******************************************************/
    /* We may be dragging the disk.  If so, calculate the  */
    /* displacement from the starting position of the drag */
    /* to the current position of the cursor and move the  */
    /* disk so that the position that was under the cursor */
    /* when the drag started is under the cursor now.      */
    /*******************************************************/

    if (hwnd == GetCapture ()) {

        /*******************************************/
        /* Get the current position of the cursor. */
        /*******************************************/

        cursor.x = x ;
        cursor.y = y ;
        ClientToScreen (hwnd, &cursor) ;

        /*******************************************/
        /* Calculate the new position of the disk. */
        /*******************************************/

        rect = rectOrigPos ;
        OffsetRect (&rect, cursor.x - ptOrigPos.x,
                           cursor.y - ptOrigPos.y) ;
        hwndParent = GetParent (hwnd) ;
        ScreenToClient (hwndParent, (LPPOINT) &rect.left) ;
        ScreenToClient (hwndParent, (LPPOINT) &rect.right) ;
        
        /**************************************/
        /* Move the disk to its new position. */
        /**************************************/
        
        MoveWindow (hwnd,
                    rect.left,
                    rect.top,
                    rect.right - rect.left,
                    rect.bottom - rect.top, TRUE) ;

        /********************************************************/
        /* Force the parent window's client area to be redrawn. */
        /* This is necessary when the disk moves off of a peg   */
        /* so that ugly white gaps in the peg aren't left for   */
        /* indeterminate periods of time.  They eventually get  */
        /* redrawn anyway but this makes it look nicer.         */
        /********************************************************/

        UpdateWindow (hwndParent) ;
    }
}

//
//  void MainFrame_OnPaint (HWND hwnd)
//
//  hwnd            Handle of window to which this message applies
//
//  PURPOSE:        Windows calls this handler when the window needs repainting.
//
//  COMMENTS:
//

static void
disk_OnPaint (HWND hwnd)
{
    HDC                 hdc ;
    PAINTSTRUCT         ps ;
    RECT                rect ;
    HBRUSH              hbrush, hbrushOld ;

    hdc = BeginPaint (hwnd, &ps) ;

    hbrush = CreateSolidBrush (RGB (238, 120, 0)) ;
    hbrushOld = SelectBrush (hdc, hbrush) ;

    GetClientRect (hwnd, &rect) ;
    RoundRect (hdc, rect.left, rect.top, rect.right, rect.bottom,
        (rect.right - rect.left) / 5,
        (rect.top - rect.bottom) / 5) ;

    hbrush = SelectBrush (hdc, hbrushOld) ;
    DeleteBrush (hbrush) ;

    EndPaint (hwnd, &ps) ;
 }
    
/*************************************************************/
/*                                                           */
/*                       AddDiskToPost                       */
/*                                                           */
/*      Add a disk to the indicated post.  A disk can only   */
/*      added to the top of the stack of disks on a post (if */
/*      any).  Additionally a larger disk may not be placed  */
/*      on top of a smaller disk. Return TRUE when the disk  */
/*      is added to the post and FALSE otherwise.            */
/*************************************************************/

static BOOL
disk_AddDiskToPost (HWND hwnd, UINT PostNum)
{
    int        topDiskSize ;

    /******************************************************/
    /* If there are already disks on the post,            */
    /* insure that the top disk is larger than this disk. */
    /******************************************************/

    if (HowMany [PostNum] > 0) {
        topDiskSize =
            disk_GetDiskSize (WhosOn [PostNum][HowMany [PostNum] - 1]) ;

        if (topDiskSize < disk_GetDiskSize (hwnd))
            return FALSE ;
    }

    /*******************************************/
    /* Add the disk to the stack on this post. */
    /*******************************************/

    WhosOn [PostNum][HowMany [PostNum]++] = hwnd ;
    disk_SetPostNum (hwnd, PostNum) ;

    return TRUE ;
}

/*************************************************************/
/*                                                           */
/*                    RemoveDiskFromPost                     */
/*                                                           */
/*      Removes this disk from its current post.  A disk can */
/*      only be removed from a post when it is the top disk  */
/*      on the post.  Return TRUE when the disk is removed   */
/*      from the post and FALSE otherwise.                   */
/*************************************************************/

static BOOL
disk_RemoveDiskFromPost (HWND hwndDisk)
{
    /****************************************/
    /* Determine which post the disk is on. */
    /****************************************/
    
    UINT PostNum = disk_GetPostNum (hwndDisk) ;

    /*************************************************/
    /* Can only remove the disk if it's the top one. */
    /*************************************************/

    if (!disk_IsTopDisk (hwndDisk))
        return FALSE ;

    /***********************************/
    /* Remove the disk from the stack. */
    /***********************************/

    WhosOn [PostNum][--HowMany [PostNum]] = 0 ;
    return TRUE ;
}

/******************************************************/
/*                                                    */
/*                    IsTopDisk                       */
/*                                                    */
/* Return TRUE if the disk is the topmost on its peg. */
/* Return FALSE otherwise.                            */
/******************************************************/

static BOOL
disk_IsTopDisk (HWND hwndDisk)
{
    UINT PostNum ;

    PostNum = disk_GetPostNum (hwndDisk) ;

    return
        (WhosOn [PostNum][HowMany [PostNum] - 1] == hwndDisk) ? TRUE : FALSE ;
}

/******************************************************/
/*                                                    */
/*                 PositionDiskWindow                 */
/*                                                    */
/* Move the window representing a disk to the proper  */
/* position in the main application window's client   */
/* area.                                              */
/******************************************************/

void
disk_PositionDiskWindow (HWND hwndDisk)
{
    HDC  hdc ;
    int  HorzPos;
    int  VertPos ;
    RECT rect ;
    UINT i;
    UINT DiskSize;
    UINT PostNum ;

    /*******************************************/
    /* Get the important info about this disk. */
    /*******************************************/

    DiskSize = disk_GetDiskSize (hwndDisk) ;
    PostNum  = disk_GetPostNum (hwndDisk) ;

    /************************************/
    /* Determine its stacking position. */
    /************************************/

    for (i = 0; i < HowMany [PostNum]; i++)
        if (WhosOn [PostNum][i] == hwndDisk)
            break ;

    /***************************************/
    /* Calculate the position of the disk. */
    /***************************************/

    SetRectEmpty (&rect) ;

    HorzPos = PostNum == FIRST ? LEFTPOSTPOS :
               PostNum == SECOND ? CENTERPOSTPOS : RIGHTPOSTPOS ;
    VertPos = (2 * i + 1) * DISKHEIGHT ;

    OffsetRect (&rect, HorzPos, VertPos) ;
    InflateRect (&rect, DISKWIDTHUNIT * (DiskSize + 2), DISKHEIGHT) ;

    /********************************************************/
    /* MoveWindow requires device coordinates.              */
    /* Translate logical coordinates to device coordinates. */
    /********************************************************/

    hdc = GetDC (GetParent (hwndDisk)) ;
    LPtoDP (hdc, (LPPOINT) &rect, 2) ;
    ReleaseDC (GetParent (hwndDisk), hdc) ;

    MoveWindow (hwndDisk, rect.left, rect.bottom,
                rect.right - rect.left, rect.top - rect.bottom, TRUE) ;
}

/******************************************************/
/*                                                    */
/*                    MoveTower                       */
/*                                                    */
/* Move an arbitrarily high tower of disks from one   */
/* peg to another using a third as an intermediate.   */
/******************************************************/

void
disk_MoveTower (UINT cDisks, UINT FromPeg, UINT AuxPeg, UINT ToPeg)
{
    if (cDisks == 1)
        disk_MoveOneDisk (FromPeg, ToPeg) ;

    else {
        disk_MoveTower (cDisks - 1, FromPeg, ToPeg, AuxPeg) ;
        disk_MoveOneDisk (FromPeg, ToPeg) ;
        disk_MoveTower (cDisks - 1, AuxPeg, FromPeg, ToPeg) ;
    }
}

/**************************************************/
/*                                                */
/*                   MoveOneDisk                  */
/*                                                */
/* Move the top disk from one peg to another peg. */
/**************************************************/

static void
disk_MoveOneDisk (UINT FromPeg, UINT ToPeg)
{
    HWND                hwndDisk ;

    /* Locate the top disk on the source peg. */

    hwndDisk = WhosOn [FromPeg][HowMany [FromPeg] - 1] ;

    /* Wait for a while... */

    disk_TwiddleThumbs (GetParent (hwndDisk), 1250) ;

    /* Take it off the source peg. */

    disk_RemoveDiskFromPost (hwndDisk) ;

    /* Put it on the destination peg. */

    disk_AddDiskToPost (hwndDisk, ToPeg) ;

    /* Update the display. */

    disk_PositionDiskWindow (hwndDisk) ;
}

/***********************************************/
/*                                             */
/*               TwiddleThumbs                 */
/*                                             */
/* Twiddle our thumbs (figuratively of course) */
/* for the specified length of time. Allow the */
/* user to close the program while we're so    */
/* occupied.                                   */
/***********************************************/

static void
disk_TwiddleThumbs (HWND hwnd, UINT Delay)
{
    MSG                         msg ;
        
    SetTimer (hwnd, 60, Delay, NULL) ;

    while (GetMessage (&msg, 0, 0, 0)) {
        if (msg.message == WM_TIMER) {
            KillTimer (hwnd, 60) ;
            return ;
        }
        else if (msg.message == WM_LBUTTONDOWN ||
                 msg.message == WM_LBUTTONUP)
            continue ;

        TranslateMessage (&msg) ;
        DispatchMessage (&msg) ;
    }
    RaiseException (STATUS_TERMINATION_REQUEST,// exception code
                    EXCEPTION_NONCONTINUABLE,  // continuable exception flag
                    0,                         // number of arguments in array
                    NULL) ;                    // address of array of arguments
}

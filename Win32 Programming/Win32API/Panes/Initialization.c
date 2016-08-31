#include "StdSDK.h"         // Standard application includes
#include "Initialization.h" // For non-static function prototypes
#include "MainFrame.h"      // For non-static function prototypes
#include "Popup.h"	    // For non-static function prototypes
#include "child.h"
#include "resource.h"       // For resource identifiers

//
// Function prototypes for static functions
//

static BOOL registerWindowClasses (HINSTANCE hinst, UINT resPoolID) ;
static HWND createMainFrameWindow (HINSTANCE hinst, int nCmdShow) ;
static HWND createOverlappedWindow(HINSTANCE HINST, HWND hwnd, int resPoolID);
static ATOM internalRegisterClass (const LPWNDCLASSEX lpwcex) ;
static HWND createPopupWindow     (HINSTANCE hinst, HWND hwnd, int titleid);
static HWND createChildWindow     (HINSTANCE hinst, HWND hwnd);

//
// Function prototypes for callback functions
//

//
// Typedefs
//

typedef ATOM (WINAPI* REGISTERCLASSEXPROC)(const LPWNDCLASSEX lpwcex) ;

//
//  BOOL initInstance (HINSTANCE hinst, UINT resPoolID, int nCmdShow)
//
//  hinst           Application module (instance) handle
//  resPoolID       Resource identifier
//  nCmdShow        Initial show window state
//
//  PURPOSE:        Initialize the application
//
//  COMMENTS:
//      Activate a previous instance, if any, by searching for a window
//      created with the same window class name that this instance uses.
//      If found, restore the previous instance's window, if necessary,
//      and make that window the foreground window.
//
//      Register the window class(es) using a WNDCLASSEX structure.
//
//      The specified resPoolID parameter is the UINT resource identifier
//      used to locate the application's main frame window class name,
//      the menu used on the main frame window, the large and small icons
//      for the main frame window.
//

BOOL initInstance (HINSTANCE hinst, UINT resPoolID, int nCmdShow)
{
    HWND  hwnd;
    TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
    int n;

    n = LoadString (hinst, resPoolID, ClassName, DIM(ClassName)) ;

#if defined(UNICODE)
    if(0 == n)
       { /* no Unicode */
        char msg[MAX_RESOURCESTRING + 1];
        char hdr[MAX_RESOURCESTRING + 1];
        LoadStringA(hinst, IDS_NO_UNICODE, msg, DIM(msg));
        LoadStringA(hinst, IDS_APP_TITLE, hdr, DIM(hdr));
        MessageBoxA(NULL, msg, hdr, MB_OK|MB_ICONERROR) ;
        return FALSE;
       } /* no Unicode */
#endif
    VERIFY(n != 0);

    // Constrain this application to run single instance
    hwnd = FindWindow (ClassName, NULL) ;
    if (hwnd) {

        // A previous instance of this application is running.

        // Activate the previous instance, tell it what the user
        // requested this instance to do, then abort initialization
        // of this instance.

        if (IsIconic (hwnd))
            ShowWindow (hwnd, SW_RESTORE) ;

        SetForegroundWindow (hwnd) ;

        // Send an application-defined message to the previous
        // instance (or use some other type of IPC mechanism)
        // to tell the previous instance what to do.

        // Determining what to do generally depends on how the
        // user started this instance.

               // ... <some application-specific code here>

        // Abort this instance's initialization
        return FALSE ;
    }

    // Register all application-specific window classes
    if (!registerWindowClasses (hinst, resPoolID))
        return FALSE ;

    // Initialize the Common Controls DLL
    // You must call this function before using any Common Control
    InitCommonControls () ;

    // Create the application's main frame window
    hwnd = createMainFrameWindow (hinst, nCmdShow);

    if (hwnd == NULL)
        return FALSE ;

    // Create the owned, overlapped window

    if(createOverlappedWindow(hinst, hwnd, resPoolID) == NULL)
       return FALSE;

    // Create the sample popup window

    UnownedPopup = createPopupWindow(hinst, NULL, IDS_UNOWNEDPOPUPTITLE);
    if(UnownedPopup == NULL)
       return FALSE;

    if(createPopupWindow(hinst, hwnd, IDS_OWNEDPOPUPTITLE) == NULL)
       return FALSE;

    if(createChildWindow(hinst, hwnd) == NULL)
       return FALSE;

    return TRUE ;
}


//  int exitInstance (PMSG msg)
//
//  hinst           Pointer to MSG structure
//
//  PURPOSE:        Perform deinitialization and return exit code
//
//  COMMENTS:
//

int exitInstance (MSG* pmsg)
{
    return pmsg->wParam ;
}

//
//  BOOL registerWindowClasses (HINSTANCE hinst, UINT resPoolID)
//
//  hinst           Application module (instance) handle
//  resPoolID       Resource identifier
//
//  PURPOSE:        Register all application-specific window classes
//
//  COMMENTS:
//      The specified resPoolID parameter is the UINT resource identifier
//      used to locate the application's main frame window class name,
//      the menu used on the main frame window, the large and small icons
//      for the main frame window.

static BOOL
registerWindowClasses (HINSTANCE hinst, UINT resPoolID)
{
    TCHAR      ClassName [MAX_RESOURCESTRING + 1] ;
    WNDCLASSEX wcex ;

    VERIFY (LoadString (hinst, resPoolID, ClassName, DIM(ClassName))) ;

    // Fill in window class structure with parameters that describe
    // the main window.
    wcex.cbSize        = sizeof (WNDCLASSEX) ;
    wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
    wcex.lpfnWndProc   = mainFrameWndProc ;
    wcex.cbClsExtra    = 0 ;
    wcex.cbWndExtra    = 0 ;
    wcex.hInstance     = hinst ;
    wcex.hIcon         = LoadIcon (hinst, MAKEINTRESOURCE (resPoolID)) ;
    wcex.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1) ;

    wcex.lpszMenuName  = NULL;  // no class menu
    wcex.lpszClassName = ClassName ;
    wcex.hIconSm       = LoadImage (hinst,
                                    MAKEINTRESOURCE (resPoolID),
                                    IMAGE_ICON,
                                    GetSystemMetrics (SM_CXSMICON),
                                    GetSystemMetrics (SM_CYSMICON),
                                    LR_SHARED) ;

    // Register the window class and return success/failure code.
    if(internalRegisterClass (&wcex) == 0)
       return FALSE;

    VERIFY (LoadString (hinst, IDR_POPUP, ClassName, DIM(ClassName))) ;

    wcex.style         = 0;
    wcex.lpfnWndProc   = popupWndProc ;
    wcex.cbClsExtra    = 0 ;
    wcex.cbWndExtra    = 0 ;
    wcex.hInstance     = hinst ;
    wcex.hIcon         = LoadIcon (hinst, MAKEINTRESOURCE (IDR_POPUP)) ;
    wcex.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
    wcex.hbrBackground = GetStockBrush(GRAY_BRUSH);

    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = ClassName ;
    wcex.hIconSm       = LoadImage (hinst,
                                    MAKEINTRESOURCE (IDR_POPUP),
                                    IMAGE_ICON,
                                    GetSystemMetrics (SM_CXSMICON),
                                    GetSystemMetrics (SM_CYSMICON),
                                    LR_SHARED) ;

    if(internalRegisterClass (&wcex) == 0)
       return FALSE;
    
    VERIFY (LoadString (hinst, IDR_CHILD, ClassName, DIM(ClassName))) ;

    wcex.style         = 0;
    wcex.lpfnWndProc   = childWndProc ;
    wcex.cbClsExtra    = 0 ;
    wcex.cbWndExtra    = 0 ;
    wcex.hInstance     = hinst ;
    wcex.hIcon         = LoadIcon (hinst, MAKEINTRESOURCE (IDR_CHILD)) ;
    wcex.hCursor       = LoadCursor (NULL, IDC_CROSS) ;
    wcex.hbrBackground = GetStockBrush(LTGRAY_BRUSH);

    wcex.lpszMenuName  = (LPCTSTR)NULL;
    wcex.lpszClassName = ClassName ;
    wcex.hIconSm       = LoadImage (hinst,
                                    MAKEINTRESOURCE (IDR_CHILD),
                                    IMAGE_ICON,
                                    GetSystemMetrics (SM_CXSMICON),
                                    GetSystemMetrics (SM_CYSMICON),
                                    LR_SHARED) ;

    if(internalRegisterClass (&wcex) == 0)
       return FALSE;
}

//
//  HWND createMainFrameWindow (HINSTANCE hinst, int nCmdShow)
//
//  hinst           Application module (instance) handle
//  nCmdShow        Initial show window state
//
//  PURPOSE:        Create the application's main frame window
//                  and show the window as requested
//
//  COMMENTS:
//

static HWND
createMainFrameWindow (HINSTANCE hinst, int nCmdShow)
{
    HWND  hwnd ;
    TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
    TCHAR Title [MAX_RESOURCESTRING + 1] ;

    // Create the main frame window
    VERIFY (LoadString (hinst, IDR_MAINFRAME, ClassName, DIM (ClassName))) ;
    VERIFY (LoadString (hinst, IDS_APP_TITLE, Title, DIM (Title))) ;

    hwnd =
        CreateWindowEx (
                        0,                  // Extended window styles
                        ClassName,          // Address of registered class name
                        Title,              // Address of window name
                        WS_OVERLAPPEDWINDOW,// Window style
                        CW_USEDEFAULT,      // Horizontal position of window
                        0,                  // Vertical position of window
                        CW_USEDEFAULT,      // Window width
                        0,                  // Window height
                        NULL,               // Handle of parent or owner window
                        LoadMenu(hinst, MAKEINTRESOURCE(IDR_MAINFRAME)), 
                                            // Handle of menu or child-window identifier
                        hinst,              // Handle of application instance
                        NULL) ;             // Address of window-creation data

    ASSERT (NULL != hwnd) ;

    if (!hwnd)
       return NULL ;

    ShowWindow (hwnd, nCmdShow) ;
    UpdateWindow (hwnd) ;

    return hwnd ;
}

/****************************************************************************
*                              createOverlappedWindow
* Inputs:
*	HINSTANCE hinst: Instance handle
*	HWND hwnd: owner window, could be NULL
*	int resPoolID: ID of main window class
* Result: HWND
*       Window handle if successful, NULL if error
* Effect: 
*       Creates a popup window which is owned by the main window
****************************************************************************/

static HWND createOverlappedWindow(HINSTANCE hinst, HWND hwnd, int resPoolID)
    {
     HWND hwndOver;
     TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
     TCHAR Title [MAX_RESOURCESTRING + 1] ;
     RECT r;
     int width;
     int height;
     POINT origin;

     GetClientRect(hwnd, &r);
     width = (r.right - r.left) / 3;
     height = (r.bottom - r.top) / 3;
     origin.x = r.left + (width / 3);
     origin.y = r.top + 3 * (height / 2);
     ClientToScreen(hwnd, &origin);

     VERIFY (LoadString (hinst, resPoolID, ClassName, DIM(ClassName))) ;
     VERIFY (LoadString (hinst, IDS_OVERLAPPED, Title, DIM(Title)));

     hwndOver = CreateWindow( ClassName,                         // Class name
                               Title,                         // Popup caption
                               WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
                               WS_POPUPWINDOW | WS_CAPTION,   // window styles
                               origin.x,                             // origin
                               origin.y,                                // ...
                               width,                                  // size
                               height,                                  // ...
                               hwnd,                                  // owner
                               (HMENU)NULL,                         // no menu
                               hinst,                       // Instance handle
                               NULL);               // user-defined parameters
     if(hwndOver == NULL)
        return NULL;

     ShowWindow(hwndOver, SW_SHOWNOACTIVATE);
     UpdateWindow(hwndOver);

     return hwndOver;
    }


/****************************************************************************
*                              createPopupWindow
* Inputs:
*	HINSTANCE hinst: Instance handle
*	HWND hwnd: owner window, could be NULL
*	int titleid: ID of title
* Result: HWND
*       Window handle if successful, NULL if error
* Effect: 
*       Creates a popup window which is owned by the main window
****************************************************************************/

static HWND createPopupWindow(HINSTANCE hinst, HWND hwnd, int titleid)
    {
     HWND hwndPopup;
     TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
     TCHAR Title [MAX_RESOURCESTRING + 1] ;
     RECT r;
     int width;
     int height;
     POINT origin;

     if(hwnd != NULL)
        { /* owned, window-relative */
	 GetClientRect(hwnd, &r);
	 width = (r.right - r.left) / 3;
	 height = (r.bottom - r.top) / 3;
	 origin.x = r.left + 2 * width;
	 origin.y = r.top + 3 * (height / 2);
	 ClientToScreen(hwnd, &origin);
	} /* owned, window-relative */
     else
        { /* unowned, screen-relative */
	 SetRect(&r, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
	 		   GetSystemMetrics(SM_CYSCREEN));
         width = (r.right - r.left) / 3;
	 height = (r.bottom - r.top) / 3;
	 // Put in the top right corner of the screen
	 origin.x = r.right - width;
	 origin.y = 0;
	} /* unowned, screen-relative */

     VERIFY (LoadString (hinst, IDR_POPUP, ClassName, DIM(ClassName))) ;
     VERIFY (LoadString (hinst, titleid, Title, DIM(Title)));

     hwndPopup = CreateWindow( ClassName, 	                 // Class name
     			       Title,		              // Popup caption
			       WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
			       WS_POPUPWINDOW | WS_CAPTION,   // window styles
			       origin.x,                             // origin
			       origin.y,                                // ...
			       width,                                  // size
			       height,                                  // ...
			       hwnd,                                  // owner
			       (HMENU)NULL,                         // no menu
			       hinst,                       // Instance handle
			       NULL);               // user-defined parameters
     if(hwndPopup == NULL)
	return NULL;

     ShowWindow(hwndPopup, SW_SHOWNOACTIVATE);
     UpdateWindow(hwndPopup);

     return hwndPopup;
    }


/****************************************************************************
*                              createChildWindow
* Inputs:
*	HINSTANCE hinst: Instance handle
*	HWND hwnd: owner window, could be NULL
* Result: HWND
*       Window handle if successful, NULL if error
* Effect: 
*       Creates a popup window which is owned by the main window
****************************************************************************/

static HWND createChildWindow(HINSTANCE hinst, HWND hwnd)
    {
     HWND hwndChild;
     TCHAR ClassName [MAX_RESOURCESTRING + 1] ;
     TCHAR Title [MAX_RESOURCESTRING + 1] ;
     RECT r;
     int width;
     int height;
     POINT pt;

     GetClientRect(hwnd, &r);
     width = (r.right - r.left) / 2;
     height = (r.bottom - r.top) / 2;

     // We want the child window centered on the parent
     pt.x = (r.right - r.left) / 2 - width / 2;
     pt.y = (r.bottom - r.top) / 2 - height / 2;

     VERIFY (LoadString (hinst, IDR_CHILD, ClassName, DIM(ClassName))) ;
     VERIFY (LoadString (hinst, IDS_CHILDTITLE, Title, DIM(Title)));

     hwndChild = CreateWindowEx( 
     			       WS_EX_DLGMODALFRAME | 
			       WS_EX_NOPARENTNOTIFY,
     			       ClassName, 	                 // Class name
     			       Title,		              // Child caption
			       WS_CHILDWINDOW | WS_CAPTION,    // window style
			       pt.x,                                 // origin
			       pt.y,                                    // ...
			       width,                                  // size
			       height,                                  // ...
			       hwnd,                                  // owner
			       (HMENU)NULL,                         // no menu
			       hinst,                       // Instance handle
			       NULL);               // user-defined parameters
     if(hwndChild == NULL)
	return NULL;

     ShowWindow(hwndChild, SW_SHOW);
     UpdateWindow(hwndChild);

     return hwndChild;
    }

//
//  ATOM internalRegisterClass (const LPWNDCLASSEX lpwcex)
//
//  lpwcex          Pointer to WNDCLASSEX structure
//
//  PURPOSE:        Registers the window class using RegisterClassEx
//                  if it is available. If not, registers the class
//                  using RegisterClass.
//
//  COMMENTS:       RegisterClassEx was introduced in Windows 95 and
//                  Windows NT 3.51. An application must register its
//                  window classes using RegisterClassEx in order to
//                  specify the small icons which should be used for
//                  the application.
//
  
ATOM
internalRegisterClass (const LPWNDCLASSEX lpwcex)
{
    WNDCLASS wc ;

    // Get the module handle of the 32-bit USER DLL
    HANDLE hModule = GetModuleHandle (TEXT("USER32")) ;
    if (NULL != hModule) {

        // If we're running on a Win32 version supporting RegisterClassEx
        //  get the address of the function so we can call it

#if defined (UNICODE)
    REGISTERCLASSEXPROC proc = 
        (REGISTERCLASSEXPROC) GetProcAddress (hModule, "RegisterClassExW") ;
#else
    REGISTERCLASSEXPROC proc =
        (REGISTERCLASSEXPROC) GetProcAddress (hModule, "RegisterClassExA") ;
#endif

    if (NULL != proc)
       // RegisterClassEx exists...
       // return RegisterClassEx (&wcex) ;
       return (*proc) (lpwcex) ;
    }

    // Convert the WNDCLASSEX structure to a WNDCLASS structure
    wc.style         = lpwcex->style ;
    wc.lpfnWndProc   = lpwcex->lpfnWndProc ;
    wc.cbClsExtra    = lpwcex->cbClsExtra ;
    wc.cbWndExtra    = lpwcex->cbWndExtra ;
    wc.hInstance     = lpwcex->hInstance ;
    wc.hIcon         = lpwcex->hIcon ;
    wc.hCursor       = lpwcex->hCursor ;
    wc.hbrBackground = lpwcex->hbrBackground ;
    wc.lpszMenuName  = lpwcex->lpszMenuName ;
    wc.lpszClassName = lpwcex->lpszClassName ;

    return RegisterClass (&wc) ;
}



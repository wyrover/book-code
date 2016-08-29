#include "StdSDK.h"         // Standard application includes
#include "Initialization.h" // For non-static function prototypes
#include "MainFrame.h"      // For non-static function prototypes
#include "resource.h"       // For resource identifiers

//
// Function prototypes for static functions
//

static BOOL registerWindowClasses (HINSTANCE hinst, UINT resPoolID) ;
static HWND createMainFrameWindow (HINSTANCE hinst, int nCmdShow) ;

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
    if (!createMainFrameWindow (hinst, nCmdShow))
        return FALSE ;

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
//
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

    wcex.lpszMenuName  = MAKEINTRESOURCE (resPoolID) ;
    wcex.lpszClassName = ClassName ;
    wcex.hIconSm       = LoadImage (hinst,
                                    MAKEINTRESOURCE (resPoolID),
                                    IMAGE_ICON,
                                    GetSystemMetrics (SM_CXSMICON),
                                    GetSystemMetrics (SM_CYSMICON),
                                    LR_SHARED) ;

    // Register the window class and return success/failure code.
    return internalRegisterClass (&wcex) ;
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
        CreateWindowEx (0,              // Extended window styles
                    ClassName,          // Address of registered class name
                    Title,              // Address of window name
                    WS_OVERLAPPEDWINDOW,// Window style
                    CW_USEDEFAULT,      // Horizontal position of window
                    0,                  // Vertical position of window
                    CW_USEDEFAULT,      // Window width
                    0,                  // Window height
                    NULL,               // Handle of parent or owner window
                    NULL,               // Handle of menu for this window
                    hinst,              // Handle of application instance
                    NULL) ;             // Address of window-creation data

    ASSERT (NULL != hwnd) ;

    if (!hwnd)
       return NULL ;

    ShowWindow (hwnd, nCmdShow) ;
    UpdateWindow (hwnd) ;

    return hwnd ;
}

//
//  BOOL internalRegisterClass (const LPWNDCLASSEX lpwcex)
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



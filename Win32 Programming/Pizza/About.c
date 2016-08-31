#include "StdSDK.h"         // Standard application includes
#include "About.h"          // For non-static function prototypes
#include "resource.h"       // For resource identifiers
#include "aboutbox.h"
#include <tchar.h>          // For _tcsicmp

//
// Typedefs
//

typedef enum { typeDefault        = 0,
               typeAdvancedServer = 1,
               typeWorkstation    = 2,
               typeServer         = 3 } NTTYPE ;

//
// Function prototypes for static functions
//

static void  displayExecutableVersionInfo (HWND hwnd) ;
static void  displayOperatingSystemVersionInfo (HWND hwnd) ;
static void  displayProcessorVersionInfo (HWND hwnd) ;
static DWORD formatMessageFromString (LPCTSTR Format, LPTSTR  Buffer, DWORD nSize, ...) ;
static NTTYPE getNTVersion () ;


//
// Function prototypes for callback functions
//

//
// Function prototypes for message handlers
//

static BOOL aboutDlg_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos) ;
static void aboutDlg_OnHelp (HWND hwnd, LPHELPINFO lphi) ;
static BOOL aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam) ;
static void aboutDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) ;
//
//  static BOOL CALLBACK
//  aboutDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//
//  PURPOSE:  
//      Dialog function for the About dialog box.
//      Display version information for the application,
//      the operating system, and the processor.
//
//  MESSAGES:
//
//
//

static BOOL CALLBACK
aboutDlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_CONTEXTMENU:           // User clicked the right mouse button
            return HANDLE_WM_CONTEXTMENU (hwnd, wParam, lParam, aboutDlg_OnContextMenu) ;

        case WM_HELP:                  // User pressed the F1 key
            return HANDLE_WM_HELP (hwnd, wParam, lParam, aboutDlg_OnHelp) ;

        case WM_INITDIALOG:            // Initialization of controls complete
            return HANDLE_WM_INITDIALOG (hwnd, wParam, lParam, aboutDlg_OnInitDialog) ;

        case WM_COMMAND:               // Notification from a control
            return HANDLE_WM_COMMAND (hwnd, wParam, lParam, aboutDlg_OnCommand) ;
    }
    return FALSE ;
}

//  Array of control identifiers and help context identifier pairs. 

#define IDH_ABOUT_FILEDESCRIPTION   IDC_ABOUT_FILEDESCRIPTION
#define IDH_ABOUT_VERSION           IDC_ABOUT_VERSION
#define IDH_ABOUT_LEGALCOPYRIGHT    IDC_ABOUT_LEGALCOPYRIGHT
#define IDH_ABOUT_COMMENTS          IDC_ABOUT_COMMENTS
#define IDH_ABOUT_OSVERSION         IDC_ABOUT_OSVERSION
#define IDH_ABOUT_PROCESSORVERSION  IDC_ABOUT_PROCESSORVERSION
#define IDH_ABOUT_LEGALTRADEMARKS   IDC_ABOUT_LEGALTRADEMARKS
 
static DWORD aContextIds [] = { 
    IDC_ABOUT_FILEDESCRIPTION,  IDH_ABOUT_FILEDESCRIPTION,
    IDC_ABOUT_VERSION,          IDH_ABOUT_VERSION,
    IDC_ABOUT_LEGALCOPYRIGHT,   IDH_ABOUT_LEGALCOPYRIGHT,
    IDC_ABOUT_COMMENTS,         IDH_ABOUT_COMMENTS,
    IDC_ABOUT_OSVERSION,        IDH_ABOUT_OSVERSION,
    IDC_ABOUT_PROCESSORVERSION, IDH_ABOUT_PROCESSORVERSION,
    IDC_ABOUT_LEGALTRADEMARKS,  IDH_ABOUT_LEGALTRADEMARKS,
    0,                          0
} ;
 

//
//  void aboutDlg_OnContextMenu (HWND hwnd, HWND hwndCtl. int xPos, int yPos)
//
//  hwnd            Handle of window to which this message applies
//  hwndCtl         Handle of the window in which the user right clicked the 
//                  mouse
//  xPos            Horizontal position of the cursor, in screen coordinates
//  yPos            Vertical position of the cursor, in screen coordinates
//
//  PURPOSE:        Notification that the user clicked the right
//                  mouse button in the window.
//
//  COMMENTS:       Normally a window processes this message by
//                  displaying a context menu using the TrackPopupMenu
//                  or TrackPopupMenuEx functions.
//
//                  However it's convenient to allow the user to right
//                  click on a control in a dialog box to display a popup
//                  help topic about the control. This function implements
//                  the latter technique.
//

static BOOL
aboutDlg_OnContextMenu (HWND hwnd, HWND hwndCtl, int xPos, int yPos)
{
    // Display popup help for the control
    WinHelp (hwndCtl, getHelpFileName (), HELP_CONTEXTMENU, 
                                               (DWORD) (LPVOID) aContextIds) ;
    return TRUE ;
}


//
//  void aboutDlg_OnHelp (HWND hwnd, LPHELPINFO lphi)
//
//  PURPOSE:        Notification that the user pressed the F1 key
//
//  COMMENTS:       Normally a window processes this message by
//                  displaying a context menu using the TrackPopupMenu
//                  or TrackPopupMenuEx functions. If a window does not
//                  display a context menu it should pass this message
//                  to the DefWindowProc function. 
//

static void
aboutDlg_OnHelp (HWND hwnd, LPHELPINFO lphi)
{
    ASSERT (HELPINFO_WINDOW == lphi->iContextType) ;

    if (HELPINFO_WINDOW == lphi->iContextType) {    // Must be for a control!

        int ID ;
        int Index ;

        // Get the control's window handle
        HWND hwndCtl = lphi->hItemHandle ;

        ASSERT (NULL != hwndCtl) ;
        ASSERT (IsWindow (hwndCtl)) ;

        // Get this control's ID
        ID = GetWindowID (hwndCtl) ;

        // Don't bother running WinHelp unless we have help for the control
        for (Index = 0 ; Index < DIM (aContextIds) - 2; Index += 2) {
            if (aContextIds [Index] == (DWORD) ID) {
                WinHelp (hwndCtl, getHelpFileName (), 
                         HELP_WM_HELP, (DWORD) (LPVOID) aContextIds) ;
                return ;
            }
        }
    }
}


//
//  void aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
//
//  hwnd            Window handle for the dialog box window
//  hwndFocus       Handle of the control which will get the initial focus
//  lParam          Parameter to DialogBoxParam (not used for About dialog) 
//
//  PURPOSE:        
//                  Initializes all the controls by extracting information
//                  from the VERSIONINFO structure
//                  

static BOOL
aboutDlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    // Center the dialog window
    centerWindow (hwnd, NULL) ;

    // Update controls with application version info
    displayExecutableVersionInfo (hwnd) ;

    // Update controls with operating system version info
    displayOperatingSystemVersionInfo (hwnd) ;

    // Update controls with processor version info
    displayProcessorVersionInfo (hwnd) ;

    return TRUE ;
}

//
// 040904B0 means US English, Unicode code page
// 040904E4 means US English, Windows MultiLingual code page
static const TCHAR ValueNameBase [] = TEXT("\\StringFileInfo\\040904B0\\") ;
static const TCHAR ProductName []   = TEXT("ProductName") ;

// Number of characters in the base portion of the value name string
#define BASECHARS    (DIM(ValueNameBase) - 1)

//
//  void displayExecutableVersionInfo (HWND hwnd)
//
//  hwnd            Window handle for the dialog box window
//
//  PURPOSE:        Retrieves the application's version resource
//                  infomation and displays the info in the
//                  appropriate dialog box controls.
//
//  COMMENTS:       This implementation uses the initial control
//                  window text as a version info value key. This
//                  function displays the value retrieved for a
//                  key in the respective control.
//

static void
displayExecutableVersionInfo (HWND hwnd)
{
    BOOL      Result ;                // Result of Boolean functions
    DWORD     dwVerInfoSize ;         // Size of version information
    DWORD     dwHandle ;              // Extraneous but required 
    HMODULE   hmod ;                  // Application's module handle
    HWND      hwndControl ;           // Control window handle
    LPVOID    pVerInfo ;              // File version info pointer
    LPVOID    pValue ;                // Value from version info
    TCHAR     FullPath [_MAX_PATH] ;  // Application executable path
    TCHAR     ValueName [256] ;       // Name of value to retrieve
    UINT      Length ;                // Length of retrieved value

    // Get the full path to this executable file
    hmod = GetModuleHandle (NULL) ;
    GetModuleFileName (hmod, FullPath, DIM(FullPath)) ;

    // Determine the size buffer needed to store the version information:
    dwVerInfoSize = GetFileVersionInfoSize (FullPath, &dwHandle) ;
    if (0 == dwVerInfoSize)
        return ;

    // Allocate a buffer for the version info block
    pVerInfo = malloc (dwVerInfoSize) ;
    ASSERT (NULL != pVerInfo) ;
    if (NULL == pVerInfo)
        return ;

    // Read the version info block into the buffer
    VERIFY(GetFileVersionInfo (FullPath, dwHandle, dwVerInfoSize, pVerInfo)) ;

    // Build value name base string...
    _tcscpy (ValueName, ValueNameBase) ;
        
    // Build the \StringFileInfo\040904E4\ProductName value name
    _tcscpy (ValueName + BASECHARS, ProductName) ;

    // Retrieve the value
    Result = VerQueryValue (pVerInfo, ValueName, &pValue, &Length) ;
    ASSERT (Result) ;

    // Format the output for the dialog caption
    // Get the current caption then append to it the ProductName value
    GetWindowText (hwnd, ValueName, DIM (ValueName)) ;
    _tcscat (ValueName, pValue) ;

    // Change the dialog caption - normally "About <ProductName>"
    SetWindowText (hwnd, ValueName) ;

    // For each control in the dialog...
    //  fetch the version info name from the control's initial window text.
    //  retrieve the value with that name,
    //  change the control's window text to the retrieved value.
    // Technique derived from GENERIC.C.

    hwndControl = GetFirstChild (hwnd) ;
    while (NULL != hwndControl) {
        // Build value name base string...
        _tcscpy (ValueName, ValueNameBase) ;

        // Build the \StringFileInfo\040904B0\<ControlText> value name
        // The Win32 API contains the following predefined version information 
        // strings:
        //     CompanyName              LegalCopyright     
        //     FileDescription          OriginalFilename   
        //     FileVersion              ProductName        
        //     InternalName             ProductVersion     
        
        // Get the control's text...
        GetWindowText (hwndControl,
                       ValueName + BASECHARS,
                       DIM(ValueName) - BASECHARS) ;

        // Retrieve the value
        Result = VerQueryValue (pVerInfo, ValueName, &pValue, &Length) ;

        // If version information is available and the version information 
        // name exists...
        if (Result)
            // If a value exists for the version information name...
            if (0 != Length && NULL != pValue)
                // Change the control's text to the version information value
                SetWindowText (hwndControl, pValue) ;

        hwndControl = GetNextSibling (hwndControl) ;
    }

    // Free the memory for the version information block
    free (pVerInfo) ;
}


//
//  DWORD displayOperatingSystemVersionInfo (HWND hwnd)
//
//  hwnd            Window handle for the dialog box window
//
//  PURPOSE:        Displays the operating system's version
//
//  COMMENTS:
//

static void
displayOperatingSystemVersionInfo (HWND hwnd)
{
    BOOL           Result ;
    HINSTANCE      hinst ;
    NTTYPE         NtOsType ;
    TCHAR          OSVer [256] ;
    TCHAR          FormatString [256] ;
    OSVERSIONINFO  osver ;

    // Get OS version information
    osver.dwOSVersionInfoSize = sizeof (osver) ;// Must initialize size member!

    Result = GetVersionEx (&osver) ;            // Retrieve version info
    ASSERT (FALSE != Result) ;
    if (FALSE == Result)
        return ;

    hinst = GetWindowInstance (hwnd) ;          // Get instance for LoadString

    switch (osver.dwPlatformId) {
        case VER_PLATFORM_WIN32_NT:             // Windows NT
            NtOsType = getNTVersion () ;
            LoadString (hinst, IDS_PLATFORM_WIN32_NT + NtOsType,
                        FormatString, DIM (FormatString)) ;
            break ;

        case VER_PLATFORM_WIN32s:           // Win32s on Windows 3.1
            LoadString (hinst, IDS_PLATFORM_WIN32s,
                        FormatString, DIM (FormatString)) ;
            break ;

        case VER_PLATFORM_WIN32_WINDOWS:     // Windows 95
            LoadString (hinst, IDS_PLATFORM_WIN32_WINDOWS,
                        FormatString, DIM (FormatString)) ;
            // Windows 95 encodes extra info in HIWORD(dwBuildNumber)
            // Remove unwanted junk
            osver.dwBuildNumber = LOWORD (osver.dwBuildNumber) ;
            break ;

        default:                            // Unknown operating system
            LoadString (hinst, IDS_PLATFORM_UNKNOWN,
                        FormatString, DIM (FormatString)) ;
            break ;
    }

    wsprintf (OSVer, FormatString,
              osver.dwMajorVersion,
              osver.dwMinorVersion,
              osver.dwBuildNumber) ;
    SetDlgItemText (hwnd, IDC_ABOUT_OSVERSION, OSVer) ;
}


//
//  void displayProcessorVersionInfo (HWND hwnd, DWORD dwPlatformId)
//
//  hwnd            Window handle for the dialog box window
//  dwPlatformId    Hardware platform ID returned by GetVersionEx
//
//  PURPOSE:        Displays the processor's version
//
//  COMMENTS:
//

static void
displayProcessorVersionInfo (HWND hwnd)
{
    BOOL        Recognized ;
    HINSTANCE   hinst ;
    SYSTEM_INFO si ;
    TCHAR       Buffer [256] ;
    TCHAR       Format [256] ;

    // Get current system information
    // Zero the structure as Windows 95 and older versions of Windows NT
    // do not initialize *all* fields of the structure (specifically,
    // wProcessorLevel and wProcessorRevision. Unfortunately, the
    // documentation does not say *which* versions of Windows NT do not
    // support setting these fields. Therefore use the new fields if
    // they seem to have been set. otherwise use the "obsolete" fields.

    ZeroMemory (&si, sizeof (si)) ;
    GetSystemInfo (&si) ;
    
    hinst = GetWindowInstance (hwnd) ;      // Get instance for LoadString

    // Determine processor architecture
    Recognized = TRUE ;
    switch (si.wProcessorArchitecture) {
        default:
            Recognized = FALSE ;
            LoadString (hinst, IDS_PROCESSOR_ARCHITECTURE_UNKNOWN,
                        Buffer, DIM (Buffer)) ;
            break ;

        case PROCESSOR_ARCHITECTURE_INTEL:  // Intel
            switch (si.wProcessorLevel) {
                default:
                    Recognized = FALSE ;
                    LoadString (hinst,
                                IDS_PROCESSOR_LEVEL_INTEL_UNKNOWN,
                                Buffer, DIM (Buffer)) ;
                    break ;

                case 3:                     // Intel 80386
                    LoadString (hinst,
                                IDS_PROCESSOR_ARCHITECTURE_INTEL_386_486,
                                Format, DIM (Format)) ;
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("80386"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 4:                     // Intel 80486
                    LoadString (hinst,
                                IDS_PROCESSOR_ARCHITECTURE_INTEL_386_486,
                                Format, DIM (Format)) ;
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("80486"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 5:                     // Intel Pentium
                    LoadString (hinst,
                                IDS_PROCESSOR_ARCHITECTURE_INTEL_PENTIUM,
                                Format, DIM (Format)) ;
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("Pentium"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_MIPS:   // MIPS
            switch (si.wProcessorLevel) {// 00xx - xx is 8-bit implementation #
                LoadString (hinst, IDS_PROCESSOR_ARCHITECTURE_MIPS,
                            Format, DIM (Format)) ;
                default:
                    Recognized = FALSE ;
                    LoadString (hinst, IDS_PROCESSOR_LEVEL_MIPS_UNKNOWN,
                                Buffer, DIM (Buffer)) ;
                    break ;

                case 0x0004:                // MIPS R4000
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("R4000"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_ALPHA:  // Alpha
            LoadString (hinst, IDS_PROCESSOR_ARCHITECTURE_ALPHA,
                        Format, DIM (Format)) ;
            switch (si.wProcessorLevel) {  // xxxx - 16-bit processor version #
                default:
                    Recognized = FALSE ;
                    LoadString (hinst, IDS_PROCESSOR_LEVEL_ALPHA_UNKNOWN,
                                Buffer, DIM (Buffer)) ;
                    break ;

                case 21064:                 // Alpha 21064
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("21064"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 21066:                 // Alpha 21066
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("21066"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 21164:                 // Alpha 21164
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("21164"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_PPC:    // Power PC
            LoadString (hinst, IDS_PROCESSOR_ARCHITECTURE_PPC,
                        Format, DIM (Format)) ;
            switch (si.wProcessorLevel) {  // xxxx - 16-bit processor version #
                default:
                    Recognized = FALSE ;
                    LoadString (hinst, IDS_PROCESSOR_LEVEL_PPC_UNKNOWN,
                                Buffer, DIM (Buffer)) ;
                    break ;

                case 1:                     // Power PC 601
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("601"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 3:                     // Power PC 603
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("603"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 4:                     // Power PC 604
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("604"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 6:                     // Power PC 603+
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("603+"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 9:                     // Power PC 604+
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("604+"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 20:                    // Power PC 620
                    formatMessageFromString (
                        Format,
                        Buffer, DIM (Buffer),
                        TEXT ("620"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;
    }

    // If the processor type isn't yet recognized, check the, supposedly,
    // "obsolete" dwProcessorType field of the SYSTEM_INFO structure for
    // a reasonable value and use it.
    if (!Recognized) {
        switch (si.dwProcessorType) {
            case PROCESSOR_INTEL_386:
                LoadString (hinst, IDS_PROCESSOR_NOREV_INTEL_386,
                            Buffer, DIM (Buffer)) ;
                break ;

            case PROCESSOR_INTEL_486:
                LoadString (hinst, IDS_PROCESSOR_NOREV_INTEL_486,
                            Buffer, DIM (Buffer)) ;
                break ;

            case PROCESSOR_INTEL_PENTIUM:
                LoadString (hinst, IDS_PROCESSOR_NOREV_INTEL_PENTIUM,
                            Buffer, DIM (Buffer)) ;
                break ;

            case PROCESSOR_MIPS_R4000:
                LoadString (hinst, IDS_PROCESSOR_NOREV_MIPS_R4000,
                            Buffer, DIM (Buffer)) ;
                break ;

            case PROCESSOR_ALPHA_21064:
                LoadString (hinst, IDS_PROCESSOR_NOREV_ALPHA_21064,
                            Buffer, DIM (Buffer)) ;
                break ;
        }
    }

    SetDlgItemText (hwnd, IDC_ABOUT_PROCESSORVERSION, Buffer) ;
}

//
//  DWORD formatMessageFromString (LPCTSTR Format, LPTSTR  Buffer, 
//                                                          DWORD Size, ...)
//
//  Format          Format string containing message insertion markers
//  Buffer          Output string buffer
//  Size            Size of output string buffer
//  ...             Variable number of optional parameter
//                  
//
//  PURPOSE:        
//                  Convenient helper function for calling FormatMessage
//
//  COMMENTS:
//

static DWORD
formatMessageFromString (LPCTSTR Format, LPTSTR  Buffer, DWORD Size, ...)
{
    DWORD   dwRet ;
    va_list marker ;

    va_start (marker, Size) ;              // Initialize variable arguments

    dwRet = FormatMessage (FORMAT_MESSAGE_FROM_STRING,
                           Format, 0, 0,
                           Buffer, Size,
                           &marker) ;

    va_end (marker) ;                       // Reset variable arguments

    return dwRet ;
}


//
//  void aboutDlg_OnCommand (HWND hwnd, int id, hwnd hwndCtl, UINT codeNotify)
//
//  hwnd            Window handle for the dialog box window
//  id              Specifies the identifier of the menu item, control, or 
//                  accelerator.
//  hwndCtl         Handle of the control sending the message if the message
//                  is from a control, otherwise, this parameter is NULL. 
//  codeNotify      Specifies the notification code if the message is from a 
//                  control. This parameter is 1 when the message is from an
//                  accelerator. This parameter is 0 when the message is from
//                  a menu.
//
//  PURPOSE:
//                  Handle the keyboard and control notifications.
//                  An OK button press, or Enter/Esc keypress
//                  all dismiss the About dialog box.
//                  
//
//  COMMENTS:
//

static
void aboutDlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
        case IDOK:                          // OK pushbutton/Enter keypress
        case IDCANCEL:                      // Esc keypress
            EndDialog (hwnd, TRUE) ;        // Dismiss the about dialog box
            break ;

        default:
            FORWARD_WM_COMMAND (hwnd, id, hwndCtl, codeNotify, DefWindowProc) ;
        }
}

//
//  NTTYPE getNTVersion ()
//
//  PURPOSE:        
//                  Determine the specific variant of Windows NT on which
//                  we're running.
//
//  COMMENTS:
//                  Windows NT 4.0 does not follow the documentation
//                  in the knowledgebase. Specifically, both versions set the
//                  appropriate registry key to "LANMANNT" rather than 
//                  "SERVERNT".
//

static NTTYPE
getNTVersion ()
{
    TCHAR Value [256] ;
    DWORD dwType = 0;
    DWORD dwSize = sizeof (Value) ; // sizeof, not DIM
    HKEY  hKey   = NULL ;
    LONG  lStatus ;

static const TCHAR ProductOptions []   = TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions") ;
static const TCHAR ProductType []      = TEXT("ProductType") ;
static const TCHAR WinNT []            = TEXT("WINNT") ;      //  Windows NT Workstation is running
static const TCHAR ServerNT []         = TEXT("SERVERNT") ;   //  Windows NT Server (3.5 or later) is running
static const TCHAR AdvancedServerNT [] = TEXT("LANMANNT") ;   //  Windows NT Advanced Server (3.1) is running

    lStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE,ProductOptions, 0, KEY_QUERY_VALUE, &hKey) ;
    if (ERROR_SUCCESS != lStatus)
        return typeDefault ;            // Windows NT

    lStatus = RegQueryValueEx (hKey, ProductType, NULL, &dwType,
                                                (LPBYTE) Value, &dwSize) ; 
    RegCloseKey (hKey) ;
    if (ERROR_SUCCESS != lStatus)
        return typeDefault ;            // Windows NT

    if (0 == _tcsicmp (WinNT, Value))
        return typeWorkstation ;        // Windows NT Workstation
    else if (0 == _tcsicmp (ServerNT, Value))
        return typeServer ;             // Windows NT Server
    else if (0 == _tcsicmp (AdvancedServerNT, Value))
        return typeServer ;             // Windows NT Advanced Server (3.1)

    return typeDefault ;                // Windows NT
}

void doAbout(HWND hwnd)
{
 HINSTANCE hinst = GetWindowInstance (hwnd) ;
 LPDLGTEMPLATE temp = makeAboutBoxTemplate();
 if(temp != NULL)
    { /* call it */
     int error;
     DialogBoxIndirect (hinst, temp, hwnd, (DLGPROC) aboutDlgProc) ;
     error = GetLastError();  // in case of error, we can look at this
     free(temp);  // release it.  
    } /* call it */
}

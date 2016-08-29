// About.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "About.h"
#include "ver.h"
#include "WinReg.h"

#define DIM(x) (sizeof(x) / sizeof(x[0]))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static DWORD aContextIds [] = { 
#if 0
    IDC_ABOUT_FILEDESCRIPTION,  IDH_ABOUT_FILEDESCRIPTION,
    IDC_ABOUT_VERSION,          IDH_ABOUT_VERSION,
    IDC_ABOUT_LEGALCOPYRIGHT,   IDH_ABOUT_LEGALCOPYRIGHT,
    IDC_ABOUT_COMMENTS,         IDH_ABOUT_COMMENTS,
    IDC_ABOUT_OSVERSION,        IDH_ABOUT_OSVERSION,
    IDC_ABOUT_PROCESSORVERSION, IDH_ABOUT_PROCESSORVERSION,
    IDC_ABOUT_LEGALTRADEMARKS,  IDH_ABOUT_LEGALTRADEMARKS,
#endif
    0,                          0
} ;

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Control(pDX, IDC_ABOUT_LEGALCOPYRIGHT, c_LegalCopyright);
	DDX_Control(pDX, IDC_ABOUT_FILEDESCRIPTION, c_FileDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	ON_WM_CONTEXTMENU()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers

void CAbout::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    AfxGetApp()->WinHelp (HELP_CONTEXTMENU, (DWORD) (LPVOID) aContextIds) ;
}

BOOL CAbout::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    ASSERT (HELPINFO_WINDOW == pHelpInfo->iContextType) ;

    if (HELPINFO_WINDOW == pHelpInfo->iContextType) {    // Must be for a control!

        int             nID ;
        int             nIndex ;

        // Get the control's window handle
        HWND hwndCtl = (HWND)pHelpInfo->hItemHandle ;

        ASSERT (NULL != hwndCtl) ;
        ASSERT (::IsWindow (hwndCtl)) ;

        // Get this control's ID
        nID = ::GetDlgCtrlID (hwndCtl) ;

        // Don't bother running WinHelp unless we have help for the control
        for (nIndex = 0 ; nIndex < DIM (aContextIds) - 2; nIndex += 2) {
            if (aContextIds [nIndex] == (DWORD) nID) {
                AfxGetApp()->WinHelp (HELP_WM_HELP, 
				     (DWORD) (LPVOID) aContextIds) ;
                return TRUE ;
            }
        }
    }
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

static const TCHAR szValueNameBase [] = TEXT("\\StringFileInfo\\040904E4\\") ;
static const TCHAR szProductName []   = TEXT("ProductName") ;

// Number of characters in the base portion of the value name string
#define BASECHARS    (DIM(szValueNameBase) - 1)

void CAbout::DisplayExecutableVersionInfo ()
{
    BOOL                bResult ;               // Result of Boolean functions
    DWORD               dwVerInfoSize ;         // Size of version information
    DWORD               dwHandle ;              // Extraneous but required parameter
    HMODULE             hmod ;                  // Application's module handle
    LPVOID              pVerInfo ;              // File version info pointer
    LPVOID              pValue ;                // Value from version info
    TCHAR               szFullPath [_MAX_PATH] ;// Application executable path
    TCHAR               szValueName [256] ;     // Name of value to retrieve
    UINT                uLength ;               // Length of retrieved value
    CWnd *		Ctl;			// Control

    // Get the full path to this executable file
    hmod = ::GetModuleHandle (NULL) ;
    ::GetModuleFileName (hmod, szFullPath, DIM(szFullPath)) ;

    // Determine the size buffer needed to store the version information:
    dwVerInfoSize = ::GetFileVersionInfoSize (szFullPath, &dwHandle) ;
    if (0 == dwVerInfoSize)
        return ;

    // Allocate a buffer for the version info block
    pVerInfo = malloc (dwVerInfoSize) ;
    ASSERT (NULL != pVerInfo) ;
    if (NULL == pVerInfo)
        return ;

    // Read the version info block into the buffer
    VERIFY(::GetFileVersionInfo (szFullPath, dwHandle, dwVerInfoSize, pVerInfo)) ;

    // Build value name base string...
	lstrcpy (szValueName, szValueNameBase) ;
	
	// Build the \StringFileInfo\040904E4\ProductName value name
	lstrcpy (szValueName + BASECHARS, szProductName) ;

	// Retrieve the value
	bResult = ::VerQueryValue (pVerInfo, szValueName, &pValue, &uLength) ;

    // Format the output for the dialog caption
    // Get the current caption then append to it the ProductName value
        GetWindowText (szValueName, sizeof (szValueName) / sizeof(TCHAR)) ;
	lstrcat (szValueName, (LPCTSTR)pValue) ;

    // Change the dialog caption - normally "About <ProductName>"
	SetWindowText (szValueName) ;

	// For each control in the dialog...
    //  fetch the version info name from the control's initial window text.
    //  retrieve the value with that name,
    //  change the control's window text to the retrieved value.
    // Technique derived from GENERIC.C.

    
    Ctl = GetWindow (GW_CHILD) ;
    while (NULL != Ctl) 
       {
        // Build value name base string...
	lstrcpy (szValueName, szValueNameBase) ;
	
    	// Build the \StringFileInfo\040904E4\<ControlText> value name
        // The Win32 API contains the following predefined version information strings:
        //     CompanyName              LegalCopyright     
        //     FileDescription          OriginalFilename   
        //     FileVersion              ProductName        
        //     InternalName             ProductVersion     
        
        // Get the control's text...
        Ctl->GetWindowText ( szValueName + BASECHARS,
                             DIM(szValueName) - BASECHARS) ;

        // Retrieve the value
	bResult = ::VerQueryValue (pVerInfo, szValueName, &pValue, &uLength) ;

        // If version information is available and the version information name exists...
        if (bResult)
            // If a value exists for the version information name...
            if (0 != uLength && NULL != pValue)
                // Change the control's text to the version information value
                Ctl->SetWindowText ((LPCTSTR)pValue) ;

        Ctl = Ctl->GetWindow (GW_HWNDNEXT) ;
    }

    // Free the memory for the version information block
    free (pVerInfo) ;
}

void CAbout::DisplayOperatingSystemVersionInfo ()
{
    BOOL      bResult ;
    NTTYPE    NtOsType ;
    TCHAR     OSVer [256] ;
    CString   FormatString ;

    // Get OS version information
    OSVERSIONINFO       osver ;
    osver.dwOSVersionInfoSize = sizeof (osver) ;    // Must initialize size member!

    bResult = ::GetVersionEx (&osver) ;       // Retrieve version info
    ASSERT (FALSE != bResult) ;
    if (FALSE == bResult)
        return ;

    switch (osver.dwPlatformId) 
       {
        case VER_PLATFORM_WIN32_NT:         // Windows NT
            NtOsType = GetNTVersion () ;
            FormatString.LoadString (IDS_PLATFORM_WIN32_NT + NtOsType);
            break ;

        case VER_PLATFORM_WIN32s:           // Win32s on Windows 3.1
            FormatString.LoadString (IDS_PLATFORM_WIN32s);
            break ;

        case VER_PLATFORM_WIN32_WINDOWS:     // Windows 95
            FormatString.LoadString(IDS_PLATFORM_WIN32_WINDOWS);
            // Windows 95 encodes extra info in HIWORD(dwBuildNumber)
            // Remove unwanted junk
            osver.dwBuildNumber = LOWORD (osver.dwBuildNumber) ;
            break ;

        default:                            // Unknown operating system
	    FormatString.LoadString(IDS_PLATFORM_UNKNOWN);
            break ;
    }

    wsprintf (OSVer, FormatString,
              osver.dwMajorVersion,
              osver.dwMinorVersion,
              osver.dwBuildNumber) ;
    SetDlgItemText (IDC_ABOUT_OSVERSION, OSVer) ;
}

//
//  void CAbout::DisplayProcessorVersionInfo (DWORD dwPlatformId)
//
//  dwPlatformId    Hardware platform ID returned by GetVersionEx
//
//  PURPOSE:        Displays the processor's version
//
//  COMMENTS:
//

void CAbout::DisplayProcessorVersionInfo ()
{
    BOOL                bRecognized ;
    SYSTEM_INFO         si ;
    CString	Buffer;
    CString	Format;

    // Get current system information
    // Zero the structure as Windows 95 and older versions of Windows NT
    // do not initialize *all* fields of the structure (specifically,
    // wProcessorLevel and wProcessorRevision. Unfortunately, the
    // documentation does not say *which* versions of Windows NT do not
    // support setting these fields. Therefore use the new fields if
    // they seem to have been set. otherwise use the "obsolete" fields.

    ZeroMemory (&si, sizeof (si)) ;
    ::GetSystemInfo (&si) ;
    

    // Determine processor architecture
    bRecognized = TRUE ;
    switch (si.wProcessorArchitecture) {
        default:
            bRecognized = FALSE ;
            Buffer.LoadString (IDS_PROCESSOR_ARCHITECTURE_UNKNOWN );
            break ;

        case PROCESSOR_ARCHITECTURE_INTEL:  // Intel
            switch (si.wProcessorLevel) {
                default:
                    bRecognized = FALSE ;
                    Buffer.LoadString ( IDS_PROCESSOR_LEVEL_INTEL_UNKNOWN) ;
                    break ;

                case 3:                     // Intel 80386
                    Format.LoadString (
		    		IDS_PROCESSOR_ARCHITECTURE_INTEL_386_486);
                    Buffer.FormatMessage ( Format,
                        TEXT ("80386"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 4:                     // Intel 80486
                    Format.LoadString (
                                IDS_PROCESSOR_ARCHITECTURE_INTEL_386_486);
                    Buffer.FormatMessage ( Format,
                        TEXT ("80486"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 5:                     // Intel Pentium
                    Format.LoadString (
                                IDS_PROCESSOR_ARCHITECTURE_INTEL_PENTIUM);
                    Buffer.FormatMessage ( Format,
                        TEXT ("Pentium"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_MIPS:   // MIPS
            switch (si.wProcessorLevel) {   // 00xx - xx is 8-bit implementation #
                Format.LoadString (IDS_PROCESSOR_ARCHITECTURE_MIPS);
                default:
                    bRecognized = FALSE ;
                    Buffer.LoadString (IDS_PROCESSOR_LEVEL_MIPS_UNKNOWN);
                    break ;

                case 0x0004:                // MIPS R4000
                    Buffer.FormatMessage (Format,
                        TEXT ("R4000"),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_ALPHA:  // Alpha
            Format.LoadString (IDS_PROCESSOR_ARCHITECTURE_ALPHA);
            switch (si.wProcessorLevel) {   // xxxx - 16-bit processor version #
                default:
                    bRecognized = FALSE ;
                    Buffer.LoadString (IDS_PROCESSOR_LEVEL_ALPHA_UNKNOWN);
                    break ;

                case 21064:                 // Alpha 21064
                    Buffer.FormatMessage (Format,
                        TEXT ("21064"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 21066:                 // Alpha 21066
                    Buffer.FormatMessage ( Format,
                        TEXT ("21066"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 21164:                 // Alpha 21164
                    Buffer.FormatMessage ( Format,
                        TEXT ("21164"),
                        (TCHAR)(HIBYTE (si.wProcessorRevision) + (TCHAR) 'A'),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;
            }
            break ;

        case PROCESSOR_ARCHITECTURE_PPC:    // Power PC
            Format.LoadString (IDS_PROCESSOR_ARCHITECTURE_PPC);
            switch (si.wProcessorLevel) {   // xxxx - 16-bit processor version #
                default:
                    bRecognized = FALSE ;
                    Buffer.LoadString (IDS_PROCESSOR_LEVEL_PPC_UNKNOWN);
                    break ;

                case 1:                     // Power PC 601
                    Buffer.FormatMessage (Format,
                        TEXT ("601"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 3:                     // Power PC 603
                    Buffer.FormatMessage (Format,
                        TEXT ("603"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 4:                     // Power PC 604
                    Buffer.FormatMessage (Format,
                        TEXT ("604"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 6:                     // Power PC 603+
                    Buffer.FormatMessage (Format,
                        TEXT ("603+"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 9:                     // Power PC 604+
                    Buffer.FormatMessage (Format,
                        TEXT ("604+"),
                        HIBYTE (si.wProcessorRevision),
                        LOBYTE (si.wProcessorRevision)) ;
                    break ;

                case 20:                    // Power PC 620
                    Buffer.FormatMessage (Format,
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
    if (!bRecognized) {
        switch (si.dwProcessorType) {
            case PROCESSOR_INTEL_386:
                Buffer.LoadString (IDS_PROCESSOR_NOREV_INTEL_386);
                break ;

            case PROCESSOR_INTEL_486:
                Buffer.LoadString (IDS_PROCESSOR_NOREV_INTEL_486);
                break ;

            case PROCESSOR_INTEL_PENTIUM:
                Buffer.LoadString (IDS_PROCESSOR_NOREV_INTEL_PENTIUM);
                break ;

            case PROCESSOR_MIPS_R4000:
                Buffer.LoadString (IDS_PROCESSOR_NOREV_MIPS_R4000);
                break ;

            case PROCESSOR_ALPHA_21064:
                Buffer.LoadString (IDS_PROCESSOR_NOREV_ALPHA_21064);
                break ;
        }
    }

    SetDlgItemText (IDC_ABOUT_PROCESSORVERSION, Buffer) ;
}

BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CenterWindow (NULL) ;

	// Update controls with application version info
	DisplayExecutableVersionInfo () ;

	// Update controls with operating system version info
	DisplayOperatingSystemVersionInfo () ;

	// Update controls with processor version info
	DisplayProcessorVersionInfo () ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

NTTYPE CAbout::GetNTVersion ()
{
    TCHAR      szValue [256] ;
    DWORD      dwType = 0;
    DWORD      dwSize = sizeof (szValue) ;
    HKEY       hKey   = NULL ;
    LONG       lStatus ;

static const TCHAR szProductOptions []   = TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions") ;
static const TCHAR szProductType []      = TEXT("ProductType") ;
static const TCHAR szWinNT []            = TEXT("WINNT") ;      //  Windows NT Workstation is running
static const TCHAR szServerNT []         = TEXT("SERVERNT") ;   //  Windows NT Server (3.5 or later) is running
static const TCHAR szAdvancedServerNT [] = TEXT("LANMANNT") ;   //  Windows NT Advanced Server (3.1) is running

    lStatus = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,szProductOptions, 0, KEY_QUERY_VALUE, &hKey) ;
    if (ERROR_SUCCESS != lStatus)
        return typeDefault ;            // Windows NT

    lStatus = ::RegQueryValueEx (hKey, szProductType, NULL, &dwType, (LPBYTE)szValue, &dwSize) ; 
    ::RegCloseKey (hKey) ;
    if (ERROR_SUCCESS != lStatus)
        return typeDefault ;            // Windows NT

    if (0 == _tcsicmp (szWinNT, szValue))
        return typeWorkstation ;        // Windows NT Workstation
    else if (0 == _tcsicmp (szServerNT, szValue))
        return typeServer ;             // Windows NT Server
    else if (0 == _tcsicmp (szAdvancedServerNT, szValue))
        return typeAdvancedServer ;     // Windows NT Advanced Server (3.1)

    return typeDefault ;                // Windows NT
}

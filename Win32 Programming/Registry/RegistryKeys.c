#include "StdSDK.h"         // Standard application includes
#include "RegistryKeys.h"   // For non-static function prototypes
#include "VersionInfo.h"    // For non-static function prototypes

#include <tchar.h>          // For _tcscat


//
// Function prototypes for static functions
//

typedef struct _tagProgIDStrings {
    LPCTSTR             pszClassID ;
    LPCTSTR             pszProgID ;
    LPCTSTR             pszShortName ;
    LPCTSTR             pszLongName ;
    LPCTSTR             pszLongAppName ;
    LPCTSTR             pszModulePath ;
    LONG                nIconIndex ;
    LONG                nVersion ;
} STRINGVALUES ;

static LPCTSTR getAppRegKeyName () ;
static HKEY    createKey (HKEY hkey, LPCTSTR pszSubKey) ;
static BOOL    setStringValue (HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue) ;
static BOOL    registerStringAndValue (LPCTSTR pszKeyFormat, STRINGVALUES* pSubstitutes) ;


//  HKEY registerFileIcon ()
//
//  PURPOSE:        Register the icon for data files for the application.
//
//  HKEY_CLASSES_ROOT\<ProgID>\DefaultIcon = "<module path>, <icon index>"
//
//  COMMENTS:       This function creates/updates the registry key which
//                  associates an icon with the applications data files.
//

//  HKEY registerFileExtension ()
//
//  PURPOSE:        Register the file extension for the application's
//                  data files.
//
//  HKEY_CLASSES_ROOT\.<ext> = <ProgID>.<Version>
//
//  COMMENTS:       This function creates/updates the registry key which
//                  associates a file extension with a programmatic identifier.
//

BOOL
registerFileExtension (LPCTSTR pszExt, LPCTSTR pszProgID, int nMajorVersion)
{
    BOOL                fResult ;
    HKEY                hkey, hkeyShellNew ;
    TCHAR               szVersionSpecificProgID [64] ;

    ASSERT (NULL != pszExt) ;                   // Must pass an extension
    ASSERT ((TCHAR) '.'  == *pszExt) ;          // with a leading period
    ASSERT ((TCHAR) '\0' != *(pszExt + 1)) ;    // and at least one character

    ASSERT (NULL != pszProgID) ;            // Must pass a programmatic id
    ASSERT (_tcslen (pszProgID) > 0) ;      // containing at least one character
    ASSERT (_tcslen (pszProgID) < 40) ;     // and no more than 39 characters
    ASSERT (!_istdigit (*pszProgID)) ;      // that doesn't begin with a digit

    wsprintf (szVersionSpecificProgID, TEXT("%s.%d"),
              pszProgID, nMajorVersion) ;

    // Insure there are no more than 39 characters
    ASSERT (_tcslen (szVersionSpecificProgID) < 40) ;

    // Open/Create the registry key "HKEY_CLASSES_ROOT\<.ext>
    hkey = createKey (HKEY_CLASSES_ROOT, pszExt) ;
    ASSERT (NULL != hkey) ;

    // Set default value for this key:
    //  HKEY_CLASSES_ROOT\<.ext> = <ProgID>.<Version>
    fResult = setStringValue (hkey, NULL, szVersionSpecificProgID) ;
    ASSERT (FALSE != fResult) ;

    // Open/Create the registry key "HKEY_CLASSES_ROOT\<.ext>\ShellNew
    hkeyShellNew = createKey (hkey, TEXT("ShellNew")) ;
    ASSERT (NULL != hkeyShellNew) ;

    // Set the "NullFile" data value for this key to an empty string
    //  HKEY_CLASSES_ROOT\<.ext>\ShellNew
    //                              NullFile = ""
    fResult = setStringValue (hkeyShellNew, TEXT("NullFile"), TEXT("")) ;
    ASSERT (FALSE != fResult) ;

    // Close and release the keys
    RegCloseKey (hkeyShellNew) ;
    RegCloseKey (hkey) ;

    return fResult ;
}

//
// Registry key name format string followed
// by the associated value format string
//

// %1       CLSID string representation
// %2       ProgID (programmatic identifer/class name)
// %3       Localized class short name (for user display)
// %4       Localized class long name (for user display)
// %5       Localized application long name (for user display)
// %6       Full module path
// %7       Index in module of the icon for this class
// %8       Version number

static const TCHAR szKey0 [] =
    TEXT("%2\0")                                  TEXT("%5") ;

static const TCHAR szKey1 [] =
    TEXT("%2\\CLSID\0")                           TEXT("%1") ;

static const TCHAR szKey2 [] =
    TEXT("%2\\CurVer\0")                          TEXT("%2.%8!d!") ;

static const TCHAR szKey3 [] =
    TEXT("%2.%8!d!\0")                            TEXT("%5") ;

static const TCHAR szKey4 [] =
    TEXT("%2.%8!d!\\CLSID\0")                     TEXT("%1") ;

static const TCHAR szKey5 [] =
    TEXT("%2.%8!d!\\protocol\\StdFileEditing\\server\0") TEXT("%6") ;

static const TCHAR szKey6 [] =
    TEXT("%2.%8!d!\\protocol\\StdFileEditing\\Verb\\0\0") TEXT("Edit") ;

static const TCHAR szKey7 [] =
    TEXT("%2.%8!d!\\shell\\open\\command\0")      TEXT("%6 %%1") ;

static const TCHAR szKey8 [] =
    TEXT("CLSID\\%1\0")                           TEXT("%4") ;

static const TCHAR szKey9 [] =
    TEXT("CLSID\\%1\\ProgID\0")                   TEXT("%2.%8!d!") ;

static const TCHAR szKey10 [] =
    TEXT("CLSID\\%1\\AuxUserType\\2\0")           TEXT("%3") ;

static const TCHAR szKey11 [] =
    TEXT("CLSID\\%1\\AuxUserType\\3\0")           TEXT("%5") ;

static const TCHAR szKey12 [] =
    TEXT("CLSID\\%1\\DefaultIcon\0")              TEXT("%6,%7!d!") ;

static const TCHAR szKey13 [] =
    TEXT("CLSID\\%1\\VersionIndependentProgID\0") TEXT("%2") ;



//
//  HKEY registerProgID (REFCLSID clsid,
//                       LPCTSTR pszProgID,
//                       UINT    uShortLongNamesID,
//                       UINT    uLongAppNameID,
//                       int     nIconIndex,
//                       int     nVersion) ;
//
//  PURPOSE:        Register the programmtic identifier for the
//                  application's data files.
//
//  HKEY_CLASSES_ROOT\<ProgID>                                 = <long application name>
//  HKEY_CLASSES_ROOT\<ProgID>\CLSID                           = {<class identifier>}
//  HKEY_CLASSES_ROOT\<ProgID>.<Version>                       = <long application name>
//  HKEY_CLASSES_ROOT\<ProgID>.<Version>\CLSID                 = {<class identifier>}
//  HKEY_CLASSES_ROOT\<ProgID>.<Version>\protocol\StdFileEditing\server = <module path>
//  HKEY_CLASSES_ROOT\<ProgID>.<Version>\protocol\StdFileEditing\Verb\0 = "Edit"
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}"              = <long type name>
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}\ProgID"       = <ProgID>.<Version>
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}\AuxUserType\2 = <short type name>
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}\AuxUserType\3 = <long type name>
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}\DefaultIcon\0 = <module path>, <icon index>
//  HKEY_CLASSES_ROOT\CLSID\{<class identifier>}\VersionIndependentProgID = <ProgID>
//
//  COMMENTS:       This function creates/updates the registry key which
//                  associates a file extension with a programmatic identifier.
//

BOOL
registerProgID (REFCLSID clsid,        LPCTSTR pszProgID,
                UINT uShortLongNameID, UINT uLongAppNameID,
                int nIconIndex,        int nMajorVersion)
{
    HMODULE             hmod ;
    int                 nChars ;
    STRINGVALUES        sv ;

    LPTSTR      pszLongName ;               // Pointer into szShortLongName buffer
    TCHAR       szShortLongName [256] ;     // In "Short name \nLong name" format
    TCHAR       szLongAppName [256] ;       // Registry long application name
    TCHAR       szModulePath [_MAX_PATH] ;  // Executable path
    TCHAR       szCLSID [40] ;              // String in default char set
    WCHAR       wszCLSID [40] ;             // String in UNICODE

    ASSERT (NULL != clsid) ;                // Must pass a non-NULL CLSID
    ASSERT (!IsEqualCLSID (&CLSID_NULL, clsid)) ;

    ASSERT (NULL != pszProgID) ;            // Must pass a programmatic id
    ASSERT (_tcslen (pszProgID) > 0) ;      // containing at least one character
    ASSERT (_tcslen (pszProgID) < 40) ;     // and no more than 39 characters
    ASSERT (!_istdigit (*pszProgID)) ;      // that doesn't begin with a digit

    // The ProgID (Programmatic identifier) should also contain no
    // punctuation (including underscores) except one or more periods.
    // This string uniquely identifies a given class and is intended
    // to be in a form that can be read by programmers. Programmatic
    // identifiers are not guaranteed to be universally unique.


    CoInitialize (NULL) ;   // Initialize the COM libraries

    // Convert the CLSID GUID to its string representation
    nChars = StringFromGUID2 (clsid, wszCLSID, DIM (wszCLSID)) ;
    if (0 == nChars) {
        return FALSE ;
    }
    CoUninitialize () ;     // Deinitialize the COM libraries

    // StringFromGUID2 returns a Unicode string.

#if !defined (UNICODE)
    // Convert to ANSI if not a UNICODE build
    WideCharToMultiByte (CP_ACP,            // Code page
                         0,                 // Performance and mapping flags
                         wszCLSID,          // Pointer to UNICODE string
                         -1,                // # of chars in string
                         szCLSID,           // Buffer to contain string
                         DIM (szCLSID),     // Size of the buffer
                         NULL,              // Default untranslatable char
                         NULL) ;            // Default used flag
#else
    // Copy it as is if a UNICODE build
    _tcscpy (szCLSID, wszCLSID) ;
#endif

    sv.pszClassID = szCLSID ;               // CLSID in string format
    sv.pszProgID  = pszProgID ;             // ProgID string

    hmod = GetModuleHandle (NULL) ;         // Load the short and long names
    VERIFY(LoadString (hmod, uShortLongNameID,
                       szShortLongName, DIM (szShortLongName))) ;

    sv.pszShortName = szShortLongName ;     // Short name first in buffer
    sv.pszLongName  = szShortLongName ;     // Long name defaults to short name

    pszLongName =                           // Scan for long name delimiter
        _tcschr (szShortLongName, TEXT('\n')) ;

    if (NULL != pszLongName) {              // If long name found
        sv.pszLongName = pszLongName + 1 ;  //  use it but need to
        *pszLongName = TEXT ('\0') ;        //  NULL terminate short name
    }

    ASSERT (_tcslen (sv.pszShortName) > 0) ;    // Validate lengths    
    ASSERT (_tcslen (sv.pszShortName) <= 15) ;  // of the registry strings
    ASSERT (_tcslen (sv.pszLongName) > 0) ;
    ASSERT (_tcslen (sv.pszLongName) <= 40) ;

    VERIFY(LoadString (hmod, uLongAppNameID,
                       szLongAppName, DIM (szLongAppName))) ;

    sv.pszLongAppName = szLongAppName ;         // Long application name

    GetModuleFileName (hmod, szModulePath, DIM (szModulePath)) ;

    sv.pszModulePath  = szModulePath ;          // Path of executable
    sv.nIconIndex     = nIconIndex ;            // Index of class icon
    sv.nVersion       = nMajorVersion ;         // Major version number

    registerStringAndValue (szKey0, &sv) ;
    registerStringAndValue (szKey1, &sv) ;
    registerStringAndValue (szKey2, &sv) ;
    registerStringAndValue (szKey3, &sv) ;
    registerStringAndValue (szKey4, &sv) ;
    registerStringAndValue (szKey5, &sv) ;
    registerStringAndValue (szKey6, &sv) ;
    registerStringAndValue (szKey7, &sv) ;
    registerStringAndValue (szKey8, &sv) ;
    registerStringAndValue (szKey9, &sv) ;
    registerStringAndValue (szKey10, &sv) ;
    registerStringAndValue (szKey11, &sv) ;
    registerStringAndValue (szKey12, &sv) ;
    registerStringAndValue (szKey13, &sv) ;

    return TRUE ;
}

//
//  BOOL registerStringAndValue (LPCTSTR pszKeyFormat,
//                               STRINGVALUES* pSubstitutes)
//
//  PURPOSE:        Substitutes the supplied parameters into
//                  the specified registry key and value strings.
//                  Opens, or creates if necessary, the specified
//                  registry key, then sets the value string as
//                  the default value of the key.
//
//  COMMENTS:       
//

BOOL
registerStringAndValue (LPCTSTR pszKeyFormat, STRINGVALUES* pSubstitutes)
{
    BOOL                fStatus ;
    DWORD               dwRet ;
    HKEY                hkey ;
    LPCTSTR             pszValueFormat ;
    TCHAR               szFormattedKey [256] ;
    TCHAR               szFormattedValue [256] ;

    pszValueFormat = pszKeyFormat + _tcslen (pszKeyFormat) + 1 ;

    // Format the registry key string
    dwRet = FormatMessage (FORMAT_MESSAGE_FROM_STRING |
                           FORMAT_MESSAGE_ARGUMENT_ARRAY,
                           pszKeyFormat,
                           0, 0,
                           szFormattedKey, DIM (szFormattedKey),
                           (va_list*) pSubstitutes) ;
    ASSERT (0 != dwRet) ;
    if (0 == dwRet)
        return FALSE ;

    // Format the registry value string
    dwRet = FormatMessage (FORMAT_MESSAGE_FROM_STRING |
                           FORMAT_MESSAGE_ARGUMENT_ARRAY,
                           pszValueFormat,
                           0, 0,
                           szFormattedValue, DIM (szFormattedValue),
                           (va_list*) pSubstitutes) ;
    ASSERT (0 != dwRet) ;
    if (0 == dwRet)
        return FALSE ;

    // Open/Create the registry key
    hkey = createKey (HKEY_CLASSES_ROOT, szFormattedKey) ;
    ASSERT (NULL != hkey) ;

    // Set the default value for this key
    fStatus = setStringValue (hkey, NULL, szFormattedValue) ;

    // Close the key and return the status
    RegCloseKey (hkey) ;
    return TRUE ;
}



//
//  HKEY getAppGlobalRegKey ()
//
//  PURPOSE:        Returns the registry key for this application's
//                  system-specific configuration information. This is:
//
//  HKEY_LOCAL_MACHINE\SOFTWARE\<CompanyName>\<ProductName>\<ProductVersion>\<InternalName>
//
//  COMMENTS:       This function returns a registry key which the
//                  caller must close when finished with the key.
//
//                  Most applications probably wouldn't need to use
//                  the <InternalName> sub-key as an additional
//                  qualifier.
//

HKEY
getAppGlobalRegKey ()
{
    // Get the application's registry key name
    LPCTSTR pKeyName = getAppRegKeyName () ;

    // Open/Create the registry key
    HKEY hkey = createKey (HKEY_LOCAL_MACHINE, pKeyName) ;

    return hkey ;
}


//  HKEY getAppUserRegKey ()
//
//  PURPOSE:        Returns the registry key for this application's
//                  user-specific configuration information. This is:
//
//  HKEY_CURRENT_USER\SOFTWARE\<CompanyName>\<ProductName>\<ProductVersion>\<InternalName>
//
//  COMMENTS:       This function returns a registry key which the
//                  caller must close when finished with the key.
//
//                  Most applications probably wouldn't need to use
//                  the <InternalName> sub-key as an additional
//                  qualifier.
//

HKEY
getAppUserRegKey ()
{
    // Get the application's registry key name
    LPCTSTR pKeyName = getAppRegKeyName () ;

    // Open/Create the registry key
    HKEY hkey = createKey (HKEY_CURRENT_USER, pKeyName) ;

    return hkey ;
}



//
//  LPCTSTR getAppRegKeyName ()
//
//  PURPOSE:        Returns the registry key name string
//                  for this application.
//
//  COMMENTS:
//

static LPCTSTR
getAppRegKeyName ()
{
    LPVOID              pVerInfo ;
    LPCTSTR             pValue ;

#define SOFTWAREKEY     TEXT("SOFTWARE\\")

static       TCHAR szAppRegKeyName [256] = SOFTWAREKEY ;
static const TCHAR szCompanyName []      = TEXT("CompanyName") ;
static const TCHAR szProductName []      = TEXT("ProductName") ;
static const TCHAR szProductVersion []   = TEXT("ProductVersion") ;
static const TCHAR szInternalName []     = TEXT("InternalName") ;

    // Get the version info block for the application
    pVerInfo = getVersionInfoBlock () ;
    ASSERT (NULL != pVerInfo) ;

    // Get the CompanyName key's value
    pValue = getVersionInfoValue (pVerInfo, szCompanyName) ;
    ASSERT (NULL != pValue) ;

    // Create the "\SOFTWARE\CompanyName" string
    _tcscpy (szAppRegKeyName + DIM(SOFTWAREKEY) - 1, pValue) ;

    // Create the "\SOFTWARE\CompanyName\" string
    _tcscat (szAppRegKeyName, TEXT("\\")) ;

    // Get the ProductName key's value
    pValue = getVersionInfoValue (pVerInfo, szProductName) ;
    ASSERT (NULL != pValue) ;

    // Create the "\SOFTWARE\CompanyName\ProductName" string
    _tcscat (szAppRegKeyName, pValue) ;

    // Create the "\SOFTWARE\CompanyName\ProductName\" string
    _tcscat (szAppRegKeyName, TEXT("\\")) ;

    // Get the Version key's value
    pValue = getVersionInfoValue (pVerInfo, szProductVersion) ;
    ASSERT (NULL != pValue) ;

    // Create the "\SOFTWARE\CompanyName\ProductName\Version" string
    _tcscat (szAppRegKeyName, pValue) ;

    // Create the "\SOFTWARE\CompanyName\ProductName\Version\" string
    _tcscat (szAppRegKeyName, TEXT("\\")) ;

    // Get the InternalName key's value
    pValue = getVersionInfoValue (pVerInfo, szInternalName) ;
    ASSERT (NULL != pValue) ;

    // Create the "\SOFTWARE\CompanyName\ProductName\Version\InternalName" string
    _tcscat (szAppRegKeyName, pValue) ;

    // Free the version info block
    free (pVerInfo) ;

    // Return the key name
    return szAppRegKeyName ;
}



//
//  BOOL setStringValue (HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue)
//
//  PURPOSE:        Sets the value for the specified subkey. When
//                  pszSubKey is NULL, sets the default value for hkey.
//
//  COMMENTS:
//

static BOOL
setStringValue (HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue)
{
    //  Write <hkey>\<SubKey> = <Value>
    LONG lResult =
        RegSetValueEx (hkey,                    // handle of open key 
                       pszSubKey,               // name of value to set
                       0,                       // reserved 
                       REG_SZ,                  // type of information
                       (CONST BYTE*) pszValue,  // the value
                       sizeof (TCHAR) *         // size, in bytes, of info
                       (_tcslen (pszValue) + 1)) ;
 
     return ERROR_SUCCESS == lResult ;
}



//
//  HKEY createKey (HKEY hkey, LPCTSTR pszSubKey)
//
//  PURPOSE:        Opens, or creates if necessary, the specified
//                  registry subkey under the already opened key 'hkey'.
//
//  COMMENTS:
//

static HKEY
createKey (HKEY hkey, LPCTSTR pszSubKey)
{
    DWORD               dwDisposition ;
    HKEY                hkeyNew ;

    // Open/Create the registry key
    LONG lResult =
        RegCreateKeyEx (hkey,                   // handle of open key 
                        pszSubKey,              // address of name of subkey to open
                        0,                      // reserved 
                        TEXT(""),               // Class string
                        REG_OPTION_NON_VOLATILE,// Special options flag
                        KEY_ALL_ACCESS,         // desired security access
                        NULL,                   // address of key security structure 
                        &hkeyNew,               // returned key handle address
                        &dwDisposition) ;       // address of disposition value buffer 

    ASSERT (ERROR_SUCCESS == lResult) ;

    return hkeyNew ;
}


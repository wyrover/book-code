#include "StdSDK.h"         // Standard application includes
#include "VersionInfo.h"    // For non-static function prototypes

//
//  LPVOID getVersionInfoBlock ()
//
//  pszName         Name of version resource value to retrieve
//
//  PURPOSE:        Retrieve the current module's version
//                  information resource block
//
//  COMMENTS:       This function returns allocates a buffer to contain
//                  the resource block and returns a pointer to the
//                  buffer. The caller must free the buffer when it
//                  is no longer required.
//

LPVOID
getVersionInfoBlock ()
{
    DWORD   dwHandle ;              // Required (unused) parameter
    DWORD   VerInfoSize ;           // Size of version information
    HMODULE hmod ;                  // Application's module handle
    LPVOID  pVerInfo ;              // File version info pointer
    TCHAR   FullPath [_MAX_PATH] ;// Application executable path

    // Get the full path to this executable file
    hmod = GetModuleHandle (NULL) ;
    GetModuleFileName (hmod, FullPath, DIM(FullPath)) ;

    // Determine the size buffer needed to store the version information:
    VerInfoSize = GetFileVersionInfoSize (FullPath, &dwHandle) ;
    if (0 == VerInfoSize)
        return NULL ;

    // Allocate a buffer for the version info block
    pVerInfo = malloc (VerInfoSize) ;
    ASSERT (NULL != pVerInfo) ;

    // Read the version info block into the buffer
    VERIFY(GetFileVersionInfo (FullPath, dwHandle, VerInfoSize, pVerInfo)) ;

    return pVerInfo ;
}


//
//  LPCTSTR getVersionInfoValue (LPVOID pVerInfo, LPCTSTR pszName)
//
//  pVerInfo        Pointer to version information block
//  pszName         Name of version resource value to retrieve
//
//  PURPOSE:        Retrieve the specified version resource value
//
//  COMMENTS:       This function returns a pointer into the block
//                  pointed to by pVerInfo. The returned value
//                  string pointer remains valid only while the
//                  pVerInfo block remains allocated.
//

LPCTSTR
getVersionInfoValue (LPVOID pVerInfo, LPCTSTR pszName)
{
    BOOL   Result ;                // Result of Boolean functions
    LPVOID pValue ;                // Value from version info
    LPTSTR pRetValue ;             // Pointer to returned string
    TCHAR  ValueName [256] ;       // Name of value to retrieve
    UINT   Length ;                // Length of retrieved value

    //
    // 040904E4 means US English, Windows MultiLingual code page
    static const TCHAR ValueNameBase [] = TEXT("\\StringFileInfo\\040904E4\\") ;

    // Builds the "\StringFileInfo\040904E4\" base string
    _tcscpy (ValueName, ValueNameBase) ;

    // Builds the "\StringFileInfo\040904E4\<ValueName>" string
    _tcscpy (ValueName + DIM(ValueNameBase) - 1, pszName) ;

    // Retrieve a pointer to the value, if one
    Result = VerQueryValue (pVerInfo, ValueName, &pValue, &Length) ;

    // Initially nothing to return
    pRetValue = NULL ;
    
    // If the version information name exists
    // and a value exists for the version information name...
    if (Result && 0 != Length && NULL != pValue) {
        pRetValue = pValue ;
    }
    
    // Return the requested version info value or NULL
    return pRetValue ;
}

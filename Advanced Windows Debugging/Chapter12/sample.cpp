/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#pragma warning(disable:4214 4201 4115 )

#ifdef __cplusplus
extern "C" {
#endif
    #include <ntddk.h>
#ifdef __cplusplus
} // extern "C"
#endif
//#include <ntrtl.h>
#//include <nturtl.h>
#define WIN32_STATUS
#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
 
//#include <strsafe.h>
 
//#include <comdef.h>
//#include <sgthelper.h>
 
WCHAR g_ModuleName[MAX_PATH];
HINSTANCE g_hModule;
 
typedef struct _TEB32 {
     BYTE  Spare[0x30];
     DWORD ProcessEnvironmentBlock;
} TEB32, *PTEB32;
 
TEB32 g_TEB32;
 
BOOL WINAPI 
DllMain(
    HINSTANCE hinstDLL, 
    DWORD dwReason,   
    LPVOID lpvReserved 
    )
/*++
 
Routine description:
 
    Loader entry point for the module. 
    In PROCESS_ATTACH, it checks the initialization outcome of the globals.
 
 
Parameters:
 
    hinstDLL     - handle to the module
 
    dwReason     - call reason from the loader
 
    lpvReserved  - reserved pointer
 
Return values:
 
    TRUE - the invocation fot the given call reason suceeded
 
    FALSE - the invocation for the given call reason failed
 
--*/    
{
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH: {
        
        g_hModule = hinstDLL;        
        DisableThreadLibraryCalls(hinstDLL);
        
    }   return TRUE;
    case DLL_PROCESS_DETACH:
        return TRUE;
    }
    return TRUE;
}
 

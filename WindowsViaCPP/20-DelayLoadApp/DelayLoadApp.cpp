/******************************************************************************
Module:  DelayLoadApp.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <Windowsx.h>
#include <tchar.h>
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


#include <Delayimp.h>   // For error handling & advanced features
#include "..\20-DelayLoadLib\DelayLoadLib.h"    // My DLL function prototypes


///////////////////////////////////////////////////////////////////////////////


// Statically link __delayLoadHelper2/__FUnloadDelayLoadedDLL2
#pragma comment(lib, "Delayimp.lib")

// Note: it is not possible to use #pragma comment(linker, "") 
//       for /DELAYLOAD and /DELAY


// The name of the Delay-Load module (only used by this sample app)
TCHAR g_szDelayLoadModuleName[] = TEXT("20-DelayLoadLib");


///////////////////////////////////////////////////////////////////////////////


// Forward function prototype
LONG WINAPI DelayLoadDllExceptionFilter(PEXCEPTION_POINTERS pep);


///////////////////////////////////////////////////////////////////////////////


void IsModuleLoaded(PCTSTR pszModuleName) {

   HMODULE hmod = GetModuleHandle(pszModuleName);
   char sz[100];
#ifdef UNICODE
   StringCchPrintfA(sz, _countof(sz), "Module \"%S\" is %Sloaded.", 
      pszModuleName, (hmod == NULL) ? L"not " : L"");
#else
   StringCchPrintfA(sz, _countof(sz), "Module \"%s\" is %sloaded.", 
      pszModuleName, (hmod == NULL) ? "not " : "");
#endif
   chMB(sz);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Wrap all calls to delay-load DLL functions inside SEH
   __try {
      int x = 0;

      // If you're in the debugger, try the new Debug.Modules menu item to 
      // see that the DLL is not loaded prior to executing the line below
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib();  // Attempt to call delay-load function

      // Use Debug.Modules to see that the DLL is now loaded
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib2(); // Attempt to call delay-load function

      // Unload the delay-loaded DLL
      // NOTE: Name must exactly match /DelayLoad:(DllName)
		PCSTR pszDll = "20-DelayLoadLib.dll";
      __FUnloadDelayLoadedDLL2(pszDll);

      // Use Debug.Modules to see that the DLL is now unloaded
      IsModuleLoaded(g_szDelayLoadModuleName);

      x = fnLib();  // Attempt to call delay-load function

      // Use Debug.Modules to see that the DLL is loaded again
      IsModuleLoaded(g_szDelayLoadModuleName);
   }
   __except (DelayLoadDllExceptionFilter(GetExceptionInformation())) {
      // Nothing to do in here, thread continues to run normally
   }

   // More code can go here...

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


LONG WINAPI DelayLoadDllExceptionFilter(PEXCEPTION_POINTERS pep) {

   // Assume we recognize this exception
   LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;  

   // If this is a Delay-load problem, ExceptionInformation[0] points 
   // to a DelayLoadInfo structure that has detailed error info
   PDelayLoadInfo pdli = 
      PDelayLoadInfo(pep->ExceptionRecord->ExceptionInformation[0]);

   // Create a buffer where we construct error messages
   char sz[500] = { 0 };

   switch (pep->ExceptionRecord->ExceptionCode) {
   case VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND):
      // The DLL module was not found at runtime
      StringCchPrintfA(sz, _countof(sz), "Dll not found: %s", pdli->szDll); 
      break;

   case VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND):
      // The DLL module was found, but it doesn't contain the function
      if (pdli->dlp.fImportByName) {
         StringCchPrintfA(sz, _countof(sz), "Function %s was not found in %s",
            pdli->dlp.szProcName, pdli->szDll);
      } else {
         StringCchPrintfA(sz, _countof(sz), "Function ordinal %d was not found in %s",
            pdli->dlp.dwOrdinal, pdli->szDll);
      }
      break; 

   default:
      // We don't recognize this exception
      lDisposition = EXCEPTION_CONTINUE_SEARCH;  
      break;
   }

   if (lDisposition == EXCEPTION_EXECUTE_HANDLER) {
      // We recognized this error and constructed a message, show it
      chMB(sz);
   }

   return(lDisposition);
}


///////////////////////////////////////////////////////////////////////////////


// Skeleton DliHook function that does nothing interesting
FARPROC WINAPI DliHook(unsigned dliNotify, PDelayLoadInfo pdli) {

   FARPROC fp = NULL;   // Default return value

   // NOTE: The members of the DelayLoadInfo structure pointed
   // to by pdli shows the results of progress made so far. 

   switch (dliNotify) {
   case dliStartProcessing:
      // Called when __delayLoadHelper2 attempts to find a DLL/function
      // Return 0 to have normal behavior or nonzero to override
      // everything (you will still get dliNoteEndProcessing)
      break;

   case dliNotePreLoadLibrary:
      // Called just before LoadLibrary
      // Return NULL to have __delayLoadHelper2 call LoadLibary
      // or you can call LoadLibrary yourself and return the HMODULE
      fp = (FARPROC) (HMODULE) NULL;
      break;

   case dliFailLoadLib:
      // Called if LoadLibrary fails
      // Again, you can call LoadLibary yourself here and return an HMODULE
      // If you return NULL, __delayLoadHelper2 raises the 
      // ERROR_MOD_NOT_FOUND exception
      fp = (FARPROC) (HMODULE) NULL;
      break;

   case dliNotePreGetProcAddress:
      // Called just before GetProcAddress
      // Return NULL to have __delayLoadHelper2 call GetProcAddress,
      // or you can call GetProcAddress yourself and return the address
      fp = (FARPROC) NULL;
      break;

   case dliFailGetProc:
      // Called if GetProcAddress fails
      // You can call GetProcAddress yourself here and return an address
      // If you return NULL, __delayLoadHelper2 raises the 
      // ERROR_PROC_NOT_FOUND exception
      fp = (FARPROC) NULL;
      break;

   case dliNoteEndProcessing:
      // A simple notification that __delayLoadHelper2 is done
      // You can examine the members of the DelayLoadInfo structure
      // pointed to by pdli and raise an exception if you desire
      break;
   }

   return(fp);
}


///////////////////////////////////////////////////////////////////////////////


// Tell __delayLoadHelper2 to call my hook function
PfnDliHook __pfnDliNotifyHook2  = DliHook;
PfnDliHook __pfnDliFailureHook2 = DliHook;


//////////////////////////////// End of File //////////////////////////////////

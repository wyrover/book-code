/******************************************************************************
Module:  CmnHdr.h
Notices: Copyright (c) 2000 Jeffrey Richter
Purpose: Common header file containing handy macros and definitions
         used throughout all the applications in the book.
         See Appendix A.
******************************************************************************/


#pragma once   // Include this header file once per compilation unit


//////////////////////// Windows Version Build Option /////////////////////////


#define _WIN32_WINNT 0x0500
//#define WINVER       0x0500


//////////////////////////// Unicode Build Option /////////////////////////////


// If we are not compiling for an x86 CPU, we always compile using Unicode.
#ifndef _M_IX86
#define UNICODE
#endif

// To compile using Unicode on the x86 CPU, uncomment the line below.
//#define UNICODE

// When using Unicode Windows functions, use Unicode C-Runtime functions too.
#ifdef UNICODE
#define _UNICODE
#endif


///////////////////////// Include Windows Definitions /////////////////////////


#pragma warning(push, 3)
#include <Windows.h>
#pragma warning(pop) 
#pragma warning(push, 4)


///////////// Verify that the proper header files are being used //////////////


#ifndef WT_EXECUTEINPERSISTENTIOTHREAD
#pragma message("You are not using the latest Platform SDK header/library ")
#pragma message("files. This may prevent the project from building correctly.")
#endif


////////////// Allow code to compile cleanly at warning level 4 ///////////////


/* nonstandard extension 'single line comment' was used */
#pragma warning(disable:4001)

// unreferenced formal parameter
#pragma warning(disable:4100)

// Note: Creating precompiled header 
#pragma warning(disable:4699)

// function not inlined
#pragma warning(disable:4710)

// unreferenced inline function has been removed
#pragma warning(disable:4514)

// assignment operator could not be generated
#pragma warning(disable:4512)


///////////////////////// Pragma message helper macro /////////////////////////


/* 
When the compiler sees a line like this:
   #pragma chMSG(Fix this later)

it outputs a line like this:

  c:\CD\CmnHdr.h(82):Fix this later

You can easily jump directly to this line and examine the surrounding code.
*/

#define chSTR2(x)	   #x
#define chSTR(x)	chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) "):" #desc)


////////////////////////////// chINRANGE Macro ////////////////////////////////


// This macro returns TRUE if a number is between two others
#define chINRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))


//////////////////////////////// chDIMOF Macro ////////////////////////////////


// This macro evaluates to the number of elements in an array. 
#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))


///////////////////////////// chBEGINTHREADEX Macro ///////////////////////////


// This macro function calls the C runtime's _beginthreadex function. 
// The C runtime library doesn't want to have any reliance on Windows' data 
// types such as HANDLE. This means that a Windows programmer needs to cast
// values when using _beginthreadex. Since this is terribly inconvenient, 
// I created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadId)                 \
      ((HANDLE)_beginthreadex(                      \
         (void *)        (psa),                     \
         (unsigned)      (cbStack),                 \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *)        (pvParam),                 \
         (unsigned)      (fdwCreate),               \
         (unsigned *)    (pdwThreadId)))


////////////////// DebugBreak Improvement for x86 platforms ///////////////////


#ifdef _X86_
#define DebugBreak()    _asm { int 3 }
#endif


/////////////////////////// Software Exception Macro //////////////////////////


// Useful macro for creating your own software exception codes
#define MAKESOFTWAREEXCEPTION(Severity, Facility, Exception) \
   ((DWORD) ( \
   /* Severity code    */  (Severity       ) |     \
   /* MS(0) or Cust(1) */  (1         << 29) |     \
   /* Reserved(0)      */  (0         << 28) |     \
   /* Facility code    */  (Facility  << 16) |     \
   /* Exception code   */  (Exception <<  0)))


/////////////////////////// Quick MessageBox Macro ////////////////////////////


inline void chMB(PCSTR s) {
   char szTMP[128];
   GetModuleFileNameA(NULL, szTMP, chDIMOF(szTMP));
   MessageBoxA(GetActiveWindow(), s, szTMP, MB_OK);
}


//////////////////////////// Assert/Verify Macros /////////////////////////////


inline void chFAIL(PSTR szMsg) {
   chMB(szMsg);
   DebugBreak();
}


// Put up an assertion failure message box.
inline void chASSERTFAIL(LPCSTR file, int line, PCSTR expr) {
   char sz[128];
   wsprintfA(sz, "File %s, line %d : %s", file, line, expr);
   chFAIL(sz);
}


// Put up a message box if an assertion fails in a debug build.
#ifdef _DEBUG
#define chASSERT(x) if (!(x)) chASSERTFAIL(__FILE__, __LINE__, #x)
#else
#define chASSERT(x)
#endif


// Assert in debug builds, but don't remove the code in retail builds.
#ifdef _DEBUG
#define chVERIFY(x) chASSERT(x)
#else
#define chVERIFY(x) (x)
#endif


/////////////////////////// chHANDLE_DLGMSG Macro /////////////////////////////


// The normal HANDLE_MSG macro in WindowsX.h does not work properly for dialog
// boxes because DlgProc return a BOOL instead of an LRESULT (like
// WndProcs). This chHANDLE_DLGMSG macro corrects the problem:
#define chHANDLE_DLGMSG(hwnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hwnd, uMsg,     \
      HANDLE_##message((hwnd), (wParam), (lParam), (fn))))


//////////////////////// Dialog Box Icon Setting Macro ////////////////////////


// Sets the dialog box icons
inline void chSETDLGICONS(HWND hwnd, int idi) {
   SendMessage(hwnd, WM_SETICON, TRUE,  (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
         MAKEINTRESOURCE(idi)));
   SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM) 
      LoadIcon((HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
      MAKEINTRESOURCE(idi)));
}
    

/////////////////////////// OS Version Check Macros ///////////////////////////


inline void chWindows9xNotAllowed() {
   OSVERSIONINFO vi = { sizeof(vi) };
   GetVersionEx(&vi);
   if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
      chMB("This application requires features not present in Windows 9x.");
      ExitProcess(0);
   }
}


inline void chWindows2000Required() {
   OSVERSIONINFO vi = { sizeof(vi) };
   GetVersionEx(&vi);
   if ((vi.dwPlatformId != VER_PLATFORM_WIN32_NT) && (vi.dwMajorVersion < 5)) {
      chMB("This application requires features present in Windows 2000.");
      ExitProcess(0);
   }
}


///////////////////////////// UNICODE Check Macro /////////////////////////////


// Since Windows 98 does not support Unicode, issue an error and terminate
// the process if this is a native Unicode build running on Windows 98

// This is accomplished by creating a global C++ object. Its constructor is 
// executed before WinMain.

#ifdef UNICODE

class CUnicodeSupported {
public:
   CUnicodeSupported() {
      if (GetWindowsDirectoryW(NULL, 0) <= 0) {
         chMB("This application requires an OS that supports Unicode.");
         ExitProcess(0);
      }
   }
};

// "static" stops the linker from complaining that multiple instances of the
// object exist when a single project contains multiple source files.
static CUnicodeSupported g_UnicodeSupported;

#endif


/////////////////////////// Force Windows subsystem ///////////////////////////


#pragma comment(linker, "/subsystem:Windows")


///////////////////////////////// End of File /////////////////////////////////

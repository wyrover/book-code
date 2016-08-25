/******************************************************************************
Module:  LastMsgBoxInfoLib.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#define WINVER       0x0500
#include "..\CmnHdr.h"
#include <WindowsX.h>
#include <tchar.h>
#include <stdio.h>
#include "APIHook.h"

#define LASTMSGBOXINFOLIBAPI extern "C" __declspec(dllexport)
#include "LastMsgBoxInfoLib.h"


///////////////////////////////////////////////////////////////////////////////


// Prototypes for the hooked functions
typedef int (WINAPI *PFNMESSAGEBOXA)(HWND hWnd, PCSTR pszText, 
   PCSTR pszCaption, UINT uType);

typedef int (WINAPI *PFNMESSAGEBOXW)(HWND hWnd, PCWSTR pszText, 
   PCWSTR pszCaption, UINT uType);


// We need to reference these variables before we create them.
extern CAPIHook g_MessageBoxA;
extern CAPIHook g_MessageBoxW;


///////////////////////////////////////////////////////////////////////////////


// This function sends the MessageBox info to our main dialog box
void SendLastMsgBoxInfo(BOOL fUnicode, 
   PVOID pvCaption, PVOID pvText, int nResult) {

   // Get the pathname of the process displaying the message box
   char szProcessPathname[MAX_PATH];
   GetModuleFileNameA(NULL, szProcessPathname, MAX_PATH);

   // Convert the return value into a human-readable string
   PCSTR pszResult = "(Unknown)";
   switch (nResult) {
      case IDOK:       pszResult = "Ok";        break;
      case IDCANCEL:   pszResult = "Cancel";    break;
      case IDABORT:    pszResult = "Abort";     break;
      case IDRETRY:    pszResult = "Retry";     break;
      case IDIGNORE:   pszResult = "Ignore";    break;
      case IDYES:      pszResult = "Yes";       break;
      case IDNO:       pszResult = "No";        break;
      case IDCLOSE:    pszResult = "Close";     break;
      case IDHELP:     pszResult = "Help";      break;
      case IDTRYAGAIN: pszResult = "Try Again"; break;
      case IDCONTINUE: pszResult = "Continue";  break;
   }

   // Construct the string to send to the main dialog box
   char sz[2048];
   wsprintfA(sz, fUnicode 
      ? "Process: (%d) %s\r\nCaption: %S\r\nMessage: %S\r\nResult: %s"
      : "Process: (%d) %s\r\nCaption: %s\r\nMessage: %s\r\nResult: %s",
      GetCurrentProcessId(), szProcessPathname,
      pvCaption, pvText, pszResult);

   // Send the string to the main dialog box
   COPYDATASTRUCT cds = { 0, lstrlenA(sz) + 1, sz };
   FORWARD_WM_COPYDATA(FindWindow(NULL, TEXT("Last MessageBox Info")), 
      NULL, &cds, SendMessage);
}


///////////////////////////////////////////////////////////////////////////////


// This is the MessageBoxW replacement function
int WINAPI Hook_MessageBoxW(HWND hWnd, PCWSTR pszText, LPCWSTR pszCaption, 
   UINT uType) {

   // Call the original MessageBoxW function
   int nResult = ((PFNMESSAGEBOXW)(PROC) g_MessageBoxW)
      (hWnd, pszText, pszCaption, uType);

   // Send the information to the main dialog box
   SendLastMsgBoxInfo(TRUE, (PVOID) pszCaption, (PVOID) pszText, nResult);

   // Return the result back to the caller
   return(nResult);
}


///////////////////////////////////////////////////////////////////////////////


// This is the MessageBoxA replacement function
int WINAPI Hook_MessageBoxA(HWND hWnd, PCSTR pszText, PCSTR pszCaption, 
   UINT uType) {

   // Call the original MessageBoxA function
   int nResult = ((PFNMESSAGEBOXA)(PROC) g_MessageBoxA)
      (hWnd, pszText, pszCaption, uType);

   // Send the infomration to the main dialog box
   SendLastMsgBoxInfo(FALSE, (PVOID) pszCaption, (PVOID) pszText, nResult);

   // Return the result back to the caller
   return(nResult);
}


///////////////////////////////////////////////////////////////////////////////


// Hook the MessageBoxA and MessageBoxW functions
CAPIHook g_MessageBoxA("User32.dll", "MessageBoxA", 
   (PROC) Hook_MessageBoxA, TRUE);

CAPIHook g_MessageBoxW("User32.dll", "MessageBoxW", 
   (PROC) Hook_MessageBoxW, TRUE);


// Since we do DLL injection with Windows' hooks, we need to save the hook
// handle in a shared memory block (Windows 2000 actually doesn't need this)
#pragma data_seg("Shared")
HHOOK g_hhook = NULL;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,rws")


///////////////////////////////////////////////////////////////////////////////


static LRESULT WINAPI GetMsgProc(int code, WPARAM wParam, LPARAM lParam) {

   // NOTE: On Windows 2000, the 1st parameter to CallNextHookEx can 
   // be NULL.  On Windows 98, it must be the hook handle.
   return(CallNextHookEx(g_hhook, code, wParam, lParam));
}


///////////////////////////////////////////////////////////////////////////////


// Returns the HMODULE that contains the specified memory address
static HMODULE ModuleFromAddress(PVOID pv) {

   MEMORY_BASIC_INFORMATION mbi;
   return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) 
      ? (HMODULE) mbi.AllocationBase : NULL);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI LastMsgBoxInfo_HookAllApps(BOOL fInstall, DWORD dwThreadId) {

   BOOL fOk;

   if (fInstall) {

      chASSERT(g_hhook == NULL); // Illegal to install twice in a row

      // Install the Windows' hook
      g_hhook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, 
         ModuleFromAddress(LastMsgBoxInfo_HookAllApps), dwThreadId);

      fOk = (g_hhook != NULL);
   } else {

      chASSERT(g_hhook != NULL); // Can't uninstall if not installed
      fOk = UnhookWindowsHookEx(g_hhook);
      g_hhook = NULL;
   }

   return(fOk);
}


//////////////////////////////// End of File //////////////////////////////////

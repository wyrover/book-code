/******************************************************************************
Module:  DIPSLib.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <CommCtrl.h>

#define DIPSLIBAPI __declspec(dllexport)
#include "DIPSLib.h"
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
// This function forces the debugger to be invoked
void ForceDebugBreak() {
   __try { DebugBreak(); }
   __except(UnhandledExceptionFilter(GetExceptionInformation())) { }
}
#else
#define ForceDebugBreak()
#endif


///////////////////////////////////////////////////////////////////////////////


// Forward references
LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////////


// Instruct the compiler to put the g_hHook data variable in 
// its own data section called Shared. We then instruct the 
// linker that we want to share the data in this section 
// with all instances of this application.
#pragma data_seg("Shared")
HHOOK g_hHook = NULL;
DWORD g_dwThreadIdDIPS = 0;
#pragma data_seg()

// Instruct the linker to make the Shared section
// readable, writable, and shared.
#pragma comment(linker, "/section:Shared,rws")


///////////////////////////////////////////////////////////////////////////////


// Nonshared variables
HINSTANCE g_hInstDll = NULL;


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, PVOID fImpLoad) {

   switch (fdwReason) {

      case DLL_PROCESS_ATTACH:
         // DLL is attaching to the address space of the current process.
         g_hInstDll = hInstDll;
         break;

      case DLL_THREAD_ATTACH:
         // A new thread is being created in the current process.
         break;

      case DLL_THREAD_DETACH:
         // A thread is exiting cleanly.
         break;

      case DLL_PROCESS_DETACH:
         // The calling process is detaching the DLL from its address space.
         break;
   }
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI SetDIPSHook(DWORD dwThreadId) {

   BOOL bOk = FALSE;
   
   if (dwThreadId != 0) {
      // Make sure that the hook is not already installed.
      chASSERT(g_hHook == NULL);

      // Save our thread ID in a shared variable so that our GetMsgProc 
      // function can post a message back to the thread when the server 
      // window has been created.
      g_dwThreadIdDIPS = GetCurrentThreadId();

      // Install the hook on the specified thread
      g_hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hInstDll, 
         dwThreadId);

      bOk = (g_hHook != NULL);
      if (bOk) {
         // The hook was installed successfully; force a benign message to 
         // the thread's queue so that the hook function gets called.
         bOk = PostThreadMessage(dwThreadId, WM_NULL, 0, 0);
      }
   } else {

      // Make sure that a hook has been installed.
      chASSERT(g_hHook != NULL);
      bOk = UnhookWindowsHookEx(g_hHook);
      g_hHook = NULL;
   }

   return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


LRESULT WINAPI GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {

   static BOOL bFirstTime = TRUE;

   if (bFirstTime) {
      // The DLL just got injected.
      bFirstTime = FALSE;

      // Uncomment the line below to invoke the debugger 
      // on the process that just got the injected DLL.
      // ForceDebugBreak();

      // Create the DIPS Server window to handle the client request.
      CreateDialog(g_hInstDll, MAKEINTRESOURCE(IDD_DIPS), NULL, Dlg_Proc);

      // Tell the DIPS application that the server is up 
      // and ready to handle requests.
      PostThreadMessage(g_dwThreadIdDIPS, WM_NULL, 0, 0);
   }

   return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnClose(HWND hWnd) {

   DestroyWindow(hWnd);
}


///////////////////////////////////////////////////////////////////////////////


static const TCHAR g_szRegSubKey[] = 
   TEXT("Software\\Wintellect\\Desktop Item Position Saver");


///////////////////////////////////////////////////////////////////////////////


void SaveListViewItemPositions(HWND hWndLV) {

   int nMaxItems = ListView_GetItemCount(hWndLV);

   // When saving new positions, delete the old position 
   // information that is currently in the registry.
   LONG l = RegDeleteKey(HKEY_CURRENT_USER, g_szRegSubKey);

   // Create the registry key to hold the info
   HKEY hkey;
   l = RegCreateKeyEx(HKEY_CURRENT_USER, g_szRegSubKey, 0, NULL, 
      REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hkey, NULL);
   chASSERT(l == ERROR_SUCCESS);

   for (int nItem = 0; nItem < nMaxItems; nItem++) {

      // Get the name and position of a ListView item.
      TCHAR szName[MAX_PATH];
      ListView_GetItemText(hWndLV, nItem, 0, szName, _countof(szName));

      POINT pt;
      ListView_GetItemPosition(hWndLV, nItem, &pt);

      // Save the name and position in the registry.
      l = RegSetValueEx(hkey, szName, 0, REG_BINARY, (PBYTE) &pt, sizeof(pt));
      chASSERT(l == ERROR_SUCCESS);
   }
   RegCloseKey(hkey);
}


///////////////////////////////////////////////////////////////////////////////


void RestoreListViewItemPositions(HWND hWndLV) {

   HKEY hkey;
   LONG l = RegOpenKeyEx(HKEY_CURRENT_USER, g_szRegSubKey,
      0, KEY_QUERY_VALUE, &hkey);
   if (l == ERROR_SUCCESS) {

      // If the ListView has AutoArrange on, temporarily turn it off.
      DWORD dwStyle = GetWindowStyle(hWndLV);
      if (dwStyle & LVS_AUTOARRANGE) 
         SetWindowLong(hWndLV, GWL_STYLE, dwStyle & ~LVS_AUTOARRANGE);

      l = NO_ERROR;
      for (int nIndex = 0; l != ERROR_NO_MORE_ITEMS; nIndex++) {
         TCHAR szName[MAX_PATH];
         DWORD cbValueName = _countof(szName);

         POINT pt;
         DWORD cbData = sizeof(pt), nItem;

         // Read a value name and position from the registry.
         DWORD dwType;
         l = RegEnumValue(hkey, nIndex, szName, &cbValueName, 
            NULL, &dwType, (PBYTE) &pt, &cbData);

         if (l == ERROR_NO_MORE_ITEMS) 
            continue;

         if ((dwType == REG_BINARY) && (cbData == sizeof(pt))) {
            // The value is something that we recognize; try to find
            // an item in the ListView control that matches the name.
            LV_FINDINFO lvfi;
            lvfi.flags = LVFI_STRING; 
            lvfi.psz = szName; 
            nItem = ListView_FindItem(hWndLV, -1, &lvfi);
            if (nItem != -1) {
               // We found a match; change the item's position.
               ListView_SetItemPosition(hWndLV, nItem, pt.x, pt.y);	
            }
         }
      }
      // Turn AutoArrange back on if it was originally on.
      SetWindowLong(hWndLV, GWL_STYLE, dwStyle);
      RegCloseKey(hkey);
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_CLOSE, Dlg_OnClose);

      case WM_APP:
         // Uncomment the line below to invoke the debugger 
         // on the process that just got the injected DLL.
         // ForceDebugBreak();

         if (lParam) 
            SaveListViewItemPositions((HWND) wParam);
         else 
            RestoreListViewItemPositions((HWND) wParam);
         break;
   }

   return(FALSE);
}



//////////////////////////////// End of File //////////////////////////////////

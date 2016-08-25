/******************************************************************************
Module:  LISWatch.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <windowsx.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


#define TIMER_DELAY (500)        // Half a second

UINT_PTR g_uTimerId = 1;
DWORD g_dwThreadIdAttachTo = 0;  // 0=System-wide; Non-zero=specifc thread


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_LISWATCH);

   // Update our contents periodically
   g_uTimerId = SetTimer(hwnd, g_uTimerId, TIMER_DELAY, NULL);

   // Make our window on top of all others
   SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, 
   UINT keyFlags) {

   chMB("To monitor a specific thread, click the left mouse button in "
        "the main window and release it in the desired window.\n"
        "To monitor all threads, double-click the left mouse button in "
        "the main window.");
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, 
   UINT keyFlags) {

   // If we're attached to a thread, detach from it
   if (g_dwThreadIdAttachTo != 0)
      AttachThreadInput(GetCurrentThreadId(), g_dwThreadIdAttachTo, FALSE);

   // Set capture to ourself and change the mouse cursor
   SetCapture(hwnd);
   SetCursor(LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_EYES)));
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {

   if (GetCapture() == hwnd) {

      // If we had mouse capture set, get the ID of the thread that
      // created the window that is under the mouse cursor.
      POINT pt;
      pt.x = LOWORD(GetMessagePos());
      pt.y = HIWORD(GetMessagePos());
      ReleaseCapture();
      g_dwThreadIdAttachTo = GetWindowThreadProcessId(
         ChildWindowFromPointEx(GetDesktopWindow(), pt, CWP_SKIPINVISIBLE), 
         NULL);

      if (g_dwThreadIdAttachTo == GetCurrentThreadId()) {

         // The mouse button is released on one of our windows; 
         // monitor local-input state on a system-wide basis
         g_dwThreadIdAttachTo = 0;

      } else {

         // The mouse button is released on a window that our thread didn't
         // create; monitor local input state for that thread only.
         AttachThreadInput(GetCurrentThreadId(), g_dwThreadIdAttachTo, TRUE);	
      }
   }
}


///////////////////////////////////////////////////////////////////////////////


static void CalcWndText(HWND hwnd, PTSTR szBuf, int nLen) {

   if (hwnd == (HWND) NULL) {
      lstrcpy(szBuf, TEXT("(no window)")); 
      return;
   }

   if (!IsWindow(hwnd)) {
      lstrcpy(szBuf, TEXT("(invalid window)")); 
      return;
   }

   TCHAR szClass[50], szCaption[50], szBufT[150];
   GetClassName(hwnd, szClass, chDIMOF(szClass));
   GetWindowText(hwnd, szCaption, chDIMOF(szCaption));
   wsprintf(szBufT, TEXT("[%s] %s"), (PTSTR) szClass,
      (szCaption[0] == 0) ? (PTSTR) TEXT("(no caption)") : (PTSTR) szCaption);
   _tcsncpy(szBuf, szBufT, nLen - 1);
   szBuf[nLen - 1] = 0; // Force zero-terminated string
}


//////////////////////////////////////////////////////////////


void Dlg_OnTimer(HWND hwnd, UINT id) {

   TCHAR szBuf[100] = TEXT("System-wide");
   HWND hwndForeground = GetForegroundWindow();
   DWORD dwThreadIdAttachTo = g_dwThreadIdAttachTo;

   if (dwThreadIdAttachTo == 0) {

      // If monitoring local input state system-wide, attach our input
      // state to the thread that created the current foreground window.
      dwThreadIdAttachTo = 
         GetWindowThreadProcessId(hwndForeground, NULL);
      AttachThreadInput(GetCurrentThreadId(), dwThreadIdAttachTo, TRUE);	

   } else {
   
      wsprintf(szBuf, TEXT("0x%08x"), dwThreadIdAttachTo);
   }

   SetWindowText(GetDlgItem(hwnd, IDC_THREADID), szBuf);

   CalcWndText(GetFocus(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDFOCUS), szBuf);

   CalcWndText(GetActiveWindow(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDACTIVE), szBuf);

   CalcWndText(GetCapture(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDCAPTURE), szBuf);

   CalcWndText(hwndForeground, szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDFOREGRND), szBuf);

   if (g_dwThreadIdAttachTo == 0) {
      // If monitoring local input state system-wide, detach our input
      // state from the thread that created the current foreground window.
      AttachThreadInput(GetCurrentThreadId(), dwThreadIdAttachTo, FALSE);	
   }
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////

        
INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG,  Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,     Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_TIMER,       Dlg_OnTimer);
      chHANDLE_DLGMSG(hwnd, WM_RBUTTONDOWN, Dlg_OnRButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_LBUTTONDOWN, Dlg_OnLButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_LBUTTONUP,   Dlg_OnLButtonUp);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_LISWATCH), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

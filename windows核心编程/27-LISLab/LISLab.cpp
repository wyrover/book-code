/******************************************************************************
Module:  LISLab.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <string.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


#define TIMER_DELAY (500)  // Half a second

UINT_PTR g_uTimerId = 1;
int      g_nEventId = 0;
DWORD    g_dwEventTime = 0;
HWND     g_hwndSubject = NULL;
HWND     g_hwndNotepad = NULL;


///////////////////////////////////////////////////////////////////////////////


void CalcWndText(HWND hwnd, PTSTR szBuf, int nLen) {

   TCHAR szClass[50], szCaption[50], szBufT[150];

   if (hwnd == (HWND) NULL) {
      _tcscpy(szBuf, TEXT("(no window)"));
      return;
   }


   if (!IsWindow(hwnd)) {
      _tcscpy(szBuf, TEXT("(invalid window)"));
      return;
   }

   GetClassName(hwnd, szClass, chDIMOF(szClass));
   GetWindowText(hwnd, szCaption, chDIMOF(szCaption));

   wsprintf(szBufT, TEXT("[%s] %s"), (PTSTR) szClass,
      (*szCaption == 0) ? (PTSTR) TEXT("(no caption)") : (PTSTR) szCaption);
   _tcsncpy(szBuf, szBufT, nLen - 1);
   szBuf[nLen - 1] = 0; // Force zero-terminated string
}


///////////////////////////////////////////////////////////////////////////////


// To minimize stack use, one instance of WALKWINDOWTREEDATA
// is created as a local variable in WalkWindowTree() and a
// pointer to it is passed to WalkWindowTreeRecurse.

// Data used by WalkWindowTreeRecurse
typedef struct {
   HWND  hwndLB;        // Handle to the output list box
   HWND  hwndParent;    // Handle to the parent
   int   nLevel;        // Nesting depth
   int   nIndex;        // List box item index
   TCHAR szBuf[100];    // Output buffer
   int   iBuf;          // Index into szBuf
} WALKWINDOWTREEDATA, *PWALKWINDOWTREEDATA;


void WalkWindowTreeRecurse(PWALKWINDOWTREEDATA pWWT) {

   if (!IsWindow(pWWT->hwndParent))
      return;

   pWWT->nLevel++;
   const int nIndexAmount = 2;

   for (pWWT->iBuf = 0; pWWT->iBuf < pWWT->nLevel * nIndexAmount; pWWT->iBuf++)
      pWWT->szBuf[pWWT->iBuf] = TEXT(' ');

   CalcWndText(pWWT->hwndParent, &pWWT->szBuf[pWWT->iBuf], 
      chDIMOF(pWWT->szBuf) - pWWT->iBuf);
   pWWT->nIndex = ListBox_AddString(pWWT->hwndLB, pWWT->szBuf);
   ListBox_SetItemData(pWWT->hwndLB, pWWT->nIndex, pWWT->hwndParent);

   HWND hwndChild = GetFirstChild(pWWT->hwndParent);
   while (hwndChild != NULL) {
      pWWT->hwndParent = hwndChild;
      WalkWindowTreeRecurse(pWWT);
      hwndChild = GetNextSibling(hwndChild);
   }

   pWWT->nLevel--;
}


///////////////////////////////////////////////////////////////////////////////


void WalkWindowTree(HWND hwndLB, HWND hwndParent) {

   WALKWINDOWTREEDATA WWT;

   WWT.hwndLB = hwndLB;
   WWT.hwndParent = hwndParent;
   WWT.nLevel = -1;

   WalkWindowTreeRecurse(&WWT);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_LISLAB);

   // Associate the Up arrow cursor with the dialog box's client area
   SetClassLongPtr(hwnd, GCLP_HCURSOR, 
      (LONG_PTR) LoadCursor(NULL, IDC_UPARROW));

   g_uTimerId = SetTimer(hwnd, g_uTimerId, TIMER_DELAY, NULL);

   HWND hwndT = GetDlgItem(hwnd, IDC_WNDFUNC);
   ComboBox_AddString(hwndT, TEXT("SetFocus"));
   ComboBox_AddString(hwndT, TEXT("SetActiveWindow"));
   ComboBox_AddString(hwndT, TEXT("SetForegroundWnd"));
   ComboBox_AddString(hwndT, TEXT("BringWindowToTop"));
   ComboBox_AddString(hwndT, TEXT("SetWindowPos-TOP"));
   ComboBox_AddString(hwndT, TEXT("SetWindowPos-BTM"));
   ComboBox_SetCurSel(hwndT, 0);

   // Fill the windows list box with our window
   hwndT = GetDlgItem(hwnd, IDC_WNDS);
   ListBox_AddString(hwndT, TEXT("---> This dialog box <---"));

   ListBox_SetItemData(hwndT, 0, hwnd);
   ListBox_SetCurSel(hwndT, 0);

   // Now add Notepad's windows
   g_hwndNotepad = FindWindow(TEXT("Notepad"), NULL);
   if (g_hwndNotepad == NULL) {

      // Notepad isn't running; run it.
      STARTUPINFO si = { sizeof(si) };
      PROCESS_INFORMATION pi;
      TCHAR szCommandLine[] = TEXT("Notepad");
      if (CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, 
         NULL, NULL, &si, &pi)) {

         // Wait for Notepad to create all its windows.
         WaitForInputIdle(pi.hProcess, INFINITE);
         CloseHandle(pi.hProcess);
         CloseHandle(pi.hThread);
         g_hwndNotepad = FindWindow(TEXT("Notepad"), NULL);
      }
   }
   WalkWindowTree(hwndT, g_hwndNotepad);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   HWND hwndT;

   switch (id) {

      case IDCANCEL:
         if (g_uTimerId != 0)
            KillTimer(hwnd, g_uTimerId);
         EndDialog(hwnd, 0);
         break;

      case IDC_FUNCSTART:
         g_dwEventTime = GetTickCount() + 1000 *
            GetDlgItemInt(hwnd, IDC_DELAY, NULL, FALSE);
         hwndT = GetDlgItem(hwnd, IDC_WNDS);
         g_hwndSubject = (HWND) 
            ListBox_GetItemData(hwndT, ListBox_GetCurSel(hwndT));
         g_nEventId = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_WNDFUNC));
         SetWindowText(GetDlgItem(hwnd, IDC_EVENTPENDING), TEXT("Pending"));
         break;

      case IDC_THREADATTACH:
         AttachThreadInput(GetWindowThreadProcessId(g_hwndNotepad, NULL),
            GetCurrentThreadId(), TRUE);
         break;

      case IDC_THREADDETACH:
         AttachThreadInput(GetWindowThreadProcessId(g_hwndNotepad, NULL),
            GetCurrentThreadId(), FALSE);
         break;

      case IDC_REMOVECLIPRECT:
         ClipCursor(NULL);
         break;

      case IDC_HIDECURSOR:
         ShowCursor(FALSE);
         break;

      case IDC_SHOWCURSOR:
         ShowCursor(TRUE);
         break;

      case IDC_INFINITELOOP:
         SetCursor(LoadCursor(NULL, IDC_NO));
         for (;;)
             ;
         break;

      case IDC_SETCLIPRECT:
         RECT rc;
         SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN) / 2, 
            GetSystemMetrics(SM_CYSCREEN) / 2);
         ClipCursor(&rc);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


void AddStr(HWND hwndLB, PCTSTR szBuf) {

   int nIndex;

   do {
      nIndex = ListBox_AddString(hwndLB, szBuf);
      if (nIndex == LB_ERR)
         ListBox_DeleteString(hwndLB, 0);
   } while (nIndex == LB_ERR);

   ListBox_SetCurSel(hwndLB, nIndex);
}


///////////////////////////////////////////////////////////////////////////////


int Dlg_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, 
   int x, int y, UINT keyFlags) {

   TCHAR szBuf[100];
   wsprintf(szBuf, 
      TEXT("Capture=%-3s, Msg=RButtonDown, DblClk=%-3s, x=%5d, y=%5d"),
      (GetCapture() == NULL) ? TEXT("No") : TEXT("Yes"),
      fDoubleClick ? TEXT("Yes") : TEXT("No"), x, y);

   AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
   if (!fDoubleClick) SetCapture(hwnd);
   else ReleaseCapture();
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


int Dlg_OnRButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {

   TCHAR szBuf[100];
   wsprintf(szBuf, TEXT("Capture=%-3s, Msg=RButtonUp,   x=%5d, y=%5d"),
      (GetCapture() == NULL) ? TEXT("No") : TEXT("Yes"), x, y);

   AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


int Dlg_OnLButtonDown(HWND hwnd, BOOL fDoubleClick,
   int x, int y, UINT keyFlags) {

   TCHAR szBuf[100];
   wsprintf(szBuf,
      TEXT("Capture=%-3s, Msg=LButtonDown, DblClk=%-3s, x=%5d, y=%5d"),
      (GetCapture() == NULL) ? TEXT("No") : TEXT("Yes"),
      fDoubleClick ? TEXT("Yes") : TEXT("No"), x, y);

   AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {

   TCHAR szBuf[100];
   wsprintf(szBuf,
      TEXT("Capture=%-3s, Msg=LButtonUp,   x=%5d, y=%5d"),
      (GetCapture() == NULL) ? TEXT("No") : TEXT("Yes"), x, y);

   AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
   
   TCHAR szBuf[100];
   wsprintf(szBuf, TEXT("Capture=%-3s, Msg=MouseMove,  x=%5d, y=%5d"),
      (GetCapture() == NULL) ? TEXT("No") : TEXT("Yes"), x, y);

   AddStr(GetDlgItem(hwnd, IDC_MOUSEMSGS), szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnTimer(HWND hwnd, UINT id) {

   TCHAR szBuf[100];

   CalcWndText(GetFocus(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDFOCUS), szBuf);

   CalcWndText(GetCapture(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDCAPTURE), szBuf);

   CalcWndText(GetActiveWindow(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDACTIVE), szBuf);

   CalcWndText(GetForegroundWindow(), szBuf, chDIMOF(szBuf));
   SetWindowText(GetDlgItem(hwnd, IDC_WNDFOREGROUND), szBuf);

   RECT rc;
   GetClipCursor(&rc);
   wsprintf(szBuf, TEXT("left=%d, top=%d, right=%d, bottom=%d"),
      rc.left, rc.top, rc.right, rc.bottom);
   SetWindowText(GetDlgItem(hwnd, IDC_CLIPCURSOR), szBuf);


   if ((g_dwEventTime == 0) || (GetTickCount() < g_dwEventTime))
      return;

   HWND hwndT;
   switch (g_nEventId) {
      case 0:  // SetFocus
         g_hwndSubject = SetFocus(g_hwndSubject);
         break;

      case 1:  // SetActiveWindow
         g_hwndSubject = SetActiveWindow(g_hwndSubject);
         break;

      case 2:  // SetForegroundWindow
         hwndT = GetForegroundWindow();
         SetForegroundWindow(g_hwndSubject);
         g_hwndSubject = hwndT;
         break;

      case 3:  // BringWindowToTop
         BringWindowToTop(g_hwndSubject);
         break;

      case 4:  // SetWindowPos w/HWND_TOP
         SetWindowPos(g_hwndSubject, HWND_TOP, 0, 0, 0, 0, 
            SWP_NOMOVE | SWP_NOSIZE);
         g_hwndSubject = (HWND) 1;
         break;

      case 5:  // SetWindowPos w/HWND_BOTTOM
         SetWindowPos(g_hwndSubject, HWND_BOTTOM, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE);
         g_hwndSubject = (HWND) 1;
         break;
   }

   if (g_hwndSubject == (HWND) 1) {
      SetWindowText(GetDlgItem(hwnd, IDC_PREVWND), TEXT("Can't tell."));
   } else {
      CalcWndText(g_hwndSubject, szBuf, chDIMOF(szBuf));
      SetWindowText(GetDlgItem(hwnd, IDC_PREVWND), szBuf);
   }

   g_hwndSubject = NULL; g_nEventId = 0; g_dwEventTime = 0;
   SetWindowText(GetDlgItem(hwnd, IDC_EVENTPENDING), TEXT("Executed"));
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG,    Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,       Dlg_OnCommand);

      chHANDLE_DLGMSG(hwnd, WM_MOUSEMOVE,     Dlg_OnMouseMove);

      chHANDLE_DLGMSG(hwnd, WM_LBUTTONDOWN,   Dlg_OnLButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_LBUTTONDBLCLK, Dlg_OnLButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_LBUTTONUP,     Dlg_OnLButtonUp);

      chHANDLE_DLGMSG(hwnd, WM_RBUTTONDOWN,   Dlg_OnRButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_RBUTTONDBLCLK, Dlg_OnRButtonDown);
      chHANDLE_DLGMSG(hwnd, WM_RBUTTONUP,     Dlg_OnRButtonUp);

      chHANDLE_DLGMSG(hwnd, WM_TIMER,         Dlg_OnTimer);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_LISLAB), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

/******************************************************************************
Module:  SchedLab.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <process.h>       // For _beginthreadex
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI ThreadFunc(PVOID pvParam) {

   HANDLE hThreadPrimary = (HANDLE) pvParam;
   SuspendThread(hThreadPrimary);
   chMB(
      "The Primary thread is suspended.\n"
      "It no longer responds to input and produces no output.\n"
      "Press OK to resume the primary thread & exit this secondary thread.\n");
   ResumeThread(hThreadPrimary);
   CloseHandle(hThreadPrimary);

   // To avoid deadlock, call EnableWindow after ResumeThread.
   EnableWindow(
      GetDlgItem(FindWindow(NULL, TEXT("Scheduling Lab")), IDC_SUSPEND), 
      TRUE);
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog (HWND hwnd, HWND hwndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hwnd, IDI_SCHEDLAB);

   // Initialize process priority classes
   HWND hwndCtl = GetDlgItem(hwnd, IDC_PROCESSPRIORITYCLASS);

   int n = ComboBox_AddString(hwndCtl, TEXT("High"));
   ComboBox_SetItemData(hwndCtl, n, HIGH_PRIORITY_CLASS);

   // Save our current priority class
   DWORD dwpc = GetPriorityClass(GetCurrentProcess());

   if (SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS)) {

      // This system supports the BELOW_NORMAL_PRIORITY_CLASS class

      // Restore our original priority class
      SetPriorityClass(GetCurrentProcess(), dwpc);

      // Add the Above Normal priority class
      n = ComboBox_AddString(hwndCtl, TEXT("Above normal"));
      ComboBox_SetItemData(hwndCtl, n, ABOVE_NORMAL_PRIORITY_CLASS);

      dwpc = 0;  // Remember that this system supports below normal
   }

   int nNormal = n = ComboBox_AddString(hwndCtl, TEXT("Normal"));
   ComboBox_SetItemData(hwndCtl, n, NORMAL_PRIORITY_CLASS);

   if (dwpc == 0) {

      // This system supports the BELOW_NORMAL_PRIORITY_CLASS class

      // Add the Below Normal priority class
      n = ComboBox_AddString(hwndCtl, TEXT("Below normal"));
      ComboBox_SetItemData(hwndCtl, n, BELOW_NORMAL_PRIORITY_CLASS);
   }

   n = ComboBox_AddString(hwndCtl, TEXT("Idle"));
   ComboBox_SetItemData(hwndCtl, n, IDLE_PRIORITY_CLASS);

   ComboBox_SetCurSel(hwndCtl, nNormal);

   // Initialize thread relative priorities
   hwndCtl = GetDlgItem(hwnd, IDC_THREADRELATIVEPRIORITY);
   
   n = ComboBox_AddString(hwndCtl, TEXT("Time critical"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_TIME_CRITICAL);

   n = ComboBox_AddString(hwndCtl, TEXT("Highest"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_HIGHEST);

   n = ComboBox_AddString(hwndCtl, TEXT("Above normal"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_ABOVE_NORMAL);

   nNormal = n = ComboBox_AddString(hwndCtl, TEXT("Normal"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_NORMAL);

   n = ComboBox_AddString(hwndCtl, TEXT("Below normal"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_BELOW_NORMAL);

   n = ComboBox_AddString(hwndCtl, TEXT("Lowest"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_LOWEST);

   n = ComboBox_AddString(hwndCtl, TEXT("Idle"));
   ComboBox_SetItemData(hwndCtl, n, THREAD_PRIORITY_IDLE);

   ComboBox_SetCurSel(hwndCtl, nNormal);

   Edit_LimitText(GetDlgItem(hwnd, IDC_SLEEPTIME), 4);   // Maximum of 9999
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         PostQuitMessage(0);
         break;

      case IDC_PROCESSPRIORITYCLASS:
         if (codeNotify == CBN_SELCHANGE) {
            SetPriorityClass(GetCurrentProcess(), (DWORD)
               ComboBox_GetItemData(hwndCtl, ComboBox_GetCurSel(hwndCtl)));
         }
         break;

      case IDC_THREADRELATIVEPRIORITY:
         if (codeNotify == CBN_SELCHANGE) {
            SetThreadPriority(GetCurrentThread(), (DWORD)
               ComboBox_GetItemData(hwndCtl, ComboBox_GetCurSel(hwndCtl)));
         }
         break;

      case IDC_SUSPEND:
         // To avoid deadlock, call EnableWindow before creating
         // the thread which calls SuspendThread.
         EnableWindow(hwndCtl, FALSE);

         HANDLE hThreadPrimary;
         DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), 
            GetCurrentProcess(), &hThreadPrimary, 
            THREAD_SUSPEND_RESUME, FALSE, DUPLICATE_SAME_ACCESS);
         DWORD dwThreadID;
         CloseHandle(chBEGINTHREADEX(NULL, 0, ThreadFunc, 
            hThreadPrimary, 0, &dwThreadID));
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   HWND hwnd = 
      CreateDialog(hinstExe, MAKEINTRESOURCE(IDD_SCHEDLAB), NULL, Dlg_Proc);
   BOOL fQuit = FALSE;

   while (!fQuit) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

         // IsDialogMessage allows keyboard navigation to work properly.
         if (!IsDialogMessage(hwnd, &msg)) {

            if (msg.message == WM_QUIT) {
               fQuit = TRUE;  // For WM_QUIT, terminate the loop.
            } else {
               // Not a WM_QUIT message. Translate it and dispatch it.
               TranslateMessage(&msg);
               DispatchMessage(&msg);
            }
         }  // if (!IsDialogMessage())
      } else {

         // Add a number to the listbox
         static int s_n = -1;
         TCHAR sz[20];
         wsprintf(sz, TEXT("%u"), ++s_n);
         HWND hwndWork = GetDlgItem(hwnd, IDC_WORK);
         ListBox_SetCurSel(hwndWork, ListBox_AddString(hwndWork, sz));

         // Remove some strings if there are too many entries
         while (ListBox_GetCount(hwndWork) > 100)
            ListBox_DeleteString(hwndWork, 0);

         // How long should the thread sleep
         int nSleep = GetDlgItemInt(hwnd, IDC_SLEEPTIME, NULL, FALSE);
         if (chINRANGE(1, nSleep, 9999))
            Sleep(nSleep);
      }
   }
   DestroyWindow(hwnd);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////


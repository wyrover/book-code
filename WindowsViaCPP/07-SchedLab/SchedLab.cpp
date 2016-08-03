/******************************************************************************
Module:  SchedLab.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"
#include <StrSafe.h>


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


BOOL Dlg_OnInitDialog (HWND hWnd, HWND hWndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hWnd, IDI_SCHEDLAB);

   // Initialize process priority classes
   HWND hWndCtl = GetDlgItem(hWnd, IDC_PROCESSPRIORITYCLASS);

   int n = ComboBox_AddString(hWndCtl, TEXT("High"));
   ComboBox_SetItemData(hWndCtl, n, HIGH_PRIORITY_CLASS);

   // Save our current priority class
   DWORD dwpc = GetPriorityClass(GetCurrentProcess());

   if (SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS)) {

      // This system supports the BELOW_NORMAL_PRIORITY_CLASS class

      // Restore our original priority class
      SetPriorityClass(GetCurrentProcess(), dwpc);

      // Add the Above Normal priority class
      n = ComboBox_AddString(hWndCtl, TEXT("Above normal"));
      ComboBox_SetItemData(hWndCtl, n, ABOVE_NORMAL_PRIORITY_CLASS);

      dwpc = 0;  // Remember that this system supports below normal
   }

   int nNormal = n = ComboBox_AddString(hWndCtl, TEXT("Normal"));
   ComboBox_SetItemData(hWndCtl, n, NORMAL_PRIORITY_CLASS);

   if (dwpc == 0) {

      // This system supports the BELOW_NORMAL_PRIORITY_CLASS class

      // Add the Below Normal priority class
      n = ComboBox_AddString(hWndCtl, TEXT("Below normal"));
      ComboBox_SetItemData(hWndCtl, n, BELOW_NORMAL_PRIORITY_CLASS);
   }

   n = ComboBox_AddString(hWndCtl, TEXT("Idle"));
   ComboBox_SetItemData(hWndCtl, n, IDLE_PRIORITY_CLASS);

   ComboBox_SetCurSel(hWndCtl, nNormal);

   // Initialize thread relative priorities
   hWndCtl = GetDlgItem(hWnd, IDC_THREADRELATIVEPRIORITY);
   
   n = ComboBox_AddString(hWndCtl, TEXT("Time critical"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_TIME_CRITICAL);

   n = ComboBox_AddString(hWndCtl, TEXT("Highest"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_HIGHEST);

   n = ComboBox_AddString(hWndCtl, TEXT("Above normal"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_ABOVE_NORMAL);

   nNormal = n = ComboBox_AddString(hWndCtl, TEXT("Normal"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_NORMAL);

   n = ComboBox_AddString(hWndCtl, TEXT("Below normal"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_BELOW_NORMAL);

   n = ComboBox_AddString(hWndCtl, TEXT("Lowest"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_LOWEST);

   n = ComboBox_AddString(hWndCtl, TEXT("Idle"));
   ComboBox_SetItemData(hWndCtl, n, THREAD_PRIORITY_IDLE);

   ComboBox_SetCurSel(hWndCtl, nNormal);

   Edit_LimitText(GetDlgItem(hWnd, IDC_SLEEPTIME), 4);   // Maximum of 9999
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand (HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         PostQuitMessage(0);
         break;

      case IDC_PROCESSPRIORITYCLASS:
         if (codeNotify == CBN_SELCHANGE) {
            SetPriorityClass(GetCurrentProcess(), (DWORD)
               ComboBox_GetItemData(hWndCtl, ComboBox_GetCurSel(hWndCtl)));
         }
         break;

      case IDC_THREADRELATIVEPRIORITY:
         if (codeNotify == CBN_SELCHANGE) {
            SetThreadPriority(GetCurrentThread(), (DWORD)
               ComboBox_GetItemData(hWndCtl, ComboBox_GetCurSel(hWndCtl)));
         }
         break;

      case IDC_SUSPEND:
         // To avoid deadlock, call EnableWindow before creating
         // the thread that calls SuspendThread.
         EnableWindow(hWndCtl, FALSE);

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


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   HWND hWnd = 
      CreateDialog(hInstExe, MAKEINTRESOURCE(IDD_SCHEDLAB), NULL, Dlg_Proc);
   BOOL fQuit = FALSE;

   while (!fQuit) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

         // IsDialogMessage allows keyboard navigation to work properly.
         if (!IsDialogMessage(hWnd, &msg)) {

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
         StringCchPrintf(sz, _countof(sz), TEXT("%u"), ++s_n);
         HWND hWndWork = GetDlgItem(hWnd, IDC_WORK);
         ListBox_SetCurSel(hWndWork, ListBox_AddString(hWndWork, sz));

         // Remove some strings if there are too many entries
         while (ListBox_GetCount(hWndWork) > 100)
            ListBox_DeleteString(hWndWork, 0);

         // How long should the thread sleep
         int nSleep = GetDlgItemInt(hWnd, IDC_SLEEPTIME, NULL, FALSE);
         if (chINRANGE(1, nSleep, 9999))
            Sleep(nSleep);
      }
   }

   DestroyWindow(hWnd);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
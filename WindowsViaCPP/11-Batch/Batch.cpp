/******************************************************************************
Module:  Batch.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <Windowsx.h>
#include <WinBase.h>
#include <WinNT.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <strsafe.h>

#include "Batch.h"


//////////////////////////////////////////////////////////////////////////////


// Global variables
HWND     g_hDlg = NULL;
PTP_WORK g_pWorkItem = NULL;
volatile LONG g_nCurrentTask = 0;


// Global definitions
#define WM_APP_COMPLETED (WM_APP+123)


//////////////////////////////////////////////////////////////////////////////


void AddMessage(LPCTSTR szMsg) {

   HWND hListBox = GetDlgItem(g_hDlg, IDC_LB_STATUS);
   ListBox_SetCurSel(hListBox, ListBox_AddString(hListBox, szMsg));
}


//////////////////////////////////////////////////////////////////////////////


void NTAPI TaskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work) {

   LONG currentTask = InterlockedIncrement(&g_nCurrentTask);

   TCHAR szMsg[MAX_PATH];
   StringCchPrintf(
      szMsg, _countof(szMsg), 
      TEXT("[%u] Task #%u is starting."), GetCurrentThreadId(), currentTask);
   AddMessage(szMsg);

   // Simulate a lot of work
   Sleep(currentTask * 1000);

   StringCchPrintf(
      szMsg, _countof(szMsg), 
      TEXT("[%u] Task #%u is done."), GetCurrentThreadId(), currentTask);
   AddMessage(szMsg);

   if (InterlockedDecrement(&g_nCurrentTask) == 0)
   {
      // Notify the UI thread for completion.
      PostMessage(g_hDlg, WM_APP_COMPLETED, 0, (LPARAM)currentTask);
   }
}


//////////////////////////////////////////////////////////////////////////////


void OnStartBatch() {

   // Disable Start button
   Button_Enable(GetDlgItem(g_hDlg, IDC_BTN_START_BATCH), FALSE);

   AddMessage(TEXT("----Start a new batch----"));
   
   // Submit 4 tasks by using the same work item
   SubmitThreadpoolWork(g_pWorkItem);
   SubmitThreadpoolWork(g_pWorkItem);
   SubmitThreadpoolWork(g_pWorkItem);
   SubmitThreadpoolWork(g_pWorkItem);

   AddMessage(TEXT("4 tasks are submitted."));
}


//////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDOK:
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;

      case IDC_BTN_START_BATCH:
         OnStartBatch();
         break;
   }
}


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   // Keep track of main dialog window for error messages
   g_hDlg = hWnd;

   return(TRUE);
}


//////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
      case WM_APP_COMPLETED: {
         TCHAR szMsg[MAX_PATH+1];
         StringCchPrintf(
            szMsg, _countof(szMsg), 
            TEXT("____Task #%u was the last task of the batch____"), lParam);
         AddMessage(szMsg);
         
         // Don't forget to enable the button
         Button_Enable(GetDlgItem(hWnd, IDC_BTN_START_BATCH), TRUE);
      }
      break;
   }

   return(FALSE);
}



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR pCmdLine, int) {

   // Create the work item that will be used by all tasks
   g_pWorkItem = CreateThreadpoolWork(TaskHandler, NULL, NULL);
   if (g_pWorkItem == NULL) {
      MessageBox(NULL, TEXT("Impossible to create the work item for tasks."), 
         TEXT(""), MB_ICONSTOP);
      return(-1);
   }
   
   DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Dlg_Proc, 
      _ttoi(pCmdLine));

   // Don't forget to delete the work item
   CloseThreadpoolWork(g_pWorkItem);

   return(0);
}


//////////////////////////////// End of File /////////////////////////////////
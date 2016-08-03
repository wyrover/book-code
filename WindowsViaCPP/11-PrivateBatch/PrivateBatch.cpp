/******************************************************************************
Module:  PrivateBatch.cpp
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

#include "PrivateBatch.h"


//////////////////////////////////////////////////////////////////////////////


// Global variables
HWND                 g_hDlg = NULL;
PTP_POOL             g_pThreadPool = NULL;
TP_CALLBACK_ENVIRON  g_callbackEnvironment;
PTP_CLEANUP_GROUP    g_pCleanupGroup = NULL;
HANDLE               g_hPrintEvent = NULL;
HANDLE               g_hEmailEvent = NULL;


// Global definitions
#define WM_APP_COMPLETED (WM_APP+123)


//////////////////////////////////////////////////////////////////////////////


void AddMessage(PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR szMsg[20 * 1024];
   _vstprintf_s(szMsg, _countof(szMsg), pszFormat, argList);
   HWND hListBox = GetDlgItem(g_hDlg, IDC_LB_STATUS);
   ListBox_SetCurSel(hListBox, ListBox_AddString(hListBox, szMsg));

   va_end(argList);
}


//////////////////////////////////////////////////////////////////////////////


typedef struct _CALLBACK_DATA
{
   HANDLE finishEvent;
   TCHAR szAction[MAX_PATH];
} CALLBACK_DATA, *PCALLBACK_DATA;


VOID ActionCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {

   PCALLBACK_DATA pData = (PCALLBACK_DATA)Context; 

   // Compute a pseudo duration for the action based on its name
   DWORD dwDuration = (DWORD)_tcslen(pData->szAction);

   AddMessage(
      TEXT("[%u] '%s' starting for %u seconds."), 
      GetCurrentThreadId(), pData->szAction, dwDuration);

   // Simulate a lot of work
   Sleep(dwDuration * 1000);

   AddMessage(
      TEXT("[%u] '%s' is done."), 
      GetCurrentThreadId(), pData->szAction);

   // Ask the thread pool to set the synchronization event 
   // after this callback finishes
   SetEventWhenCallbackReturns(Instance, pData->finishEvent);

   // Don't forget to clean up the given parameter data block
   delete (pData);
}


//////////////////////////////////////////////////////////////////////////////


typedef struct _SYNCHRO_DATA
{
   DWORD    Count;
   HANDLE*  Handles;
} SYNCHRO_DATA, *PSYNCHRO_DATA;

VOID EndOfBatchCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {

   PSYNCHRO_DATA pData = (PSYNCHRO_DATA)Context;
   
   AddMessage(
      TEXT("[%u] waiting for batch to finish"), GetCurrentThreadId()
      );

   DWORD BatchItemsCount = pData->Count;
   do {
      DWORD dwResult = 
         WaitForMultipleObjects(pData->Count, pData->Handles, FALSE, 1000);
      switch(dwResult) {
         case WAIT_TIMEOUT:
            // We could check against user cancellation or 
            // provide processing feedback
            break;

         default: // One of the action is finished
            BatchItemsCount--;
            break;
      }
   } while (BatchItemsCount > 0);

   // Notify the UI thread for completion
   PostMessage(g_hDlg, WM_APP_COMPLETED, 0, (LPARAM)pData);
}


//////////////////////////////////////////////////////////////////////////////


void OnStartBatch() {

   // Sanity checks
   if (g_hEmailEvent != NULL)
   {
      AddMessage(TEXT("Previous batch is still running..."));
      return;
   }

   AddMessage(
      TEXT("[%u] ----Start a new batch----"), GetCurrentThreadId());
   
   // Create the synchronization events
   g_hEmailEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("EmailEvent"));
   g_hPrintEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("PrintEvent"));

   // Define the work items to be processed by the thread pool
   // 1. Keep track of the events that will be set when each action 
   //    if finished
   PSYNCHRO_DATA pSynchData = new SYNCHRO_DATA();
   pSynchData->Count = 2;
   pSynchData->Handles = new HANDLE[2];
   pSynchData->Handles[0] = g_hEmailEvent;
   pSynchData->Handles[1] = g_hPrintEvent;
   TrySubmitThreadpoolCallback((PTP_SIMPLE_CALLBACK)
      EndOfBatchCallback, pSynchData, &g_callbackEnvironment);

   // 2. Start the email action
   PCALLBACK_DATA pData = new CALLBACK_DATA();
   pData->finishEvent = g_hEmailEvent;
   _tcscpy_s(pData->szAction, MAX_PATH, TEXT("Email"));
   TrySubmitThreadpoolCallback((PTP_SIMPLE_CALLBACK)
      ActionCallback, pData, &g_callbackEnvironment);
   
   // 3. Start the printing action
   pData = new CALLBACK_DATA();
   pData->finishEvent = g_hPrintEvent;
   _tcscpy_s(pData->szAction, MAX_PATH, TEXT("Printing"));
   TrySubmitThreadpoolCallback((PTP_SIMPLE_CALLBACK)
      ActionCallback, pData, &g_callbackEnvironment);
}


//////////////////////////////////////////////////////////////////////////////


int lastWorkTime = 0;

VOID SimpleHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context) {

   // Ensure not bad random
   int workTime = ((rand() % 4) +1);
   while (workTime == lastWorkTime) {
      workTime = ((rand() % 4) +1);
   }
   lastWorkTime = workTime;
   
   AddMessage(
      TEXT("[%u] '%s' starting for %u seconds."), 
      GetCurrentThreadId(), (PCTSTR)Context, workTime);

   // Simulate a lot of work
   Sleep(workTime * 1000);

   AddMessage(
      TEXT("[%u] Instant '%s' is done."), 
      GetCurrentThreadId(), (PCTSTR)Context);

   delete ((PTSTR)Context);
}


//////////////////////////////////////////////////////////////////////////////


void OnRun() {

   // Setup randomizer
   srand(GetTickCount());
   
   for(int current = 1; current <= 6; current++) {
      PTSTR pRequest = new TCHAR[MAX_PATH];
      StringCchPrintf(pRequest, MAX_PATH, TEXT("Request %u"), current);
      if (TrySubmitThreadpoolCallback((PTP_SIMPLE_CALLBACK)
         SimpleHandler, pRequest, &g_callbackEnvironment))
      {
         AddMessage(
            TEXT("[%u] Request %u is submitted"), 
            GetCurrentThreadId(), current);
      } else {
         AddMessage(
            TEXT("[%u] Request %u can't be submitted"), 
            GetCurrentThreadId(), current);
      }
   }
}


//////////////////////////////////////////////////////////////////////////////


void OnDeletePool() {

   // Note: DestroyThreadpoolEnvironment() in winbase.h
   //       calls TpDestroyCallbackEnviron() in winnt.h 
   //       that does... nothing
   //       So, we need to destroy the thread pool ourself
   if (g_pThreadPool != NULL) {
      CloseThreadpool(g_pThreadPool);
   }

   // Clean up callback environment; maybe useful in next version
   // See previous comment
   DestroyThreadpoolEnvironment(&g_callbackEnvironment);
}


void OnCreatePool() {

   // Sanity checks
   if (g_pThreadPool != NULL) {
      OnDeletePool();
   }
   
   // Initialize the private thread pool
   g_pThreadPool = CreateThreadpool(NULL);
   if (g_pThreadPool == NULL) {
      MessageBox(
         NULL, 
         TEXT("Impossible to create private thread pool"), 
         TEXT("Initialization error"), 
         MB_ICONERROR | MB_OK
         );
      return;
   }

   // Change  min=2 / max=4 thread pool parameters
   SetThreadpoolThreadMaximum(g_pThreadPool, 4);
   if (!SetThreadpoolThreadMinimum(g_pThreadPool, 2)) {
      MessageBox(
         NULL, 
         TEXT("SetThreadpoolThreadMinimum failed."), 
         TEXT("Initialization error"), 
         MB_ICONERROR | MB_OK
         );
      return;
   }

   // Reset the callback environment
   // Note: this is an inline function from winbase.h
   //       that calls TpInitializeCallbackEnviron() in winnt.h.
   //       --> 
   //       CallbackEnviron->Version = 1;
   //       CallbackEnviron->Pool = NULL;
   //       CallbackEnviron->CleanupGroup = NULL;
   //       CallbackEnviron->CleanupGroupCancelCallback = NULL;
   //       CallbackEnviron->RaceDll = NULL;
   //       CallbackEnviron->ActivationContext = NULL;
   //       CallbackEnviron->FinalizationCallback = NULL;
   //       CallbackEnviron->u.Flags = 0;
   InitializeThreadpoolEnvironment(&g_callbackEnvironment);

   // Associate our private thread pool with the callback environment
   // Note: this is an inline function from winbase.h
   //       that calls TpSetCallbackThreadpool() in winnt.h.
   //       --> 
   //       CallbackEnviron->Pool = Pool;
   SetThreadpoolCallbackPool(&g_callbackEnvironment, g_pThreadPool);  
}


//////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDOK:
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_BTN_START_BATCH:
         OnStartBatch();
         break;

      case IDC_BTN_CREATE_POOL:
         OnCreatePool();
         break;

      case IDC_BTN_RUN:
         OnRun();
         break;

      case IDC_BTN_DELETE_POOL:
         OnDeletePool();
         break;
   }
}


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   // Keep track of main dialog window for error messages.
   g_hDlg = hwnd;

   return(TRUE);
}


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      case WM_APP_COMPLETED:
      {
         AddMessage(
            TEXT("[%u] ____Whole batch is over____"), 
            GetCurrentThreadId());

         // Don't forget to clean up given synchronization data
         PSYNCHRO_DATA pData = (PSYNCHRO_DATA)lParam;
         for (DWORD current = 0; current < pData->Count; current++) {
            if (!CloseHandle(pData->Handles[current])) {
               AddMessage(
                  TEXT("[%u] Error %u when closing handle %u"), 
                  GetCurrentThreadId(), GetLastError(), current);
            }
         }
         g_hEmailEvent = NULL;
         g_hPrintEvent = NULL;
         delete (pData);
      }
      break;
   }

   return(FALSE);
}


//////////////////////////////////////////////////////////////////////////////


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);

   DialogBoxParam(
      hInstance, 
      MAKEINTRESOURCE(IDD_MAIN), 
      NULL, 
      Dlg_Proc, 
      _ttoi(lpCmdLine)
      );

   return(0);
}


//////////////////////////////// End of File /////////////////////////////////

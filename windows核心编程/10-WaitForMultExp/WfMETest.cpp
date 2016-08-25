/******************************************************************************
Module:  TestWfME.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <process.h>
#include "resource.h"
#include "WaitForMultExp.h"


///////////////////////////////////////////////////////////////////////////////


// g_ahObjs contains the list of event kernel object
// handles referenced in the Boolean expression.
#define MAX_KERNEL_OBJS     1000
HANDLE g_ahObjs[MAX_KERNEL_OBJS];


// ahExpObjs contains all the expressions. A single expression
// consists of a contiguous set of kernel object handles that 
// is TRUE when all the objects are signaled at the same time. 
// A NULL handle is used to separate OR expressions. 

// A handle value may NOT appear multiple times within an AND 
// expression but the same handle value may appear in 
// different OR expressions.

// An expresion can have a maximum of 64 sets with no more 
// than 63 handles/set plus a NULL handle to separate each set
#define MAX_EXPRESSION_SIZE   ((64 * 63) + 63)


// m_nExpObjects is the number of entries used in the ahExpObjects array.
typedef struct {
   HWND     m_hwnd;                            // Where to send results
   DWORD    m_dwMilliseconds;                  // How long before timeout
   DWORD    m_nExpObjects;                     // # of entries in object list
   HANDLE   m_ahExpObjs[MAX_EXPRESSION_SIZE];  // List of objs
} AWFME, *PAWFME;
AWFME g_awfme;


// This message is posted to the UI thread when an expression 
// comes true or when we timeout while waiting for an 
// expression to come TRUE.
#define WM_WAITEND      (WM_USER + 101)


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_WFMETEXT);

   // Initialize the controls in the dialog box
   SetDlgItemInt(hwnd,  IDC_NUMOBJS, 4, FALSE);
   SetDlgItemInt(hwnd,  IDC_TIMEOUT, 30000, FALSE);
   SetDlgItemText(hwnd, IDC_EXPRESSION, 
      _T("1 2 | 2 3 4 | 1 4"));

   // Set the multi-column listbox's column size
   ListBox_SetColumnWidth(GetDlgItem(hwnd, IDC_OBJLIST), 
      LOWORD(GetDialogBaseUnits()) * 4);

   return(TRUE);  // Accept default focus window.
}


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI AsyncWaitForMultipleExpressions(PVOID pvParam) {

   PAWFME pawfme = (PAWFME) pvParam;

   DWORD dw = WaitForMultipleExpressions(pawfme->m_nExpObjects, 
      pawfme->m_ahExpObjs, pawfme->m_dwMilliseconds);
   PostMessage(pawfme->m_hwnd, WM_WAITEND, dw, 0);
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


LRESULT Dlg_OnWaitEnd(HWND hwnd, WPARAM wParam, LPARAM lParam) {

   // Close all the event kernel object handles
   for (int n = 0; g_ahObjs[n] != NULL; n++)
      CloseHandle(g_ahObjs[n]);

   // Tell the user the result of running the test
   if (wParam == WAIT_TIMEOUT) 
      SetDlgItemText(hwnd, IDC_RESULT, __TEXT("Timeout"));
   else 
      SetDlgItemInt(hwnd, IDC_RESULT, (DWORD) wParam - WAIT_OBJECT_0, FALSE);

   // Allow the user to change values and run the test again
   EnableWindow(GetDlgItem(hwnd, IDC_NUMOBJS),    TRUE);
   EnableWindow(GetDlgItem(hwnd, IDC_TIMEOUT),    TRUE);
   EnableWindow(GetDlgItem(hwnd, IDC_EXPRESSION), TRUE);
   EnableWindow(GetDlgItem(hwnd, IDOK),           TRUE);
   SetFocus(GetDlgItem(hwnd, IDC_EXPRESSION));

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   // Obtain the user's settings from the dialog box controls.
   TCHAR szExpression[100];
   ComboBox_GetText(GetDlgItem(hwnd, IDC_EXPRESSION), szExpression,
      sizeof(szExpression) / sizeof(szExpression[0]));

   int nObjects = GetDlgItemInt(hwnd, IDC_NUMOBJS, NULL, FALSE);

   switch (id) {
   case IDCANCEL:
      EndDialog(hwnd, id);
      break;

   case IDC_OBJLIST:
      switch (codeNotify) {
      case LBN_SELCHANGE:
         // An item changed state, reset all items and set the selected ones.
         for (int n = 0; n < nObjects; n++)
            ResetEvent(g_ahObjs[n]);

         for (n = 0; n < nObjects; n++) {
            if (ListBox_GetSel(GetDlgItem(hwnd, IDC_OBJLIST), n)) 
               SetEvent(g_ahObjs[n]);
         }
         break;
      }
      break;

   case IDOK:
      // Prevent the user from changing values while the test is running
      SetFocus(GetDlgItem(hwnd, IDC_OBJLIST));
      EnableWindow(GetDlgItem(hwnd, IDC_NUMOBJS),    FALSE);
      EnableWindow(GetDlgItem(hwnd, IDC_TIMEOUT),    FALSE);
      EnableWindow(GetDlgItem(hwnd, IDC_EXPRESSION), FALSE);
      EnableWindow(GetDlgItem(hwnd, IDOK),           FALSE);

      // Notify the user that the test is running
      SetDlgItemText(hwnd, IDC_RESULT, TEXT("Waiting..."));

      // Create all of the desired kernel objects
      ZeroMemory(g_ahObjs, sizeof(g_ahObjs));
      g_awfme.m_nExpObjects = 0;
      ZeroMemory(g_awfme.m_ahExpObjs, sizeof(g_awfme.m_ahExpObjs));
      g_awfme.m_hwnd = hwnd;
      g_awfme.m_dwMilliseconds = GetDlgItemInt(hwnd, IDC_TIMEOUT, NULL, FALSE);

      ListBox_ResetContent(GetDlgItem(hwnd, IDC_OBJLIST));
      for (int n = 0; n < nObjects; n++) {
         TCHAR szBuf[20];
         g_ahObjs[n] = CreateEvent(NULL, FALSE, FALSE, NULL);

         wsprintf(szBuf, TEXT("  %d"), n + 1);
         ListBox_AddString(GetDlgItem(hwnd, IDC_OBJLIST), 
            &szBuf[lstrlen(szBuf) - 3]);
      }

      PTSTR p = _tcstok(szExpression, TEXT(" "));
      while (p != NULL) {
         g_awfme.m_ahExpObjs[g_awfme.m_nExpObjects++] = 
            (*p == TEXT('|')) ? NULL : g_ahObjs[_ttoi(p) - 1];
         p = _tcstok(NULL, TEXT(" "));
      }

      DWORD dwThreadId;
      CloseHandle(chBEGINTHREADEX(NULL, 0, 
         AsyncWaitForMultipleExpressions, &g_awfme, 
         0, &dwThreadId));
      break;
   }   
}


///////////////////////////////////////////////////////////////////////////////

        
INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);

      case WM_WAITEND:
         return(Dlg_OnWaitEnd(hwnd, wParam, lParam)); 
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_TESTW4ME), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

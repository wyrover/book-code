/******************************************************************************
Module:  Summation.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <limits.h>
#include <process.h>       // For _beginthreadex
#include <tchar.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// An example of calling Sum for uNum = 0 through 9
// uNum: 0 1 2 3  4  5  6  7  8  9 ...
// Sum:  0 1 3 6 10 15 21 28 36 45 ...
UINT Sum(UINT uNum) {

   // Call Sum recursively.
   return((uNum == 0) ? 0 : (uNum + Sum(uNum - 1)));
}


///////////////////////////////////////////////////////////////////////////////


LONG WINAPI FilterFunc(DWORD dwExceptionCode) {

   return((dwExceptionCode == STATUS_STACK_OVERFLOW) 
      ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH);
}


///////////////////////////////////////////////////////////////////////////////


// The separate thread that is responsible for calculating the sum. 
// I use a separate thread for the following reasons:
//    1. A separate thread gets its own 1 MB of stack space.
//    2. A thread can be notified of a stack overflow only once.
//    3. The stack's storage is freed when the thread exits.
DWORD WINAPI SumThreadFunc(PVOID pvParam) {

   // The parameter pvParam, contains the number of integers to sum.
   UINT uSumNum = PtrToUlong(pvParam);

   // uSum contains the summation of the numbers from 0 through uSumNum. 
   // If the sum cannot be calculated, a sum of UINT_MAX is returned.
   UINT uSum = UINT_MAX;

   __try {
      // To catch the stack overflow exception, we must 
      // execute the Sum function while inside an SEH block.
      uSum = Sum(uSumNum); 
   }
   __except (FilterFunc(GetExceptionCode())) {
      // If we get in here, it's because we have trapped a stack overflow. 
      // We can now do whatever is necessary to gracefully continue execution
      // This sample application has nothing to do, so no code is placed 
      // in this exception handler block.
   }

   // The thread's exit code is the sum of the first uSumNum 
   // numbers, or UINT_MAX if a stack overflow occurred.
   return(uSum);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_SUMMATION);

   // Don't accept integers more that 9 digits long
   Edit_LimitText(GetDlgItem(hwnd, IDC_SUMNUM), 9);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_CALC:
         // Get the number of integers the user wants to sum.
         UINT uSum = GetDlgItemInt(hwnd, IDC_SUMNUM, NULL, FALSE);

         // Create a thread (with its own stack) that is
         // responsible for performing the summation.
         DWORD dwThreadId;
         HANDLE hThread = chBEGINTHREADEX(NULL, 0, 
            SumThreadFunc, (PVOID) (UINT_PTR) uSum, 0, &dwThreadId);

         // Wait for the thread to terminate.
         WaitForSingleObject(hThread, INFINITE);

         // The thread's exit code is the resulting summation.
         GetExitCodeThread(hThread, (PDWORD) &uSum);

         // Allow the system to destroy the thread kernel object
         CloseHandle(hThread);

         // Update the dialog box to show the result.
         if (uSum == UINT_MAX) {
            // If result is UINT_MAX, a stack overflow occurred.
            SetDlgItemText(hwnd, IDC_ANSWER, TEXT("Error"));
            chMB("The number is too big, please enter a smaller number");
         } else {
            // The sum was calculated successfully; 
            SetDlgItemInt(hwnd, IDC_ANSWER, uSum, FALSE);
         }
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

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_SUMMATION), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

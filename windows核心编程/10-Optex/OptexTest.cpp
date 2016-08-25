/******************************************************************************
Module name: OptexTest.cpp
Written by:  Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <process.h>
#include "Optex.h"


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI SecondFunc(PVOID pvParam) {

   COptex& optex = * (COptex*) pvParam;

   // The primary thread should own the optex here, this should fail
   chVERIFY(optex.TryEnter() == FALSE);

   // Wait for the primary thread to give up the optex
   optex.Enter();

   optex.Enter();  // Test recursive ownership
   chMB("Secondary: Entered the optex\n(Dismiss me 2nd)");

   // Leave the optex but we still own it once
   optex.Leave();
   chMB("Secondary: The primary thread should not display a box yet");
   optex.Leave();  // The primary thread should be able to run now

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


VOID FirstFunc(BOOL fLocal, COptex& optex) {

   optex.Enter();  // Gain ownership of the optex

   // Since this thread owns the optex, we should be able to get it again
   chVERIFY(optex.TryEnter()); 

   HANDLE hOtherThread = NULL;
   if (fLocal) {
      // Spawn a secondary thread for testing purposes (pass it the optex)

      DWORD dwThreadId;
      hOtherThread = chBEGINTHREADEX(NULL, 0, 
         SecondFunc, (PVOID) &optex, 0, &dwThreadId);

   } else {
      // Spawn a secondary process for testing purposes
      STARTUPINFO si = { sizeof(si) };
      PROCESS_INFORMATION pi;
      TCHAR szPath[MAX_PATH];
      GetModuleFileName(NULL, szPath, chDIMOF(szPath));
      CreateProcess(NULL, szPath, NULL, NULL, 
         FALSE, 0, NULL, NULL, &si, &pi);
      hOtherThread = pi.hProcess;
      CloseHandle(pi.hThread);
   }

   // Wait for the second thread to own the optex
   chMB("Primary: Hit OK to give optex to secondary");

   // Let the second thread gain ownership of the optex
   optex.Leave();
   optex.Leave();

   // Wait for the second thread to own the optex
   chMB("Primary: Hit OK to wait for the optex\n(Dismiss me 1st)");

   optex.Enter();  // Try to gain ownership back

   WaitForSingleObject(hOtherThread, INFINITE);
   CloseHandle(hOtherThread);
   optex.Leave();
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // This event is just used to determine which instance this is.
   HANDLE hevt = CreateEvent(NULL, FALSE, FALSE, TEXT("OptexTest"));

   if (GetLastError() != ERROR_ALREADY_EXISTS) {

      // This is the first instance of this test application

      // First, let's test the single-process optex
      COptex optexSingle;   // Create a single-process optex
      FirstFunc(TRUE, optexSingle);

      // Now, let's test the cross-process optex
      COptex optexCross("CrossOptexTest");   // Create a cross-process optex
      FirstFunc(FALSE, optexCross);

   } else {

      // This is the second instance of this test application
      DebugBreak();  // Force debugger connection for tracing

      // Test the cross-process optex
      COptex optexCross("CrossOptexTest");   // Create a cross-process optex
      SecondFunc((PVOID) &optexCross);
   }

   CloseHandle(hevt);
   return(0);
}


///////////////////////////////// End of File /////////////////////////////////

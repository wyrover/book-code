/******************************************************************************
Module:  IntLockTest.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include "Interlocked.h"


///////////////////////////////////////////////////////////////////////////////


// Set to TRUE when worker threads should terminate cleanly.
volatile BOOL g_fQuit = FALSE;


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI WorkerThread(PVOID pvParam) {

   CWhenZero<BYTE>& bVal = * (CWhenZero<BYTE> *) pvParam;

   // Should worker thread terminate?
   while (!g_fQuit) {

      // Wait for something to do
      WaitForSingleObject(bVal.GetNotZeroHandle(), INFINITE);

      // If we should quit, quit
      if (g_fQuit) 
         continue;

      // Do something
      chMB("Worker thread: We have something to do");

      bVal--;     // We're done

      // Wait for all worker threads to stop
      WaitForSingleObject(bVal, INFINITE);
   }

   chMB("Worker thread: terminating");
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Initialize to indicate that NO worker threads have anything to do
   CWhenZero<BYTE> bVal = 0;

   // Create the worker threads
   const int nMaxThreads = 2;
   HANDLE hThreads[nMaxThreads];
   for (int nThread = 0; nThread < nMaxThreads; nThread++) {
      DWORD dwThreadId;
      hThreads[nThread] = CreateThread(NULL, 0, 
         WorkerThread, (PVOID) &bVal, 0, &dwThreadId);
   }

   int n;
   do {
      // Do more work or stop running?
      n = MessageBox(NULL, 
         TEXT("Yes: Give worker threads something to do\nNo: Quit"), 
         TEXT("Primary thread"), MB_YESNO);

      // Tell worker threads that we're quitting
      if (n == IDNO) 
         InterlockedExchangePointer((PVOID*) &g_fQuit, (PVOID) TRUE);

      bVal = nMaxThreads;  // Wake the worker threads

      if (n == IDYES) {

         // There is work to do, wait for the worker threads to finish
         WaitForSingleObject(bVal, INFINITE);
      }

   } while (n == IDYES);

   // There is no more work to do, the process wants to die.
   // Wait for the worker threads to terminate
   WaitForMultipleObjects(nMaxThreads, hThreads, TRUE, INFINITE);

   // Close the worker thread handles.
   for (nThread = 0; nThread < nMaxThreads; nThread++)
      CloseHandle(hThreads[nThread]);

   // Tell the user that the process is dying
   chMB("Primary thread: terminating");

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

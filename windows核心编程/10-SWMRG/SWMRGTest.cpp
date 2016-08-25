/******************************************************************************
Module:  SWMRGTest.Cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <process.h>       // for _beginthreadex
#include "SWMRG.h"


///////////////////////////////////////////////////////////////////////////////


// Global Single-Writer/Multiple-Reader Guard synchronization object
CSWMRG g_swmrg;


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI Thread(PVOID pvParam) {

   TCHAR sz[50];
   wsprintf(sz, TEXT("SWMRG Test: Thread %d"), PtrToShort(pvParam));
   int n = MessageBox(NULL, 
      TEXT("YES: Attempt to read\nNO: Attempt to write"), sz, MB_YESNO);

   // Attempt to read or write
   if (n == IDYES) 
      g_swmrg.WaitToRead();
   else 
      g_swmrg.WaitToWrite();

   MessageBox(NULL, 
      (n == IDYES) ? TEXT("OK stops READING") : TEXT("OK stops WRITING"),
      sz, MB_OK);

   // Stop reading/writing
   g_swmrg.Done();
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Spawn a bunch of threads that will attempt to read/write
   HANDLE hThreads[MAXIMUM_WAIT_OBJECTS];
   for (int nThreads = 0; nThreads < 8; nThreads++) {
      DWORD dwThreadId;
      hThreads[nThreads] = 
         chBEGINTHREADEX(NULL, 0, Thread, (PVOID) (DWORD_PTR) nThreads, 
            0, &dwThreadId);
   }

   // Wait for all the threads to exit
   WaitForMultipleObjects(nThreads, hThreads, TRUE, INFINITE);
   while (nThreads--)
      CloseHandle(hThreads[nThreads]);

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

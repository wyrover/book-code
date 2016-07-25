/*--------------------------------------------------
      TEST.C
      Creates threads that copy data into memory
      that the Share DLL shares among its clients.
  --------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "share.h"

#define NUM_THREADS 5
#define NUM_STRINGS 5000

LONG StoreData(CONST DWORD dwUnused);

/*--------------------------------------------------
      MAIN
  --------------------------------------------------*/

int main( )
{
   HANDLE hThreads[NUM_THREADS];
   DWORD dwID;
   int i;

   /* Create threads to put data in the buffer. */
   for (i=0; i<NUM_THREADS; i++) {
      hThreads[i] = CreateThread(NULL, 0,
         (LPTHREAD_START_ROUTINE)StoreData,
         0, 0, &dwID);
      printf("Created thread 0x%08lX.\n", dwID);        
   }

   /* When threads have finished, show all data. */
   WaitForMultipleObjects( NUM_THREADS, hThreads,
      TRUE, INFINITE );
   ShowAllStrings( );
   return(0);
}

/*--------------------------------------------------
      STORE DATA
  --------------------------------------------------*/

LONG StoreData(CONST DWORD dwUnused)
{
   int iEntry;          /* counter for data strings */
   char szData[236];    /* leave room for thread ID */

   for (iEntry=0; iEntry<NUM_STRINGS; iEntry++) {

      /* Create a new string. */
      wsprintf(szData, "entry #%d", iEntry);

      /* Add it to the buffer. */
      if (!AddString(szData)) {
         /* Is the buffer full? */
         break;
      }
   }
   return(0);
}

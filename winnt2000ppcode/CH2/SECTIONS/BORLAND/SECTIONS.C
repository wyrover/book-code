/*--------------------------------------------------
      SECTIONS.C

      A library that counts the number of threads
      and processes that attach themselves to it.  
      The library's exported functions return the
      current tally values.

      written by Brian G. Myers      
  --------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "sections.h"

/*--------------------------------------------------
      GLOBAL VARIABLES

      uNumClients          total number of processes
      uNumThreadsGlobal    total number of threads
      uNumThreadsLocal     total threads from the
                              current process only
  --------------------------------------------------*/

/* shared data: one value shared by all clients
   (defined in GLOBAL.C) */
extern unsigned int uNumClients;
extern unsigned int uNumThreadsGlobal;

/* private data: unique value for each client */
UINT uNumThreadsLocal = 0;

/*--------------------------------------------------
      DLL MAIN
      Adjust totals as clients attach and
      detach themselves.
  --------------------------------------------------*/

BOOL WINAPI DllMain(HINSTANCE hinstDLL,
   DWORD dwReason, LPVOID lpReserved)
{
   switch (dwReason)
   {
      case DLL_PROCESS_ATTACH:
         uNumClients++;
         /* fall through for the primary thread */

      case DLL_THREAD_ATTACH:
         uNumThreadsLocal++;     
         uNumThreadsGlobal++;    
         break;

      case DLL_THREAD_DETACH:    
         uNumThreadsLocal--;     
         uNumThreadsGlobal--;    
         break;

      case DLL_PROCESS_DETACH:   
         uNumClients--;
         break;
   }
   return(TRUE);
}

/*--------------------------------------------------
      GET NUM CLIENTS
  --------------------------------------------------*/

UINT GetNumClients(void)
{
   return(uNumClients);
}

/*--------------------------------------------------
      GET NUM THREADS LOCAL
  --------------------------------------------------*/

UINT GetNumThreadsLocal(void)
{
   return(uNumThreadsLocal);
}

/*--------------------------------------------------
      GET NUM THREADS GLOBAL
  --------------------------------------------------*/

UINT GetNumThreadsGlobal(void)
{
   return(uNumThreadsGlobal);
}

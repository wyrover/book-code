/******************************************************************************
Module:  ImgWalk.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) {

   if (fdwReason == DLL_PROCESS_ATTACH) {
      char szBuf[MAX_PATH * 100] = { 0 };

      PBYTE pb = NULL;
      MEMORY_BASIC_INFORMATION mbi;
      while (VirtualQuery(pb, &mbi, sizeof(mbi)) == sizeof(mbi)) {

         int nLen;
         char szModName[MAX_PATH];

         if (mbi.State == MEM_FREE)
            mbi.AllocationBase = mbi.BaseAddress;

         if ((mbi.AllocationBase == hinstDll) ||
             (mbi.AllocationBase != mbi.BaseAddress) ||
             (mbi.AllocationBase == NULL)) {
            // Do not add the module name to the list
            // if any of the following is true:
            // 1. If this region contains this DLL
            // 2. If this block is NOT the beginning of a region
            // 3. If the address is NULL
            nLen = 0;
         } else {
            nLen = GetModuleFileNameA((HINSTANCE) mbi.AllocationBase, 
               szModName, chDIMOF(szModName));
         }

         if (nLen > 0) {
            wsprintfA(strchr(szBuf, 0), "\n%p-%s", 
               mbi.AllocationBase, szModName);
         }

         pb += mbi.RegionSize;
      }

      chMB(&szBuf[1]);
   }

   return(TRUE);
}


//////////////////////////////// End of File //////////////////////////////////

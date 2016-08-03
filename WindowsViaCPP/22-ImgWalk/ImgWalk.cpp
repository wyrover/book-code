/******************************************************************************
Module:  ImgWalk.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, PVOID fImpLoad) {

   if (fdwReason == DLL_PROCESS_ATTACH) {
      char szBuf[MAX_PATH * 100] = { 0 };

      PBYTE pb = NULL;
      MEMORY_BASIC_INFORMATION mbi;
      while (VirtualQuery(pb, &mbi, sizeof(mbi)) == sizeof(mbi)) {

         int nLen;
         char szModName[MAX_PATH];

         if (mbi.State == MEM_FREE)
            mbi.AllocationBase = mbi.BaseAddress;

         if ((mbi.AllocationBase == hInstDll) ||
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
               szModName, _countof(szModName));
         }

         if (nLen > 0) {
            wsprintfA(strchr(szBuf, 0), "\n%p-%s", 
               mbi.AllocationBase, szModName);
         }

         pb += mbi.RegionSize;
      }

      // NOTE: Normally, you should not display a message box in DllMain
      // due to the loader lock described in Chapter 20. However, to keep
      // this sample application simple, I am violating this rule.
      chMB(&szBuf[1]);
   }

   return(TRUE);
}


//////////////////////////////// End of File //////////////////////////////////

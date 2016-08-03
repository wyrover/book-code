/******************************************************************************
Module:  MemReset.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, PTSTR, int) {

   TCHAR szAppName[]  = TEXT("MEM_RESET tester");
   TCHAR szTestData[] = TEXT("Some text data");

   // Commit a page of storage and modify its contents.
   PTSTR pszData = (PTSTR) VirtualAlloc(NULL, 1024, 
      MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
   _tcscpy_s(pszData, 1024, szTestData);

   if (MessageBox(NULL, TEXT("Do you want to access this data later?"), 
      szAppName, MB_YESNO) == IDNO) {

      // We want this page of storage to remain in our process but the 
      // contents aren't important to us anymore. 
      // Tell the system that the data is not modified.

      // Note: Because MEM_RESET destroys data, VirtualAlloc rounds
      // the base address and size parameters to their safest range.
      // Here is an example: 
      //    VirtualAlloc(pvData, 5000, MEM_RESET, PAGE_READWRITE)
      // resets 0 pages on CPUs where the page size is greater than 4 KB 
      // and resets 1 page on CPUs with a 4 KB page. So that our call to 
      // VirtualAlloc to reset memory below always succeeds, VirtualQuery 
      // is called first to get the exact region size.
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQuery(pszData, &mbi, sizeof(mbi));
      VirtualAlloc(pszData, mbi.RegionSize, MEM_RESET, PAGE_READWRITE);
   }
   
   // Commit as much storage as there is physical RAM.
   MEMORYSTATUS mst;
   GlobalMemoryStatus(&mst);
   PVOID pvDummy = VirtualAlloc(NULL, mst.dwTotalPhys, 
      MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

   // Touch all the pages in the dummy region so that any
   // modified pages in RAM are written to the paging file.
   if (pvDummy != NULL)
      ZeroMemory(pvDummy, mst.dwTotalPhys);

   // Compare our data page with what we originally wrote there.
   if (_tcscmp(pszData, szTestData) == 0) {

      // The data in the page matches what we originally put there.
      // ZeroMemory forced our page to be written to the paging file.
      MessageBox(NULL, TEXT("Modified data page was saved."), 
         szAppName, MB_OK);
   } else {

      // The data in the page does NOT match what we originally put there
      // ZeroMemory didn't cause our page to be written to the paging file
      MessageBox(NULL, TEXT("Modified data page was NOT saved."), 
         szAppName, MB_OK);
   }
   
   // Don't forget to release part of the address space.
   // Note that it is not mandatory here since the application is exiting.
   if (pvDummy != NULL)
      VirtualFree(pvDummy, 0, MEM_RELEASE);
   VirtualFree(pszData, 0, MEM_RELEASE);
   
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////

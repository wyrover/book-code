/******************************************************************************
Module:  VMQuery.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include "VMQuery.h"


///////////////////////////////////////////////////////////////////////////////


// Helper structure
typedef struct {
   SIZE_T RgnSize;
   DWORD  dwRgnStorage;     // MEM_*: Free, Image, Mapped, Private
   DWORD  dwRgnBlocks;
   DWORD  dwRgnGuardBlks;   // If > 0, region contains thread stack
   BOOL   fRgnIsAStack;     // TRUE if region contains thread stack
} VMQUERY_HELP;


// This global, static variable holds the allocation granularity value for 
// this CPU platform. Initialized the first time VMQuery is called.
static DWORD gs_dwAllocGran = 0;


///////////////////////////////////////////////////////////////////////////////


// Iterates through a region's blocks and returns findings in VMQUERY_HELP
static BOOL VMQueryHelp(HANDLE hProcess, LPCVOID pvAddress, 
   VMQUERY_HELP *pVMQHelp) {

   // Each element contains a page protection
   // (i.e.: 0=reserved, PAGE_NOACCESS, PAGE_READWRITE, etc.)
   DWORD dwProtectBlock[4] = { 0 }; 

   ZeroMemory(pVMQHelp, sizeof(*pVMQHelp));

   // Get address of region containing passed memory address.
   MEMORY_BASIC_INFORMATION mbi;
   BOOL fOk = (VirtualQueryEx(hProcess, pvAddress, &mbi, sizeof(mbi)) 
      == sizeof(mbi));

   if (!fOk)
      return(fOk);   // Bad memory address, return failure

   // Walk starting at the region's base address (which never changes)
   PVOID pvRgnBaseAddress = mbi.AllocationBase;

   // Walk starting at the first block in the region (changes in the loop)
   PVOID pvAddressBlk = pvRgnBaseAddress;

   // Save the memory type of the physical storage block.
   pVMQHelp->dwRgnStorage = mbi.Type;

   for (;;) {
      // Get info about the current block.
      fOk = (VirtualQueryEx(hProcess, pvAddressBlk, &mbi, sizeof(mbi)) 
         == sizeof(mbi));
      if (!fOk)
         break;   // Couldn't get the information, end loop.

      // Is this block in the same region?
      if (mbi.AllocationBase != pvRgnBaseAddress)
         break;   // Found a block in the next region; end loop.

      // We have a block contained in the region.

      // The following if statement is for detecting stacks in Windows 98.
      // A Windows 98 stack region's last 4 blocks look like this:
      //    reserved block, no access block, read-write block, reserved block
      if (pVMQHelp->dwRgnBlocks < 4) {
         // 0th through 3rd block, remember the block's protection
         dwProtectBlock[pVMQHelp->dwRgnBlocks] = 
            (mbi.State == MEM_RESERVE) ? 0 : mbi.Protect;
      } else {
         // We've seen 4 blocks in this region.
         // Shift the protection values down in the array.
         MoveMemory(&dwProtectBlock[0], &dwProtectBlock[1], 
            sizeof(dwProtectBlock) - sizeof(DWORD));

         // Add the new protection value to the end of the array.
         dwProtectBlock[3] = (mbi.State == MEM_RESERVE) ? 0 : mbi.Protect;
      }

      pVMQHelp->dwRgnBlocks++;             // Add another block to the region
      pVMQHelp->RgnSize += mbi.RegionSize; // Add block's size to region size

      // If block has PAGE_GUARD attribute, add 1 to this counter
      if ((mbi.Protect & PAGE_GUARD) == PAGE_GUARD)
         pVMQHelp->dwRgnGuardBlks++;

      // Take a best guess as to the type of physical storage committed to the
      // block. This is a guess because some blocks can convert from MEM_IMAGE
      // to MEM_PRIVATE or from MEM_MAPPED to MEM_PRIVATE; MEM_PRIVATE can
      // always be overridden by MEM_IMAGE or MEM_MAPPED.
      if (pVMQHelp->dwRgnStorage == MEM_PRIVATE)
         pVMQHelp->dwRgnStorage = mbi.Type;

      // Get the address of the next block.
      pvAddressBlk = (PVOID) ((PBYTE) pvAddressBlk + mbi.RegionSize);
   }

   // After examining the region, check to see whether it is a thread stack
   // Windows 2000: Assume stack if region has at least 1 PAGE_GUARD block
   // Windows 9x:   Assume stack if region has at least 4 blocks with
   //               3rd block from end: reserved
   //               2nd block from end: PAGE_NOACCESS
   //               1st block from end: PAGE_READWRITE
   //               block at end: another reserved block.
   pVMQHelp->fRgnIsAStack =
      (pVMQHelp->dwRgnGuardBlks > 0)         ||
      ((pVMQHelp->dwRgnBlocks >= 4)          &&
       (dwProtectBlock[0] == 0)              && 
       (dwProtectBlock[1] == PAGE_NOACCESS)  &&
       (dwProtectBlock[2] == PAGE_READWRITE) &&
       (dwProtectBlock[3] == 0));

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


BOOL VMQuery(HANDLE hProcess, LPCVOID pvAddress, PVMQUERY pVMQ) {

   if (gs_dwAllocGran == 0) {
      // Set allocation granularity if this is the first call
      SYSTEM_INFO sinf;
      GetSystemInfo(&sinf);
      gs_dwAllocGran = sinf.dwAllocationGranularity;
   }

   ZeroMemory(pVMQ, sizeof(*pVMQ));

   // Get the MEMORY_BASIC_INFORMATION for the passed address.
   MEMORY_BASIC_INFORMATION mbi;
   BOOL fOk = (VirtualQueryEx(hProcess, pvAddress, &mbi, sizeof(mbi)) 
      == sizeof(mbi));

   if (!fOk)
      return(fOk);   // Bad memory address, return failure

   // The MEMORY_BASIC_INFORMATION structure contains valid information.
   // Time to start setting the members of our own VMQUERY structure.

   // First, fill in the block members. We'll fill the region members later.
   switch (mbi.State) {
      case MEM_FREE:       // Free block (not reserved)
         pVMQ->pvBlkBaseAddress = NULL;
         pVMQ->BlkSize = 0;
         pVMQ->dwBlkProtection = 0;
         pVMQ->dwBlkStorage = MEM_FREE;
         break;

      case MEM_RESERVE:    // Reserved block without committed storage in it.
         pVMQ->pvBlkBaseAddress = mbi.BaseAddress;
         pVMQ->BlkSize = mbi.RegionSize;

         // For an uncommitted block, mbi.Protect is invalid. So we will 
         // show that the reserved block inherits the protection attribute 
         // of the region in which it is contained.
         pVMQ->dwBlkProtection = mbi.AllocationProtect;  
         pVMQ->dwBlkStorage = MEM_RESERVE;
         break;

      case MEM_COMMIT:     // Reserved block with committed storage in it.
         pVMQ->pvBlkBaseAddress = mbi.BaseAddress;
         pVMQ->BlkSize = mbi.RegionSize;
         pVMQ->dwBlkProtection = mbi.Protect;   
         pVMQ->dwBlkStorage = mbi.Type;
         break;

      default:
          DebugBreak();
          break;
   }

   // Now fill in the region data members.
   VMQUERY_HELP VMQHelp;
   switch (mbi.State) {
      case MEM_FREE:       // Free block (not reserved)
         pVMQ->pvRgnBaseAddress = mbi.BaseAddress;
         pVMQ->dwRgnProtection  = mbi.AllocationProtect;
         pVMQ->RgnSize          = mbi.RegionSize;
         pVMQ->dwRgnStorage     = MEM_FREE;
         pVMQ->dwRgnBlocks      = 0;
         pVMQ->dwRgnGuardBlks   = 0;
         pVMQ->fRgnIsAStack     = FALSE;
         break;

      case MEM_RESERVE:    // Reserved block without committed storage in it.
         pVMQ->pvRgnBaseAddress = mbi.AllocationBase;
         pVMQ->dwRgnProtection  = mbi.AllocationProtect;

         // Iterate through all blocks to get complete region information.         
         VMQueryHelp(hProcess, pvAddress, &VMQHelp);

         pVMQ->RgnSize          = VMQHelp.RgnSize;
         pVMQ->dwRgnStorage     = VMQHelp.dwRgnStorage;
         pVMQ->dwRgnBlocks      = VMQHelp.dwRgnBlocks;
         pVMQ->dwRgnGuardBlks   = VMQHelp.dwRgnGuardBlks;
         pVMQ->fRgnIsAStack     = VMQHelp.fRgnIsAStack;
         break;

      case MEM_COMMIT:     // Reserved block with committed storage in it.
         pVMQ->pvRgnBaseAddress = mbi.AllocationBase;
         pVMQ->dwRgnProtection  = mbi.AllocationProtect;

         // Iterate through all blocks to get complete region information.         
         VMQueryHelp(hProcess, pvAddress, &VMQHelp);

         pVMQ->RgnSize          = VMQHelp.RgnSize;
         pVMQ->dwRgnStorage     = VMQHelp.dwRgnStorage;
         pVMQ->dwRgnBlocks      = VMQHelp.dwRgnBlocks;
         pVMQ->dwRgnGuardBlks   = VMQHelp.dwRgnGuardBlks;
         pVMQ->fRgnIsAStack     = VMQHelp.fRgnIsAStack;
         break;

      default:
          DebugBreak();
          break;
   }

   return(fOk);
}


//////////////////////////////// End of File //////////////////////////////////

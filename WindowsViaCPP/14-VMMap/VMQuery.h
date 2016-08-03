/******************************************************************************
Module:  VMQuery.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


typedef struct {
   // Region information
   PVOID  pvRgnBaseAddress;
   DWORD  dwRgnProtection;  // PAGE_*
   SIZE_T RgnSize;
   DWORD  dwRgnStorage;     // MEM_*: Free, Image, Mapped, Private
   DWORD  dwRgnBlocks;
   DWORD  dwRgnGuardBlks;   // If > 0, region contains thread stack
   BOOL   bRgnIsAStack;     // TRUE if region contains thread stack

   // Block information
   PVOID  pvBlkBaseAddress;
   DWORD  dwBlkProtection;  // PAGE_*
   SIZE_T BlkSize;
   DWORD  dwBlkStorage;     // MEM_*: Free, Reserve, Image, Mapped, Private
} VMQUERY, *PVMQUERY;


///////////////////////////////////////////////////////////////////////////////


BOOL VMQuery(HANDLE hProcess, LPCVOID pvAddress, PVMQUERY pVMQ);


//////////////////////////////// End of File //////////////////////////////////

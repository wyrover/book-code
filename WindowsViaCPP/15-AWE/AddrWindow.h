/******************************************************************************
Module:  AddrWindow.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


class CSystemInfo : public SYSTEM_INFO {
public:
   CSystemInfo() { GetSystemInfo(this); }
};


///////////////////////////////////////////////////////////////////////////////


class CAddrWindow {
public:
   CAddrWindow()  { m_pvWindow = NULL; }
   ~CAddrWindow() { Destroy(); }

   BOOL Create(SIZE_T dwBytes, PVOID pvPreferredWindowBase = NULL) {
      // Reserve address window region to view physical storage
      m_pvWindow = VirtualAlloc(pvPreferredWindowBase, dwBytes, 
         MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE);
      return(m_pvWindow != NULL);
   }

   BOOL Destroy() {
      BOOL bOk = TRUE;
      if (m_pvWindow != NULL) {
         // Destroy address window region
         bOk = VirtualFree(m_pvWindow, 0, MEM_RELEASE);
         m_pvWindow = NULL;
      }
      return(bOk);
   }

   BOOL UnmapStorage() {
      // Unmap all storage from address window region
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQuery(m_pvWindow, &mbi, sizeof(mbi));
      return(MapUserPhysicalPages(m_pvWindow, 
         mbi.RegionSize / sm_sinf.dwPageSize, NULL));
   }

   // Returns virtual address of address window
   operator PVOID() { return(m_pvWindow); }

private:
   PVOID m_pvWindow;    // Virtual address of address window region
   static CSystemInfo sm_sinf;
};


///////////////////////////////////////////////////////////////////////////////


CSystemInfo CAddrWindow::sm_sinf;


///////////////////////////////////////////////////////////////////////////////


class CAddrWindowStorage {
public:
   CAddrWindowStorage()  { m_ulPages = 0; m_pulUserPfnArray = NULL; }
   ~CAddrWindowStorage() { Free(); }

   BOOL Allocate(ULONG_PTR ulBytes) {
      // Allocate storage intended for an address window

      Free();  // Clean up this object's existing address window

      // Calculate number of pages from number of bytes
      m_ulPages = (ulBytes + sm_sinf.dwPageSize - 1) / sm_sinf.dwPageSize;

      // Allocate array of page frame numbers
      m_pulUserPfnArray = (PULONG_PTR) 
         HeapAlloc(GetProcessHeap(), 0, m_ulPages * sizeof(ULONG_PTR));

      BOOL bOk = (m_pulUserPfnArray != NULL);
      if (bOk) {
         // The "Lock Pages in Memory" privilege must be enabled
         EnablePrivilege(SE_LOCK_MEMORY_NAME, TRUE);
         bOk = AllocateUserPhysicalPages(GetCurrentProcess(), 
            &m_ulPages, m_pulUserPfnArray);
         EnablePrivilege(SE_LOCK_MEMORY_NAME, FALSE);
      }
      return(bOk);
   }

   BOOL Free() {
      BOOL bOk = TRUE;
      if (m_pulUserPfnArray != NULL) {
         bOk = FreeUserPhysicalPages(GetCurrentProcess(), 
            &m_ulPages, m_pulUserPfnArray);
         if (bOk) {
            // Free the array of page frame numbers
            HeapFree(GetProcessHeap(), 0, m_pulUserPfnArray);
            m_ulPages = 0; 
            m_pulUserPfnArray = NULL; 
         }
      }
      return(bOk);
   }

   ULONG_PTR HowManyPagesAllocated() { return(m_ulPages); }

   BOOL MapStorage(CAddrWindow& aw) {
      return(MapUserPhysicalPages(aw, 
         HowManyPagesAllocated(), m_pulUserPfnArray));
   }

   BOOL UnmapStorage(CAddrWindow& aw) {
      return(MapUserPhysicalPages(aw, 
         HowManyPagesAllocated(), NULL));
   }

private:
   static BOOL EnablePrivilege(PCTSTR pszPrivName, BOOL bEnable = TRUE) {

      BOOL bOk = FALSE;    // Assume function fails
      HANDLE hToken;

      // Try to open this process' access token
      if (OpenProcessToken(GetCurrentProcess(), 
         TOKEN_ADJUST_PRIVILEGES, &hToken)) {

         // Attempt to modify the "Lock pages in Memory" privilege
         TOKEN_PRIVILEGES tp = { 1 };
         LookupPrivilegeValue(NULL, pszPrivName, &tp.Privileges[0].Luid);
         tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
         AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
         bOk = (GetLastError() == ERROR_SUCCESS);
         CloseHandle(hToken);
      }
      return(bOk);
   }

private:
   ULONG_PTR  m_ulPages;         // Number of storage pages
   PULONG_PTR m_pulUserPfnArray; // Page frame number array

private:
   static CSystemInfo sm_sinf;
};


///////////////////////////////////////////////////////////////////////////////


CSystemInfo CAddrWindowStorage::sm_sinf;


//////////////////////////////// End of File //////////////////////////////////

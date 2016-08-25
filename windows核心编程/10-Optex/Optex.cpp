/******************************************************************************
Module:  Optex.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include "Optex.h"


///////////////////////////////////////////////////////////////////////////////


// 0=multi-CPU, 1=single-CPU, -1=not set yet
BOOL COptex::sm_fUniprocessorHost = -1;


///////////////////////////////////////////////////////////////////////////////


PSTR COptex::ConstructObjectName(PSTR pszResult, 
   PCSTR pszPrefix, BOOL fUnicode, PVOID pszName) {

   pszResult[0] = 0;
   if (pszName == NULL) 
      return(NULL);

   wsprintfA(pszResult, fUnicode ? "%s%S" : "%s%s", pszPrefix, pszName);
   return(pszResult);
}


///////////////////////////////////////////////////////////////////////////////


void COptex::CommonConstructor(DWORD dwSpinCount, 
   BOOL fUnicode, PVOID pszName) {

   if (sm_fUniprocessorHost == -1) {
      // This is the 1st object constructed, get the number of CPUs
      SYSTEM_INFO sinf;
      GetSystemInfo(&sinf);
      sm_fUniprocessorHost = (sinf.dwNumberOfProcessors == 1);
   }


   m_hevt = m_hfm = NULL;
   m_psi  = NULL;

   if (pszName == NULL) {  // Creating a single-process optex

      m_hevt = CreateEventA(NULL, FALSE, FALSE, NULL);
      chASSERT(m_hevt != NULL);

      m_psi = new SHAREDINFO;
      chASSERT(m_psi != NULL);
      ZeroMemory(m_psi, sizeof(*m_psi));

   } else {                // Creating a cross-process optex

      // Always use ANSI so that this works on Win9x and Windows 2000
      char szResult[100];
      ConstructObjectName(szResult, "Optex_Event_", fUnicode, pszName);
      m_hevt = CreateEventA(NULL, FALSE, FALSE, szResult);
      chASSERT(m_hevt != NULL);

      ConstructObjectName(szResult, "Optex_MMF_", fUnicode, pszName);
      m_hfm = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, 
         PAGE_READWRITE, 0, sizeof(*m_psi), szResult);
      chASSERT(m_hfm != NULL);

      m_psi = (PSHAREDINFO) MapViewOfFile(m_hfm, 
         FILE_MAP_WRITE, 0, 0, 0);
      chASSERT(m_psi != NULL);

      // Note: SHAREDINFO's m_lLockCount, m_dwThreadId, and m_lRecurseCount
      // members need to be initialized to 0. Fortunately, a new pagefile 
      // MMF sets all of its data to 0 when created. This saves use from 
      // some thread synchronization work.
   }

   SetSpinCount(dwSpinCount);
}


///////////////////////////////////////////////////////////////////////////////


COptex::~COptex() {

#ifdef _DEBUG
   if (IsSingleProcessOptex() && (m_psi->m_dwThreadId != 0)) {
      // A single-process optex shouldn't be destroyed if any thread owns it
      DebugBreak();
   }

   if (!IsSingleProcessOptex() && 
      (m_psi->m_dwThreadId == GetCurrentThreadId())) {

      // A cross-process optex shouldn't be destroyed if our thread owns it
      DebugBreak();
   }
#endif

   CloseHandle(m_hevt);

   if (IsSingleProcessOptex()) {
      delete m_psi;
   } else {
      UnmapViewOfFile(m_psi);
      CloseHandle(m_hfm);
   }
}


///////////////////////////////////////////////////////////////////////////////


void COptex::SetSpinCount(DWORD dwSpinCount) {

   // No spinning on single CPU machines
   if (!sm_fUniprocessorHost)
      InterlockedExchangePointer((PVOID*) &m_psi->m_dwSpinCount, 
        (PVOID) (DWORD_PTR) dwSpinCount);
}


///////////////////////////////////////////////////////////////////////////////


void COptex::Enter() {

   // Spin, trying to get the optex
   if (TryEnter()) 
      return;  // We got it, return

   // We couldn't get the optex, wait for it.
   DWORD dwThreadId = GetCurrentThreadId();

   if (InterlockedIncrement(&m_psi->m_lLockCount) == 1) {

      // Optex is unowned, let this thread own it once
      m_psi->m_dwThreadId = dwThreadId;
      m_psi->m_lRecurseCount = 1;

   } else {

      if (m_psi->m_dwThreadId == dwThreadId) {

         // If optex is owned by this thread, own it again
         m_psi->m_lRecurseCount++;

      } else {

         // Optex is owned by another thread, wait for it
         WaitForSingleObject(m_hevt, INFINITE);

         // Optex is unowned, let this thread own it once
         m_psi->m_dwThreadId = dwThreadId;
         m_psi->m_lRecurseCount = 1;
      }
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL COptex::TryEnter() {

   DWORD dwThreadId = GetCurrentThreadId();

   BOOL fThisThreadOwnsTheOptex = FALSE;     // Assume a thread owns the optex
   DWORD dwSpinCount = m_psi->m_dwSpinCount; // How many times to spin

   do {
      // If lock count = 0, optex is unowned, we can own it
      fThisThreadOwnsTheOptex = (0 == 
         InterlockedCompareExchange(&m_psi->m_lLockCount, 1, 0)); 

      if (fThisThreadOwnsTheOptex) {

         // Optex is unowned, let this thread own it once
         m_psi->m_dwThreadId = dwThreadId;
         m_psi->m_lRecurseCount = 1;

      } else {

         if (m_psi->m_dwThreadId == dwThreadId) {

            // If optex is owned by this thread, own it again
            InterlockedIncrement(&m_psi->m_lLockCount);
            m_psi->m_lRecurseCount++;
            fThisThreadOwnsTheOptex = TRUE;
         }
      }

   } while (!fThisThreadOwnsTheOptex && (dwSpinCount-- > 0));

   // Return whether or not this thread owns the optex
   return(fThisThreadOwnsTheOptex);
}


///////////////////////////////////////////////////////////////////////////////


void COptex::Leave() {

#ifdef _DEBUG
   // Only the owning thread can leave the optex
   if (m_psi->m_dwThreadId != GetCurrentThreadId()) 
      DebugBreak();
#endif

   // Reduce this thread's ownership of the optex
   if (--m_psi->m_lRecurseCount > 0) {

      // We still own the optex
      InterlockedDecrement(&m_psi->m_lLockCount);

   } else {

      // We don't own the optex anymore
      m_psi->m_dwThreadId = 0;

      if (InterlockedDecrement(&m_psi->m_lLockCount) > 0) {

         // Other threads are waiting, the auto-reset event wakes one of them
         SetEvent(m_hevt);
      }
   }
}


//////////////////////////////// End of File //////////////////////////////////

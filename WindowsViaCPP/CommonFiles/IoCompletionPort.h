/******************************************************************************
Module:  IOCP.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
Purpose: This class wraps an I/O Completion Port.
         See Appendix B.
******************************************************************************/


#pragma once   // Include this header file once per compilation unit


///////////////////////////////////////////////////////////////////////////////


#include "..\CommonFiles\CmnHdr.h"              // See Appendix A.


///////////////////////////////////////////////////////////////////////////////


class CIOCP {
public:
   CIOCP(int nMaxConcurrency = -1) { 
      m_hIOCP = NULL; 
      if (nMaxConcurrency != -1)
         (void) Create(nMaxConcurrency);
   }

   ~CIOCP() { 
      if (m_hIOCP != NULL) 
         chVERIFY(CloseHandle(m_hIOCP)); 
   }


   BOOL Close() {
      BOOL bResult = CloseHandle(m_hIOCP);
      m_hIOCP = NULL;
      return(bResult);
   }

   BOOL Create(int nMaxConcurrency = 0) {
      m_hIOCP = CreateIoCompletionPort(
         INVALID_HANDLE_VALUE, NULL, 0, nMaxConcurrency);
      chASSERT(m_hIOCP != NULL);
      return(m_hIOCP != NULL);
   }

   BOOL AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey) {
      BOOL fOk = (CreateIoCompletionPort(hDevice, m_hIOCP, CompKey, 0) 
         == m_hIOCP);
      chASSERT(fOk);
      return(fOk);
   }

   BOOL AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey) {
      return(AssociateDevice((HANDLE) hSocket, CompKey));
   }

   BOOL PostStatus(ULONG_PTR CompKey, DWORD dwNumBytes = 0, 
      OVERLAPPED* po = NULL) {

      BOOL fOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
      chASSERT(fOk);
      return(fOk);
   }

   BOOL GetStatus(ULONG_PTR* pCompKey, PDWORD pdwNumBytes,
      OVERLAPPED** ppo, DWORD dwMilliseconds = INFINITE) {

      return(GetQueuedCompletionStatus(m_hIOCP, pdwNumBytes, 
         pCompKey, ppo, dwMilliseconds));
   }

private:
   HANDLE m_hIOCP;
};


///////////////////////////////// End of File /////////////////////////////////

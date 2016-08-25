/******************************************************************************
Module:  WaitForMultExp.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <malloc.h>
#include <process.h>
#include "WaitForMultExp.h"


///////////////////////////////////////////////////////////////////////////////


// Internal data structure representing a single expression.
// Used to tell OR-threads what objects to wait on.
typedef struct {
   PHANDLE m_phExpObjects;   // Points to set of handles
   DWORD   m_nExpObjects;    // Number of handles
} EXPRESSION, *PEXPRESSION;


///////////////////////////////////////////////////////////////////////////////


// The OR-thread function
DWORD WINAPI WFME_ThreadExpression(PVOID pvParam) {

   // This thread function just waits for an expression to come true. 
   // The thread waits in an alertable state so that it can be forced
   // to stop waiting by queuing an entry to its APC queue.
   PEXPRESSION pExpression = (PEXPRESSION) pvParam;
   return(WaitForMultipleObjectsEx(
      pExpression->m_nExpObjects, pExpression->m_phExpObjects, 
      TRUE, INFINITE, TRUE));
}


///////////////////////////////////////////////////////////////////////////////


// This is the APC callback routine function
VOID WINAPI WFME_ExpressionAPC(ULONG_PTR dwData) {

   // This function intentionally left blank
}


///////////////////////////////////////////////////////////////////////////////


// Function to wait on mutiple Boolean expressions
DWORD WINAPI WaitForMultipleExpressions(DWORD nExpObjects, 
   CONST HANDLE* phExpObjects, DWORD dwMilliseconds) {

   // Allocate a temporary array because we modify the passed array and
   // we need to add a handle at the end for the hsemOnlyOne semaphore.
   PHANDLE phExpObjectsTemp = (PHANDLE)
      _alloca(sizeof(HANDLE) * (nExpObjects + 1));
   CopyMemory(phExpObjectsTemp, phExpObjects, sizeof(HANDLE) * nExpObjects);
   phExpObjectsTemp[nExpObjects] = NULL;  // Put sentinel at end

   // Semaphore to guarantee that only one expression gets satisfied
   HANDLE hsemOnlyOne = CreateSemaphore(NULL, 1, 1, NULL);
   
   // Expression information: 1 per possible thread
   EXPRESSION Expression[MAXIMUM_WAIT_OBJECTS];

   DWORD dwExpNum  = 0;    // Current expression number
   DWORD dwNumExps = 0;    // Total number of expressions

   DWORD dwObjBegin = 0;   // First index of a set
   DWORD dwObjCur   = 0;   // Current index of object in a set

   DWORD dwThreadId, dwWaitRet = 0;

   // Array of thread handles for threads: 1 per expression
   HANDLE ahThreads[MAXIMUM_WAIT_OBJECTS];

   // Parse the callers handle list by initializing a structure for
   // each expression and adding hsemOnlyOne to each expression.
   while ((dwWaitRet != WAIT_FAILED) && (dwObjCur <= nExpObjects)) {

      // While no errors, and object handles are in the caller's list...

      // Find next expression (OR-expressions are separated by NULL handles)
      while (phExpObjectsTemp[dwObjCur] != NULL) 
         dwObjCur++;
      
      // Initialize Expression structure which an OR-thread waits on
      phExpObjectsTemp[dwObjCur] = hsemOnlyOne;
      Expression[dwNumExps].m_phExpObjects =  &phExpObjectsTemp[dwObjBegin];
      Expression[dwNumExps].m_nExpObjects  =  dwObjCur - dwObjBegin + 1;

      if (Expression[dwNumExps].m_nExpObjects > MAXIMUM_WAIT_OBJECTS) {
         // Error: Too many handles in single expression
         dwWaitRet = WAIT_FAILED;
         SetLastError(ERROR_SECRET_TOO_LONG);
      }

      // Advance to the next expression
      dwObjBegin = ++dwObjCur;
      if (++dwNumExps == MAXIMUM_WAIT_OBJECTS) {
         // Error: Too many expressions
         dwWaitRet = WAIT_FAILED;
         SetLastError(ERROR_TOO_MANY_SECRETS);
      }
   }

   if (dwWaitRet != WAIT_FAILED) {

      // No errors occurred while parsing the handle list

      // Spawn thread to wait on each expression
      for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++) {

         ahThreads[dwExpNum] = chBEGINTHREADEX(NULL, 
            1, // We only require a small stack
            WFME_ThreadExpression, &Expression[dwExpNum], 
            0, &dwThreadId);
      }

      // Wait for an expression to come TRUE or for a timeout
      dwWaitRet = WaitForMultipleObjects(dwExpNum, ahThreads, 
         FALSE, dwMilliseconds);

      if (WAIT_TIMEOUT == dwWaitRet) {

         // We timed-out, check if any expressions were satisfied by 
         // checking the state of the hsemOnlyOne semaphore.
         dwWaitRet = WaitForSingleObject(hsemOnlyOne, 0);

         if (WAIT_TIMEOUT == dwWaitRet) {

            // If the semaphore was not signaled, some thread expressions
            // was satisfied; we need to determine which expression.
            dwWaitRet = WaitForMultipleObjects(dwExpNum, 
               ahThreads, FALSE, INFINITE);

         } else {

            // No expression was satisfied and WaitForSingleObject just gave
            // us the semaphore so we know that no expression can ever be 
            // satisfied now -- waiting for an expression has timed-out.
            dwWaitRet = WAIT_TIMEOUT;
         }
      }

      // Break all the waiting expression threads out of their 
      // wait state so that they can terminate cleanly.
      for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++) {

         if ((WAIT_TIMEOUT == dwWaitRet) || 
             (dwExpNum != (dwWaitRet - WAIT_OBJECT_0))) {

            QueueUserAPC(WFME_ExpressionAPC, ahThreads[dwExpNum], 0);
         }
      }

#ifdef _DEBUG
      // In debug builds, wait for all of expression threads to terminate 
      // to make sure that we are forcing the threads to wake up. 
      // In non-debug builds, we'll assume that this works and 
      // not keep this thread waiting any longer.
      WaitForMultipleObjects(dwExpNum, ahThreads, TRUE, INFINITE);
#endif

      // Close our handles to all the expression threads
      for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++) {
         CloseHandle(ahThreads[dwExpNum]);
      }
   }  // error occurred while parsing

   CloseHandle(hsemOnlyOne);
   return(dwWaitRet);
}


//////////////////////////////// End of File //////////////////////////////////

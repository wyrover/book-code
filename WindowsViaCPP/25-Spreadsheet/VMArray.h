/******************************************************************************
Module:  VMArray.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


// NOTE: This C++ class is not thread safe. You cannot have multiple threads
// creating and destroying objects of this class at the same time. 

// However, once created, multiple threads can access different CVMArray 
// objects simultaneously and you can have multiple threads accessing a single
// CVMArray object if you manually synchronize access to the object yourself.


///////////////////////////////////////////////////////////////////////////////


template <class TYPE>
class CVMArray {
public:
   // Reserves sparse array of elements
   CVMArray(DWORD dwReserveElements);

   // Frees sparse array of elements
   virtual ~CVMArray();

   // Allows accessing an element in the array
   operator TYPE*()             { return(m_pArray); }
   operator const TYPE*() const { return(m_pArray); }

   // Can be called for fine-tuned handling if commit fails
   LONG ExceptionFilter(PEXCEPTION_POINTERS pep, 
      BOOL bRetryUntilSuccessful = FALSE);

protected:
   // Override this to fine-tune handling of access violation
   virtual LONG OnAccessViolation(PVOID pvAddrTouched, BOOL bAttemptedRead,
      PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful);

private:
   static CVMArray* sm_pHead;    // Address of first object
   CVMArray* m_pNext;            // Address of next  object

   TYPE* m_pArray;               // Pointer to reserved region array
   DWORD m_cbReserve;            // Size of reserved region array (in bytes)

private:
   // Address of previous unhandled exception filter
   static PTOP_LEVEL_EXCEPTION_FILTER sm_pfnUnhandledExceptionFilterPrev;

   // Our global unhandled exception filter for instances of this class
   static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pep);
};


///////////////////////////////////////////////////////////////////////////////


// The head of the linked-list of objects
template <class TYPE>
CVMArray<TYPE>* CVMArray<TYPE>::sm_pHead = NULL;

// Address of previous unhandled exception filter
template <class TYPE>
PTOP_LEVEL_EXCEPTION_FILTER CVMArray<TYPE>::sm_pfnUnhandledExceptionFilterPrev;

   
///////////////////////////////////////////////////////////////////////////////


template <class TYPE>
CVMArray<TYPE>::CVMArray(DWORD dwReserveElements) {

   if (sm_pHead == NULL) {
      // Install our global unhandled exception filter when
      // creating the first instance of the class.
      sm_pfnUnhandledExceptionFilterPrev = 
         SetUnhandledExceptionFilter(UnhandledExceptionFilter);
   }

   m_pNext = sm_pHead;  // The next node was at the head
   sm_pHead = this;     // This node is now at the head

   m_cbReserve = sizeof(TYPE) * dwReserveElements;

   // Reserve a region for the entire array
   m_pArray = (TYPE*) VirtualAlloc(NULL, m_cbReserve, 
      MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
   chASSERT(m_pArray != NULL);
}


///////////////////////////////////////////////////////////////////////////////


template <class TYPE>
CVMArray<TYPE>::~CVMArray() {

   // Free the array's region (decommitting all storage within it)
   VirtualFree(m_pArray, 0, MEM_RELEASE);

   // Remove this object from the linked list
   CVMArray* p = sm_pHead; 
   if (p == this) {     // Removing the head node
      sm_pHead = p->m_pNext; 
   } else {

      BOOL bFound = FALSE;

      // Walk list from head and fix pointers
      for (; !bFound && (p->m_pNext != NULL); p = p->m_pNext) {
         if (p->m_pNext == this) { 
            // Make the node that points to us point to the next node
            p->m_pNext = p->m_pNext->m_pNext; 
            break; 
         }
      }
      chASSERT(bFound);
   }
}


///////////////////////////////////////////////////////////////////////////////


// Default handling of access violations attempts to commit storage
template <class TYPE>
LONG CVMArray<TYPE>::OnAccessViolation(PVOID pvAddrTouched, 
   BOOL bAttemptedRead, PEXCEPTION_POINTERS pep, BOOL bRetryUntilSuccessful) {

   BOOL bCommittedStorage = FALSE;  // Assume committing storage fails

   do {
      // Attempt to commit storage
      bCommittedStorage = (NULL != VirtualAlloc(pvAddrTouched, 
         sizeof(TYPE), MEM_COMMIT, PAGE_READWRITE));

      // If storage could not be committed and we're supposed to keep trying
      // until we succeed, prompt user to free storage
      if (!bCommittedStorage && bRetryUntilSuccessful) {
         MessageBox(NULL, 
            TEXT("Please close some other applications and Press OK."), 
            TEXT("Insufficient Memory Available"), MB_ICONWARNING | MB_OK);
      }
   } while (!bCommittedStorage && bRetryUntilSuccessful);

   // If storage committed, try again. If not, execute the handler
   return(bCommittedStorage 
      ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER);
}


///////////////////////////////////////////////////////////////////////////////


// The filter associated with a single CVMArray object
template <class TYPE>
LONG CVMArray<TYPE>::ExceptionFilter(PEXCEPTION_POINTERS pep, 
   BOOL bRetryUntilSuccessful) {

   // Default to trying another filter (safest thing to do)
   LONG lDisposition = EXCEPTION_CONTINUE_SEARCH;

   // We only fix access violations
   if (pep->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
      return(lDisposition);

   // Get address of attempted access and get attempted read or write
   PVOID pvAddrTouched = (PVOID) pep->ExceptionRecord->ExceptionInformation[1];
   BOOL bAttemptedRead = (pep->ExceptionRecord->ExceptionInformation[0] == 0);

   // Is attempted access within this VMArray's reserved region?
   if ((m_pArray <= pvAddrTouched) && 
       (pvAddrTouched < ((PBYTE) m_pArray + m_cbReserve))) {

      // Access is in this array; try to fix the problem
      lDisposition = OnAccessViolation(pvAddrTouched, bAttemptedRead, 
         pep, bRetryUntilSuccessful);
   }

   return(lDisposition);
}


///////////////////////////////////////////////////////////////////////////////


// The filter associated with all CVMArray objects
template <class TYPE>
LONG WINAPI CVMArray<TYPE>::UnhandledExceptionFilter(PEXCEPTION_POINTERS pep) {

   // Default to trying another filter (safest thing to do)
   LONG lDisposition = EXCEPTION_CONTINUE_SEARCH;

   // We only fix access violations
   if (pep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {

      // Walk all the nodes in the linked-list
      for (CVMArray* p = sm_pHead; p != NULL; p = p->m_pNext) {

         // Ask this node if it can fix the problem.
         // NOTE: The problem MUST be fixed or the process will be terminated!
         lDisposition = p->ExceptionFilter(pep, TRUE);

         // If we found the node and it fixed the problem, stop the loop
         if (lDisposition != EXCEPTION_CONTINUE_SEARCH)
            break;
      }
   }

   // If no node fixed the problem, try the previous exception filter
   if (lDisposition == EXCEPTION_CONTINUE_SEARCH)
      lDisposition = sm_pfnUnhandledExceptionFilterPrev(pep);

   return(lDisposition);
}


//////////////////////////////// End of File //////////////////////////////////

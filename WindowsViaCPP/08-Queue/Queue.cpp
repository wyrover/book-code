/******************************************************************************
Module:  Queue.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <StrSafe.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


class CQueue {
public:
   struct ELEMENT {
      int   m_nThreadNum;
      int   m_nRequestNum;
      // Other element data should go here
   };
   typedef ELEMENT* PELEMENT;

private:
   struct INNER_ELEMENT {
      int      m_nStamp;  // 0 means empty
      ELEMENT  m_element;
   };
   typedef INNER_ELEMENT* PINNER_ELEMENT;

private:
   PINNER_ELEMENT m_pElements;      // Array of elements to be processed
   int            m_nMaxElements;   // Maximum # of elements in the array
   int            m_nCurrentStamp;  // Keep track of the # of added elements
   
private:
   int GetFreeSlot();
   int GetNextSlot(int nThreadNum);

public:
   CQueue(int nMaxElements);
   ~CQueue();
   BOOL IsFull();
   BOOL IsEmpty(int nThreadNum);
   void AddElement(ELEMENT e);
   BOOL GetNewElement(int nThreadNum, ELEMENT& e);
};


///////////////////////////////////////////////////////////////////////////////


CQueue::CQueue(int nMaxElements) {

   // Allocate and initialize the elements
   m_pElements = (PINNER_ELEMENT) 
      HeapAlloc(GetProcessHeap(), 0, sizeof(INNER_ELEMENT) * nMaxElements);
   ZeroMemory(m_pElements, sizeof(INNER_ELEMENT) * nMaxElements);

   // Initialize the element counter
   m_nCurrentStamp = 0;

   // Remember the max number of elements
   m_nMaxElements = nMaxElements;
}


CQueue::~CQueue() {

   HeapFree(GetProcessHeap(), 0, m_pElements);
}


BOOL CQueue::IsFull() {
   
   return(GetFreeSlot() == -1);
}


BOOL CQueue::IsEmpty(int nThreadNum) {

   return(GetNextSlot(nThreadNum) == -1);
}


int CQueue::GetFreeSlot() {

   // Look for the first element with a 0 stamp
   for(int current = 0; current < m_nMaxElements; current++) {
      if (m_pElements[current].m_nStamp == 0)
         return(current);
   }
   
   // No free slot was found
   return(-1);
}


int CQueue::GetNextSlot(int nThreadNum) {
   
   // By default, there is no slot for this thread
   int firstSlot = -1;
   
   // The element can't have a stamp higher than the last added
   int firstStamp = m_nCurrentStamp+1;
   
   // Look for the even (thread 0) / odd (thread 1) element that is not free 
   for(int current = 0; current < m_nMaxElements; current++) {
   
      // Keep track of the first added (lowest stamp) in the queue
      // --> so that "first in first out" behavior is ensured
      if ((m_pElements[current].m_nStamp != 0) &&  // free element
          ((m_pElements[current].m_element.m_nRequestNum % 2) == nThreadNum) &&
          (m_pElements[current].m_nStamp < firstStamp)) {

         firstStamp = m_pElements[current].m_nStamp;
         firstSlot = current;
      }
   }
   
   return(firstSlot);
}


void CQueue::AddElement(ELEMENT e) {

   // Do nothing if the queue is full
   int nFreeSlot = GetFreeSlot();
   if (nFreeSlot == -1)
      return;

   // Copy the content of the element
   m_pElements[nFreeSlot].m_element = e;

   // Mark the element with the new stamp
   m_pElements[nFreeSlot].m_nStamp = ++m_nCurrentStamp;
}


BOOL CQueue::GetNewElement(int nThreadNum, ELEMENT& e) {

   int nNewSlot = GetNextSlot(nThreadNum);
   if (nNewSlot == -1)
      return(FALSE);

   // Copy the content of the element
   e = m_pElements[nNewSlot].m_element;

   // Mark the element as read
   m_pElements[nNewSlot].m_nStamp = 0;

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


CQueue               g_q(10);    // The shared queue
volatile LONG        g_fShutdown;// Signals client/server threads to die
HWND                 g_hWnd;     // How client/server threads give status
SRWLOCK              g_srwLock;  // Reader/writer lock to protect the queue
CONDITION_VARIABLE   g_cvReadyToConsume;  // Signaled by writers
CONDITION_VARIABLE   g_cvReadyToProduce;  // Signaled by readers


// Handles to all reader/writer threads
HANDLE g_hThreads[MAXIMUM_WAIT_OBJECTS];

// Number of reader/writer threads  
int    g_nNumThreads = 0;


///////////////////////////////////////////////////////////////////////////////


void AddText(HWND hWndLB, PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];
   _vstprintf_s(sz, _countof(sz), pszFormat, argList);
   ListBox_SetCurSel(hWndLB, ListBox_AddString(hWndLB, sz));
   
   va_end(argList);
}


BOOL ConsumeElement(int nThreadNum, int nRequestNum, HWND hWndLB) {

   // Get access to the queue to consume a new element
   AcquireSRWLockShared(&g_srwLock); 

   // Fall asleep until there is something to read.
   // Check if, while it was asleep, 
   // it was not decided that the thread should stop
   while (g_q.IsEmpty(nThreadNum) && !g_fShutdown) {
      // There was not a readable element
      AddText(hWndLB, TEXT("[%d] Nothing to process"), nThreadNum);
         
      // The queue is empty
      // --> Wait until a writer adds a new element to read
      //     and come back with the lock acquired in shared mode
      SleepConditionVariableSRW(&g_cvReadyToConsume, &g_srwLock, 
         INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
   }

   // When thread is exiting, the lock should be released for writer
   // and readers should be signaled through the condition variable
   if (g_fShutdown) {
      // Show that the current thread is exiting
      AddText(hWndLB, TEXT("[%d] bye bye"), nThreadNum);

      // Another writer thread might still be blocked on the lock
      // --> release it before exiting
      ReleaseSRWLockShared(&g_srwLock);

      // Notify other readers that it is time to exit
      // --> release readers
      WakeConditionVariable(&g_cvReadyToConsume);

      return(FALSE);
   }

   // Get the first new element
   CQueue::ELEMENT e;
   // Note: No need to test the return value since IsEmpty
   //       returned FALSE
   g_q.GetNewElement(nThreadNum, e);
      
   // No need to keep the lock any longer
   ReleaseSRWLockShared(&g_srwLock);

   // Show result of consuming the element
   AddText(hWndLB, TEXT("[%d] Processing %d:%d"), 
      nThreadNum, e.m_nThreadNum, e.m_nRequestNum);

   // A free slot is now available for writer threads to produce
   // --> wake up a writer thread
   WakeConditionVariable(&g_cvReadyToProduce);

   return(TRUE);
}


DWORD WINAPI ReaderThread(PVOID pvParam) {

   int nThreadNum = PtrToUlong(pvParam);
   HWND hWndLB = GetDlgItem(g_hWnd, IDC_SERVERS);

   for (int nRequestNum = 1; !g_fShutdown; nRequestNum++) {

      if (!ConsumeElement(nThreadNum, nRequestNum, hWndLB))
         return(0);

      Sleep(2500);   // Wait before reading another element
   }
   
   // g_fShutdown has been set during Sleep
   // --> Show that the current thread is exiting
   AddText(hWndLB, TEXT("[%d] bye bye"), nThreadNum);

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI WriterThread(PVOID pvParam) {

   int nThreadNum = PtrToUlong(pvParam);
   HWND hWndLB = GetDlgItem(g_hWnd, IDC_CLIENTS);

   for (int nRequestNum = 1; !g_fShutdown; nRequestNum++) {

      CQueue::ELEMENT e = { nThreadNum, nRequestNum };

      // Require access for writing
      AcquireSRWLockExclusive(&g_srwLock);

      // If the queue is full, fall asleep as long as the condition variable 
      // is not signaled
      // Note: During the wait for acquiring the lock, 
      //       a stop might have been received
      if (g_q.IsFull() & !g_fShutdown) {
         // No more room in the queue
         AddText(hWndLB, TEXT("[%d] Queue is full: impossible to add %d"), 
            nThreadNum, nRequestNum);

         // --> Need to wait for a reader to empty a slot before acquiring 
         //     the lock again 
         SleepConditionVariableSRW(&g_cvReadyToProduce, &g_srwLock, 
            INFINITE, 0);
      }

      // Other writer threads might still be blocked on the lock
      // --> Release the lock and notify the remaining writer threads to quit
      if (g_fShutdown) {
         // Show that the current thread is exiting
         AddText(hWndLB, TEXT("[%d] bye bye"), nThreadNum);

         // No need to keep the lock any longer
         ReleaseSRWLockExclusive(&g_srwLock);

         // Signal other blocked writers threads that it is time to exit
         WakeAllConditionVariable(&g_cvReadyToProduce);

         // Bye bye
         return(0);
      } else {
         // Add the new ELEMENT into the queue
         g_q.AddElement(e);

         // Show result of processing element
         AddText(hWndLB, TEXT("[%d] Adding %d"), nThreadNum, nRequestNum);

         // No need to keep the lock any longer
         ReleaseSRWLockExclusive(&g_srwLock);

         // Signal reader threads that there is an element to consume
         WakeAllConditionVariable(&g_cvReadyToConsume);

         // Wait before adding a new element
         Sleep(1500);
      }
   }

   // Show that the current thread is exiting
   AddText(hWndLB, TEXT("[%d] bye bye"), nThreadNum);

   return(0);
}



///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_QUEUE);

   g_hWnd = hWnd; // Used by client/server threads to show status

   // Prepare the SRWLock to be used
   InitializeSRWLock(&g_srwLock);

   // Prepare the condition variables to be used
   InitializeConditionVariable(&g_cvReadyToConsume);
   InitializeConditionVariable(&g_cvReadyToProduce);

   // Will be set to TRUE in order to end threads
   g_fShutdown = FALSE;

   // Create the writer threads
   DWORD dwThreadID;
   for (int x = 0; x < 4; x++)
      g_hThreads[g_nNumThreads++] = 
         chBEGINTHREADEX(NULL, 0, WriterThread, (PVOID) (INT_PTR) x, 
            0, &dwThreadID);

   // Create the reader threads
   for (int x = 0; x < 2; x++)
      g_hThreads[g_nNumThreads++] = 
         chBEGINTHREADEX(NULL, 0, ReaderThread, (PVOID) (INT_PTR) x, 
            0, &dwThreadID);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void StopProcessing() {

   if (!g_fShutdown) {
      // Ask all threads to end
      InterlockedExchange(&g_fShutdown, TRUE);

      // Free all threads waiting on condition variables
      WakeAllConditionVariable(&g_cvReadyToConsume);
      WakeAllConditionVariable(&g_cvReadyToProduce);

      // Wait for all the threads to terminate & then clean up
      WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);

      // Don't forget to clean up kernel resources
      // Note: This is not really mandatory since the process is exiting
      while (g_nNumThreads--)
         CloseHandle(g_hThreads[g_nNumThreads]);

      // Close each list box
      AddText(GetDlgItem(g_hWnd, IDC_SERVERS), TEXT("---------------------"));
      AddText(GetDlgItem(g_hWnd, IDC_CLIENTS), TEXT("---------------------"));
   }
}


DWORD WINAPI StoppingThread(PVOID pvParam) {

   StopProcessing();
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;

      case IDC_BTN_STOP: 
      {
         // StopProcessing can't be called from the UI thread
         // or a deadlock will occur: SendMessage() is used 
         // to fill up the listboxes
         // --> Another thread is required
         DWORD dwThreadID;
         CloseHandle(chBEGINTHREADEX(NULL, 0, StoppingThread, 
            NULL, 0, &dwThreadID));
         
         // This button can't be pushed twice
         Button_Enable(hWndCtl, FALSE);
      }
      break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_QUEUE), NULL, Dlg_Proc);
   StopProcessing();
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
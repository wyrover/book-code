#include "stdafx.h"
#include "WorkQueue.h"
#include <assert.h>

typedef struct _THREAD_CONTEXT {
    CWorkQueue* pWorkQueue;
    void*       pThreadData;
} THREAD_CONTEXT, *PTHREAD_CONTEXT;

/*------------------------------------------------------------------------
Create

  Initialize the work queue mechanism

  const unsigned int  nNumberOfThreads      - Number of threads to create in the work queue,
  void*         *ThreadData                 - This is data for each thread that will be passed
  to the DoWork function for each WorkItem, this  can be used to give each thread a work area
  to increase epitomization of allocations and free.

------------------------------------------------------------------------*/

bool CWorkQueue::Create(const unsigned int  nNumberOfThreads,
                        void *         *ThreadData      /*=NULL*/)
{
    //init work items list
    m_pWorkItemQueue = new WorkItemQueue();

    if (NULL == m_pWorkItemQueue) {
        return false;
    }

    //create semaphore
    m_phSincObjectsArray[SEMAPHORE_INDEX] = CreateSemaphore(NULL, 0, LONG_MAX, NULL);

    if (m_phSincObjectsArray[SEMAPHORE_INDEX] == NULL) {
        delete m_pWorkItemQueue;
        m_pWorkItemQueue = NULL;
        return false;
    }

    //create abort event
    m_phSincObjectsArray[ABORT_EVENT_INDEX] = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (m_phSincObjectsArray[ABORT_EVENT_INDEX]  == NULL) {
        delete m_pWorkItemQueue;
        m_pWorkItemQueue = NULL;
        CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
        return false;
    }

    //create  critical section garding work item list
    InitializeCriticalSection(&m_CriticalSection);
    //allocate thread handle array
    m_phThreads = new HANDLE[nNumberOfThreads];

    if (m_phThreads == NULL) {
        delete m_pWorkItemQueue;
        m_pWorkItemQueue = NULL;
        CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
        CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
        DeleteCriticalSection(&m_CriticalSection);
        return false;
    }

    unsigned int i;
    m_nNumberOfThreads = nNumberOfThreads;
    DWORD dwThreadId;
    PTHREAD_CONTEXT pThreadsContext ;

    //create all threads
    for (i = 0 ; i < nNumberOfThreads ; i++) {
        //init each thread context
        pThreadsContext = new THREAD_CONTEXT;
        pThreadsContext->pWorkQueue  = this;
        pThreadsContext->pThreadData = ThreadData == NULL ? NULL : ThreadData[i];
        //create thread
        m_phThreads[i] = CreateThread(NULL,
                                      0,
                                      CWorkQueue::ThreadFunc,
                                      pThreadsContext,
                                      0,
                                      &dwThreadId);

        if (m_phThreads[i] == NULL) {
            delete pThreadsContext;
            m_nNumberOfThreads = i;
            Destroy();
            return false;
        }
    }

    return true;
}


/*------------------------------------------------------------------------
InsertWorkItem

  Insert the work Item to the queue for precessing
------------------------------------------------------------------------*/
bool CWorkQueue::InsertWorkItem(WorkItemBase* pWorkItem)
{
    assert(pWorkItem != NULL);
    //lock
    EnterCriticalSection(&m_CriticalSection);
    //insert the item to queue
    m_pWorkItemQueue->push(pWorkItem);
    //lock
    LeaveCriticalSection(&m_CriticalSection);

    //signal semaphore
    if (!ReleaseSemaphore(m_phSincObjectsArray[SEMAPHORE_INDEX], 1, NULL)) {
        assert(false);
        return false;
    }

    return true;
}

/*------------------------------------------------------------------------
RemoveWorkItem()

  extract the net item from the queue and return it
------------------------------------------------------------------------*/
WorkItemBase*  CWorkQueue::RemoveWorkItem()
{
    WorkItemBase* pWorkItem;
    //lock
    EnterCriticalSection(&m_CriticalSection);
    //remove the item to queue
    pWorkItem = m_pWorkItemQueue->front();
    m_pWorkItemQueue->pop();
    //release lock
    LeaveCriticalSection(&m_CriticalSection);
    assert(pWorkItem != NULL);
    return pWorkItem;
}
/*------------------------------------------------------------------------
ThreadFunc

  This is the thread function that is waiting on the
  arrival of work item to process them, or to an abort event
  in case the work queue is destroied
------------------------------------------------------------------------*/
unsigned long __stdcall CWorkQueue::ThreadFunc(void*  pParam)
{
    PTHREAD_CONTEXT       pThreadContext = (PTHREAD_CONTEXT)pParam;
    WorkItemBase*         pWorkItem      = NULL;
    CWorkQueue*           pWorkQueue     = pThreadContext->pWorkQueue;
    void*                 pThreadData    = pThreadContext->pThreadData;
    DWORD dwWaitResult;

    for (;;) {
        //wait for one of the two
        // 1. Abort Event -
        dwWaitResult = WaitForMultipleObjects(NUMBER_OF_SYNC_OBJ, pWorkQueue->m_phSincObjectsArray, FALSE, INFINITE);

        switch (dwWaitResult - WAIT_OBJECT_0) {
        case ABORT_EVENT_INDEX:
            //Abort Event was set
            //delete the context of this thread
            delete pThreadContext;
            return 0;

        case SEMAPHORE_INDEX:
            //take the alerted first work item from queue
            pWorkItem = pWorkQueue->RemoveWorkItem();

            if (pWorkItem == NULL) {
                assert(false);
                break;
            }

            //do all that needed with the work item
            pWorkItem->DoWork(pThreadData);
            break;

        default:
            assert(false);
            delete pThreadContext;
            return 0;
        }
    }

    //delete the context of this thread
    delete pThreadContext;
    return 1;
}


/*------------------------------------------------------------------------
Destroy

  set the abort event and wait for all threads to finish
  then cleans up the module
------------------------------------------------------------------------*/
void CWorkQueue::Destroy()
{
//set the abort event
    if (!SetEvent(m_phSincObjectsArray[ABORT_EVENT_INDEX])) {
        assert(false);
        return;
    }

    //wait for all the threads to end
    WaitForMultipleObjects(m_nNumberOfThreads, m_phThreads, true, INFINITE);

    //clean queue
    while (!m_pWorkItemQueue->empty()) {
        m_pWorkItemQueue->front()->Abort();
        m_pWorkItemQueue->pop();
    }

    delete m_pWorkItemQueue;
    m_pWorkItemQueue = NULL;
    CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
    CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
    DeleteCriticalSection(&m_CriticalSection);

    //close all threads handles
    for (int i = 0 ; i < m_nNumberOfThreads ; i++)
        CloseHandle(m_phThreads[i]);

    delete[] m_phThreads;
}
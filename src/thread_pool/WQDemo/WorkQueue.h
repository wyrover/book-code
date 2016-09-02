#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H
#include <queue>

class CWorkQueue;


/*------------------------------------------------------------------------
WorkItemBase

this is the basic WorkItem that the Work Queue Use its interface
This class should be inherited and these virtual abstract functions
implemented.

  DoWork()

  virtual abstract function is the function that is called when the
  work item turn has came to be poped out of the queue and be processed.


  Abort ()

  This function is called, when the Destroy function is called, for each of the WorkItems
  That are left in the queue.


------------------------------------------------------------------------*/
class WorkItemBase
{
    virtual void   DoWork(void* pThreadContext)    = 0;
    virtual void   Abort() = 0;
    friend CWorkQueue;
};


typedef std::queue<WorkItemBase*>           WorkItemQueue, *PWorkItemQueue;


/*------------------------------------------------------------------------
CWorkQueue

  This is the WorkOueue class also known as thread pool,
  the basic idea of this class is creating thread that are waiting on a queue
  of work item when the queue is inserted with items the threads wake up and
  perform the requered work and go to sleep again.
------------------------------------------------------------------------*/

class  CWorkQueue
{
public:

    virtual ~CWorkQueue() {};

    bool Create(const unsigned int       nNumberOfThreads,
                void *                    *pThreadDataArray             = NULL);

    bool InsertWorkItem(WorkItemBase* pWorkItem);

    void Destroy();

private:

    static unsigned long __stdcall ThreadFunc(void* pParam);
    WorkItemBase* RemoveWorkItem();



    enum {
        ABORT_EVENT_INDEX = 0,
        SEMAPHORE_INDEX,
        NUMBER_OF_SYNC_OBJ,
    };

    PHANDLE                  m_phThreads;
    unsigned int             m_nNumberOfThreads;
    void*                    m_pThreadDataArray;

    HANDLE                   m_phSincObjectsArray[NUMBER_OF_SYNC_OBJ];

    CRITICAL_SECTION         m_CriticalSection;
    PWorkItemQueue           m_pWorkItemQueue;
};




#endif // WORK_QUEUE_H

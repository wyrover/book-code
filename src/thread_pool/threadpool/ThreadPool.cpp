// ThreadPool.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadPool2.h"
#include "ThreadPool.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadPool::CThreadPool()
{
}

CThreadPool::~CThreadPool()
{
}

void CThreadPool::Start(unsigned short nStatic, unsigned short nMax)
{
    ASSERT(nMax >= nStatic);
    m_nNumberOfStaticThreads = nStatic;
    m_nNumberOfTotalThreads = nMax;
    //lock the resource
    CSingleLock singleLock(&m_arrayCs);
    singleLock.Lock();  // Attempt to lock the shared resource
    //create an IO port
    m_hMgrIoPort = CreateIoCompletionPort((HANDLE)INVALID_HANDLE_VALUE, NULL, 0, 0);
    CWinThread* pWinThread = AfxBeginThread((AFX_THREADPROC)ManagerProc, this);
    m_hMgrThread = pWinThread->m_hThread;
    //now we start these worker threads
    m_hWorkerIoPort = CreateIoCompletionPort((HANDLE)INVALID_HANDLE_VALUE, NULL, 0, 0);

    for (long n = 0; n < nStatic; n++) {
        CWinThread* pWinThread = AfxBeginThread((AFX_THREADPROC)WorkerProc, this);
        HANDLE handle = pWinThread->m_hThread;
        DWORD threadId = pWinThread->m_nThreadID;
        TRACE1("generate a worker thread handle id is %d.\n", threadId);
        m_threadMap.SetAt(threadId, ThreadInfo(handle, false));
    }

    singleLock.Unlock();
}

void CThreadPool::Stop(bool bHash)
{
    CSingleLock singleLock(&m_arrayCs);
    singleLock.Lock();  // Attempt to lock the shared resource
    ::PostQueuedCompletionStatus(m_hMgrIoPort, 0, 0, (OVERLAPPED*)0xFFFFFFFF);
    WaitForSingleObject(m_hMgrThread, INFINITE);
    CloseHandle(m_hMgrIoPort);
    //shut down all the worker threads
    UINT nCount = m_threadMap.GetCount();
    HANDLE* pThread = new HANDLE[nCount];
    long n = 0;
    POSITION pos = m_threadMap.GetStartPosition();
    DWORD threadId;
    ThreadInfo info;

    while (pos != NULL) {
        ::PostQueuedCompletionStatus(m_hWorkerIoPort, 0, 0, (OVERLAPPED*)0xFFFFFFFF);
        m_threadMap.GetNextAssoc(pos, threadId, info);
        pThread[n++] = info.m_hThread;
    }

    singleLock.Unlock();
    DWORD rc = WaitForMultipleObjects(nCount, pThread, TRUE, 120000); //wait for 2 minutes, then start to kill threads
    CloseHandle(m_hWorkerIoPort);

    if (rc == WAIT_TIMEOUT && bHash) {
        //some threads not terminated, we have to stop them.
        DWORD exitCode;

        for (int i = 0; i < nCount; i++)
            if (::GetExitCodeThread(pThread[i], &exitCode) == STILL_ACTIVE)
                TerminateThread(pThread[i], 99);
    }

    delete[] pThread;
}

unsigned int CThreadPool::ManagerProc(void* p)
{
    //convert the parameter to the server pointer.
    CThreadPool* pServer = (CThreadPool*)p;
    HANDLE                              IoPort  = pServer->GetMgrIoPort();
    unsigned long                       pN1, pN2;
    OVERLAPPED*                         pOverLapped;
LABEL_MANAGER_PROCESSING:

    while (::GetQueuedCompletionStatus(IoPort, &pN1, &pN2,
                                       &pOverLapped, pServer->GetMgrWaitTime())) {
        if (pOverLapped == (OVERLAPPED*)0xFFFFFFFF)
            break;
        else {
            TRACE0("mgr events comes in!\n");
        }
    }

    //time out processing
    if (::GetLastError() == WAIT_TIMEOUT) {
        //time out processing
        TRACE0("Time out processing!\n");

        //the manager will take a look at all the worker's status. The
        if (pServer->GetThreadPoolStatus() == CThreadPool::BUSY)
            pServer->AddThreads();

        if (pServer->GetThreadPoolStatus() == CThreadPool::IDLE)
            pServer->RemoveThreads();

        goto LABEL_MANAGER_PROCESSING;
    }

    return 0;
}

unsigned int CThreadPool::WorkerProc(void* p)
{
    //convert the parameter to the server pointer.
    CThreadPool* pServer = (CThreadPool*)p;
    HANDLE                              IoPort  = pServer->GetWorkerIoPort();
    unsigned long                       pN1, pN2;
    OVERLAPPED*                         pOverLapped;
    DWORD threadId =::GetCurrentThreadId();
    TRACE1("worker thread id is %d.\n", threadId);

    while (::GetQueuedCompletionStatus(IoPort, &pN1, &pN2,
                                       &pOverLapped, INFINITE)) {
        if (pOverLapped == (OVERLAPPED*)0xFFFFFFFE) {
            pServer->RemoveThread(threadId);
            break;
        } else if (pOverLapped == (OVERLAPPED*)0xFFFFFFFF) {
            break;
        } else {
            TRACE0("worker events comes in!\n");
            //before processing, we need to change the status to busy.
            pServer->ChangeStatus(threadId, true);
            //retrieve the job description and agent pointer
            IWorker* pIWorker = reinterpret_cast<IWorker*>(pN1);
            IJobDesc* pIJob = reinterpret_cast<IJobDesc*>(pN2);
            pIWorker->ProcessJob(pIJob);
            delete pIJob;
            pServer->ChangeStatus(threadId, false);
        }
    }

    return 0;
}

void CThreadPool::ChangeStatus(DWORD threadId, bool status)
{
    CSingleLock singleLock(&m_arrayCs);
    singleLock.Lock();  // Attempt to lock the shared resource
    //retrieve the current thread handle
    ThreadInfo info;
    m_threadMap.Lookup(threadId, info);
    info.m_bBusyWorking = status;
    m_threadMap.SetAt(threadId, info);
    singleLock.Unlock();
}

void CThreadPool::ProcessJob(IJobDesc* pJob, IWorker* pWorker) const
{
    ::PostQueuedCompletionStatus(m_hWorkerIoPort, \
                                 reinterpret_cast<DWORD>(pWorker), \
                                 reinterpret_cast<DWORD>(pJob), \
                                 NULL);
}

void CThreadPool::AddThreads()
{
    int nCount = m_threadMap.GetCount();
    int nTotal = min(nCount + 2, m_nNumberOfTotalThreads);

    for (int i = 0; i < nTotal - nCount; i++) {
        CWinThread* pWinThread = AfxBeginThread((AFX_THREADPROC)WorkerProc, this);
        HANDLE handle = pWinThread->m_hThread;
        DWORD threadId = pWinThread->m_nThreadID;
        m_threadMap.SetAt(threadId, ThreadInfo(handle, false));
    }
}

void CThreadPool::RemoveThreads()
{
    int nCount = m_threadMap.GetCount();
    int nTotal = max(nCount - 2, m_nNumberOfStaticThreads);

    for (int i = 0; i < nCount - nTotal; i++)
        ::PostQueuedCompletionStatus(m_hWorkerIoPort, 0, 0, (OVERLAPPED*)0xFFFFFFFE);
}

CThreadPool::ThreadPoolStatus CThreadPool::GetThreadPoolStatus()
{
    int nTotal = m_threadMap.GetCount();
    POSITION pos = m_threadMap.GetStartPosition();
    DWORD threadId;
    ThreadInfo info;
    int nCount = 0;

    while (pos != NULL) {
        m_threadMap.GetNextAssoc(pos, threadId, info);

        if (info.m_bBusyWorking == true) nCount++;
    }

    if (nCount / (1.0 * nTotal) > 0.8)
        return BUSY;

    if (nCount / (1.0 * nTotal) < 0.2)
        return IDLE;

    return NORMAL;
}
#ifndef __THREADPOOL__
#define __THREADPOOL__

#include "main.h"
#include "CList.h"

class CThreadPool
{
public:
    CThreadPool();
    CThreadPool(unsigned uMaxThreads) : dwMaxCount(uMaxThreads), threadList(new CList()) { }
    ~CThreadPool()
    {
        RemoveAll();
    }
    DWORD Count()
    {
        return threadList->Count();
    }
    void RemoveThread(DWORD dwThreadId);
    void WaitAll();
    void RemoveAll();
    DWORD& MaxCount()
    {
        return dwMaxCount;
    }
    void ReleaseThread(DWORD dwThreadId);
    CThread* RequestThread(LPTHREAD_START_ROUTINE threadStart);
protected:
    CThread* Find(DWORD dwThreadId);
private:
    CList* threadList;
    DWORD dwMaxCount;
};

CThreadPool::CThreadPool()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    dwMaxCount = sysInfo.dwNumberOfProcessors;
    threadList = new CList();
}

void CThreadPool::RemoveThread(DWORD dwThreadId)
{
    CThread* thread = Find(dwThreadId);

    if (thread) {
        delete thread;
    }
}

void CThreadPool::WaitAll()
{
    HANDLE* hThreads = new HANDLE[threadList->Count()];
    CThread* thread = 0;

    for (unsigned uIndex = 0; uIndex < threadList->Count(); uIndex++) {
        thread = threadList->GetNext(thread);
        hThreads[uIndex] = thread->Handle();
    }

    WaitForMultipleObjects(threadList->Count(), hThreads, TRUE, INFINITE);
    delete[] hThreads;
}

void CThreadPool::RemoveAll()
{
    CThread* thread = 0;

    while (thread = threadList->GetNext(thread)) {
        threadList->Remove(thread->ThreadId());
        thread = 0;
    }

    delete threadList;
}

void CThreadPool::ReleaseThread(DWORD dwThreadId)
{
    CThread* thread = Find(dwThreadId);

    if (thread != NULL) {
        TerminateThread(thread->Handle(), (DWORD) - 1);
    }
}

CThread* CThreadPool::RequestThread(LPTHREAD_START_ROUTINE lpThreadStart)
{
    CThread* thread = NULL;

    if (Count() < MaxCount()) {
        threadList->Insert(threadList->Head(), thread = new CThread(lpThreadStart));
        return thread;
    }

    while (thread = threadList->Iterate(thread)) {
        if (!thread->StillAlive()) {
            break;
        }

        Sleep(100);
    }

    if (thread == NULL) {
        thread = threadList->GetFirst();
    }

    threadList->Remove(thread->ThreadId());
    threadList->Insert(threadList->Head(), thread = new CThread(lpThreadStart));
    return thread;
}

CThread* CThreadPool::Find(DWORD dwThreadId)
{
    CThread* thread = NULL;

    while (thread = threadList->GetNext(thread)) {
        if (thread->ThreadId() == dwThreadId) {
            return thread;
        }
    }

    return NULL;
}

#endif

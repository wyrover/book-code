#ifndef __THREADPOOL__
#define __THREADPOOL__

#include "main.h"
#include "CList.h"
#include <Wct.h>

#pragma comment (lib, "Advapi32.lib")

class CThreadPool
{
public:
    CThreadPool();
    CThreadPool(unsigned uMaxThreads);
    ~CThreadPool();
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
    void InitializePool();
    static DWORD WINAPI ProblemSolver(LPVOID lpParam);
    void ClearMessageQueue()
    {
        MSG msg;

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
    }
    HANDLE hProblemSolver;
    HANDLE hEvent;
    CList* threadList;
    DWORD dwMaxCount;
};

CThreadPool::CThreadPool()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    dwMaxCount = sysInfo.dwNumberOfProcessors;
    InitializePool();
}

CThreadPool::CThreadPool(unsigned uMaxThreads)
{
    dwMaxCount = uMaxThreads;
    InitializePool();
}

CThreadPool::~CThreadPool()
{
    SetEvent(hEvent);

    if (WaitForSingleObject(hProblemSolver, MAX_WAIT_TIME) != WAIT_OBJECT_0) {
        TerminateThread(hProblemSolver, (DWORD) - 1);
    }

    CloseHandle(hProblemSolver);
    CloseHandle(hEvent);
    RemoveAll();
    ClearMessageQueue();
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

CThread* CThreadPool::RequestThread(LPTHREAD_START_ROUTINE threadStart)
{
    CThread* thread = NULL;

    if (Count() < MaxCount()) {
        threadList->Insert(threadList->Head(), thread = new CThread(threadStart, GetCurrentThreadId()));
        return thread;
    }

    while (thread = threadList->GetNext(thread)) {
        if (!thread->StillAlive()) {
            break;
        }
    }

    if (thread == NULL) {
        MSG msg = { 0 };

        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            thread = Find((DWORD)msg.wParam);

            if (thread) {
                break;
            }
        }
    }

    threadList->Remove(thread->ThreadId());
    threadList->Insert(threadList->Head(), thread = new CThread(threadStart, GetCurrentThreadId()));
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

void CThreadPool::InitializePool()
{
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    hEvent = CreateEvent(NULL, TRUE, FALSE, _T("__prbslv_1342__"));
    hProblemSolver = CreateThread(NULL, 0, ProblemSolver, this, 0, NULL);
    threadList = new CList();
}

DWORD WINAPI CThreadPool::ProblemSolver(LPVOID lpParam)
{
    CThreadPool* pool = (CThreadPool*)lpParam;
    DWORD dwWaitStatus = 0;

    while (true) {
        dwWaitStatus = WaitForSingleObject(pool->hEvent, 10);

        if (dwWaitStatus == WAIT_OBJECT_0) {
            break;
        }

        CThread* thread = NULL;

        while (thread = pool->threadList->GetNext(thread)) {
            if (thread->StillAlive()) {
                HWCT hWct = NULL;
                DWORD dwNodeCount = WCT_MAX_NODE_COUNT;
                BOOL bDeadlock = FALSE;
                WAITCHAIN_NODE_INFO NodeInfoArray[WCT_MAX_NODE_COUNT];
                hWct = OpenThreadWaitChainSession(0, NULL);

                if (GetThreadWaitChain(hWct, NULL, WCTP_GETINFO_ALL_FLAGS, thread->ContextThreadId(), &dwNodeCount, NodeInfoArray, &bDeadlock)) {
                    if (bDeadlock) {
                        if (TerminateThread(thread->ContextHandle(), (DWORD) - 1)) {
                            _tprintf_s(_T("Error! Deadlock found!\nThread:\t[%u] terminated!\n\n"), thread->ContextThreadId());
                        }
                    }
                }

                CloseThreadWaitChainSession(hWct);
            }
        }

        Sleep(1000);
    }

    return 0L;
}

#endif

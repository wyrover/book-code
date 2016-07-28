#ifndef __THREAD__
#define __THREAD__

#include "main.h"

class CThread
{
public:
    CThread(LPTHREAD_START_ROUTINE lpThreadStart, DWORD dwDispatcherId);
    ~CThread();
    void Start(LPVOID lpContext = 0)
    {
        pThreadContext->lpParameter = lpContext;
        ResumeThread(hThread);
    }
    bool StillAlive()
    {
        return ExitStatus() == STILL_ACTIVE;
    }
    HANDLE Handle() const
    {
        return hThread;
    }
    HANDLE ContextHandle() const
    {
        return pThreadContext->hThread;
    }
    DWORD ThreadId() const
    {
        return dwThreadId;
    }
    DWORD ContextThreadId() const
    {
        return pThreadContext->dwThreadId;
    }
    DWORD ExitStatus()
    {
        GetExitCodeThread(hThread, &dwExitCode);
        return dwExitCode;
    }
    CThread* SetMaxWaitTime(DWORD dwMilliseconds)
    {
        pThreadContext->dwMaxWaitTime = dwMilliseconds;
        return this;
    }
protected:
    static DWORD WINAPI StartAddress(LPVOID lpParam);
private:
    typedef struct {
        LPTHREAD_START_ROUTINE lpThreadStart;
        LPVOID lpParameter;
        HANDLE hThread;
        DWORD dwThreadId;
        DWORD dwDispatcherId;
        DWORD dwMaxWaitTime;
    } STARTCONTEXT, *PSTARTCONTEXT;
    HANDLE hThread;
    DWORD dwThreadId;
    DWORD dwExitCode;
    PSTARTCONTEXT pThreadContext;
};

CThread::CThread(LPTHREAD_START_ROUTINE lpThreadStart, DWORD dwDispatcherId) : dwExitCode(0)
{
    pThreadContext = new STARTCONTEXT();
    pThreadContext->lpThreadStart = lpThreadStart;
    pThreadContext->dwDispatcherId = dwDispatcherId;
    pThreadContext->dwMaxWaitTime = MAX_WAIT_TIME;
    hThread = CreateThread(NULL, 0, StartAddress, this, CREATE_SUSPENDED, &dwThreadId);
}

CThread::~CThread()
{
    delete pThreadContext;

    if (StillAlive()) {
        TerminateThread(hThread, (DWORD) - 1);
    }

    CloseHandle(hThread);
}

DWORD WINAPI CThread::StartAddress(LPVOID lpParam)
{
    CThread* thread = (CThread*)lpParam;
    thread->pThreadContext->hThread = CreateThread(NULL, 0, thread->pThreadContext->lpThreadStart, thread->pThreadContext->lpParameter, 0, &thread->pThreadContext->dwThreadId);
    DWORD dwStatus = WaitForSingleObject(thread->pThreadContext->hThread, thread->pThreadContext->dwMaxWaitTime);
    CloseHandle(thread->pThreadContext->hThread);
    PostThreadMessage(thread->pThreadContext->dwDispatcherId, MSG_ENDTASK, (WPARAM)thread->dwThreadId, 0);
    return 0L;
}

#endif

#ifndef __THREAD__
#define __THREAD__

#include "main.h"

class CThread
{
public:
    CThread(LPTHREAD_START_ROUTINE lpThreadStart) : dwExitCode(0), lpThreadStart(lpThreadStart) { }
    ~CThread();
    void Start(LPVOID lpContext = 0);
    bool StillAlive()
    {
        return ExitStatus() == STILL_ACTIVE;
    }
    HANDLE Handle() const
    {
        return hThread;
    }
    DWORD ThreadId() const
    {
        return dwThreadId;
    }
    DWORD ExitStatus()
    {
        GetExitCodeThread(hThread, &dwExitCode);
        return dwExitCode;
    }
private:
    LPTHREAD_START_ROUTINE lpThreadStart;
    HANDLE hThread;
    DWORD dwThreadId;
    DWORD dwExitCode;
};

CThread::~CThread()
{
    if (StillAlive()) {
        TerminateThread(hThread, (DWORD) - 1);
    }

    CloseHandle(hThread);
}

void CThread::Start(LPVOID lpContext)
{
    hThread = CreateThread(NULL, 0, lpThreadStart, lpContext, 0, &dwThreadId);
}

#endif

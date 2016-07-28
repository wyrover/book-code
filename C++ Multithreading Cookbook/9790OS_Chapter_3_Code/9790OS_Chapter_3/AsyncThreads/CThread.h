#ifndef _CTHREAD_
#define _CTHREAD_

#include <windows.h>

#define STATE_RUNNING       0x0001
#define STATE_READY         0x0002
#define STATE_BLOCKED       0x0004
#define STATE_ALIVE         0x0008
#define STATE_ASYNC         0x0010
#define STATE_SYNC          0x0020
#define STATE_CONTINUOUS    0x0040

class CLock
{
public:
    CLock();
    ~CLock();
private:
    HANDLE hMutex;
};

class CThread
{
public:
    CThread() : hThread(0), dwThreadId(0), dwState(0), lpUserData(0), lpParameter(0) { }
    HANDLE Create(LPVOID lpParameter, DWORD dwInitialState = STATE_ASYNC, DWORD dwCreationFlag = 0);
    void Join(DWORD dwWaitInterval = INFINITE);
    DWORD Suspend();
    DWORD Resume();
    void SetUserData(void* lpUserData);
    void* GetUserData() const;
    DWORD GetId() const;
    HANDLE GetHandle() const;
    DWORD GetAsyncState() const;
    DWORD GetState() const;
    void SetState(DWORD dwNewState);
    BOOL Alert();
protected:
    virtual void Run(LPVOID lpParameter = 0) = 0;
    LPVOID lpParameter;
private:
    static DWORD WINAPI StartAddress(LPVOID lpParameter);
    HANDLE hThread;
    DWORD dwThreadId;
    DWORD dwState;
    void* lpUserData;
    HANDLE hEvent;
};

inline DWORD CThread::GetId() const
{
    return dwThreadId;
}

inline HANDLE CThread::GetHandle() const
{
    return hThread;
}

#endif

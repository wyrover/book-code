// ThreadPool.h: interface for the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADPOOL_H__E60DFA90_8F3E_11D4_8AF0_0000C03A07C8__INCLUDED_)
#define AFX_THREADPOOL_H__E60DFA90_8F3E_11D4_8AF0_0000C03A07C8__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

struct IJobDesc {
};

struct IWorker {
    virtual void ProcessJob(IJobDesc* pJob) = 0;
};


struct ThreadInfo {
    HANDLE m_hThread;
    bool    m_bBusyWorking;
    ThreadInfo()
    {
        m_hThread = 0;
        m_bBusyWorking = false;
    }
    ThreadInfo(HANDLE handle, bool bBusy)
    {
        m_hThread = handle;
        m_bBusyWorking = bBusy;
    }
    ThreadInfo(const ThreadInfo& info)
    {
        m_hThread = info.m_hThread;
        m_bBusyWorking = info.m_bBusyWorking;
    }
};


class CThreadPool
{
    friend static unsigned int CThreadPool::ManagerProc(void* p);
    friend static unsigned int CThreadPool::WorkerProc(void* p);
protected:
    enum ThreadPoolStatus { BUSY, IDLE, NORMAL };
public:
    //interface to the outside
    void Start(unsigned short nStatic, unsigned short nmax);
    void Stop(bool bHash = false);
    void ProcessJob(IJobDesc* pJob, IWorker* pWorker) const;

    //constructor and destructor
    CThreadPool();
    virtual ~CThreadPool();

protected:
    //interfaces public:
    HANDLE GetMgrIoPort() const
    {
        return m_hMgrIoPort;
    }
    UINT GetMgrWaitTime() const
    {
        return 1000 * 6;
    }
    HANDLE GetWorkerIoPort() const
    {
        return m_hWorkerIoPort;
    }

private:
    static unsigned int ManagerProc(void* p);
    static unsigned int WorkerProc(void* p);
protected:
    //manager thread
    HANDLE m_hMgrThread;
    HANDLE m_hMgrIoPort;
protected:
    //configuration parameters
    mutable UINT m_nNumberOfStaticThreads;
    mutable UINT m_nNumberOfTotalThreads;

protected:
    //helper functions
    void AddThreads();
    void RemoveThreads();
    ThreadPoolStatus GetThreadPoolStatus();
    void ChangeStatus(DWORD threadId, bool status);
    void RemoveThread(DWORD threadId)
    {
        CSingleLock lock(&m_arrayCs);
        lock.Lock();
        m_threadMap.RemoveKey(threadId);
        lock.Unlock();
    }

protected:
    //all the work threads
    CMap<DWORD, DWORD&, ThreadInfo, ThreadInfo&> m_threadMap;
    CCriticalSection m_arrayCs;
    HANDLE m_hWorkerIoPort;
};

#endif // !defined(AFX_THREADPOOL_H__E60DFA90_8F3E_11D4_8AF0_0000C03A07C8__INCLUDED_)

// AsyncInvoke.h
//
//  .NET Asynchronous Design Pattern for C++ (ATL/WTL) using Windows
//  I/O Completion Ports
//
//  Copyright (C) 2009. All rights reserved.
//  "Daniel Ranger" <dranger003@gmail.com>
//
// DESCRIPTION
//
//   For more information about the .NET Asynchronous Programming Design
//   Patterns please visit MSDN at:
//   http://msdn.microsoft.com/en-ca/library/ms228969.aspx
//
// LIMITATIONS/RESTRICTIONS/DETAILS
// (See source code comments for in-depth information)
//
//   1. Delegate functions are not type safe (using ULONG_PTR)
//   2. Delegate objects must always be created on the heap (with new)
//   3. Callback wrapper delegates are always deleted automatically
//   4. Delegate signatures must match LPFNDELEGATE or LPCLSDELEGATE
//   5. IAsyncResult should NEVER be used after calling EndInvoke() as
//      EndInvoke() sets it to NULL
//   6. EndInvoke() should always be called to terminate a BeginInvoke()
//      not calling EndInvoke will lead to a memory leak (for non
//      "fire & forget" scenario only)
//   7. Calling EndInvoke() within a callback is safe
//   8. There is no support for catching exceptions
//   9. Callback calls are not included in a call completion timeframe,
//      that is, a callback may well be executed after calling EndInvoke()
//  10. Callbacks are called from the receiving thread
//  11. On shutdown the thread pool waits for unfinished threads therefore
//      applications should always manage their asynchronous calls &
//      callbacks completion
//
//   For more details on this implementation visit The Code Project at:
//   http://www.codeproject.com/KB/threads/asyncinvoke.aspx
//
// LICENSE
//
//   This is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This file is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlutil.h>                    // CThreadPool
#include "IoCompletionPort.h"

// Macros
//
/////////////////////////////////////////////////////////////////////////////

// Global delegate
#define MAKEDELEGATE(MEMBER) \
    (new CDelegate(&MEMBER))
// Class delegate
#define MAKECLSDELEGATE(CLASS, MEMBER) \
    (new CDelegate(new CClassDelegate<CLASS>(this, &CLASS::MEMBER)))
// Class instance delegate
#define MAKECLSINSTDELEGATE(CLASS, INSTANCE, MEMBER) \
    (new CDelegate(new CClassDelegate<CLASS>(INSTANCE, &CLASS::MEMBER)))

// IDeletable
//
// Self deletable object
//
/////////////////////////////////////////////////////////////////////////////

class IDeletable
{
public:
    virtual VOID Delete() = 0;
};

// IAsyncResult
// http://msdn.microsoft.com/en-ca/library/system.iasyncresult.aspx
//
// Client interface to the asynchronous call.
//
/////////////////////////////////////////////////////////////////////////////

class IAsyncResult
{
public:
    virtual LPVOID GetAsyncState() const = 0;
    virtual HANDLE GetAsyncWaitHandle() const = 0;
    virtual BOOL IsCompleted() const = 0;
};

// IDelegate
//
// A delegate wraps a given function and provides a mean to call the
// function asynchronously or synchronously. C# delegate mimic interface.
//
/////////////////////////////////////////////////////////////////////////////

class IDelegate : public IDeletable
{
public:
    virtual IAsyncResult *BeginInvoke(ULONG_PTR = 0, IDelegate * = NULL, LPVOID = NULL, BOOL = FALSE) = 0;
    virtual ULONG_PTR EndInvoke(IAsyncResult **) = 0;
    virtual ULONG_PTR Invoke(ULONG_PTR = 0) = 0;
};

// ISynchronizeInvoke
// http://msdn.microsoft.com/en-ca/library/system.componentmodel.isynchronizeinvoke.aspx
//
// An object implementing this interface provides capability to execute a
// given delegate on a specific thread.
//
/////////////////////////////////////////////////////////////////////////////

class ISynchronizeInvoke
{
public:
    virtual IAsyncResult *BeginInvoke(IDelegate *, ULONG_PTR = 0, BOOL = TRUE) = 0;
    virtual ULONG_PTR EndInvoke(IAsyncResult **) = 0;
    virtual ULONG_PTR Invoke(IDelegate *, ULONG_PTR = 0) = 0;
    virtual BOOL InvokeRequired() const = 0;
};

// CAsyncRequest
//
// Abstract base class to model an asynchronous request - implemented by
// CAsyncResult and CThreadMethodEntry. This class is also modeled to be
// used by CDelegateWorker for requests being queued by CThreadPool
//
/////////////////////////////////////////////////////////////////////////////

class CAsyncRequest : public IAsyncResult
{
public:
    CAsyncRequest(LPVOID pvState, BOOL bFireAndForget) :
        m_pvState(pvState),
        m_bFireAndForget(bFireAndForget),
        m_hAsyncWaitEvent(NULL),
        m_bIsCompleted(FALSE),
        m_ulResult(NULL)
    {
        // Create an auto-reset event to signal the end of a request
        ATLASSERT(m_hAsyncWaitEvent == NULL);
        m_hAsyncWaitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        ATLASSERT(m_hAsyncWaitEvent != NULL);
    }

    virtual ~CAsyncRequest()
    {
        // Event cleanup
        ATLASSERT(m_hAsyncWaitEvent != NULL);

        if (m_hAsyncWaitEvent != NULL) {
            BOOL bRes = ::CloseHandle(m_hAsyncWaitEvent);
            ATLASSERT(bRes != FALSE);
            m_hAsyncWaitEvent = NULL;
        }
    }

    virtual BOOL Initialize() = 0;
    virtual VOID Execute() = 0;
    virtual VOID Terminate() = 0;


    // IAsyncResult /////////////////////////////////////////////////////////

    LPVOID GetAsyncState() const
    {
        return m_pvState;
    }

    HANDLE GetAsyncWaitHandle() const
    {
        ATLASSERT(m_hAsyncWaitEvent != NULL);
        return m_hAsyncWaitEvent;
    }

    BOOL IsCompleted() const
    {
        return m_bIsCompleted;
    }

protected:
    // This function can only be called once the call has been completed
    // Used internally by CDelegate
    ULONG_PTR GetResult() const
    {
        ATLASSERT(IsCompleted() == TRUE);
        return m_ulResult;
    }

protected:
    LPVOID m_pvState;
    BOOL m_bFireAndForget;
    HANDLE m_hAsyncWaitEvent;
    BOOL m_bIsCompleted;
    ULONG_PTR m_ulResult;
};

// CAsyncResult
// http://msdn.microsoft.com/en-ca/library/system.runtime.remoting.messaging.asyncresult.aspx
//
/////////////////////////////////////////////////////////////////////////////

class CDelegate;

class CAsyncResult : public CAsyncRequest
{
    friend class CDelegate;

public:
    CAsyncResult(IDelegate *pDelegate, ULONG_PTR ulParam, IDelegate *pCallback, LPVOID pvState, BOOL bFireAndForget) :
        CAsyncRequest(pvState, bFireAndForget),
        m_pDelegate(pDelegate),
        m_ulParam(ulParam),
        m_pCallback(pCallback)
    { }

    virtual ~CAsyncResult()
    {
        // If a callback was provided, delete it as well
        if (m_pCallback != NULL)
            m_pCallback->Delete();

        ATLASSERT(m_pDelegate != NULL);
        m_pDelegate->Delete();
    }

    BOOL Initialize()
    {
        return TRUE;
    }

    VOID Execute()
    {
        ATLASSERT(m_pDelegate != NULL);
        m_ulResult = m_pDelegate->Invoke(m_ulParam);
    }

    VOID Terminate()
    {
        // Flag the call as completed
        ATLASSERT(m_bIsCompleted == FALSE);
        m_bIsCompleted = TRUE;
        // Signal the call has completed
        // Signaling will delete us only if there is no callback
        // This is important because we are used as the callback
        // parameter
        ATLASSERT(m_hAsyncWaitEvent != NULL);
        BOOL bRes = ::SetEvent(m_hAsyncWaitEvent);
        ATLASSERT(bRes != FALSE);

        // Invoke the callback if one is provided
        if (m_pCallback != NULL) {
            m_pCallback->Invoke((ULONG_PTR)this);

            if (m_bFireAndForget)
                EndInvoke();

            // Since there was a callback EndInvoke() didn't delete
            // this request so delete it now
            delete this;
            // WARNING: Object has been deleted!
            return;
        }

        // WARNING: Object has been deleted passed EndInvoke!
        if (m_bFireAndForget)
            EndInvoke();
    }

    IDelegate *GetAsyncDelegate() const
    {
        ATLASSERT(m_pDelegate != NULL);
        return m_pDelegate;
    }

private:
    // Internal function called only within a "fire & forget" context
    // Cleans up the request & delegate allocated resources
    VOID EndInvoke()
    {
        IAsyncResult *pAsyncResult = this;
        // Call EndInvoke, ignoring the return value
        ATLASSERT(m_pDelegate != NULL);
        m_pDelegate->EndInvoke(&pAsyncResult);
        ATLASSERT(pAsyncResult == NULL);
        // WARNING: This object is potentially deleted now!
        // Potentially meaning if m_pCallback != NULL
    }

private:
    IDelegate *m_pDelegate;
    ULONG_PTR m_ulParam;
    IDelegate *m_pCallback;
};

// CThreadMethodEntry
//
/////////////////////////////////////////////////////////////////////////////

class CSynchronizeInvoke;

class CThreadMethodEntry : public CAsyncRequest
{
    friend class CSynchronizeInvoke;

public:
    CThreadMethodEntry(ISynchronizeInvoke *pCaller, IDelegate *pDelegate, ULONG_PTR ulParam, BOOL bFireAndForget) :
        CAsyncRequest(NULL, bFireAndForget),
        m_pCaller(pCaller),
        m_pDelegate(pDelegate),
        m_ulParam(ulParam)
    { }

    virtual ~CThreadMethodEntry()
    {
        ATLASSERT(m_pDelegate != NULL);
        m_pDelegate->Delete();
    }

    ;
    BOOL Initialize()
    {
        return TRUE;
    }

    VOID Execute()
    {
        ATLASSERT(m_pDelegate != NULL);
        m_ulResult = m_pDelegate->Invoke(m_ulParam);
    }

    VOID Terminate()
    {
        // Flag the call as completed
        ATLASSERT(m_bIsCompleted == FALSE);
        m_bIsCompleted = TRUE;
        // Signal the call has completed
        // Signaling will delete us only if there is no callback
        ATLASSERT(m_hAsyncWaitEvent != NULL);
        BOOL bRes = ::SetEvent(m_hAsyncWaitEvent);
        ATLASSERT(bRes != FALSE);

        // If this is a fire and forget type of call then call
        // EndInvoke on behalf of the caller
        if (m_bFireAndForget) {
            IAsyncResult *pAsyncResult = this;
            // Call EndInvoke, ignoring the return value
            ATLASSERT(m_pCaller != NULL);
            m_pCaller->EndInvoke(&pAsyncResult);
            ATLASSERT(pAsyncResult == NULL);
            // WARNING: This object has now been deleted!
            // Don't access members of this object passed this point
        }
    }

private:
    ISynchronizeInvoke *m_pCaller;
    IDelegate *m_pDelegate;
    ULONG_PTR m_ulParam;
};

// CDelegateWorker
//
// An ATL worker archetype compliant class.
// http://msdn.microsoft.com/en-us/library/ytkt93h8.aspx
//
// NOTES
//
//   1. RequestType is the type of request queued
//   2. Initialize() & Terminate() are called once for each thread
//      respectively at the beginning and ending of the thread's life
//   3. Execute() is called for each queued request and executed in
//      recycled threads from the thread pool
//
/////////////////////////////////////////////////////////////////////////////

class CDelegateWorker
{
public:
    typedef CAsyncResult * RequestType;

    BOOL Initialize(LPVOID /*pvState*/) throw()
    {
        // Thread initialization code goes here
        // An example could be calling CoInitialize() to add COM support
        // to the thread
        return TRUE;
    }

    VOID Execute(
        CAsyncResult *pAsyncResult,
        LPVOID /*pvState*/,
        LPOVERLAPPED /*pOverlapped*/)
    {
        // A request is always initialized on the calling thread to
        // initialize the wait handle before moving to the receiving
        // thread (here)
        pAsyncResult->Execute();
        pAsyncResult->Terminate();
    }

    VOID Terminate(LPVOID /*pvState*/) throw()
    {
        // Thread cleanup code goes here
        // An example could be calling CoUninitialize()
    }
};

// IClassDelegate
//
// Interface provided to decouple the template definition from the delegate
// class
//
/////////////////////////////////////////////////////////////////////////////

class IClassDelegate : public IDeletable
{
public:
    virtual ULONG_PTR Invoke(ULONG_PTR) = 0;
};

// CClassDelegate
//
// Class delegate is a wrapper object to keeps an instance/member function
// pointer reference
//
/////////////////////////////////////////////////////////////////////////////

template <class T>
class CClassDelegate : public IClassDelegate
{
    // Delegate function signature
    typedef ULONG_PTR(__thiscall T::*LPFNCLSDELEGATE)(ULONG_PTR);

public:
    CClassDelegate(T *pInstance, LPFNCLSDELEGATE pfn) :
        m_pInstance(pInstance),
        m_pfn(pfn)
    { }

    ~CClassDelegate()
    {
    }

    ULONG_PTR Invoke(ULONG_PTR ulParam)
    {
        return (m_pInstance->*m_pfn)(ulParam);
    }

    // A Delete() function is provided here since a client using
    // an IClassDelegate would not be able to delete its class pointer
    // unless they could cast it to CClassDelegate<T> and since they are
    // not templatized the cast is not possible
    VOID Delete()
    {
        delete this;
    }

private:
    T *m_pInstance;
    LPFNCLSDELEGATE m_pfn;
};

// CDelegate
//
// A function wrapper adding the capability to call that function
// asynchronously.
//
/////////////////////////////////////////////////////////////////////////////

class CDelegate : public IDelegate
{
    typedef IClassDelegate * LPCLSDELEGATE;

public:
    // A delegate associated function must comply to this signature
    // Cast both the result & parameter types as required
    typedef ULONG_PTR(*LPFNDELEGATE)(ULONG_PTR);

    // A delegate can accept a function pointer (1st constructor) or a
    // class delegate (2nd constructor) which refers to a class member
    // function
    CDelegate(LPFNDELEGATE pfn) :
        m_pfn(pfn),
        m_pcd(NULL),
        m_bFireAndForget(FALSE)
    {
        Initialize();
    }

    CDelegate(LPCLSDELEGATE pcd) :
        m_pfn(NULL),
        m_pcd(pcd),
        m_bFireAndForget(FALSE)
    {
        Initialize();
    }

    ~CDelegate()
    {
        // Cleanup the class delegate if one was used
        if (IsClassDelegate())
            GetClassDelegate()->Delete();
    }

    // IDelegate ///////////////////////////////////////////////////////////////

    IAsyncResult *BeginInvoke(ULONG_PTR ulParam = 0, IDelegate *pCallback = NULL, LPVOID pvState = NULL, BOOL bFireAndForget = FALSE)
    {
        // Create a new asynchronous request for this delegate call
        CAsyncResult *pAsyncResult = new CAsyncResult(this, ulParam, pCallback, pvState, bFireAndForget);
        BOOL bRes = pAsyncResult->Initialize();
        ATLASSERT(bRes != FALSE);
        // Queue the new request to be executed in the thread pool
        bRes = m_cThreadPool.QueueRequest(pAsyncResult);
        ATLASSERT(bRes != FALSE);
        return pAsyncResult;
    }

    ULONG_PTR EndInvoke(IAsyncResult **ppAsyncResult)
    {
        ATLASSERT(*ppAsyncResult != NULL);
        // Extract the asynchronous request
        CAsyncResult *pAsyncResult = static_cast<CAsyncResult *>(*ppAsyncResult);
        ATLASSERT(pAsyncResult != NULL);
        ATLASSERT(pAsyncResult->GetAsyncDelegate() == (IDelegate *)this);
        // Retrieve the call wait handle
        HANDLE hAsyncWaitEvent = pAsyncResult->GetAsyncWaitHandle();
        ATLASSERT(hAsyncWaitEvent != NULL);
        // Wait for the call to be completed
        DWORD dwRes = ::WaitForSingleObject(hAsyncWaitEvent, INFINITE);
        ATLASSERT(dwRes == WAIT_OBJECT_0);
        // Retrieve the result of the call
        ULONG_PTR ulResult = pAsyncResult->GetResult();

        // Delete the request only if there is no callback associated with it
        // otherwise the request is deleted after the callback is invoked
        if (pAsyncResult->m_pCallback == NULL)
            delete pAsyncResult;

        // Set pointers
        pAsyncResult = NULL;
        *ppAsyncResult = NULL;
        return ulResult;
    }

    ULONG_PTR Invoke(ULONG_PTR ulParam)
    {
        // Verify if we're calling a class member or not then invoke
        if (IsClassDelegate())
            return GetClassDelegate()->Invoke(ulParam);

        return (*m_pfn)(ulParam);
    }

    // IDeletable //////////////////////////////////////////////////////////////

    VOID Delete()
    {
        delete this;
    }

protected:
    // Internal use
    BOOL IsClassDelegate() const
    {
        return m_pcd != NULL;
    }

    IClassDelegate *GetClassDelegate() const
    {
        ATLASSERT(m_pcd != NULL);
        return m_pcd;
    }

private:
    // Internal
    VOID Initialize()
    {
        if (!m_bInitialized) {
            // Initialize the thread pool
            // By default it will initialize with 2x[CPU count] threads
            HRESULT hr = m_cThreadPool.Initialize();
            ATLASSERT(SUCCEEDED(hr));
            m_bInitialized = TRUE;
        }
    }

private:
    LPFNDELEGATE m_pfn;
    LPCLSDELEGATE m_pcd;
    BOOL m_bFireAndForget;

    static BOOL m_bInitialized;
    static CThreadPool<CDelegateWorker> m_cThreadPool;
};

// Statics
BOOL CDelegate::m_bInitialized = FALSE;
CThreadPool<CDelegateWorker> CDelegate::m_cThreadPool;

// CSynchronizeInvoke
//
// There is basically more than one use of this class but this
// implementation would typically be used from an object living on a
// message pumping thread that would call ProcessPendingInvoke() within
// its message pump
//
/////////////////////////////////////////////////////////////////////////////

class CSynchronizeInvoke :
    public ISynchronizeInvoke
{
public:
    CSynchronizeInvoke() :
        m_dwThreadId(0),
        WM_THREADCALLBACK(0)
    {
        // Save thread ID to synchronize with
        m_dwThreadId = ::GetCurrentThreadId();
        // Create the I/O completion port used to queue asynchronous calls
        m_cPort.Create();
        WM_THREADCALLBACK = ::RegisterWindowMessage(_T("411F33F1-E18B-48dc-B357-1727BA4E1A50"));
        ATLASSERT(WM_THREADCALLBACK != 0);
    }

    // ISynchronizeInvoke //////////////////////////////////////////////////////

    IAsyncResult *BeginInvoke(IDelegate *pDelegate, ULONG_PTR ulParam = 0, BOOL bFireAndForget = TRUE)
    {
        // Create a new thread method entry for this call
        CThreadMethodEntry *pMethod = new CThreadMethodEntry(this, pDelegate, ulParam, bFireAndForget);
        BOOL bRes = pMethod->Initialize();
        ATLASSERT(bRes != FALSE);
        // Queue the new request for execution
        bRes = m_cPort.PostQueuedCompletionStatus(0, (ULONG_PTR)pMethod, NULL);
        ATLASSERT(bRes != FALSE);
        // Inform thread there is an asynchronous call pending
        ::PostThreadMessage(m_dwThreadId, WM_THREADCALLBACK, 0, 0);
        return pMethod;
    }

    ULONG_PTR EndInvoke(IAsyncResult **ppAsyncResult)
    {
        ATLASSERT(*ppAsyncResult != NULL);
        CThreadMethodEntry *pMethod = static_cast<CThreadMethodEntry *>(*ppAsyncResult);
        ATLASSERT(pMethod != NULL);
        // Retrieve the call wait handle
        HANDLE hAsyncWaitEvent = pMethod->GetAsyncWaitHandle();
        ATLASSERT(hAsyncWaitEvent != NULL);
        // Wait for the call to be completed
        DWORD dwRes = ::WaitForSingleObject(hAsyncWaitEvent, INFINITE);
        ATLASSERT(dwRes == WAIT_OBJECT_0);
        // Retrieve the result of the call
        ULONG_PTR ulResult = pMethod->GetResult();
        delete pMethod;
        pMethod = NULL;
        *ppAsyncResult = NULL;
        return ulResult;
    }

    ULONG_PTR Invoke(IDelegate *pDelegate, ULONG_PTR ulParam = 0)
    {
        return pDelegate->Invoke(ulParam);
    }

    BOOL InvokeRequired() const
    {
        // An invoke is required if the calling thread is different from
        // the thread the CSynchronizeInvoke object was created on
        return ::GetCurrentThreadId() != m_dwThreadId;
    }

    // This function determines whether the message is a thread
    // callback message. If so, it calls ProcessPendingInvoke to
    // process pending calls.
    // Typically, this function is used as part of a message pump
    BOOL IsThreadCallbackMessage(MSG *pMsg)
    {
        if (pMsg->message == WM_THREADCALLBACK) {
            ProcessPendingInvoke();
            return TRUE;
        }

        return FALSE;
    }

    // This function is to be called on the desired thread, most likely
    // the GUI thread) to process pending calls (queued by BeginInvoke)
    // Typically it would be used with IsThreadCallbackMessage
    // The default is to process 2 pending calls
    // If this function is called from a GUI thread all pending calls
    // should complete rapidly so the GUI thread is not disturbed
    VOID ProcessPendingInvoke(DWORD dwCount = 2)
    {
        if (m_cPort.GetQueuedCount() < 1)
            return;

        DWORD dwBytes = 0;                  // Unused
        ULONG_PTR ulKey = 0;
        LPOVERLAPPED pOverlapped = NULL;    // Unused

        // Retrieve one pending call at a time up to dwCount calls
        // If no calls are queued then this loop exits immediately
        while (dwCount-- > 0 && m_cPort.GetQueuedCompletionStatus(&dwBytes, &ulKey, &pOverlapped, 0)) {
            CThreadMethodEntry *pMethod = reinterpret_cast<CThreadMethodEntry *>(ulKey);
            ATLASSERT(pMethod != NULL);
            // Execute & terminate the call
            pMethod->Execute();
            pMethod->Terminate();
        }
    }

private:
    DWORD m_dwThreadId;
    CIoCompletionPort m_cPort;
    UINT WM_THREADCALLBACK;
};

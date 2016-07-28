/*++

Copyright (c) Microsoft Corporation, All Rights Reserved

Module Name:

    umdf_vdev_parallelqueue.h

Abstract:

    This file defines the queue callback object for handling parallel device I/O
    control requests.

Environment:

    Windows User-Mode Driver Framework (WUDF)


--*/

#pragma once

class CVDevParallelQueue : 
    public CUnknown,
    public IQueueCallbackDeviceIoControl,
    public IRequestCallbackRequestCompletion
{

private:

    //
    // Weak reference to the critical section object
    //
    CRITICAL_SECTION m_Crit;

    //
    // Weak reference to queue object
    //
    IWDFIoQueue * m_FxQueue;

    //
    // Reference to the FX device class
    // 
    IWDFDevice * m_FxDevice;

    //
    // Reference to our Device Class
    //
    PCVDevDevice m_VdevDevice;

    HRESULT
        Initialize();

    __inline 
        void
        Lock ()
    {
        ::EnterCriticalSection (&m_Crit);
    }

    __inline 
        void
        Unlock ()
    {
        ::LeaveCriticalSection (&m_Crit);
    }


public:


    CVDevParallelQueue::CVDevParallelQueue (
        PCVDevDevice VdevDevice,
        IWDFDevice * FxDevice) : 
        m_FxDevice (FxDevice),
        m_FxQueue (NULL),
        m_VdevDevice (VdevDevice)
    {

    }

    virtual ~CVDevParallelQueue ();

    IWDFIoQueue *
        GetFxQueue (
        VOID)
    {
        return m_FxQueue;
    }

    IWDFDevice * 
        GetFxDevice (
        VOID)
    {
        return m_FxDevice;
    }

    static 
        HRESULT 
        CreateInstance ( 
        __in PCVDevDevice Device,
        __in IWDFDevice * FxDevice,
        __out PCVDevParallelQueue *Queue);

    HRESULT
        Configure (
        VOID)
    {
        return S_OK;
    }

    IQueueCallbackDeviceIoControl *
        DeviceIoControl (
        VOID)
    {
        AddRef();
        return static_cast<IQueueCallbackDeviceIoControl *>(this);
    }


    IRequestCallbackRequestCompletion *
        RequestCompletion (
        VOID)
    {
        AddRef();
        return static_cast <IRequestCallbackRequestCompletion *>(this);
    }


    virtual
        ULONG
        STDMETHODCALLTYPE
        AddRef (VOID) 
    {
        return CUnknown::AddRef();
    }

    __drv_arg(this, __drv_freesMem(object))
    virtual
        ULONG
        STDMETHODCALLTYPE
        Release (VOID) 
    {
        return CUnknown::Release();
    }

    virtual
        HRESULT
        STDMETHODCALLTYPE
        QueryInterface (
        __in REFIID InterfaceId, 
        __out PVOID *Object);

    virtual
        VOID
        STDMETHODCALLTYPE
        OnDeviceIoControl ( 
        __in IWDFIoQueue *pWdfQueue,
        __in IWDFIoRequest *pWdfRequest,
        __in ULONG ControlCode,
        __in SIZE_T InputBufferSizeInBytes,
        __in SIZE_T OutputBufferSizeInBytes);

    virtual
        VOID
        STDMETHODCALLTYPE
        OnCompletion (  
        __in IWDFIoRequest*  pWdfRequest,
        __in IWDFIoTarget*  pIoTarget,
        __in IWDFRequestCompletionParams*  pParams,
        __in PVOID  pContext);

    virtual
        void
        STDMETHODCALLTYPE
        OnAllocateAddrRangeCompletion (
        __in IWDFIoRequest *  pWdfRequest,
        __in IWDFRequestCompletionParams*  pParams);

    virtual
        VOID
        STDMETHODCALLTYPE
        OnFreeAddrRangeCompletion (
        __in IWDFIoRequest *  pWdfRequest,
        __in IWDFRequestCompletionParams*  pParams);


    virtual
        HRESULT 
        STDMETHODCALLTYPE
        SubmitAsyncRequestToLower (
        __in IWDFIoRequest * Request);

};



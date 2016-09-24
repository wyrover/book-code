/*++

Copyright (c) Microsoft Corporation, All Rights Reserved

Module Name:

    umdf_vdev_sequentialqueue.h

Abstract:

    This file defines the queue callback object for handling serialized device I/O
    control requests. 

Environment:

    Windows User-Mode Driver Framework (WUDF)


--*/

#pragma once

class CVDevSequentialQueue : 
    public CUnknown,
    public IQueueCallbackDeviceIoControl
{

private:

    //
    // Request input memory block
    //
    PVOID m_InputMemory;

    // 
    // Request output memory block
    //
    PVOID m_OutputMemory;

    //
    // Weak reference to the critical section object
    //
    CRITICAL_SECTION m_Crit;

    //
    // Weak reference to queue object
    //
    IWDFIoQueue * m_FxQueue;

    //
    // Reference to the device class
    // 
    IWDFDevice * m_FxDevice;

    //
    // Reference to the Lower IO Target
    //
    IWDFIoTarget * m_kmdfIoTarget;

    HRESULT
        Initialize();

    __inline 
        void
        Lock ()
    {
        ::EnterCriticalSection(&m_Crit);
    }

    __inline 
        void 
        Unlock ()
    {
        ::LeaveCriticalSection(&m_Crit);
    }

public:


    CVDevSequentialQueue::CVDevSequentialQueue (
        IWDFDevice * FxDevice):
    m_FxDevice(FxDevice)
    {
    }

    CVDevSequentialQueue::~CVDevSequentialQueue()
    {
        DeleteCriticalSection (&this->m_Crit);
    }

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
        __out PCVDevSequentialQueue *Queue);



    IQueueCallbackDeviceIoControl *
        DeviceIoControl (VOID)
    {
        AddRef();
        return static_cast<IQueueCallbackDeviceIoControl *>(this);
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
        HRESULT
        STDMETHODCALLTYPE
        SubmitRequestToLower (
        __in IWDFIoRequest * Request);
};



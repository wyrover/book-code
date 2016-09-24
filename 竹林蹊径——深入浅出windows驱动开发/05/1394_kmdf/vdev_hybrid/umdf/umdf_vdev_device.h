/*++

Copyright (C) Microsoft Corporation, All Rights Reserved

Module Name:

    umdf_vdev_device.h

Abstract:

    This module contains the type definitions for the 1394 vdev hybrid sample
    driver's device callback class.

Environment:

    Windows User-Mode Driver Framework (WUDF)


--*/


#pragma once

#include "umdf_vdev.h"


class CVDevDevice : 
    public CUnknown,
    public IPnpCallbackHardware
{

protected:
 
    //
    // Weak reference to the FX device
    //
    IWDFDevice * m_FxDevice;

    //
    // Weak reference to the Parallel Queue
    //
    PCVDevParallelQueue m_ParallelQueue;

    //
    // Weak reference to the Sequential Queue.
    //
    PCVDevSequentialQueue m_SequentialQueue;


private:

    CVDevDevice (VOID) :
        m_FxDevice (NULL),
        m_SequentialQueue (NULL),
        m_ParallelQueue (NULL)
    {
    }

    ~CVDevDevice();


    HRESULT
    Initialize (
                __in IWDFDriver * FxDriver,
                __in IWDFDeviceInitialize * FxDeviceInit);
     
public:

    static
        HRESULT
        CreateInstance (
        __in IWDFDriver * FxDriver,
        __in IWDFDeviceInitialize * FxDeviceInit,
        __out PCVDevDevice * Device);

    IWDFDevice *
        GetFxDevice (VOID)
    {
        return m_FxDevice;
    }

    PCVDevSequentialQueue
        GetSequentialQueue (VOID)
    {
        return m_SequentialQueue;
    }

    HRESULT
        Configure (VOID);


    IPnpCallbackHardware * 
        HardwareCallback (VOID)
    {
        AddRef ();
        return static_cast <IPnpCallbackHardware *> (this);
    }


    virtual
        ULONG
        STDMETHODCALLTYPE
        AddRef (VOID)
    {
        return __super::AddRef ();
    }

    __drv_arg(this, __drv_freesMem(object))
    virtual
        ULONG
        STDMETHODCALLTYPE
        Release (VOID)
    {
        return __super::Release ();
    }

    virtual
        HRESULT
        STDMETHODCALLTYPE 
        QueryInterface (
                __in REFIID InterfaceId,
                __deref_out PVOID * Object);
    
    virtual
        HRESULT
        STDMETHODCALLTYPE
        OnReleaseHardware (
                            __in IWDFDevice * wdfDevice);
    
    virtual
        HRESULT
        STDMETHODCALLTYPE
        OnPrepareHardware (
                            __in IWDFDevice  * wdfDevice)
    {
        UNREFERENCED_PARAMETER (wdfDevice);
        //
        // We don't really need to do anything here, but to 
        // we have to invoke this method to get the OnReleaseHardware functionality.
        //
        return S_OK;
    }
};


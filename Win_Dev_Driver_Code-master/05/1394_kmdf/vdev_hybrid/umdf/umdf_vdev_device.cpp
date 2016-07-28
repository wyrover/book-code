/*++

Copyright (C) Microsoft Corporation, All Rights Reserved.

Module Name:

umdf_vdev_device.cpp

Abstract:

This module contains the implementation of the UMDF 1394 hybrid sample driver's
device callback object.


Environment:

Windows User-Mode Driver Framework (WUDF)

--*/

#include <initguid.h>
#include "umdf_vdev.h"
#include "wdf_common.h"
#include "umdf_vdev_device.tmh"


//
// This is a false positive in PreFast
// The object (device) uses the COM model of release
// upon the refcount reaching 0, so the object is then freed from 
// memory.
//
#pragma warning (disable: 28197 )


CVDevDevice::~CVDevDevice ()
{
}

HRESULT
CVDevDevice::CreateInstance ( 
                             __in IWDFDriver *FxDriver,
                             __in IWDFDeviceInitialize * FxDeviceInit,
                             __out PCVDevDevice *Device)
/*++

Routine Description:

    This method creates and initializs an instance of the hybrid driver's 
    device callback object.

Arguments:
    FxDriver -  Framework driver interface.  
    FxDeviceInit - the settings for the device.
    Device - a location to store the referenced pointer to the device object.

Return Value:
    HRESTULT indication of success or failure

--*/
{
    PCVDevDevice vDevice;
    HRESULT hr;

    vDevice = new CVDevDevice ();

    if (NULL == vDevice)
    {
        return E_OUTOFMEMORY;
    }


    hr = vDevice->Initialize (FxDriver, FxDeviceInit);

    if (SUCCEEDED (hr)) 
    {
        *Device = vDevice;
    } 
    else 
    {
        vDevice->Release ();
    }

    return hr;
}

HRESULT
CVDevDevice::Initialize ( 
                         __in IWDFDriver           * FxDriver,
                         __in IWDFDeviceInitialize * FxDeviceInit)
/*++

    Routine Description:
        This method initializes the device callback object and creates the
        partner device object.
        
    Arguments:
        FxDeviceInit - the settings for this device.

    Return Value:
        HRESTULT indication of success or failure

--*/
{
    IWDFDevice * fxDevice = NULL;

    HRESULT hr = S_OK;

    FxDeviceInit->SetLockingConstraint (None);

    {
        IUnknown * unknown = this->QueryIUnknown ();

        hr = FxDriver->CreateDevice (FxDeviceInit, unknown, &fxDevice);

        unknown->Release ();
    }

    if (SUCCEEDED (hr)) 
    {
        m_FxDevice = fxDevice;

        //
        // We can release the reference as the lifespan is tied to the 
        // framework object.
        //
        fxDevice->Release();
    }

    return hr;
}

HRESULT
CVDevDevice::Configure (VOID)
/*++

    Routine Description:
        This method is called after the device callback object has been initialized 
        and returned to the driver.  It would setup the device's queues and their 
        corresponding callback objects.

    Arguments:
        None
        
    Return Value:
        HRESTULT indication of success or failure

--*/
{   
    HRESULT hr = S_OK;

    //
    // Create the control queues and configure forwarding for IOCTL requests.
    //
    hr = CVDevParallelQueue::CreateInstance (
        this, 
        m_FxDevice, 
        &m_ParallelQueue);

    if (SUCCEEDED (hr)) 
    {       
        m_ParallelQueue->Release ();
    }
    else
    {
        return hr;
    }


    hr = CVDevSequentialQueue::CreateInstance (
        this, 
        m_FxDevice, 
        &m_SequentialQueue);

    if (SUCCEEDED (hr)) 
    {
        m_SequentialQueue->Release ();
    }
    else
    {
        return hr;
    }


    hr = m_FxDevice->CreateDeviceInterface (&GUID_UMDF_VDEV_HYBRID, NULL);

    return hr;
}

HRESULT
CVDevDevice::QueryInterface (
                             __in REFIID InterfaceId,
                             __deref_out PVOID *Object)
/*++

    Routine Description:
        This method is called to get a pointer to one of the object's callback
        interfaces.  
        
    Arguments:
        InterfaceId - the interface being requested
        Object - a location to store the interface pointer if successful

    Return Value:
        S_OK or E_NOINTERFACE

--*/
{
    HRESULT hr;

    if (IsEqualIID(InterfaceId, __uuidof (IPnpCallbackHardware))) 
    {
        *Object = HardwareCallback ();
        hr = S_OK;
    }
    else
    {
        hr = CUnknown::QueryInterface (InterfaceId, Object);
    }

    return hr;
}

HRESULT
CVDevDevice::OnReleaseHardware (
                                __in IWDFDevice * wdfDevice)
{

    UNREFERENCED_PARAMETER (wdfDevice);

    //
    // TODO: Well handle resource clean up here
    //

    return S_OK;
}
                               




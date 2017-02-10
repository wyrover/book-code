/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#pragma once
#include "resource.h"       // main symbols

#include "SRV.h"



// CCalculator

class ATL_NO_VTABLE CCalculator :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CCalculator, &CLSID_Calculator>,
    public ISupportErrorInfo,
    public ICalculator
{
public:
    CCalculator()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_CALCULATOR)

DECLARE_NOT_AGGREGATABLE(CCalculator)

BEGIN_COM_MAP(CCalculator)
    COM_INTERFACE_ENTRY(ICalculator)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:
    virtual HRESULT STDMETHODCALLTYPE Sum( 
            __int32 a,
            __int32 b,
            __int32 *result);
    virtual HRESULT STDMETHODCALLTYPE SumSlow( 
            __int32 a,
            __int32 b,
            __int32 *result);
        
};

OBJECT_ENTRY_AUTO(__uuidof(Calculator), CCalculator)

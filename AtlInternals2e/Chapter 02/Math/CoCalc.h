// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols

#include "Math.h"


// CCalc

class ATL_NO_VTABLE CCalc :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCalc, &CLSID_Calc>,
    public ISupportErrorInfo,
    public IDispatchImpl<ICalc, &IID_ICalc, &LIBID_MathLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CCalc()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_CALC)


    BEGIN_COM_MAP(CCalc)
    COM_INTERFACE_ENTRY(ICalc)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        CAutoPtr<int> ap;
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:

    STDMETHOD(Add)(DOUBLE Op1, DOUBLE Op2, DOUBLE* Result);
};

OBJECT_ENTRY_AUTO(__uuidof(Calc), CCalc)

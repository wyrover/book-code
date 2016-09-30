// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols

#include "MathServiceNoAttr.h"


// CCalc

class ATL_NO_VTABLE CCalc :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCalc, &CLSID_Calc>,
    public IDispatchImpl<ICalc, &IID_ICalc, &LIBID_MathServiceNoAttrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CCalc()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_CALC)


    BEGIN_COM_MAP(CCalc)
    COM_INTERFACE_ENTRY(ICalc)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:

};

OBJECT_ENTRY_AUTO(__uuidof(Calc), CCalc)

// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols

#include "MathDllNoAttr.h"


static const GUID CATID_ATLINTERNALS_SAMPLES =
{0xd22ff22, 0x28cc, 0x11d2, {0xab, 0xdd, 0x0, 0xa0, 0xc9, 0xc8, 0xe5, 0xd}};

// CCalc

class Fred
{
public:
    Fred()
    {
        int a = 5;
        int b = 7;
    }
};

Fred f;

class ATL_NO_VTABLE CCalc :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCalc, &CLSID_Calc>,
    public IDispatchImpl<ICalc, &IID_ICalc, &LIBID_MathDllNoAttrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
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


    BEGIN_CATEGORY_MAP(CDemagogue)
    IMPLEMENTED_CATEGORY(CATID_ATLINTERNALS_SAMPLES)
    END_CATEGORY_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        __if_exists(_Module) {
            int x = 5;
        }
        __if_not_exists(_Module) {
            int y = 5;
        }
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:

};

OBJECT_ENTRY_AUTO(__uuidof(Calc), CCalc)

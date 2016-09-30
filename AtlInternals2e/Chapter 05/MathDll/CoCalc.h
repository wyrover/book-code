// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("D4A66BDB-EF09-4968-A8AA-7BA5C48AF6BD"),
    dual,   helpstring("ICalc Interface"),
    pointer_default(unique)
]
__interface ICalc :
IDispatch {
};


static const GUID CATID_ATLINTERNALS_SAMPLES =
{0xd22ff22, 0x28cc, 0x11d2, {0xab, 0xdd, 0x0, 0xa0, 0xc9, 0xc8, 0xe5, 0xd}};

static const GUID CATID_IMPLEMENTED_1 =
{ 0x24344b98, 0xd4df, 0x4313, { 0x99, 0x57, 0xce, 0x6c, 0xae, 0xf4, 0xe9, 0x2c } };

static const GUID CATID_REQUIRED_1 =
{ 0x35a3ddfa, 0x1ee5, 0x472b, { 0x9b, 0xa8, 0x1a, 0xb8, 0x72, 0x90, 0x35, 0x43 } };



// CCalc

[
    coclass,
    threading("apartment"),
    vi_progid("MathDll.Calc"),
    progid("MathDll.Calc.1"),
    version(1.0),
    uuid("7A077EBD-04CC-41BA-995C-C62DC6EE61D7"),
//  implements_category("CATID_ATLINTERNALS_SAMPLES"),
    helpstring("Calc Class")
]
class ATL_NO_VTABLE CCalc :
    public ICalc
{
public:
    CCalc()
    {
    }

    BEGIN_CATEGORY_MAP(CCalc)
    IMPLEMENTED_CATEGORY(CATID_IMPLEMENTED_1)
    REQUIRED_CATEGORY(CATID_REQUIRED_1)
    END_CATEGORY_MAP()

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


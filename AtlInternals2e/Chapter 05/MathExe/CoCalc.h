// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("B39333DF-28AE-477D-A3C0-BE4EF0F95809"),
    dual,   helpstring("ICalc Interface"),
    pointer_default(unique)
]
__interface ICalc :
IDispatch {
};



// CCalc

[
    coclass,
    threading("apartment"),
    vi_progid("MathExe.Calc"),
    progid("MathExe.Calc.1"),
    version(1.0),
    uuid("3F68AD23-21AD-44A7-8E10-3C57C66BB43F"),
    helpstring("Calc Class")
]
class ATL_NO_VTABLE CCalc :
    public ICalc
{
public:
    CCalc()
    {
    }


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


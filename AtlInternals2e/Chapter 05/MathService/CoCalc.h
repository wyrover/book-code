// CoCalc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("6D9014D4-3ADC-422D-B471-2880C824A3B5"),
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
    vi_progid("MathService.Calc"),
    progid("MathService.Calc.1"),
    version(1.0),
    uuid("6D45D859-615D-449B-A06C-355D905AF2FE"),
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


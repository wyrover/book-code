// Calc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("296E91A9-7F72-49FA-88F7-7B3AFEE9422D"),
    oleautomation,  helpstring("ICalc Interface"),
    pointer_default(unique)
]
__interface ICalc :
IUnknown {
    [id(1), helpstring("method Add")] HRESULT Add([in] DOUBLE Op1, [in] DOUBLE Op2, [out, retval] DOUBLE * Result);
};



// CCalc

[
    coclass,
    threading("both"),
    vi_progid("GITTest.Calc"),
    progid("GITTest.Calc.1"),
    version(1.0),
    uuid("10AFDDFC-CBD6-4072-9065-D89D88028444"),
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

    STDMETHOD(Add)(DOUBLE Op1, DOUBLE Op2, DOUBLE* Result);
};


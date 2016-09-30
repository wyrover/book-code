// Calc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("64A6A06E-782A-4700-B4A3-7853546C3B12"),
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
    vi_progid("CAutoPtrTest.Calc"),
    progid("CAutoPtrTest.Calc.1"),
    version(1.0),
    uuid("88E83AB4-3E22-4190-9A7D-0416A3AB12FC"),
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


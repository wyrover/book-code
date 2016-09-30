// Calc.h : Declaration of the CCalc

#pragma once
#include "resource.h"       // main symbols


// ICalc
[
    object,
    uuid("5B9343BD-D717-4DD7-86F3-E4015381D340"),
    oleautomation,  helpstring("ICalc Interface"),
    pointer_default(unique)
]
__interface ICalc :
IUnknown {
};



// CCalc

[
    coclass,
    threading("both"),
    support_error_info("ICalc"),
    vi_progid("MemMgrTest.Calc"),
    progid("MemMgrTest.Calc.1"),
    version(1.0),
    uuid("70C6F14A-A0AA-416F-A548-02B65463C47A"),
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


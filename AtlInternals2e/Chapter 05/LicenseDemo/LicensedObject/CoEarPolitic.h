// CoEarPolitic.h : Declaration of the CEarPolitic

#pragma once
#include "resource.h"       // main symbols
#include "MyLicense.h"


// IEarPolitic
[
    object,
    uuid("525DE96A-ABF6-46DF-8EF5-7E677A3F128C"),
    oleautomation,  helpstring("IEarPolitic Interface"),
    pointer_default(unique)
]
__interface IEarPolitic :
IUnknown {
};



// CEarPolitic

[
    coclass,
    threading("apartment"),
    vi_progid("LicenseDemo.EarPolitic"),
    progid("LicenseDemo.EarPolitic.1"),
    version(1.0),
    uuid("90B1E18D-2976-41A3-A4CC-7DA344318C58"),
    helpstring("EarPolitic Class")
]
class ATL_NO_VTABLE CEarPolitic :
    public IEarPolitic
{
public:
    CEarPolitic()
    {
    }


    DECLARE_CLASSFACTORY2(CMyLicense)

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


// Demagogue.h : Declaration of the CDemagogue

#pragma once
#include "resource.h"       // main symbols


// IDemagogue
[
    object,
    uuid("46C6F946-B3AA-481C-AE6B-CBBE65D95CF3"),
    oleautomation,  helpstring("IDemagogue Interface"),
    pointer_default(unique)
]
__interface IDemagogue :
IUnknown {
};



// CDemagogue

[
    coclass,
    threading("apartment"),
    vi_progid("LicenseDemo.Demagogue"),
    progid("LicenseDemo.Demagogue.1"),
    version(1.0),
    uuid("2777C6F6-959F-4CBE-8C03-D100C942CFF4"),
    helpstring("Demagogue Class")
]
class ATL_NO_VTABLE CDemagogue :
    public IDemagogue
{
public:
    CDemagogue()
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


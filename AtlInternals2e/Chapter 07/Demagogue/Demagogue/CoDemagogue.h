// CoDemagogue.h : Declaration of the CDemagogue

#pragma once
#include "resource.h"       // main symbols


// IDemagogue
[
    object,
    uuid("02C6F955-986D-4358-8ECB-04553F1D7D32"),
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
    support_error_info("IDemagogue"),
    vi_progid("Demagogue.Demagogue"),
    progid("Demagogue.Demagogue.1"),
    version(1.0),
    uuid("8FB5429F-5457-4ED7-8093-EDA710A68EB8"),
    helpstring("Demagogue Class")
]
class ATL_NO_VTABLE CDemagogue :
    public IDemagogue,
    public IPersistPropertyBagImpl<CDemagogue>
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
    bool m_bRequiresSave;
};


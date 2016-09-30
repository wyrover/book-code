// CoArchitect.h : Declaration of the CArchitect

#pragma once
#include "resource.h"       // main symbols


// IArchitect
[
    object,
    uuid("09016650-D78A-41AC-81DB-078E670A131F"),
    oleautomation,  helpstring("IArchitect Interface"),
    pointer_default(unique)
]
__interface IArchitect :
IUnknown {
    HRESULT Draw(void);
};



// CArchitect

[
    coclass,
    threading("apartment"),
    vi_progid("Architect.Architect"),
    progid("Architect.Architect.1"),
    version(1.0),
    uuid("3F6FAEE0-E243-466E-93A3-4F5C2C388A73"),
    helpstring("Architect Class")
]
class ATL_NO_VTABLE CArchitect :
    public IArchitect
{
public:
    CArchitect()
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

    STDMETHOD(Draw)(void);
};


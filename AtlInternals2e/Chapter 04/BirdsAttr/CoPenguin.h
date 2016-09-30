// CoPenguin.h : Declaration of the CPenguin

#pragma once
#include "resource.h"       // main symbols


// IPenguin
[
    object,
    dual,
    uuid("3C093AF2-1EFC-4FE9-B602-D5A23D967B77"),
    helpstring("IPenguin Interface"),
    pointer_default(unique)
]
__interface IPenguin :
IDispatch {
    [propget, id(1)] HRESULT Name([out, retval] BSTR * pVal);
    [propput, id(1)] HRESULT Name([in] BSTR newVal);
    [id(2)] HRESULT StraightenTie(void);
};

// IBird
[
    object,
    dual,
    uuid("92247816-FA98-4861-83AB-56FD45F37991"),
    helpstring("IBird Interface"),
    pointer_default(unique)
]
__interface IBird :
IDispatch {
    [propget, id(1)] HRESULT Wingspan([out, retval] LONG * pVal);
    [propput, id(1)] HRESULT Wingspan([in] LONG newVal);
    [id(2)] HRESULT Fly(void);
};

// CPenguin

[
    coclass,
    threading("single"),
    vi_progid("BirdsAttr.Penguin"),
    progid("BirdsAttr.Penguin.1"),
    version(1.0),
    uuid("F591F114-57D0-45DA-B627-5E859AB62248"),
    helpstring("Penguin Class")
]
class ATL_NO_VTABLE CPenguin :
    public IPenguin,
    public IBird
{
public:
    CPenguin()
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

    // IPenguin
    STDMETHOD(get_Name)(BSTR* pVal);
    STDMETHOD(put_Name)(BSTR newVal);
    STDMETHOD(StraightenTie)(void);

    // IBird
    STDMETHOD(get_Wingspan)(LONG* pVal);
    STDMETHOD(put_Wingspan)(LONG newVal);
    STDMETHOD(Fly)(void);
};


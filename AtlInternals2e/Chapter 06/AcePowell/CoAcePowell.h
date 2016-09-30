// CoAcePowell.h : Declaration of the CAcePowell

#pragma once
#include "resource.h"       // main symbols
#import "..\\Architect\\Debug\\Architect.dll" no_namespace, named_guids, raw_interfaces_only


// ICowboy
[
    object,
    uuid("251DF313-37DE-445C-8115-AFC550B812AA"),
    helpstring("ICowboy Interface"),
    pointer_default(unique)
]
__interface ICowboy :
IUnknown {
    HRESULT Draw();
};

// IArtist
[
    object,
    uuid("C78F9C90-F937-4CD5-B737-B9AE2B1BC42D"),
    helpstring("ICowboy Interface"),
    pointer_default(unique)
]
__interface IArtist :
IUnknown {
    HRESULT Draw();
};



// CAcePowell

[
    coclass,
    threading("apartment"),
    vi_progid("AcePowell.AcePowell"),
    progid("AcePowell.AcePowell.1"),
    version(1.0),
    uuid("17287156-31BC-4C50-8FFD-40DE67B02BA2"),
    helpstring("AcePowell Class")
]
class ATL_NO_VTABLE CAcePowell :
    public ICowboy,
    public IArtist,
    public IArchitect
{
public:
    CAcePowell()
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

    STDMETHOD(ICowboy::Draw)();
    STDMETHOD(IArtist::Draw)();
    //STDMETHOD(IArchitect::Draw)();

    //STDMETHODIMP ICowboy::Draw(void)
    //{
    //  return S_OK;
    //}

    //STDMETHODIMP IArtist::Draw(void)
    //{
    //  return S_OK;
    //}

    STDMETHODIMP IArchitect::Draw(void)
    {
        return S_OK;
    }
};

STDMETHODIMP CAcePowell::ICowboy::Draw(void)
{
    return S_OK;
}

STDMETHODIMP CAcePowell::IArtist::Draw(void)
{
    return S_OK;
}

//STDMETHODIMP CAcePowell::IArchitect::Draw(void)
//{
//  return S_OK;
//}


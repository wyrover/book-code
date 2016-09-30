// CoAcePowell.h : Declaration of the CAcePowell

#pragma once
#include "resource.h"       // main symbols

#include "AcePowellNoAttr.h"


// CAcePowell

class ATL_NO_VTABLE CAcePowell :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAcePowell, &CLSID_AcePowell>,
    public ICowboy,
    public IArtist
{
public:
    CAcePowell()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ACEPOWELL)


    BEGIN_COM_MAP(CAcePowell)
    COM_INTERFACE_ENTRY(ICowboy)
    COM_INTERFACE_ENTRY(IArtist)
    END_COM_MAP()


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

    //STDMETHODIMP ICowboy::Draw()
    //{
    //  return S_OK;
    //}

    //STDMETHODIMP IArtist::Draw()
    //{
    //  return S_OK;
    //}
};

STDMETHODIMP CAcePowell::ICowboy::Draw()
{
    return S_OK;
}

STDMETHODIMP CAcePowell::IArtist::Draw()
{
    return S_OK;
}

OBJECT_ENTRY_AUTO(__uuidof(AcePowell), CAcePowell)

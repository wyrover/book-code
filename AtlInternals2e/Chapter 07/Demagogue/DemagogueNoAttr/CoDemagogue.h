// CoDemagogue.h : Declaration of the CDemagogue

#pragma once
#include "resource.h"       // main symbols

#include "DemagogueNoAttr.h"


// CDemagogue

class ATL_NO_VTABLE CDemagogue :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDemagogue, &CLSID_Demagogue>,
    public IDemagogue,
    public IPersistPropertyBagImpl<CDemagogue>
{
public:
    CDemagogue()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_DEMAGOGUE)


    BEGIN_COM_MAP(CDemagogue)
    COM_INTERFACE_ENTRY(IDemagogue)
    COM_INTERFACE_ENTRY2(IPersist, IPersistPropertyBag)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    END_COM_MAP()

    BEGIN_PROP_MAP(CDemagogue)
    PROP_DATA_ENTRY("Volume", m_dVolume, VT_R8)
    END_PROP_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:
    STDMETHOD(get_Volume)(DOUBLE* pVal);
    STDMETHOD(put_Volume)(DOUBLE newVal);

    double m_dVolume;
    bool m_bRequiresSave;
};

OBJECT_ENTRY_AUTO(__uuidof(Demagogue), CDemagogue)

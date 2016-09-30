// CoDemagogue.h : Declaration of the CDemagogue

#pragma once
#include "resource.h"       // main symbols

#include "SpeakerNoAttr.h"
#include "_ISpeakerEvents_CP.h"


// CDemagogue

class ATL_NO_VTABLE CDemagogue :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDemagogue, &CLSID_Demagogue>,
    public ISupportErrorInfo,
    public IConnectionPointContainerImpl<CDemagogue>,
    public CProxy_ISpeakerEvents<CDemagogue>,
    public IDispatchImpl<ISpeaker, &__uuidof(ISpeaker), &LIBID_SpeakerNoAttrLib, LIBRARY_MAJOR, LIBRARY_MINOR>,
    public IDispatchImpl<INamedObject, &__uuidof(INamedObject), &LIBID_SpeakerNoAttrLib, LIBRARY_MAJOR, LIBRARY_MINOR>

{
public:

    DECLARE_REGISTRY_RESOURCEID(IDR_DEMAGOGUE)


    BEGIN_COM_MAP(CDemagogue)
    COM_INTERFACE_ENTRY(ISpeaker)
    COM_INTERFACE_ENTRY2(IDispatch, ISpeaker)
    COM_INTERFACE_ENTRY(INamedObject)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    END_COM_MAP()

    BEGIN_CONNECTION_POINT_MAP(CDemagogue)
    CONNECTION_POINT_ENTRY(__uuidof(_ISpeakerEvents))
    END_CONNECTION_POINT_MAP()

public:
// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

// INamedObject
    STDMETHOD(get_Name)(BSTR * pVal)
    {
        if (pVal == NULL) return E_POINTER;

        *pVal = m_bstrName.Copy();
        return S_OK;
    }
    STDMETHOD(put_Name)(BSTR pVal)
    {
        m_bstrName = pVal;
        return S_OK;
    }

    STDMETHOD(get_Volume)(LONG* pVal);
    STDMETHOD(put_Volume)(LONG newVal);
    STDMETHOD(get_Speech)(BSTR* pVal);
    STDMETHOD(put_Speech)(BSTR newVal);
    STDMETHOD(Speak)(void);

private:
    CComBSTR m_bstrName;
    long     m_nVolume;
    CComBSTR m_bstrSpeech;
};

OBJECT_ENTRY_AUTO(__uuidof(Demagogue), CDemagogue)

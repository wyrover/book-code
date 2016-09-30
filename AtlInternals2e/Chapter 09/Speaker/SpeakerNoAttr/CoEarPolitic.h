// CoEarPolitic.h : Declaration of the CEarPolitic

#pragma once
#include "resource.h"       // main symbols

#include "SpeakerNoAttr.h"

static const int  DEFENDANT_SOURCE_ID = 0;
static const int  PLAINTIFF_SOURCE_ID = 1;

class CEarPolitic;

typedef IDispEventImpl<DEFENDANT_SOURCE_ID, CEarPolitic, &__uuidof(_ISpeakerEvents), &LIBID_SpeakerNoAttrLib, LIBRARY_MAJOR, LIBRARY_MINOR>
DefendantEventImpl;

typedef IDispEventSimpleImpl<PLAINTIFF_SOURCE_ID, CEarPolitic, &__uuidof(_ISpeakerEvents)>
PlaintiffEventImpl;

// CEarPolitic

class ATL_NO_VTABLE CEarPolitic :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CEarPolitic, &CLSID_EarPolitic>,
    public ISupportErrorInfo,
    public DefendantEventImpl,
    public PlaintiffEventImpl,
    public IDispatchImpl<IListener, &__uuidof(IListener), &LIBID_SpeakerNoAttrLib, LIBRARY_MAJOR, LIBRARY_MINOR>,
    public IDispatchImpl<INamedObject, &__uuidof(INamedObject), &LIBID_SpeakerNoAttrLib, LIBRARY_MAJOR, LIBRARY_MINOR>
{
public:

    DECLARE_REGISTRY_RESOURCEID(IDR_EARPOLITIC)


    BEGIN_COM_MAP(CEarPolitic)
    COM_INTERFACE_ENTRY(IListener)
    COM_INTERFACE_ENTRY(INamedObject)
    COM_INTERFACE_ENTRY2(IDispatch, IListener)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

    BEGIN_SINK_MAP(CEarPolitic)
    SINK_ENTRY_EX(DEFENDANT_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONWHISPER, OnHearDefendantWhisper)
    SINK_ENTRY_EX(DEFENDANT_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONTALK,    OnHearDefendantTalk)
    SINK_ENTRY_EX(DEFENDANT_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONYELL,    OnHearDefendantYell)

    SINK_ENTRY_INFO(PLAINTIFF_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONWHISPER, OnHearPlaintiffWhisper, &OnHearWhisperInfo)
    SINK_ENTRY_INFO(PLAINTIFF_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONTALK,    OnHearPlaintiffTalk,    &OnHearTalkInfo)
    SINK_ENTRY_INFO(PLAINTIFF_SOURCE_ID, __uuidof(_ISpeakerEvents), DISPID_ONYELL,    OnHearPlaintiffYell,    &OnHearYellInfo)
    END_SINK_MAP()

public:
// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

    DECLARE_PROTECT_FINAL_CONSTRUCT()

// IListener
    STDMETHOD(ListenTo)(SpeakerRole role, IUnknown* pSpeaker);
    STDMETHOD(StopListening)(SpeakerRole role);

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

// ISpeakerEvents
    void __stdcall OnHearDefendantWhisper(BSTR bstrText);
    void __stdcall OnHearDefendantTalk(BSTR bstrText);
    void __stdcall OnHearDefendantYell(BSTR bstrText);
    void __stdcall OnHearPlaintiffWhisper(BSTR bstrText);
    void __stdcall OnHearPlaintiffTalk(BSTR bstrText);
    void __stdcall OnHearPlaintiffYell(BSTR bstrText);

private:
    CComBSTR            m_bstrName;
    CComPtr<IUnknown>   m_spDefendant;
    CComPtr<IUnknown>   m_spPlaintiff;

    void CreateText(CComBSTR& bstrTitle, LPCOLESTR strRole, LPCOLESTR strAction, LPUNKNOWN punk);

    static _ATL_FUNC_INFO OnHearWhisperInfo;
    static _ATL_FUNC_INFO OnHearTalkInfo;
    static _ATL_FUNC_INFO OnHearYellInfo;
};

OBJECT_ENTRY_AUTO(__uuidof(EarPolitic), CEarPolitic)

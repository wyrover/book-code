// CoEarPolitic.h : Declaration of the CEarPolitic

#pragma once
#include "resource.h"       // main symbols
#include "CoDemagogue.h"

// IListener
[export]
typedef enum SpeakerRole { Defendant, Plaintiff } SpeakerRole ;
[
    object,
    uuid("CC46569D-9123-435E-AE10-06DF0FCA6767"),
    dual,   helpstring("IListener Interface"),
    pointer_default(unique)
]
__interface IListener :
IDispatch {
    [id(1), helpstring("method ListenTo")] HRESULT ListenTo([in] SpeakerRole role, [in] IUnknown * pSpeaker);
    [id(2), helpstring("method StopListening")] HRESULT StopListening([in] SpeakerRole role, [in] IUnknown * pSpeaker);
};


// CEarPolitic

[
    coclass,
    event_receiver(com),
    threading("apartment"),
    support_error_info("IListener"),
    vi_progid("Speaker.EarPolitic"),
    progid("Speaker.EarPolitic.1"),
    version(1.0),
    uuid("D6C76965-DF46-45B7-A6BE-8D4A943C39A1"),
    helpstring("EarPolitic Class")
]
class ATL_NO_VTABLE CEarPolitic :
//public _ISpeakerEvents,
    public IDispatchImpl<IListener, &__uuidof(IListener), &CAtlModule::m_libid, LIBRARY_MAJOR, LIBRARY_MINOR>
{
public:

    DECLARE_PROTECT_FINAL_CONSTRUCT()

// IListener
    STDMETHOD(ListenTo)(SpeakerRole role, IUnknown* pSpeaker);
    STDMETHOD(StopListening)(SpeakerRole role, IUnknown* pSpeaker);

    void __stdcall OnHearDefendantWhisper(BSTR bstrText);
    void __stdcall OnHearDefendantTalk(BSTR bstrText);
    void __stdcall OnHearDefendantYell(BSTR bstrText);
    void __stdcall OnHearPlaintiffWhisper(BSTR bstrText);
    void __stdcall OnHearPlaintiffTalk(BSTR bstrText);
    void __stdcall OnHearPlaintiffYell(BSTR bstrText);

// ISpeakerEvents
    HRESULT OnWhisper(BSTR bstrSpeech)
    {
        OnHearDefendantWhisper(bstrSpeech);
        return S_OK;
    }
    HRESULT OnTalk(BSTR bstrSpeech)
    {
        OnHearDefendantTalk(bstrSpeech);
        return S_OK;
    }
    HRESULT OnYell(BSTR bstrSpeech)
    {
        OnHearDefendantYell(bstrSpeech);
        return S_OK;
    }

private:
    CComBSTR            m_bstrName;
    CComPtr<IUnknown>   m_spDefendant;
    CComPtr<IUnknown>   m_spPlaintiff;

    void CreateText(CComBSTR& bstrTitle, LPCOLESTR strRole, LPCOLESTR strAction, LPUNKNOWN punk);
};


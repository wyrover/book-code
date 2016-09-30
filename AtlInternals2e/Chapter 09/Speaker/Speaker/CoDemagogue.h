// CoDemagogue.h : Declaration of the CDemagogue

#pragma once
#include "resource.h"       // main symbols


// IDemagogue
const int DISPID_VOLUME         = 1;
[idl_quote("const int DISPID_VOLUME         = 1;")];
const int DISPID_SPEECH         = 2;
[idl_quote("const int DISPID_SPEECH         = 2;")];
const int DISPID_SPEAK          = 3;
[idl_quote("const int DISPID_SPEAK          = 3;")];
[
    object,
    uuid("4C5AC360-51D5-4DA2-84F2-992D8584B807"),
    dual,   helpstring("ISpeaker Interface"),
    pointer_default(unique)
]
__interface ISpeaker :
IDispatch {
    [propget, id(DISPID_VOLUME), helpstring("property Volume")] HRESULT Volume([out, retval] LONG * pVal);
    [propput, id(DISPID_VOLUME), helpstring("property Volume")] HRESULT Volume([in] LONG newVal);
    [propget, id(DISPID_SPEECH), helpstring("property Speech")] HRESULT Speech([out, retval] BSTR * pVal);
    [propput, id(DISPID_SPEECH), helpstring("property Speech")] HRESULT Speech([in] BSTR newVal);
    [id(DISPID_SPEAK), helpstring("method Speak")] HRESULT Speak(void);
};

// INamedObject
const int DISPID_NAME           = 1;
[idl_quote("const int DISPID_NAME           = 1;")];
[
    object,
    uuid(450D6780 - FC73 - 11d1 - 8CC3 - 00A0C9C8E50D),
    dual,
    helpstring("INamedObject Interface"),
    pointer_default(unique)
]
__interface INamedObject :
IDispatch {
    [propget, id(DISPID_NAME), helpstring("property Name")] HRESULT Name([out, retval] BSTR * pVal);
    [propput, id(DISPID_NAME), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
};

// _IDemagogueEvents
const int DISPID_ONWHISPER      = 1;
[idl_quote("const int DISPID_ONWHISPER      = 1;")];
const int DISPID_ONTALK         = 2;
[idl_quote("const int DISPID_ONTALK         = 2;")];
const int DISPID_ONYELL         = 3;
[idl_quote("const int DISPID_ONYELL         = 3;")];
[
    dispinterface,
    uuid("EF75866F-28FA-4411-8A7D-AE7B65BB7F95"),
    helpstring("_ISpeakerEvents Interface")
]
__interface _ISpeakerEvents {
    [id(DISPID_ONWHISPER), helpstring("method OnWhisper")] HRESULT OnWhisper(BSTR bstrSpeech);
    [id(DISPID_ONTALK), helpstring("method OnTalk")]    HRESULT OnTalk(BSTR bstrSpeech);
    [id(DISPID_ONYELL), helpstring("method OnYell")]    HRESULT OnYell(BSTR bstrSpeech);
};

// CDemagogue

[
    coclass,
    event_source(com),
    threading("apartment"),
    support_error_info("ISpeaker"),
    vi_progid("Speaker.Demagogue"),
    progid("Speaker.Demagogue.1"),
    version(1.0),
    uuid("798AC41B-03D6-45A7-8311-8056A7EF0CFD"),
    helpstring("Demagogue Class"),
    default(INamedObject, _ISpeakerEvents)
]
class ATL_NO_VTABLE CDemagogue :
    public ISpeaker,
    public INamedObject
{
public:

    __event __interface _ISpeakerEvents;

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


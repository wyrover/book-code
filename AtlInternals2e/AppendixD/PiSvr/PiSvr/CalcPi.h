// CalcPi.h : Declaration of the CCalcPi

#pragma once
#include "resource.h"       // main symbols



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
    #error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// ICalcPi
[
    object,
    uuid("8E3ABD67-5075-4C38-BA00-8289E336E7F9"),
    dual, helpstring("ICalcPi Interface"),
    pointer_default(unique)
]
__interface ICalcPi :
IDispatch {
    [propget, id(1), helpstring("property Digits")] HRESULT Digits([out, retval] LONG * pVal);
    [propput, id(1), helpstring("property Digits")] HRESULT Digits([in] LONG newVal);
    [id(2), helpstring("method CalcPi")] HRESULT CalcPi([out, retval] BSTR * pbstrPi);
};


// _ICalcPiEvents
[
    dispinterface,
    uuid("9822AB1A-8031-4914-BD73-3459A91B98A9"),
    helpstring("_ICalcPiEvents Interface")
]
__interface _ICalcPiEvents {
    [id(1), helpstring("method OnDigit")] void OnDigit([in] SHORT nIndex, [in] SHORT nDigit);
};


// CCalcPi

[
    coclass,
    default(ICalcPi, _ICalcPiEvents),
    threading(apartment),
    support_error_info("ICalcPi"),
    event_source(com),
    vi_progid("PiSvr.CalcPi"),
    progid("PiSvr.CalcPi.1"),
    version(1.0),
    uuid("A892A09D-98C9-4AD4-98C5-769F7743F204"),
    helpstring("CalcPi Class")
]
class ATL_NO_VTABLE CCalcPi :
    public ICalcPi
{
public:
    CCalcPi()
    {
    }

    __event __interface _ICalcPiEvents;


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:
    STDMETHOD(get_Digits)(LONG* pVal);
    STDMETHOD(put_Digits)(LONG newVal);
    STDMETHOD(CalcPi)(BSTR* pbstrPi);

private:
    long m_nDigits;
};


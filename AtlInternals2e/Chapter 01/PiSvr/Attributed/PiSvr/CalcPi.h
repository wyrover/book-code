// CalcPi.h : Declaration of the CCalcPi

#pragma once
#include "resource.h"       // main symbols


// ICalcPi
[
    object,
    uuid("6E9761C5-336F-4C41-AF16-57BBDE593FF0"),
    dual,   helpstring("ICalcPi Interface"),
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
    uuid("B830F523-D87B-434F-933A-623CEF6FC4AA"),
    helpstring("_ICalcPiEvents Interface")
]
__interface _ICalcPiEvents {
    [id(1)] void OnDigit([in] short nIndex, [in] short nDigit);
};


// IAdvertiseMyself
[
    object,
    uuid("48AC9A07-9E9E-438B-AC87-DCF0F62E729E"),
    helpstring("IAdvertiseMyself Interface"),
    pointer_default(unique)
]
__interface IAdvertiseMyself :
IUnknown {
    [helpstring("method ShowAd")] HRESULT ShowAd([in] BSTR bstrClient);
};

// CCalcPi
[
    coclass,
    threading("apartment"),
    support_error_info("ICalcPi"),
    event_source("com"),
    vi_progid("PiSvr.CalcPi"),
    progid("PiSvr.CalcPi.1"),
    version(1.0),
    uuid("E5F91723-E7AD-4596-AC90-17586D400BF7"),
    helpstring("CalcPi Class"),
    implements_category("CATID_SafeForScripting"),
    implements_category("CATID_SafeForInitializing")
]
class ATL_NO_VTABLE CCalcPi :
    public ICalcPi,
    public IAdvertiseMyself,
    public IPersistPropertyBagImpl<CCalcPi>
//  public IDispatchImpl<ICalcPi, &__uuidof(ICalcPi), &__uuidof(PiSvr), /*wMajor =*/ 1, /*wMinor=*/ 0>

{
public:
    CCalcPi()
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

    BEGIN_PROP_MAP(CCalcPi)
    PROP_ENTRY("Digits", 1, CLSID_NULL)
    END_PROP_MAP()


    // ICalcPi
    STDMETHOD(get_Digits)(LONG* pVal);
    STDMETHOD(put_Digits)(LONG newVal);
    STDMETHOD(CalcPi)(BSTR* pbstrPi);


    // IAdvertiseMyself Methods
public:
    STDMETHOD(ShowAd)(BSTR  bstrClient)
    {
        // Add your function implementation here.
        return E_NOTIMPL;
    }

public:
    BOOL m_bRequiresSave;

    __event __interface _ICalcPiEvents;

private:
    long m_nDigits;
};


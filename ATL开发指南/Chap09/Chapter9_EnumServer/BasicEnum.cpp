// BasicEnum.cpp : Implementation of CBasicEnum
#include "stdafx.h"
#include "Chapter9_EnumServer.h"
#include "BasicEnum.h"

/////////////////////////////////////////////////////////////////////////////
// CBasicEnum

STDMETHODIMP CBasicEnum::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IBasicEnum
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CBasicEnum::get_Enum(IEnumString **ppUnk)
{
    // Create an enumerator of type IEnumString
    typedef CComObject<CComEnum< IEnumString, &IID_IEnumString,
            LPOLESTR, _Copy<LPOLESTR> > >
            enumString;
    enumString* pEnum = 0;
    enumString::CreateInstance(&pEnum);
    HRESULT hr = pEnum->Init(m_vecStrings.begin(),
                             m_vecStrings.end(),
                             0,
                             AtlFlagCopy);

    if (SUCCEEDED(hr))
        hr = pEnum->QueryInterface(IID_IEnumString,
                                   (void**) ppUnk);

    if (FAILED(hr))
        delete pEnum;

    return S_OK;
}

// CoDemagogue.cpp : Implementation of CDemagogue

#include "stdafx.h"
#include "CoDemagogue.h"


// CDemagogue

STDMETHODIMP CDemagogue::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_ISpeaker
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CDemagogue::get_Volume(LONG* pVal)
{
    if (NULL == pVal) return E_POINTER;

    *pVal = m_nVolume;
    return S_OK;
}

STDMETHODIMP CDemagogue::put_Volume(LONG newVal)
{
    m_nVolume = newVal ;
    return S_OK;
}

STDMETHODIMP CDemagogue::get_Speech(BSTR* pVal)
{
    if (NULL == pVal) return E_POINTER ;

    *pVal = m_bstrSpeech.Copy() ;
    return S_OK;
}

STDMETHODIMP CDemagogue::put_Speech(BSTR newVal)
{
    m_bstrSpeech = newVal ;
    return S_OK;
}

STDMETHODIMP CDemagogue::Speak(void)
{
    if (m_nVolume <= -100)
        Fire_OnWhisper(m_bstrSpeech);

    if (m_nVolume >= 100)
        Fire_OnYell(m_bstrSpeech);

    Fire_OnTalk(m_bstrSpeech);
    return S_OK;
}

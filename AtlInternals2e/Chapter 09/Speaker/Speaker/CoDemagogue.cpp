// CoDemagogue.cpp : Implementation of CDemagogue

#include "stdafx.h"
#include "CoDemagogue.h"


// CDemagogue

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
        __raise OnWhisper(m_bstrSpeech);

    if (m_nVolume >= 100)
        __raise OnYell(m_bstrSpeech);

    HRESULT hr = __raise OnTalk(m_bstrSpeech);
    return S_OK;
}
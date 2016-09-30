// CoEarPolitic.cpp : Implementation of CEarPolitic

#include "stdafx.h"
#include "CoEarPolitic.h"
#include "CoDemagogue.h"


// CEarPolitic

void __stdcall CEarPolitic::OnHearDefendantWhisper(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("defendant"), OLESTR("whispering"), m_spDefendant);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void CEarPolitic::OnHearDefendantTalk(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("defendant"), OLESTR("talking"), m_spDefendant);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void __stdcall CEarPolitic::OnHearDefendantYell(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("defendant"), OLESTR("yelling"), m_spDefendant);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void __stdcall CEarPolitic::OnHearPlaintiffWhisper(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("plaintiff"), OLESTR("whispering"), m_spPlaintiff);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void __stdcall CEarPolitic::OnHearPlaintiffTalk(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("plaintiff"), OLESTR("talking"), m_spPlaintiff);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void __stdcall CEarPolitic::OnHearPlaintiffYell(BSTR bstrText)
{
    USES_CONVERSION;
    CComBSTR title ;
    CreateText(title, OLESTR("plaintiff"), OLESTR("yelling"), m_spPlaintiff);
    MessageBox(NULL, OLE2CT(bstrText), OLE2CT(title), MB_OK);
}

void CEarPolitic::CreateText(CComBSTR& text, LPCOLESTR strRole, LPCOLESTR strAction, LPUNKNOWN punk)
{
    text.Empty();
    text = m_bstrName;
    text += OLESTR(" hears the ");
    text += strRole;
    text += OLESTR(" (");
    CComQIPtr<INamedObject> pno = punk;
    CComBSTR bstrName;
    HRESULT hr = pno->get_Name(&bstrName) ;
    text.AppendBSTR(bstrName);
    text += OLESTR(") ");
    text += strAction;
}

STDMETHODIMP CEarPolitic::ListenTo(SpeakerRole role, IUnknown *pSpeaker)
{
    HRESULT hr = StopListening(role, pSpeaker) ; // Validates role

    if (FAILED(hr)) return hr ;

    switch (role) {
    case Defendant: {
        long lRet = 0;
        lRet |= __hook(&_ISpeakerEvents::OnTalk, pSpeaker, OnHearDefendantTalk);
        lRet |= __hook(&_ISpeakerEvents::OnWhisper, pSpeaker, OnHearDefendantWhisper);
        lRet |= __hook(&_ISpeakerEvents::OnYell, pSpeaker, OnHearDefendantYell);

        //lRet |= __hook(_ISpeakerEvents, pSpeaker);
        if (lRet == 0)
            m_spDefendant = pSpeaker ;
        else
            Error(OLESTR("The defendant does not support listening"), __uuidof(IListener), hr);

        break;
    }

    case Plaintiff: {
        long lRet = 0;

        //lRet |= __hook(&_ISpeakerEvents::OnTalk, pSpeaker, OnHearPlaintiffTalk);
        //lRet |= __hook(&_ISpeakerEvents::OnWhisper, pSpeaker, OnHearPlaintiffWhisper);
        //lRet |= __hook(&_ISpeakerEvents::OnYell, pSpeaker, OnHearPlaintiffYell);
        //lRet |= __hook(_ISpeakerEvents, pSpeaker);
        if (lRet == 0)
            m_spPlaintiff = pSpeaker ;
        else
            Error(OLESTR("The Plaintiff does not support listening"), __uuidof(IListener), hr);

        break;
    }
    }

    return hr;
}

STDMETHODIMP CEarPolitic::StopListening(SpeakerRole role, IUnknown* pSpeaker)
{
    HRESULT hr = S_OK ;

    switch (role) {
    case Defendant: {
        long lRet = 0;

        if (m_spDefendant) {
            //lRet |= __unhook(&_ISpeakerEvents::OnTalk, pSpeaker, &CEarPolitic::OnHearDefendantTalk);
            //lRet |= __unhook(&_ISpeakerEvents::OnWhisper, pSpeaker, &CEarPolitic::OnHearDefendantWhisper);
            //lRet |= __unhook(&_ISpeakerEvents::OnYell, pSpeaker, &CEarPolitic::OnHearDefendantYell);
            //lRet |= __unhook(_ISpeakerEvents, pSpeaker);
            hr = lRet ? E_FAIL : S_OK;
        }

        if (FAILED(hr))
            Error(OLESTR("Unexpected error trying to stop listening to the defendant"), __uuidof(IListener), hr);

        m_spDefendant = NULL;
        break;
    }

    case Plaintiff: {
        long lRet = 0;

        if (m_spPlaintiff) {
            //lRet |= __unhook(&_ISpeakerEvents::OnTalk, pSpeaker, &CEarPolitic::OnHearPlaintiffTalk);
            //lRet |= __unhook(&_ISpeakerEvents::OnWhisper, pSpeaker, &CEarPolitic::OnHearPlaintiffWhisper);
            //lRet |= __unhook(&_ISpeakerEvents::OnYell, pSpeaker, &CEarPolitic::OnHearPlaintiffYell);
            //lRet |= __unhook(_ISpeakerEvents, pSpeaker);
            hr = lRet ? E_FAIL : S_OK;
        }

        if (FAILED(hr))
            Error(OLESTR("Unexpected error trying to stop listening to the Plaintiff"), __uuidof(IListener), hr);

        m_spPlaintiff = NULL;
        break;
    }

    default:
        hr = E_INVALIDARG;
        break;
    }

    return hr;
}

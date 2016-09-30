#pragma once

template<class T>
class CProxy_ISpeakerEvents :
    public IConnectionPointImpl<T, &__uuidof(_ISpeakerEvents)>
{
public:
    HRESULT Fire_OnWhisper(BSTR  bstrSpeech)
    {
        HRESULT hr = S_OK;
        T * pThis = static_cast<T *>(this);
        int cConnections = m_vec.GetSize();

        for (int iConnection = 0; iConnection < cConnections; iConnection++) {
            pThis->Lock();
            CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
            pThis->Unlock();
            IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

            if (pConnection) {
                CComVariant avarParams[1];
                avarParams[0] = bstrSpeech;
                DISPPARAMS params = { avarParams, NULL, 1, 0 };
                hr = pConnection->Invoke(DISPID_ONWHISPER, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
    HRESULT Fire_OnTalk(BSTR  bstrSpeech)
    {
        HRESULT hr = S_OK;
        T * pThis = static_cast<T *>(this);
        int cConnections = m_vec.GetSize();

        for (int iConnection = 0; iConnection < cConnections; iConnection++) {
            pThis->Lock();
            CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
            pThis->Unlock();
            IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

            if (pConnection) {
                CComVariant avarParams[1];
                avarParams[0] = bstrSpeech;
                DISPPARAMS params = { avarParams, NULL, 1, 0 };
                hr = pConnection->Invoke(DISPID_ONTALK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
    HRESULT Fire_OnYell(BSTR  bstrSpeech)
    {
        HRESULT hr = S_OK;
        T * pThis = static_cast<T *>(this);
        int cConnections = m_vec.GetSize();

        for (int iConnection = 0; iConnection < cConnections; iConnection++) {
            pThis->Lock();
            CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
            pThis->Unlock();
            IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

            if (pConnection) {
                CComVariant avarParams[1];
                avarParams[0] = bstrSpeech;
                DISPPARAMS params = { avarParams, NULL, 1, 0 };
                hr = pConnection->Invoke(DISPID_ONYELL, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
};


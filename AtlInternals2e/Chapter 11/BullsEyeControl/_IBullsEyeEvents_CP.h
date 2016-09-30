#pragma once

template<class T>
class CProxy_IBullsEyeEvents :
    public IConnectionPointImpl<T, &__uuidof(_IBullsEyeEvents)>
{
public:
    HRESULT Fire_OnRingHit(short ringNum)
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
                avarParams[0] = ringNum;
                avarParams[0].vt = VT_I2;
                DISPPARAMS params = { avarParams, NULL, 1, 0 };
                hr = pConnection->Invoke(DISPID_ONRINGHIT, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
    HRESULT Fire_OnScoreChanged(long ringValue)
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
                avarParams[0] = ringValue;
                avarParams[0].vt = VT_I4;
                DISPPARAMS params = { avarParams, NULL, 1, 0 };
                hr = pConnection->Invoke(DISPID_ONSCORECHANGED, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
};


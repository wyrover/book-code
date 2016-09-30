#pragma once

template<class T>
class CProxy_ICalcPiEvents :
    public IConnectionPointImpl<T, &__uuidof(_ICalcPiEvents)>
{
public:
    HRESULT Fire_OnDigit(short  nIndex,  short  nDigit)
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
                CComVariant avarParams[2];
                avarParams[1] = nIndex;
                avarParams[0] = nDigit;
                DISPPARAMS params = { avarParams, NULL, 2, 0 };
                hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
};


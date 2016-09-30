#ifndef _CONNECTIONCOMCP_H_
#define _CONNECTIONCOMCP_H_

template <class T>
class CProxy_IAddEvents : public IConnectionPointImpl<T, &IID__IAddEvents, CComDynamicUnkArray>
{
    //Warning this class may be recreated by the wizard.
public:
    HRESULT Fire_ExecutionOver(INT Result)
    {
        HRESULT ret;
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++) {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            _IAddEvents* p_IAddEvents = reinterpret_cast<_IAddEvents*>(sp.p);

            if (p_IAddEvents != NULL)
                ret = p_IAddEvents->ExecutionOver(Result);
        }

        return ret;
    }
};
#endif
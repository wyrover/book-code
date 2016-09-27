#ifndef _POSTITCP_H_
#define _POSTITCP_H_


template <class T>
class CProxy_IPostItCtlEvents : public IConnectionPointImpl<T, &DIID__IPostItCtlEvents, CComDynamicUnkArray>
{
    //Warning this class may be recreated by the wizard.
public:
    VOID Fire_Click()
    {
        T* pT = static_cast<T*>(this);
        int nConnectionIndex;
        int nConnections = m_vec.GetSize();

        for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++) {
            pT->Lock();
            CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
            pT->Unlock();
            IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);

            if (pDispatch != NULL) {
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                pDispatch->Invoke(DISPID_CLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }
        }
    }
};
#endif
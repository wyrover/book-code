
//////////////////////////////////////////////////////////////////////////////
// CProxyIPostItEvents
template <class T>
class CProxyIPostItEvents : public IConnectionPointImpl<T, &DIID__PostItEvents, CComDynamicUnkArray>
{
public:
//methods:
//IPostItEvents : IDispatch
public:
    void Fire_Click()
    {
        T* pT = (T*)this;
        pT->Lock();
        IUnknown** pp = m_vec.begin();

        while (pp < m_vec.end()) {
            if (*pp != NULL) {
                DISPPARAMS disp = { NULL, NULL, 0, 0 };
                IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
                pDispatch->Invoke(DISPID_CLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
            }

            pp++;
        }

        pT->Unlock();
    }

};


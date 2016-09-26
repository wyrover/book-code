// Account.h : Declaration of the CAccount

#ifndef __ACCOUNT_H_
#define __ACCOUNT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAccount
class ATL_NO_VTABLE CAccount :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAccount, &CLSID_Account>,
    public IDispatchImpl<IAccount, &IID_IAccount, &LIBID_ATLSAMPLELib>
{
public:
    CAccount(): m_balance(1000)
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ACCOUNT)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CAccount)
    COM_INTERFACE_ENTRY(IAccount)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

// IAccount
public:
    STDMETHOD(Post)(/*[in]*/long lAmount, /*[out,retval]*/BSTR *pbstrResult);
    STDMETHOD(get_Balance)(/*[out, retval]*/ long *pVal);
    STDMETHOD(put_Balance)(/*[in]*/ long newVal);
private:
    long m_balance;
};

#endif //__ACCOUNT_H_

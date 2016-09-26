// QuoteSupplier.h : Declaration of the CQuoteSupplier

#ifndef __QUOTESUPPLIER_H_
#define __QUOTESUPPLIER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CQuoteSupplier
class ATL_NO_VTABLE CQuoteSupplier :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CQuoteSupplier, &CLSID_QuoteSupplier>,
    public IDispatchImpl<IQuoteSupplier, &IID_IQuoteSupplier, &LIBID_DCOMSERVLib>
{
public:
    CQuoteSupplier()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_QUOTESUPPLIER)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CQuoteSupplier)
    COM_INTERFACE_ENTRY(IQuoteSupplier)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

// IQuoteSupplier
public:
    STDMETHOD(GetQuote)(/*[in]*/ BSTR Ticker, /*[out, retval]*/ double * Price);
};
extern CServiceModule _Module;
#endif //__QUOTESUPPLIER_H_

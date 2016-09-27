// Receipt.h : Declaration of the CReceipt

#ifndef __RECEIPT_H_
#define __RECEIPT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReceipt
class ATL_NO_VTABLE CReceipt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CReceipt, &CLSID_Receipt>,
    public ISupportErrorInfo,
    public IDispatchImpl<IReceipt, &IID_IReceipt, &LIBID_BANKLib>
{
public:
    CReceipt()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_RECEIPT)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CReceipt)
    COM_INTERFACE_ENTRY(IReceipt)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IReceipt
public:
    STDMETHOD(GetNextReceipt)(/*[out,retval]*/ long* plReceiptNo);
};

#endif //__RECEIPT_H_

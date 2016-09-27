// UpdateReceipt.h : Declaration of the CUpdateReceipt

#ifndef __UPDATERECEIPT_H_
#define __UPDATERECEIPT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUpdateReceipt
class ATL_NO_VTABLE CUpdateReceipt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CUpdateReceipt, &CLSID_UpdateReceipt>,
    public ISupportErrorInfo,
    public IDispatchImpl<IUpdateReceipt, &IID_IUpdateReceipt, &LIBID_BANKLib>
{
public:
    CUpdateReceipt()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_UPDATERECEIPT)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CUpdateReceipt)
    COM_INTERFACE_ENTRY(IUpdateReceipt)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IUpdateReceipt
public:
    STDMETHOD(Update)(/*[out,retval]*/ long* plReceiptNo);
};

#endif //__UPDATERECEIPT_H_

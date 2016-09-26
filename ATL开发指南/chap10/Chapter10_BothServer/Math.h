
// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CMath, &CLSID_Math>,
    public IMath
{
public:
    CMath()
    {
        m_pUnkMarshaler = NULL;
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)
    DECLARE_NOT_AGGREGATABLE(CMath)
    DECLARE_GET_CONTROLLING_UNKNOWN()

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
    END_COM_MAP()

    HRESULT FinalConstruct()
    {
        return CoCreateFreeThreadedMarshaler(
                   GetControllingUnknown(), &m_pUnkMarshaler.p);
    }

    void FinalRelease()
    {
        m_pUnkMarshaler.Release();
    }

    CComPtr<IUnknown> m_pUnkMarshaler;

// IMath
public:
    STDMETHOD(Divide)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Multiply)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Subtract)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Add)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
};

#endif //__MATH_H_

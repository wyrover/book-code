// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols
#include "Chapter7_CPServerCP.h"

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMath, &CLSID_Math>,
    public IConnectionPointContainerImpl<CMath>,
    public IDispatchImpl<IMath, &IID_IMath, &LIBID_CHAPTER7_CPSERVERLib>,
    public CProxy_IMathEvents< CMath >
{
public:
    CMath()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    END_COM_MAP()

    BEGIN_CONNECTION_POINT_MAP(CMath)
    CONNECTION_POINT_ENTRY(DIID__IMathEvents)
    END_CONNECTION_POINT_MAP()


// IMath
public:
    STDMETHOD(Divide)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Multiply)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Subtract)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
    STDMETHOD(Add)(/*[in]*/ long lOp1, /*[in]*/ long lOp2, /*[out,retval]*/ long* plResult);
};

#endif //__MATH_H_

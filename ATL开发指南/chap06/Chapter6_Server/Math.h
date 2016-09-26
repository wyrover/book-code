// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMath, &CLSID_Math>,
    public ISupportErrorInfo,
    public IDispatchImpl<IMath, &IID_IMath, &LIBID_CHAPTER6_SERVERLib>,
    public IDispatchImpl<IAdvancedMath, &IID_IAdvancedMath, &LIBID_CHAPTER6_SERVERLib>,
    public IDispatchImpl<IMath2, &IID_IMath2, &LIBID_CHAPTER6_SERVERLib>
{
public:
    CMath()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    COM_INTERFACE_ENTRY2(IDispatch, IMath)
    COM_INTERFACE_ENTRY(IAdvancedMath)
    COM_INTERFACE_ENTRY(IMath2)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMath
public:
    STDMETHOD(Divide)(VARIANT varOp1, VARIANT varOp2, VARIANT* pvarResult);
    STDMETHOD(Multiply)(VARIANT varOp1, VARIANT varOp2, VARIANT* pvarResult);
    STDMETHOD(Subtract)(VARIANT varOp1, VARIANT varOp2, VARIANT* pvarResult);
    STDMETHOD(Add)(VARIANT varOp1, VARIANT varOp2, VARIANT* pvarResult);

// IAdvancedMath
public:
    STDMETHOD(Fibonacci)(short sOp, long* plResult);
    STDMETHOD(Factorial)(short sOp, long* plResult);

// IMath2
public:
    STDMETHOD(Sum)(VARIANT varOp1, long* plResult);
};

#endif //__MATH_H_

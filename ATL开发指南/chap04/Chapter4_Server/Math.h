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
    public IMath,
    public IMath2,
    public IAdvancedMath,
    public IComponentInfo
{
public:
    CMath()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    COM_INTERFACE_ENTRY(IMath2)
    COM_INTERFACE_ENTRY(IAdvancedMath)
    COM_INTERFACE_ENTRY(IComponentInfo)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMath
public:
    STDMETHOD(Divide)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Multiply)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Subtract)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Add)(long lOp1, long lOp2, long* plResult);

// IAdvancedMath
public:
    STDMETHOD(Factorial)(short, long*);
    STDMETHOD(Fibonacci)(short, long*);

// IMath2
public:
    STDMETHOD(Sum)(short sArraySize,
                   short sArray[],
                   long* lResult);
    STDMETHOD(Compute)(mathOPERATION enumOp,
                       long lOp1,
                       long lOp2,
                       long* plResult);
    STDMETHOD(get_AdvancedMath)(IAdvancedMath** pVal);

// IComponentInfo
public:
    STDMETHOD(get_Info)(COMPONENT_INFO** ppInfo);
    STDMETHOD(get_Name)(BSTR*);
};

#endif //__MATH_H_

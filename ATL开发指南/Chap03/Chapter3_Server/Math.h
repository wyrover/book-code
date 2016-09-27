// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols
#include "atldevguide.h"

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMath, &CLSID_Math>,
    public IMath,
    public IAdvancedMath
{
public:
    CMath()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

    BEGIN_CATEGORY_MAP(CMath)
    IMPLEMENTED_CATEGORY(CATID_ATLDevGuide)
    END_CATEGORY_MAP()

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    COM_INTERFACE_ENTRY(IAdvancedMath)
    END_COM_MAP()

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

};

#endif //__MATH_H_

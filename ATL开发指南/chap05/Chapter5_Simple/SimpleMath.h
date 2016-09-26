// SimpleMath.h : Declaration of the CSimpleMath

#ifndef __SIMPLEMATH_H_
#define __SIMPLEMATH_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimpleMath
class ATL_NO_VTABLE CSimpleMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CSimpleMath, &CLSID_SimpleMath>,
    public IMath
{
public:
    CSimpleMath()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLEMATH)

    BEGIN_COM_MAP(CSimpleMath)
    COM_INTERFACE_ENTRY(IMath)
    END_COM_MAP()

// IMath
public:
    STDMETHOD(Divide)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Multiply)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Subtract)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Add)(long lOp1, long lOp2, long* plResult);
};

#endif //__SIMPLEMATH_H_

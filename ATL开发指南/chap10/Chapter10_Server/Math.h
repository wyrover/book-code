//
// Math.h : Declaration of the CMath
//

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols

typedef enum mathOPERATOR {
    mathAdd,
    mathSubtract,
    mathMultiply,
    mathDivide
} mathOPERATOR;

struct ThreadParameters {
    mathOPERATOR   op;
    long           lOp1;
    long           lOp2;
    IStream*       pStream;
};

/////////////////////////////////////////////////////////////////////////////
// CMath
class ATL_NO_VTABLE CMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMath, &CLSID_Math>,
    public IMath
{
public:
    CMath()
    {
        m_pCallBack = 0;
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_MATH)

    BEGIN_COM_MAP(CMath)
    COM_INTERFACE_ENTRY(IMath)
    END_COM_MAP()

// IMath
public:
    STDMETHOD(Unadvise)();
    STDMETHOD(Advise)(/*[in]*/ ICallBack* pCallBack);
    STDMETHOD(Divide)(/*[in]*/ long lOp1, /*[in]*/ long lOp2);
    STDMETHOD(Multiply)(/*[in]*/ long lOp1, /*[in]*/ long lOp2);
    STDMETHOD(Subtract)(/*[in]*/ long lOp1, /*[in]*/ long lOp2);
    STDMETHOD(Add)(/*[in]*/ long lOp1, /*[in]*/ long lOp2);

private:
    ICallBack* m_pCallBack;
    HRESULT SimulateLongComputation(mathOPERATOR op, long lOp1, long lOp2);
};

#endif //__MATH_H_

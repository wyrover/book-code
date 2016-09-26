// Math.h : Declaration of the CMath

#ifndef __MATH_H_
#define __MATH_H_

#include "resource.h"       // main symbols

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
    STDMETHOD(Add)(long lOp1, long lOp2);
    STDMETHOD(Multiply)(long lOp1, long lOp2);
    STDMETHOD(Subtract)(long lOp1, long lOp2);
    STDMETHOD(Divide)(long lOp1, long lOp2);
    STDMETHOD(Advise)(ICallBack* pCallBack);
    STDMETHOD(UnAdvise)();

private:
    ICallBack* m_pCallBack;
};

#endif //__MATH_H_

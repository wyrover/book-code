// AdvancedMath.h : Declaration of the CAdvancedMath

#ifndef __ADVANCEDMATH_H_
#define __ADVANCEDMATH_H_

#include "resource.h"       // main symbols

const CLSID CLSID_SimpleMath
    = {0x380F79EB, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

/////////////////////////////////////////////////////////////////////////////
// CAdvancedMath
class ATL_NO_VTABLE CAdvancedMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAdvancedMath, &CLSID_AdvancedMath>,
    public IAdvancedMath,
    public IMath
{
public:
    CAdvancedMath() : m_pSimpleMath(0)
    {
    }

    HRESULT FinalConstruct()
    {
        HRESULT hr = CoCreateInstance(CLSID_SimpleMath,
                                      0,
                                      CLSCTX_INPROC_SERVER,
                                      IID_IMath,
                                      (void**) &m_pSimpleMath);
        return hr;
    }

    void FinalRelease()
    {
        if (m_pSimpleMath)
            m_pSimpleMath->Release();
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ADVANCEDMATH)

    BEGIN_COM_MAP(CAdvancedMath)
    COM_INTERFACE_ENTRY(IAdvancedMath)
    COM_INTERFACE_ENTRY(IMath)
    END_COM_MAP()

// IMath - We now must explicitly implement these methods
public:
    STDMETHOD(Divide)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Multiply)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Subtract)(long lOp1, long lOp2, long* plResult);
    STDMETHOD(Add)(long lOp1, long lOp2, long* plResult);

// IAdvancedMath
public:
    STDMETHOD(Fibonacci)(short sOp, long* plResult);
    STDMETHOD(Factorial)(short sOp, long* plResult);

private:
    IMath* m_pSimpleMath;

};

#endif //__ADVANCEDMATH_H_

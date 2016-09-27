//
// AdvancedMath.h : Declaration of the CAdvancedMath
//

#ifndef __ADVANCEDMATH_H_
#define __ADVANCEDMATH_H_

#include "resource.h"       // main symbols

// Include the CLSID and IID of the inner component
#include "..\Chapter5_Simple\Chapter5_Simple_i.c"

/////////////////////////////////////////////////////////////////////////////
// CAdvancedMath
class ATL_NO_VTABLE CAdvancedMath :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAdvancedMath, &CLSID_AdvancedMath>,
    public IAdvancedMath
{
public:
    CAdvancedMath() : m_pSimpleUnknown(0)
    {
    }

    DECLARE_GET_CONTROLLING_UNKNOWN()
    HRESULT FinalConstruct()
    {
        HRESULT hr;
        hr = CoCreateInstance(CLSID_SimpleMath,
                              GetControllingUnknown(),
                              CLSCTX_INPROC_SERVER,
                              IID_IUnknown,
                              (void**) &m_pSimpleUnknown);
        return hr;
    }

    void FinalRelease()
    {
        if (m_pSimpleUnknown)
            m_pSimpleUnknown->Release();
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ADVANCEDMATH)

    BEGIN_COM_MAP(CAdvancedMath)
    COM_INTERFACE_ENTRY(IAdvancedMath)
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMath, m_pSimpleUnknown)
    END_COM_MAP()

private:
    IUnknown* m_pSimpleUnknown;

// IAdvancedMath
public:
    STDMETHOD(Fibonacci)(short sOp, long* plResult);
    STDMETHOD(Factorial)(short sOp, long* plResult);
};

#endif //__ADVANCEDMATH_H_

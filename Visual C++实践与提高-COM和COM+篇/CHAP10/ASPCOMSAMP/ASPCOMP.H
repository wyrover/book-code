// ASPComp.h : Declaration of the CASPComp

#ifndef __ASPCOMP_H_
#define __ASPCOMP_H_

#include "resource.h"       // main symbols
#include <mtx.h>
#include <asptlb.h>
/////////////////////////////////////////////////////////////////////////////
// CASPComp
class ATL_NO_VTABLE CASPComp :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CASPComp, &CLSID_ASPComp>,
    public IDispatchImpl<IASPComp, &IID_IASPComp, &LIBID_ASPCOMSAMPLib>
{
public:
    CASPComp()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ASPCOMP)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CASPComp)
    COM_INTERFACE_ENTRY(IASPComp)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

// IASPComp
public:
    STDMETHOD(ProcessRequest)();
};

#endif //__ASPCOMP_H_

// Add.h : Declaration of the CAdd

#ifndef __ADD_H_
#define __ADD_H_

#include "resource.h"       // main symbols
#include "ConnectionCOMCP.h"

/////////////////////////////////////////////////////////////////////////////
// CAdd
class ATL_NO_VTABLE CAdd :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CAdd, &CLSID_Add>,
    public IConnectionPointContainerImpl<CAdd>,
    public IDispatchImpl<IAdd, &IID_IAdd, &LIBID_CONNECTIONCOMLib>,
    public CProxy_IAddEvents< CAdd >
{
public:
    CAdd()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_ADD)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CAdd)
    COM_INTERFACE_ENTRY(IAdd)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    END_COM_MAP()
    BEGIN_CONNECTION_POINT_MAP(CAdd)
    CONNECTION_POINT_ENTRY(IID__IAddEvents)
    END_CONNECTION_POINT_MAP()


// IAdd
public:
    STDMETHOD(Add)(int a, int b);
};

#endif //__ADD_H_

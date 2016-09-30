// Application.h : Declaration of the CApplication

#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "resource.h"       // main symbols
#include "Documents.h"

/////////////////////////////////////////////////////////////////////////////
// CApplication
class ATL_NO_VTABLE CApplication :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CApplication, &CLSID_Application>,
    public IDispatchImpl<IApplication, &IID_IApplication, &LIBID_OBJECTMODELLib>
{
public:
    DECLARE_REGISTRY_RESOURCEID(IDR_APPLICATION)
    DECLARE_NOT_AGGREGATABLE(CApplication)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CApplication)
    COM_INTERFACE_ENTRY(IApplication)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    HRESULT CApplication::FinalConstruct()
    {
        return CDocuments::CreateInstance(&m_spDocuments);
    }

// IApplication
public:
    STDMETHODIMP CApplication::get_Documents(IDocuments** pVal)
    {
        return m_spDocuments.CopyTo(pVal);
    }

private:
    CComPtr<IDocuments> m_spDocuments;
};

#endif //__APPLICATION_H_

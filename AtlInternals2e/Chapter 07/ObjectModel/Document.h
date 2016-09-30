// Document.h : Declaration of the CDocument

#ifndef __DOCUMENT_H_
#define __DOCUMENT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDocument
class ATL_NO_VTABLE CDocument :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDocument>,
    public IDispatchImpl<IDocument, &IID_IDocument>
{
public:
    CDocument()
    {
    }

    DECLARE_NO_REGISTRY()
    DECLARE_NOT_AGGREGATABLE(CDocument)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CDocument)
    COM_INTERFACE_ENTRY(IDocument)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

// IDocument
public:
    STDMETHOD(get_Data)(/*[out, retval]*/ BSTR *pVal);
    STDMETHOD(put_Data)(/*[in]*/ BSTR newVal);

private:
    CComBSTR  m_bstrData;
};

#endif //__DOCUMENT_H_

// Documents.h : Declaration of the CDocuments

#ifndef __DOCUMENTS_H_
#define __DOCUMENTS_H_

#include "resource.h"       // main symbols
#include "Document.h"

template <typename T>
struct _CopyVariantFromAdaptItf {
    static HRESULT copy(VARIANT* p1, CAdapt< CComPtr<T> >* p2)
    {
        HRESULT hr = p2->m_T->QueryInterface(IID_IDispatch, (void**)&p1->pdispVal);

        if (SUCCEEDED(hr)) {
            p1->vt = VT_DISPATCH;
        } else {
            hr = p2->m_T->QueryInterface(IID_IUnknown, (void**)&p1->punkVal);

            if (SUCCEEDED(hr)) {
                p1->vt = VT_UNKNOWN;
            }
        }

        return hr;
    }

    static void init(VARIANT* p)
    {
        VariantInit(p);
    }
    static void destroy(VARIANT* p)
    {
        VariantClear(p);
    }
};

typedef CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
        _CopyVariantFromAdaptItf<IDocument>,
        list< CAdapt< CComPtr<IDocument> > > >
        CComEnumVariantOnListOfDocuments;

template <typename T>
struct _CopyItfFromAdaptItf {
    static HRESULT copy(T** p1, CAdapt< CComPtr<T> >* p2)
    {
        if (*p1 = p2->m_T) return (*p1)->AddRef(), S_OK;

        return E_POINTER;
    }

    static void init(T** p)    {}
    static void destroy(T** p)
    {
        if (*p)(*p)->Release();
    }
};

typedef ICollectionOnSTLImpl<IDispatchImpl<IDocuments, &IID_IDocuments>,
        list< CAdapt< CComPtr<IDocument> > >,
        IDocument*,
        _CopyItfFromAdaptItf<IDocument>,
        CComEnumVariantOnListOfDocuments>
        IDocumentsCollImpl;

/////////////////////////////////////////////////////////////////////////////
// CDocuments
class ATL_NO_VTABLE CDocuments :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDocuments>, // non-creatable
    public IDocumentsCollImpl
{
public:
    DECLARE_NO_REGISTRY()
    DECLARE_NOT_AGGREGATABLE(CDocuments)
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CDocuments)
    COM_INTERFACE_ENTRY(IDocuments)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

// IDocuments
public:
    STDMETHODIMP AddDocument(IDocument** ppDocument)
    {
        // Create a document to hand back to the client
        HRESULT hr = CDocument::CreateInstance(ppDocument);

        if (SUCCEEDED(hr)) {
            // Put the document on the list
            CComPtr<IDocument>  spDoc = *ppDocument;
            m_coll.push_back(spDoc);
        }

        return hr;
    }
};

#endif //__DOCUMENTS_H_

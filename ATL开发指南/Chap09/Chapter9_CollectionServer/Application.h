// Application.h : Declaration of the CApplication

#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CApplication
class ATL_NO_VTABLE CApplication :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CApplication, &CLSID_Application>,
    public ISupportErrorInfo,
    public IDispatchImpl<IApplication, &IID_IApplication, &LIBID_CHAPTER9_COLLECTIONSERVERLib>
{
public:
    CApplication()
    {
    }

    HRESULT FinalConstruct()
    {
        HRESULT hr;
        // Create our movies collection
        hr = CComObject<CMovieCollection>::CreateInstance(&m_pMovies);

        if (FAILED(hr))
            return hr;

        // Bump the reference count to 1
        m_pMovies->GetUnknown()->AddRef();
        // Create some movies and add them
        // to the collection
        AddMovies(m_pMovies);
        return S_OK;
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_APPLICATION)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CApplication)
    COM_INTERFACE_ENTRY(IApplication)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IApplication
public:
    STDMETHOD(CreateMovie)(/*[out, retval]*/ IDispatch** ppVal);
    STDMETHOD(get_Movies)(/*[out, retval]*/ IDispatch** ppVal);

private:
    CComObject<CMovieCollection>* m_pMovies;
    void AddMovies(CComObject<CMovieCollection>* pMovies);

};

#endif //__APPLICATION_H_

//
// Movies.h
//

#include <vector>

class ATL_NO_VTABLE CMovie :
    public IDispatchImpl<IMovie, &IID_IMovie, &LIBID_CHAPTER9_COLLECTIONSERVERLib>,
    public CComObjectRoot
{
public:
    CMovie() {};

    BEGIN_COM_MAP(CMovie)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IMovie)
    END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CMovie)

// IMovie
public:
    STDMETHOD(get_Title)(BSTR *pVal);
    STDMETHOD(put_Title)(BSTR newVal);
    STDMETHOD(get_Director)(BSTR*);
    STDMETHOD(put_Director)(BSTR);

private:
    CComBSTR m_bstrDirector;
    CComBSTR m_bstrTitle;
};

//
// CMovieCollection
//
class ATL_NO_VTABLE CMovieCollection :
    public IDispatchImpl<IMovieCollection, &IID_IMovieCollection,
    &LIBID_CHAPTER9_COLLECTIONSERVERLib>,
    public CComObjectRoot
{
public:
    CMovieCollection() {};

    BEGIN_COM_MAP(CMovieCollection)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IMovieCollection)
    END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CMovieCollection)

public:
    STDMETHODIMP RemoveAll()
    {
        m_vecMovies.erase(m_vecMovies.begin(),
                          m_vecMovies.end());
        return S_OK;
    }
    STDMETHODIMP Add(IMovie* pMovie)
    {
        // Add the variant (dispatch) to the collection
        m_vecMovies.push_back(CComVariant(pMovie));
        return S_OK;
    }
    STDMETHODIMP Remove(long lIndex)
    {
        // Make sure the index is within range (index is 1-based)
        if (lIndex < 1 || lIndex > m_vecMovies.size())
            return E_INVALIDARG;

        m_vecMovies.erase(m_vecMovies.begin() + lIndex - 1);
        return S_OK;
    }

    STDMETHODIMP get_Count(long* plCount)
    {
        *plCount = m_vecMovies.size();
        return S_OK;
    }

    STDMETHODIMP get_Item(long lItem, IMovie** ppIMovie)
    {
        if (lItem < 1 || lItem > m_vecMovies.size())
            return E_INVALIDARG;

        // Get the item
        CComVariant& var = m_vecMovies[lItem - 1];
        HRESULT hr = var.pdispVal->QueryInterface(IID_IMovie,
                     (void**) ppIMovie);

        if (FAILED(hr))
            return E_UNEXPECTED;

        return S_OK;
    }
    STDMETHODIMP get__NewEnum(IUnknown** ppUnk)
    {
        typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT,
                VARIANT, _Copy<VARIANT> > >
                enumVariant;
        enumVariant* pEnum = new enumVariant;
        HRESULT hr = pEnum->Init(m_vecMovies.begin(),
                                 m_vecMovies.end(),
                                 0,
                                 AtlFlagCopy);

        if (SUCCEEDED(hr))
            hr = pEnum->QueryInterface(IID_IEnumVARIANT,
                                       (void**) ppUnk);

        if (FAILED(hr))
            delete pEnum;

        return S_OK;
    }

private:
    std::vector<CComVariant> m_vecMovies;
};

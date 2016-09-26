//
// Application.cpp : Implementation of CApplication
//

#include "stdafx.h"
#include "Chapter9_CollectionServer.h"
#include "movies.h"
#include "Application.h"

/////////////////////////////////////////////////////////////////////////////
// CApplication

STDMETHODIMP CApplication::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = {
        &IID_IApplication
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

void CApplication::AddMovies(CComObject<CMovieCollection>* pMovies)
{
    CComObject<CMovie>* pMovie;
    IMovie* pIMovie;
    {
        CComObject<CMovie>::CreateInstance(&pMovie);
        pMovie->QueryInterface(IID_IMovie,
                               (void**) &pIMovie);
        CComBSTR bstrDir("George Lucas");
        pIMovie->put_Director(bstrDir);
        CComBSTR bstrTitle("Star Wars");
        pIMovie->put_Title(bstrTitle);
        pMovies->Add(pIMovie);
    }
    {
        CComObject<CMovie>::CreateInstance(&pMovie);
        pMovie->QueryInterface(IID_IMovie,
                               (void**) &pIMovie);
        CComBSTR bstrDir("Steven Spielberg");
        pIMovie->put_Director(bstrDir);
        CComBSTR bstrTitle("Jurassic Park");
        pIMovie->put_Title(bstrTitle);
        pMovies->Add(pIMovie);
    }
}

STDMETHODIMP CApplication::get_Movies(IDispatch **ppVal)
{
    // Pass back a movies collection
    // Return the IDispatch* of the collection
    m_pMovies->QueryInterface(IID_IDispatch,
                              (void**) ppVal);
    return S_OK;
}

STDMETHODIMP CApplication::CreateMovie(IDispatch **ppVal)
{
    CComObject<CMovie>* pCMovie;
    CComObject<CMovie>::CreateInstance(&pCMovie);
    pCMovie->AddRef();
    pCMovie->QueryInterface(IID_IMovie,
                            (void**) ppVal);
    return S_OK;
}

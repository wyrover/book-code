//
// Movies.cpp
//

#include "stdafx.h"
#include "Chapter9_CollectionServer.h"
#include "Movies.h"

// CMovie
STDMETHODIMP CMovie::get_Director(BSTR* pbstr)
{
    *pbstr = m_bstrDirector.Copy();
    return S_OK;
}

STDMETHODIMP CMovie::put_Director(BSTR bstr)
{
    m_bstrDirector = bstr;
    return S_OK;
}

STDMETHODIMP CMovie::get_Title(BSTR * pbstr)
{
    *pbstr = m_bstrTitle.Copy();
    return S_OK;
}

STDMETHODIMP CMovie::put_Title(BSTR bstr)
{
    m_bstrTitle = bstr;
    return S_OK;
}

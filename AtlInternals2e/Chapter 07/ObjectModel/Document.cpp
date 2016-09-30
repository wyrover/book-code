// Document.cpp : Implementation of CDocument
#include "stdafx.h"
#include "ObjectModel.h"
#include "Document.h"

/////////////////////////////////////////////////////////////////////////////
// CDocument

STDMETHODIMP CDocument::get_Data(BSTR *pVal)
{
    return m_bstrData.CopyTo(pVal);
}

STDMETHODIMP CDocument::put_Data(BSTR newVal)
{
    m_bstrData = newVal;
    return (m_bstrData || !newVal ? S_OK : E_OUTOFMEMORY);
}

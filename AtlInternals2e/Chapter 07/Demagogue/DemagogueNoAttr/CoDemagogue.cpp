// CoDemagogue.cpp : Implementation of CDemagogue

#include "stdafx.h"
#include "CoDemagogue.h"


// CDemagogue


STDMETHODIMP CDemagogue::get_Volume(DOUBLE* pVal)
{
    if (!pVal) return E_POINTER;

    *pVal = m_dVolume;
    return S_OK;
}

STDMETHODIMP CDemagogue::put_Volume(DOUBLE newVal)
{
    m_dVolume = newVal;
    return S_OK;
}

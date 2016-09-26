// QuoteSupplier.cpp : Implementation of CQuoteSupplier
#include "stdafx.h"
#include "DCOMServ.h"
#include "QuoteSupplier.h"

/////////////////////////////////////////////////////////////////////////////
// CQuoteSupplier


STDMETHODIMP CQuoteSupplier::GetQuote(BSTR Ticker, double *Price)
{
    _Module.GetQuote(Ticker, Price);
    return S_OK;
}

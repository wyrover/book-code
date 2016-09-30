// Calc.cpp : Implementation of CCalc

#include "stdafx.h"
#include "Calc.h"


// CCalc


STDMETHODIMP CCalc::Add(DOUBLE Op1, DOUBLE Op2, DOUBLE* Result)
{
//  CComGITPtr<ICalc> spgit;
    *Result = Op1 + Op2;
    return S_OK;
}

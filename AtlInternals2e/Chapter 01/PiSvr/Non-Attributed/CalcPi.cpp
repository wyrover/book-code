// CalcPi.cpp : Implementation of CCalcPi
#include "stdafx.h"
#include "CalcPi.h"
#include "NineDigitsOfPiAt.h"

// CCalcPi


STDMETHODIMP CCalcPi::get_Digits(long *pVal)
{
    *pVal = m_nDigits;
    return S_OK;
}

STDMETHODIMP CCalcPi::put_Digits(long newVal)
{
    if (newVal < 0)
        return Error(L"Can't calculate negative digits of PI");

    m_nDigits = newVal;
    FireViewChange();
    return S_OK;
}

STDMETHODIMP CCalcPi::CalcPi(BSTR *pbstrPi)
{
    _ASSERTE(m_nDigits >= 0);

    if (m_nDigits) {
        *pbstrPi = SysAllocStringLen(L"3.", m_nDigits + 2);

        if (*pbstrPi) {
            for (int i = 0; i < m_nDigits; i += 9) {
                long nNineDigits = NineDigitsOfPiStartingAt(i + 1);
                swprintf(*pbstrPi + i + 2, L"%09d", nNineDigits);
            }

            // Truncate to number of digits
            (*pbstrPi)[m_nDigits + 2] = 0;

            // Fire each digit
            for (short j = 0; j != m_nDigits; ++j) {
                Fire_OnDigit(j, (*pbstrPi)[j + 2] - L'0');
            }
        }
    } else {
        *pbstrPi = SysAllocString(L"3");
    }

    return *pbstrPi ? S_OK : E_OUTOFMEMORY;
}

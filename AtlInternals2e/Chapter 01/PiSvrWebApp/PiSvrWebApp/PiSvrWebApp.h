// PiSvrWebApp.h : Defines the ATL Server request handler class
//
#pragma once

#include <oleauto.h>
#include "NineDigitsOfPiAt.h"

[ request_handler("Default") ]
class CPiSvrWebAppHandler
{
private:
    // Put private members here

protected:
    // Put protected members here

public:
    // Put public members here

    HTTP_CODE ValidateAndExchange()
    {
        // TODO: Put all initialization and validation code here
        // Set the content-type
        m_HttpResponse.SetContentType("text/html");
        return HTTP_SUCCESS;
    }

protected:
    // Here is an example of how to use a replacement tag with the stencil processor
    [ tag_name(name = "Hello") ]
    HTTP_CODE OnHello(void)
    {
        m_HttpResponse << "Hello World!";
        return HTTP_SUCCESS;
    }

    [ tag_name(name = "Pi") ]
    HTTP_CODE OnPi(void)
    {
        USES_CONVERSION;
        LPCSTR pszDigits = m_HttpRequest.m_QueryParams.Lookup("digits");
        long nDigits = 0;

        if (pszDigits)
            nDigits = atoi(pszDigits);

        BSTR bstrPi = NULL;
        CalcPi(nDigits, &bstrPi);
        m_HttpResponse << OLE2A(bstrPi);
        return HTTP_SUCCESS;
    }

    HRESULT CalcPi(long nDigits, BSTR* pbstrPi)
    {
        if (nDigits) {
            *pbstrPi = SysAllocStringLen(L"3.", nDigits + 2);

            if (*pbstrPi) {
                for (int i = 0; i < nDigits; i += 9) {
                    long nNineDigits = NineDigitsOfPiStartingAt(i + 1);
                    swprintf(*pbstrPi + i + 2, L"%09d", nNineDigits);
                }

                // Truncate to number of digits
                (*pbstrPi)[nDigits + 2] = 0;
            }
        } else {
            *pbstrPi = SysAllocString(L"3");
        }

        return *pbstrPi ? S_OK : E_OUTOFMEMORY;
    }

}; // class CPiSvrWebAppHandler
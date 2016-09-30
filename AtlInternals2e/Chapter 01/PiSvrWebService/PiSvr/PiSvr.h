// PiSvr.h : Defines the ATL Server request handler class
//
#pragma once

#include "NineDigitsOfPiAt.h"

namespace PiSvrService
{
// all struct, enum, and typedefs for your webservice should go inside the namespace

// IPiSvrService - web service interface declaration
//
[
    uuid("E5FEC44B-68F7-4F25-9B29-1C85891226A6"),
    object
]
__interface IPiSvrService {
    // HelloWorld is a sample ATL Server web service method.  It shows how to
    // declare a web service method and its in-parameters and out-parameters
    [id(1)] HRESULT HelloWorld([in] BSTR bstrInput, [out, retval] BSTR * bstrOutput);
    [id(2)] HRESULT CalcPi([in] LONG nDigits, [out, retval] BSTR * bstrPi);
};


// PiSvrService - web service implementation
//
[
    request_handler(name = "Default", sdl = "GenPiSvrWSDL"),
    soap_handler(
        name = "PiSvrService",
        namespace = "urn:PiSvrService",
        protocol = "soap"
    )
]
class CPiSvrService :
    public IPiSvrService
{
public:
    [ soap_method ]
    HRESULT HelloWorld(BSTR bstrInput, BSTR *bstrOutput)
    {
        CComBSTR bstrOut(L"Hello ");
        bstrOut += bstrInput;
        bstrOut += L"!";
        *bstrOutput = bstrOut.Detach();
        return S_OK;
    }

    [ soap_method ]
    HRESULT CalcPi(LONG nDigits, BSTR *pbstrPi)
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
}; // class CPiSvrService

} // namespace PiSvrService

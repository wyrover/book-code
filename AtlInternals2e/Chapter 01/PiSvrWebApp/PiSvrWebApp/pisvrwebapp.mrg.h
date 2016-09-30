// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\second edition\src\chapter 01\pisvrwebapp\pisvrwebapp\pisvrwebapp.mrg.h
// compiler-generated file created 05/13/02 at 21:29:47
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

// PiSvrWebApp.h : Defines the ATL Server request handler class
//
#pragma once

#include <oleauto.h>
#include "NineDigitsOfPiAt.h"

[ request_handler("Default") ]
class CPiSvrWebAppHandler
    :
/*+++ Added Baseclass */ public CRequestHandlerT<CPiSvrWebAppHandler>
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
        BSTR bstr = NULL;
        bstr += L"Fred";
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


    //+++ Start Injected Code For Attribute 'tag_name'
#injected_line 32 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebapp\\pisvrwebapp.h"
    BEGIN_ATTR_REPLACEMENT_METHOD_MAP(CPiSvrWebAppHandler)
    REPLACEMENT_METHOD_ENTRY("Hello", OnHello)
    REPLACEMENT_METHOD_ENTRY("Pi", OnPi)
    END_ATTR_REPLACEMENT_METHOD_MAP()
    //--- End Injected Code For Attribute 'tag_name'
};

//+++ Start Injected Code For Attribute 'request_handler'
#injected_line 8 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebapp\\pisvrwebapp\\pisvrwebapp.h"


DECLARE_REQUEST_HANDLER("Default", CPiSvrWebAppHandler, ::CPiSvrWebAppHandler)


//--- End Injected Code For Attribute 'request_handler'
// class CPiSvrWebAppHandler
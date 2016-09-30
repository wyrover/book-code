// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\second edition\src\chapter 01\pisvrwebservice\pisvr\pisvr.mrg.h
// compiler-generated file created 05/12/02 at 00:28:51
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
    ,
/*+++ Added Baseclass */ public CSoapHandler<CPiSvrService>
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

    //+++ Start Injected Code For Attribute 'soap_handler'
#injected_line 30 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.h"
    const _soapmap ** GetFunctionMap() throw();
    const _soapmap ** GetHeaderMap() throw();
    void * GetHeaderValue() throw();
    const wchar_t * GetNamespaceUri() throw();
    const char * GetNamespaceUriA() throw();
    const char * GetServiceName() throw();
    HRESULT CallFunction(void *pvParam, const wchar_t *wszLocalName, int cchLocalName, size_t nItem);

    //--- End Injected Code For Attribute 'soap_handler'
};

//+++ Start Injected Code For Attribute 'request_handler'
#injected_line 29 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.h"


HANDLER_ENTRY_SDL("Default", CPiSvrService, ::PiSvrService::CPiSvrService, GenPiSvrWSDL)


//--- End Injected Code For Attribute 'request_handler'


//+++ Start Injected Code For Attribute 'soap_handler'
#injected_line 30 "c:\\kirk\\atl internals 2e\\second edition\\src\\chapter 01\\pisvrwebservice\\pisvr\\pisvr.h"
struct ___PiSvrService_CPiSvrService_HelloWorld_struct {
    BSTR bstrInput;
    BSTR bstrOutput;
};

extern __declspec(selectany) const _soapmapentry ___PiSvrService_CPiSvrService_HelloWorld_entries[] = {

    {
        0xF6041A8C,
        "bstrOutput",
        L"bstrOutput",
        sizeof("bstrOutput") - 1,
        SOAPTYPE_STRING,
        SOAPFLAG_RETVAL | SOAPFLAG_OUT | SOAPFLAG_NULLABLE,
        offsetof(___PiSvrService_CPiSvrService_HelloWorld_struct, bstrOutput),
        NULL,
        NULL,
        -1,
    },

    {
        0xA9ECBD0B,
        "bstrInput",
        L"bstrInput",
        sizeof("bstrInput") - 1,
        SOAPTYPE_STRING,
        SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
        offsetof(___PiSvrService_CPiSvrService_HelloWorld_struct, bstrInput),
        NULL,
        NULL,
        -1,
    },
    { 0x00000000 }
};

extern __declspec(selectany) const _soapmap ___PiSvrService_CPiSvrService_HelloWorld_map = {
    0x46BA99FC,
    "HelloWorld",
    L"HelloWorld",
    sizeof("HelloWorld") - 1,
    sizeof("HelloWorld") - 1,
    SOAPMAP_FUNC,
    ___PiSvrService_CPiSvrService_HelloWorld_entries,
    sizeof(___PiSvrService_CPiSvrService_HelloWorld_struct),
    1,
    0,
    SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
    0xD973B494,
    "urn:PiSvrService",
    L"urn:PiSvrService",
    sizeof("urn:PiSvrService") - 1
};

extern __declspec(selectany) const _soapmapentry ___PiSvrService_CPiSvrService_HelloWorld_atlsoapheader_entries[] = {
    { 0x00000000 }
};

extern __declspec(selectany) const _soapmap ___PiSvrService_CPiSvrService_HelloWorld_atlsoapheader_map = {
    0x46BA99FC,
    "HelloWorld",
    L"HelloWorld",
    sizeof("HelloWorld") - 1,
    sizeof("HelloWorld") - 1,
    SOAPMAP_HEADER,
    ___PiSvrService_CPiSvrService_HelloWorld_atlsoapheader_entries,
    0,
    0,
    -1,
    SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
    0xD973B494,
    "urn:PiSvrService",
    L"urn:PiSvrService",
    sizeof("urn:PiSvrService") - 1
};
struct ___PiSvrService_CPiSvrService_CalcPi_struct {
    long nDigits;
    BSTR pbstrPi;
};

extern __declspec(selectany) const _soapmapentry ___PiSvrService_CPiSvrService_CalcPi_entries[] = {

    {
        0x9A76AFA4,
        "pbstrPi",
        L"pbstrPi",
        sizeof("pbstrPi") - 1,
        SOAPTYPE_STRING,
        SOAPFLAG_RETVAL | SOAPFLAG_OUT | SOAPFLAG_NULLABLE,
        offsetof(___PiSvrService_CPiSvrService_CalcPi_struct, pbstrPi),
        NULL,
        NULL,
        -1,
    },

    {
        0xB9CB3BB2,
        "nDigits",
        L"nDigits",
        sizeof("nDigits") - 1,
        SOAPTYPE_INT,
        SOAPFLAG_NONE | SOAPFLAG_IN,
        offsetof(___PiSvrService_CPiSvrService_CalcPi_struct, nDigits),
        NULL,
        NULL,
        -1,
    },
    { 0x00000000 }
};

extern __declspec(selectany) const _soapmap ___PiSvrService_CPiSvrService_CalcPi_map = {
    0xA361D3CC,
    "CalcPi",
    L"CalcPi",
    sizeof("CalcPi") - 1,
    sizeof("CalcPi") - 1,
    SOAPMAP_FUNC,
    ___PiSvrService_CPiSvrService_CalcPi_entries,
    sizeof(___PiSvrService_CPiSvrService_CalcPi_struct),
    1,
    0,
    SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
    0xD973B494,
    "urn:PiSvrService",
    L"urn:PiSvrService",
    sizeof("urn:PiSvrService") - 1
};

extern __declspec(selectany) const _soapmapentry ___PiSvrService_CPiSvrService_CalcPi_atlsoapheader_entries[] = {
    { 0x00000000 }
};

extern __declspec(selectany) const _soapmap ___PiSvrService_CPiSvrService_CalcPi_atlsoapheader_map = {
    0xA361D3CC,
    "CalcPi",
    L"CalcPi",
    sizeof("CalcPi") - 1,
    sizeof("CalcPi") - 1,
    SOAPMAP_HEADER,
    ___PiSvrService_CPiSvrService_CalcPi_atlsoapheader_entries,
    0,
    0,
    -1,
    SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
    0xD973B494,
    "urn:PiSvrService",
    L"urn:PiSvrService",
    sizeof("urn:PiSvrService") - 1
};
extern __declspec(selectany) const _soapmap * ___PiSvrService_CPiSvrService_funcs[] = {
    &___PiSvrService_CPiSvrService_HelloWorld_map,
    &___PiSvrService_CPiSvrService_CalcPi_map,
    NULL
};

extern __declspec(selectany) const _soapmap * ___PiSvrService_CPiSvrService_headers[] = {
    &___PiSvrService_CPiSvrService_HelloWorld_atlsoapheader_map,
    &___PiSvrService_CPiSvrService_CalcPi_atlsoapheader_map,
    NULL
};

ATL_NOINLINE inline const _soapmap ** CPiSvrService::GetFunctionMap()
{
    return ___PiSvrService_CPiSvrService_funcs;
};

ATL_NOINLINE inline const _soapmap ** CPiSvrService::GetHeaderMap()
{
    return ___PiSvrService_CPiSvrService_headers;
}

ATL_NOINLINE inline void * CPiSvrService::GetHeaderValue()
{
    return this;
}

ATL_NOINLINE inline HRESULT CPiSvrService::CallFunction(
    void *pvParam,
    const wchar_t *wszLocalName,
    int cchLocalName,
    size_t nItem)
{
    wszLocalName;
    cchLocalName;
    HRESULT hr = S_OK;

    switch (nItem) {
    case 0: {
        ___PiSvrService_CPiSvrService_HelloWorld_struct *p = (___PiSvrService_CPiSvrService_HelloWorld_struct *) pvParam;
        hr = HelloWorld(p->bstrInput, &p->bstrOutput);
        break;
    }

    case 1: {
        ___PiSvrService_CPiSvrService_CalcPi_struct *p = (___PiSvrService_CPiSvrService_CalcPi_struct *) pvParam;
        hr = CalcPi(p->nDigits, &p->pbstrPi);
        break;
    }

    default:
        hr = E_FAIL;
    }

    return hr;
}

ATL_NOINLINE inline const wchar_t * CPiSvrService::GetNamespaceUri()
{
    return L"urn:PiSvrService";
}

ATL_NOINLINE inline const char * CPiSvrService::GetNamespaceUriA()
{
    return "urn:PiSvrService";
}

ATL_NOINLINE inline const char * CPiSvrService::GetServiceName()
{
    return "PiSvrService";
}

//--- End Injected Code For Attribute 'soap_handler'
// class CPiSvrService

} // namespace PiSvrService

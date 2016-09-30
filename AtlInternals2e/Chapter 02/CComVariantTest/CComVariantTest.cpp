// CComVariantTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>

int _tmain(int argc, _TCHAR* argv[])
{
    USES_CONVERSION;
    A2W;
    CComVariant var;
    LPCOLESTR osz = OLESTR("This is a\0BSTR string");
    BSTR bstrInput = ::SysAllocStringLen(osz, 21) ;
    CComVariant v1(bstrInput);  // calls CComVariant (BSTR)
    // calls CComVariant (BSTR)
    CComVariant v2(OLESTR("This is an OLECHAR string")) ;
    OLECHAR* ps = OLESTR("This is another OLECHAR string") ;
    CComVariant v3(ps);  // calls CComVariant (BSTR)
    const OLECHAR* pcs = OLESTR("This is another OLECHAR string") ;
    CComVariant v4(pcs);  // Only this calls CComVariant (LPCOLESTR)
    CComBSTR bstr(::SysStringLen(bstrInput), bstrInput);
    int n = bstr.Length();
    CComVariant v5(bstr);
    CComVariant v6(osz);
    ::SysFreeString(bstrInput) ;
    return 0;
}


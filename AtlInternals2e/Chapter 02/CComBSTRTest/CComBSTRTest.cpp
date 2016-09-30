// CComBSTRTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>
#include <atlsafe.h>

int _tmain(int argc, _TCHAR* argv[])
{
    USES_CONVERSION;
    BSTR bstrIn1 = ::SysAllocStringLen(OLESTR("This is part one\0and here's part two"), 36);
    BSTR bstrIn2 = ::SysAllocStringLen(OLESTR("This is part one\0and here's part two"), 36);
    LPOLESTR osz1 = OLESTR("This is xxxx one\0and xxxx's part two");
    LPOLESTR osz2 = OLESTR("This is part one\0and here's part two");
    CComBSTR bstr1(::SysStringLen(bstrIn1), bstrIn1);
    CComBSTR bstr2(::SysStringLen(bstrIn2), bstrIn2);
    CComBSTR bstr3(osz1);
    CComBSTR bstr4(osz2);
    CComBSTR bstr5 = osz1;
    CComBSTR bstr6;
    bstr6.AssignBSTR(bstr1);
    bool b = bstr1 == bstr2;
    b = bstr3 == bstr4;
    CComSafeArrayBound bound1;
    bound1.SetCount(4);
    CComSafeArrayBound bound2;
    bound2.SetCount(5);
    CComSafeArrayBound rgBound[] = { bound1, bound2 };
    CComSafeArray<char> sa1(rgBound);
    long rgind1[] = { 0, 0 };
    sa1.MultiDimSetAt(rgind1, 41);
    long rgind2[] = { 0, 1 };
    sa1.MultiDimSetAt(rgind2, 42);
    long rgind3[] = { 0, 2 };
    sa1.MultiDimSetAt(rgind3, 43);
    CComSafeArray<int> sa4(6);
    /*
    sa4[0] = 3;
    sa4[1] = 4;
    sa4[2] = 5;
    sa4[3] = 6;
    sa4[4] = 7;
    sa4[5] = 8;
    */
    HRESULT hr = bstr1.ArrayToBSTR(sa4.m_psa);
    int n = bstr1.Length();
    CComSafeArray<char> sa2;
    CComSafeArray<wchar_t> sa3;
    CComBSTR bstr7("Fredie");
    hr = bstr7.BSTRToArray(&sa2.m_psa);
    hr = bstr7.BSTRToArray(&sa3.m_psa);
    n = sa3.GetCount();
    hr = ::SafeArrayDestroy(sa3.m_psa);
    hr = ::SafeArrayDestroy(sa3.m_psa);
    return 0;
}


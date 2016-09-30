// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include <atlbase.h>
#import "..\GITTest\Debug\GITTest.dll" no_namespace, named_guids, raw_interfaces_only

void func(void*);

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    ICalc* pCalc = NULL;
    HRESULT hr = ::CoCreateInstance(CLSID_CCalc, NULL, CLSCTX_ALL, __uuidof(pCalc), (void**)&pCalc);
    CComGITPtr<ICalc> spgit(pCalc);
    _beginthread(func, 0, &spgit);
    Sleep(6000);
    ::CoUninitialize();
    return 0;
}

void func(void* pArg)
{
    ::CoInitialize(NULL);
    CComGITPtr<ICalc>* pgit = (CComGITPtr<ICalc>*)pArg;
    ICalc* pCalc = NULL;
    HRESULT hr = pgit->CopyTo(&pCalc);
    double dRes = 0;
    hr = pCalc->Add(4, 5, &dRes);
    ::CoUninitialize();
}


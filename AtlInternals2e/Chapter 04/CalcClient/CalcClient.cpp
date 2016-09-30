// CalcClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#import "..\\Calc\\Debug\\Calc.dll" no_namespace, named_guids, raw_interfaces_only

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    HRESULT hr = S_OK;
    ICalc* pCalc = NULL;
    hr = ::CoCreateInstance(CLSID_Calc, NULL, CLSCTX_ALL, IID_ICalc, (void**)&pCalc);
    pCalc->AddRef();
    pCalc->AddRef();
    pCalc->AddRef();
    ::CoUninitialize();
    return 0;
}


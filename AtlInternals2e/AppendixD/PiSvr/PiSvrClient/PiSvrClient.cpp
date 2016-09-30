// PiSvrClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#import "../PiSvr/Debug/PiSvr.dll" no_namespace

using namespace std;

class CoInit
{
public:
    CoInit()
    {
        CoInitialize(0);
    }
    ~CoInit()
    {
        CoUninitialize();
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    CoInit coInit;
    ICalcPiPtr spPiCalc(__uuidof(CCalcPi));
    spPiCalc->Digits = 20;
    _bstr_t bstrPi = spPiCalc->CalcPi();
    wcout << L"Pi to " << spPiCalc->Digits << " digits is " << bstrPi << endl;
    return 0;
}


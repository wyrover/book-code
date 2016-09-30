// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#import "..\\MathDllNoAttr\\Debug\\MathDllNoAttr.dll" no_namespace, named_guids
#import "..\\MathExeNoAttr\\Debug\\MathExeNoAttr.exe" no_namespace, named_guids

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);

    try {
        ICalcPtr spCalc(__uuidof(Calc));
    } catch (_com_error& e) {
    }

    ::CoUninitialize();
    return 0;
}


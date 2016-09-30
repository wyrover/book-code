// DispatchClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#import "..\\Math\\Debug\\Math.dll" no_namespace, named_guids, raw_interfaces_only
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);

    try {
        BSTR bstr = ::SysAllocString(OLESTR("Fred"));
        int nLen = ::SysStringLen(NULL);
        IDispatch* pdisp = NULL;
        HRESULT hr = ::CoCreateInstance(CLSID_Calc, NULL, CLSCTX_ALL, __uuidof(IDispatch), (void**)&pdisp);

        if (FAILED(hr))
            return hr;

        // Get the DISPID
        LPOLESTR pszMethod = OLESTR("Add");
        DISPID dispid;
        hr = pdisp->GetIDsOfNames(IID_NULL,
                                  &pszMethod,
                                  1,
                                  LOCALE_SYSTEM_DEFAULT,
                                  &dispid);

        if (FAILED(hr))
            return hr;

        // Set up the parameters
        DISPPARAMS dispparms;
        memset(&dispparms, 0 , sizeof(DISPPARAMS));
        dispparms.cArgs = 2;
        // Parameters are passed right to left
        VARIANTARG rgvarg[2];
        rgvarg[0].vt = VT_R8;
        rgvarg[0].dblVal = 6;
        rgvarg[1].vt = VT_R8;
        rgvarg[1].dblVal = 7;
        dispparms.rgvarg = &rgvarg[0];
        // Set up variable to hold method return value
        VARIANTARG vaResult;
        ::VariantInit(&vaResult);
        // Invoke the method
        UINT rguArg[2];
        hr = pdisp->Invoke(dispid,
                           IID_NULL,
                           LOCALE_SYSTEM_DEFAULT,
                           DISPATCH_METHOD,
                           &dispparms,
                           &vaResult,
                           NULL,
                           &rguArg[0]);
        // varResult now holds sum of 15.7 and 19.3
        int a = 5;
    } catch (_com_error& e) {
    }

    ::CoUninitialize();
    return 0;
}


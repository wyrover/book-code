// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#import "..\\LicensedObject\\Debug\\LicensedObject.dll" no_namespace, named_guids, raw_interfaces_only
#include <atlbase.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    {
        CComPtr<IClassFactory2> pcf = NULL;
        HRESULT hr = ::CoGetClassObject(__uuidof(CEarPolitic), CLSCTX_ALL, NULL, __uuidof(pcf), (void**)&pcf);

        if (SUCCEEDED(hr)) {
            WCHAR szKey[1024];
            FILE* f = fopen("c:\\temp\\license.txt", "r");
            int n = fread((void**)&szKey, sizeof(wchar_t), 1024, f);
            szKey[n] = '\0';
            fclose(f);
            CComBSTR bstrKey(szKey);
            CComPtr<IEarPolitic> p;
            hr = pcf->CreateInstanceLic(NULL, NULL, __uuidof(p), bstrKey, (void**)&p);
        }
    }
    ::CoUninitialize();
    return 0;
}


// LicensedClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdafx.h"
#import "..\\LicensedObject\\Debug\\LicensedObject.dll" no_namespace, named_guids, raw_interfaces_only
#include <atlbase.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    {
        CComPtr<IClassFactory2> pcf;
        HRESULT hr = ::CoGetClassObject(__uuidof(CEarPolitic), CLSCTX_ALL, NULL, __uuidof(pcf), (void**)&pcf);

        if (SUCCEEDED(hr)) {
            CComBSTR bstrKey;
            hr = pcf->RequestLicKey(NULL, &bstrKey);
            FILE* f = fopen("c:\\temp\\license.txt", "w+");
            fwrite(bstrKey, sizeof(wchar_t), bstrKey.Length(), f);
            fclose(f);
        }
    }
    ::CoUninitialize();
    return 0;
}


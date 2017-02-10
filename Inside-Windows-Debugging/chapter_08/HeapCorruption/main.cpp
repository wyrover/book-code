#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR()
    {
        CAutoPtr<WCHAR> pwszBuffer;
        const WCHAR* pwszStringToCopy = L"abc.txt";

        ChkProlog();

        pwszBuffer.Attach(new WCHAR[wcslen(pwszStringToCopy)]);
        ChkAlloc(pwszBuffer);

        // BUG! destination buffer is smaller than the number of bytes copied.
        wcscpy(pwszBuffer, pwszStringToCopy);
        wprintf(L"Copied string is %s\n", pwszBuffer);

        ChkNoCleanup();
    }
};

int 
__cdecl
wmain()
{
    HRESULT hr = S_OK;

    hr = CMainApp::MainHR();
    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return hr;
}

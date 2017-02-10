#include "stdafx.h"

class CMainApp
{
private:
    static
    HRESULT
    FillString(
        __in WCHAR ch,
        __in DWORD cchBuffer,
        __out_ecount(cchBuffer) WCHAR* pwszBuffer
        )
    {
        ChkProlog();

        for (DWORD i=0; i < cchBuffer; i++)
        {
            pwszBuffer[i] = ch;
        }

        ChkNoCleanup();
    }

public:
    static
    HRESULT
    MainHR()
    {
        WCHAR wszBuffer[MAX_PATH];

        ChkProlog();

        // BUG! Use of incorrect size in the function call
        ChkHr(FillString(L'\0', sizeof(wszBuffer), wszBuffer));

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

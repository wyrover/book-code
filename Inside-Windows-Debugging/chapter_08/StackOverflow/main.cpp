#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR()
    {
        const char* pszTest = "Test";
        int n;

        ChkProlog();

        USES_CONVERSION;
        for (n = 0; n < 1000000; n++)
        {
            // BUG! A2W is a macro that allocates memory on the stack.
            wprintf(L"%s: %d.\n", A2W(pszTest), n);
        }

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


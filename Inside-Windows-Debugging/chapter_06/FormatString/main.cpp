#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        ChkProlog();

        // BUG! Use of %s with an integer printf argument type
        wprintf(L"Error code value is: %s\n", E_ACCESSDENIED);

        ChkNoCleanup();
    }
};

int 
__cdecl
wmain(
    VOID
    )
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

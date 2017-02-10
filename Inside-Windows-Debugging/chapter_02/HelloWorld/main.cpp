#include "stdafx.h"

class CMainApp
{
public:
    HRESULT
    MainHR(
        VOID
        )
    {
        ChkProlog();

        wprintf(L"Hello World!\n");

        ChkNoCleanup();
    }
};

int
__cdecl
wmain(
    VOID
    )
{
    HRESULT hr;
    CMainApp app;

    hr = app.MainHR();
    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return 0;
}

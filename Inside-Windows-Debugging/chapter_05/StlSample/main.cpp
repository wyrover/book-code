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
        std::string message = "abc";

        ChkProlog();

        std::reverse(message.begin(), message.end());
        printf("Reversed string: %s\n", message.c_str());

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

    hr = CMainApp::MainHR();
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

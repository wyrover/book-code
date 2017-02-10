#include "stdafx.h"

class CBreakOnFailureApp
{
public:
    HRESULT
    MainHR()
    {
        ChkProlog();

        ChkHr(FunctionCall());

        ChkNoCleanup();
    }

private:
    HRESULT
    FunctionCall()
    {
        ChkProlog();

        ChkReturn(E_ACCESSDENIED); // raise an "access denied" failure

        ChkNoCleanup();
    }
};

int
__cdecl
wmain()
{
    HRESULT hr;
    CBreakOnFailureApp app;

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

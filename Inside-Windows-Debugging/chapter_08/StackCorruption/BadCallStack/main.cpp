#include "stdafx.h"

class CMainApp
{
public:
    DECLSPEC_NOINLINE
    static
    HRESULT
    MainHR()
    {
        ChkProlog();

        wprintf(L"Inside MainHR.\n");
        FunctionCall1();
        wprintf(L"Returning from MainHR.\n");

        ChkNoCleanup();
    }

    DECLSPEC_NOINLINE
    static
    VOID
    FunctionCall1()
    {
        wprintf(L"Inside FunctionCall1.\n");
        FunctionCall2();
        wprintf(L"Returning from FunctionCall1.\n");
    }

    DECLSPEC_NOINLINE
    static
    VOID
    FunctionCall2()
    {
        wprintf(L"Inside FunctionCall2.\n");
        FunctionCall3();
        wprintf(L"Returning from FunctionCall2.\n");
    }

    DECLSPEC_NOINLINE
    static
    VOID
    FunctionCall3()
    {
        wprintf(L"Inside FunctionCall3.\n");
        FunctionCall4();
        wprintf(L"Returning from FunctionCall3.\n");
    }

    DECLSPEC_NOINLINE
    static
    VOID
    FunctionCall4()
    {
        volatile PVOID p;
        wprintf(L"Local variable address is %x\n", &p);
        //
        // Overwrite saved frame pointer and return address using offsets
        // from the address on the stack of the local variable
        //
        *(&p + 1) = 0;
        *(&p + 2) = 0;
        *(&p + 3) = 0;
        *(&p + 4) = 0;
        DebugBreak();
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

#include "stdafx.h"

class CMainApp
{
private:
    typedef
    HRESULT
    (WINAPI *PFN_AsynchronousCall)(
        VOID
        );

public:
    static
    HRESULT
    MainHR(
        __in int argc,
        __in_ecount(argc) WCHAR* argv[]
        )
    {
        HMODULE hModule = NULL;
        PFN_AsynchronousCall pfnAsynchronousCall;

        ChkProlog();

        if (argc != 2)
        {
            wprintf(L"USAGE: host.exe <dllpath>\n");
            ChkReturn(E_FAIL);
        }

        hModule = ::LoadLibraryW(argv[1]);
        ChkWin32(hModule);

        pfnAsynchronousCall = (PFN_AsynchronousCall) GetProcAddress(
            hModule, "AsynchronousCall");
        ChkBool(pfnAsynchronousCall, E_FAIL);

        ChkHr(pfnAsynchronousCall());

        ChkCleanup();

        if (hModule)
        {
            FreeLibrary(hModule);
        }

        ChkEpilog();
    }
};

int
__cdecl
wmain(
    __in int argc,
    __in_ecount(argc) WCHAR* argv[]
    )
{
    HRESULT hr;

    hr = CMainApp::MainHR(argc, argv);
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

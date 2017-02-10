#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR(
        __in int argc,
        __in_ecount(argc) WCHAR* argv[]
        )
    {
        HMODULE hModule;

        ChkProlog();

        hModule = ::LoadLibraryW(argv[1]);
        ChkWin32(hModule);
        FreeLibrary(hModule);

        ChkNoCleanup();
    }
};

int 
__cdecl
wmain(
    __in int argc,
    __in_ecount(argc) WCHAR* argv[]
    )
{
    HRESULT hr = S_OK;
    CMainApp app;

    if (argc == 1)
    {
        wprintf(L"USAGE: host.exe [dll_path_to_load]\n");
    }
    else
    {
        hr = app.MainHR(argc, argv);
        if (FAILED(hr))
        {
            wprintf(L"HRESULT: 0x%08X\n", hr);
        }
        else
        {
            wprintf(L"Success.\n");
        }
    }
    return hr;
}

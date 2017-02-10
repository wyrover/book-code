#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR()
    {
        BYTE* pMem;

        ChkProlog();

        for ( ; ; )
        {
            // BUG! Leaked NT heap memory allocation.
            pMem = new BYTE[1024];
            ChkAlloc(pMem);
            ::Sleep(50);
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

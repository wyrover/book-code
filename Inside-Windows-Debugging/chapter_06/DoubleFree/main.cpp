#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR()
    {
        CHAR* psz = NULL;

        ChkProlog();

        psz = new CHAR[10];
        ChkAlloc(psz);

        delete[] psz;
        //psz = NULL;

        ChkCleanup();

        // BUG! Double-free of the psz buffer
        //if (psz)
        {
            delete[] psz;
        }

        ChkEpilog();
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

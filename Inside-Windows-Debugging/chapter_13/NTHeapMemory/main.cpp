#include "stdafx.h"

class CMainApp
{
private:
    static
    HRESULT
    LeakOneBuffer(
        )
    {
        LPBYTE pMem = NULL;
        DWORD n;

        ChkProlog();

        wprintf(L"Leaking the last buffer in %d allocations.\n", NUM_BUFFERS);
        for (n = 0; n < NUM_BUFFERS; n++)
        {
            if (pMem != NULL)
            {
                delete[] pMem;
            }
            pMem = new BYTE[SIZE_OF_EACH_BUFFER];
            ChkAlloc(pMem);
        }

        ChkNoCleanup();
    }

public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        DWORD n;

        ChkProlog();

        wprintf(L"Sleep for 3 seconds...\n\n");
        ::Sleep(3000);

        for (n = 0; n < NUM_ITERATIONS; n++)
        {
            wprintf(L"Interation #%d...\n", n + 1);
            ::Sleep(500);
            ChkHr(LeakOneBuffer());
        }

        wprintf(L"Sleep for 3 seconds...\n\n");
        ::Sleep(3000);

        ChkNoCleanup();
    }

private:
    static const NUM_ITERATIONS = 10;
    static const NUM_BUFFERS = 50;
    static const SIZE_OF_EACH_BUFFER = 1024;
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

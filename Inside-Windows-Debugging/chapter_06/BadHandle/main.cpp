#include "stdafx.h"

class CMainApp
{
public:
    static
    HRESULT
    MainHR()
    {
        CHandle shThread;

        ChkProlog();

        shThread.Attach(OpenThread(
            THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId()));
        ChkWin32(shThread);

        // BUG! Use of a thread instead of a process handle
        ChkWin32(TerminateProcess(shThread, EXIT_SUCCESS));

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

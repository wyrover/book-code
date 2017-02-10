#include "stdafx.h"

class CThreadParams :
    CZeroInit<CThreadParams>
{
public:
    HRESULT
    Init(
        VOID
        )
    {
        ChkProlog();

        ChkHr(m_csOne.Init());
        ChkHr(m_csTwo.Init());

        ChkNoCleanup();
    }

    CWin32CriticalSection m_csOne;
    CWin32CriticalSection m_csTwo;
    LONG m_nThreadsStarted;
};

class CMainApp
{
private:
    static
    DWORD
    WINAPI
    WorkerThread(
        __in LPVOID lpParameter
        )
    {
        CThreadParams* pParameter;
        CWin32CriticalSectionHolder autoLockOne, autoLockTwo;

        pParameter = reinterpret_cast<CThreadParams*>(lpParameter);
        InterlockedIncrement(&(pParameter->m_nThreadsStarted));
        wprintf(L"Thread #%d Callback.\n", pParameter->m_nThreadsStarted);

        if (pParameter->m_nThreadsStarted % 2 == 0)
        {
            autoLockOne.Lock(pParameter->m_csOne);
            wprintf(L"One-Two locking order...\n");
            Sleep(1000);
            autoLockTwo.Lock(pParameter->m_csTwo);
        }
        else
        {
            autoLockTwo.Lock(pParameter->m_csTwo);
            wprintf(L"Two-One locking order...\n");
            Sleep(1000);
            autoLockOne.Lock(pParameter->m_csOne);
        }

        return EXIT_SUCCESS;
    }

public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        CHandle shThreads[NUM_THREADS];
        CThreadParams params;
        int n, nWait;

        ChkProlog();

        ChkHr(params.Init());

        //
        // Create new worker threads
        //
        for (n = 0; n < ARRAYSIZE(shThreads); n++)
        {
            shThreads[n].Attach(::CreateThread(
                NULL, 0, WorkerThread, &params, 0, NULL));
            ChkWin32(shThreads[n]);
        }

        //
        // Wait for the thread callback routines to finish
        //
        nWait = WaitForMultipleObjects(
            ARRAYSIZE(shThreads), (HANDLE*)shThreads, TRUE, INFINITE);
        ChkWin32(nWait != WAIT_FAILED);

        ChkNoCleanup();
    }

private:
    static const NUM_THREADS = 2;
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

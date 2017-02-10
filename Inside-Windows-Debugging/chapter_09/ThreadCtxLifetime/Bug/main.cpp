#include "stdafx.h"

class CThreadParams :
    CZeroInit<CThreadParams>
{
public:
    HRESULT
    Init(
        __in const WCHAR* pwszMessage
        )
    {
        size_t cch;

        ChkProlog();

        ASSERT(pwszMessage);

        if (m_spMessage)
        {
            m_spMessage.Free();
        }

        cch = wcslen(pwszMessage);
        m_spMessage.Attach(new WCHAR[cch + 1]);
        ChkAlloc(m_spMessage);

        ChkBool(!wcscpy_s(m_spMessage, cch + 1, pwszMessage), E_FAIL);

        ChkNoCleanup();
    }

    CAutoPtr<WCHAR> m_spMessage;
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

        pParameter = reinterpret_cast<CThreadParams*>(lpParameter);
        wprintf(L"Test message... %s\n", pParameter->m_spMessage);

        return EXIT_SUCCESS;
    }

public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        CAutoPtr<CThreadParams> spParameter;
        CHandle shThreads[NUM_THREADS];
        int n;

        ChkProlog();

        spParameter.Attach(new CThreadParams());
        ChkAlloc(spParameter);

        ChkHr(spParameter->Init(L"Hello World!"));

        //
        // Create new worker threads with non-ref counted shared state
        //
        for (n = 0; n < ARRAYSIZE(shThreads); n++)
        {
            shThreads[n].Attach(::CreateThread(
                NULL, 0, WorkerThread, spParameter, 0, NULL));
            ChkWin32(shThreads[n]);
        }

        //
        // Do not Wait for the worker threads to exit
        //   DWORD nWait = WaitForMultipleObjects(ARRAYSIZE(shThreads), (HANDLE*)shThreads, TRUE, INFINITE);
        //   ChkWin32(nWait != WAIT_FAILED);
        //
        Sleep(10);

        ChkNoCleanup();
    }

private:
    static const NUM_THREADS = 16;
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

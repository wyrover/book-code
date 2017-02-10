#include "stdafx.h"
#include "refcountimpl.h"

class CThreadParams :
    CZeroInit<CThreadParams>,
    public CRefCountImpl
{
public:
    ~CThreadParams(
        VOID
        )
    {
        wprintf(L"Object 0x%p destroyed\n", this);
    }

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
        CComPtr<CThreadParams> spRefCountedParameter;

        spRefCountedParameter.Attach(reinterpret_cast<CThreadParams*>(lpParameter));
        wprintf(L"Test message... %s\n", spRefCountedParameter->m_spMessage);

        return EXIT_SUCCESS;
    }

public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        CThreadParams* pRefCountedParameter;
        CComPtr<CThreadParams> spRefCountedParameter;
        CHandle shThreads[NUM_THREADS];
        DWORD dwLastError;
        int n;

        ChkProlog();

        spRefCountedParameter.Attach(new CThreadParams());
        ChkAlloc(spRefCountedParameter);
        pRefCountedParameter = static_cast<CThreadParams*>(spRefCountedParameter);

        ChkHr(spRefCountedParameter->Init(L"Hello World!"));

        //
        // Create new worker threads with reference-counted shared state
        //
        for (n = 0; n < ARRAYSIZE(shThreads); n++)
        {
            pRefCountedParameter->AddRef();
            shThreads[n].Attach(::CreateThread(
                NULL, 0, WorkerThread, spRefCountedParameter, 0, NULL));
            dwLastError = ::GetLastError();
            if (shThreads[n] == NULL)
            {
                pRefCountedParameter->Release();
            }
            ChkBool(shThreads[n], HRESULT_FROM_WIN32(dwLastError));
        }

        //
        // Do not Wait for worker threads to exit
        //    DWORD nWait = WaitForMultipleObjects(
        //        ARRAYSIZE(shThreads), (HANDLE*)shThreads, TRUE, INFINITE);
        //    ChkWin32(nWait != WAIT_FAILED);
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

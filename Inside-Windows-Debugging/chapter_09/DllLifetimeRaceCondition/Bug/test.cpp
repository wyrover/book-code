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
    AsynchronousCall(
        VOID
        )
    {
        CHandle shThreads[NUM_THREADS];
        int n;

        ChkProlog();

        ChkHr(g_params.Init(L"Hello World!"));

        //
        // Create new worker threads
        //
        for (n = 0; n < ARRAYSIZE(shThreads); n++)
        {
            shThreads[n].Attach(::CreateThread(
                NULL, 0, WorkerThread, &g_params, 0, NULL));
            ChkWin32(shThreads[n]);
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
    static CThreadParams g_params;
};

CThreadParams CMainApp::g_params;

#ifdef __cplusplus
extern "C" {
#endif

    //
    // Export a C-style DLL function (with __stdcall calling convention)
    //
    __control_entrypoint(DllExport)
    HRESULT
    WINAPI
    AsynchronousCall(
        VOID
        )
    {
        ChkProlog();

        ChkHr(CMainApp::AsynchronousCall());

        ChkNoCleanup();
    }

#ifdef __cplusplus
}
#endif

BOOL
WINAPI
DllMain(
    __in                        HINSTANCE               hInstance,
    __in                        DWORD                   dwReason,
    __in                        PVOID                   pReserved
    )
{
    HRESULT                                             hr = S_OK;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            ::DisableThreadLibraryCalls(hInstance);
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            ASSERT(FALSE);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return SUCCEEDED(hr);
    UNREFERENCED_PARAMETER(pReserved);
}

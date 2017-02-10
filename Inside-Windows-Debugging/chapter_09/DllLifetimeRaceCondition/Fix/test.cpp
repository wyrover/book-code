#include "stdafx.h"

class CThreadParams :
    CZeroInit<CThreadParams>
{
public:
    HRESULT
    Init(
        __in const WCHAR* pwszMessage,
        __in const WCHAR* pwszDllCallbackPath
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

        m_hCallbackLibrary = LoadLibraryW(pwszDllCallbackPath);
        ChkWin32(m_hCallbackLibrary);

        ChkNoCleanup();
    }

    ~CThreadParams(
        VOID
        )
    {
        if (m_hCallbackLibrary)
        {
            FreeLibrary(m_hCallbackLibrary);
        }
    }

    CAutoPtr<WCHAR> m_spMessage;
    HMODULE m_hCallbackLibrary;
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
        HMODULE hLib = NULL;

        pParameter = reinterpret_cast<CThreadParams*>(lpParameter);
        wprintf(L"Test message... %s\n", pParameter->m_spMessage);

        hLib = pParameter->m_hCallbackLibrary;
        ASSERT(hLib);

        //
        // Assume ownership of the module handle, then free it
        // and exit the thread in a single transaction
        //
        pParameter->m_hCallbackLibrary = NULL;
        FreeLibraryAndExitThread(hLib, EXIT_SUCCESS);

        //
        // The code should never reach this point
        //
        ASSERT(FALSE);
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

        //
        // Create new worker threads
        //
        for (n = 0; n < ARRAYSIZE(shThreads); n++)
        {
            ChkHr(g_params[n].Init(L"Hello World!", L"test.dll"));

            shThreads[n].Attach(::CreateThread(
                NULL, 0, WorkerThread, &(g_params[n]), 0, NULL));
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
    static CThreadParams g_params[NUM_THREADS] ;
};

CThreadParams CMainApp::g_params[NUM_THREADS];

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

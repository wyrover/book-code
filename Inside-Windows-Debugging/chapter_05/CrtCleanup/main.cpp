#include "stdafx.h"

class CBuffer :
    CZeroInit<CBuffer>
{
public:
    HRESULT
    Init(
        VOID
        )
    {
        ChkProlog();

        if (m_spFirstBuffer)
        {
            m_spFirstBuffer.Free();
        }
        m_spFirstBuffer.Attach(new BYTE[BUFFER_SIZE]);
        ChkAlloc(m_spFirstBuffer);

        if (m_spSecondBuffer)
        {
            m_spSecondBuffer.Free();
        }
        m_spSecondBuffer.Attach(new BYTE[BUFFER_SIZE]);
        ChkAlloc(m_spSecondBuffer);

        ChkNoCleanup();
    }

private:
    CAutoPtr<BYTE> m_spFirstBuffer;
    CAutoPtr<BYTE> m_spSecondBuffer;
    static const BUFFER_SIZE = 1024;
};

class CMainApp
{
public:
    HRESULT
    MainHR(
        VOID
        )
    {
        ChkProlog();

        wprintf(L"Initializing Global C++ Object...\n");
        ChkHr(g_buffer.Init());

        ChkNoCleanup();
    }

private:
    static CBuffer g_buffer;
};

CBuffer CMainApp::g_buffer;

int
__cdecl
wmain(
    VOID
    )
{
    HRESULT hr;
    CMainApp app;

    hr = app.MainHR();
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

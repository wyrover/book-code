#include "stdafx.h"
#include "etwtrace.h"

// 35f7872e-9b6d-4a9b-a674-66f1edd66d5c
const GUID GUID_AppEtwProvider = 
    { 0x35f7872e, 0x9b6d, 0x4a9b, { 0xa6, 0x74, 0x66, 0xf1, 0xed, 0xd6, 0x6d, 0x5c } };

class CMainApp
{
private:
    static
    VOID
    LogEvent(
        __in USHORT EventId,
        __in UCHAR Opcode,
        __in UCHAR Level,
        __in ULONGLONG Keyword,
        __in USHORT Task
        )
    {
        EVENT_DESCRIPTOR eventDesc = {0};

        eventDesc.Id = EventId;
        eventDesc.Version = 1;
        eventDesc.Task = Task;
        eventDesc.Opcode = Opcode;
        eventDesc.Level = Level;
        eventDesc.Keyword = Keyword;

        g_AppEtwProvider.Write(eventDesc);
    }

public:
    static
    HRESULT
    MainHR(
        VOID
        )
    {
        ChkProlog();

        //
        // Mark the start of this routine with a Start opcode event
        //
        LogEvent(1, WINEVENT_OPCODE_START, WINEVENT_LEVEL_INFO, WINEVT_KEYWORD_ANY, WINEVENT_TASK_NONE);

        Sleep(5000);

        //
        // Mark the end of this routine with a Stop opcode event
        //
        LogEvent(2, WINEVENT_OPCODE_STOP, WINEVENT_LEVEL_INFO, WINEVT_KEYWORD_ANY, WINEVENT_TASK_NONE);

        ChkNoCleanup();
    }

private:
    static CEtwProvider g_AppEtwProvider;
};

CEtwProvider
CMainApp::g_AppEtwProvider(GUID_AppEtwProvider);

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

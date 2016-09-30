// SimpleForums.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
    CDebugReportHook g_ReportHook;
#endif

#include "SimpleForums.h"
#include "ForumListHandler.h"
#include "EditForumHandler.h"

class CSimpleForumsModule : public CAtlDllModuleT<CSimpleForumsModule>
{
public:
};

CSimpleForumsModule _AtlModule;


// TODO: Add additional request handlers to the handler map

BEGIN_HANDLER_MAP()
HANDLER_ENTRY("Default", CSimpleForumsHandler)
END_HANDLER_MAP()

#ifdef _MANAGED
    #pragma managed(push, off)
#endif

// DLL Entry Point
//
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}

#ifdef _MANAGED
    #pragma managed(pop)
#endif


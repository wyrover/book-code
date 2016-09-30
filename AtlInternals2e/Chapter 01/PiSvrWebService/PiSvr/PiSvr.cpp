// PiSvr.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
    CDebugReportHook g_ReportHook;
#endif

#include "PiSvr.h"
[ module(name = "MyPiSvr", type = "dll") ]
class CDllMainOverride
{
public:
    BOOL WINAPI DllMain(DWORD dwReason, LPVOID lpReserved)
    {
#if defined(_M_IX86)

        if (dwReason == DLL_PROCESS_ATTACH) {
            // stack overflow handler
            _set_security_error_handler(AtlsSecErrHandlerFunc);
        }

#endif
        return __super::DllMain(dwReason, lpReserved);
    }
};

[ emitidl(restricted) ];

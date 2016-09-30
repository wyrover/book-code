// SimpleForumsIsapi.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
    CDebugReportHook g_ReportHook;
#endif

class CSimpleForumsModule : public CAtlDllModuleT<CSimpleForumsModule>
{
public:
};

CSimpleForumsModule _AtlModule;


#include "SimpleForumsextension.h"

typedef CSimpleForumsExtension<CThreadPool<CSimpleForumsExtensionWorker> > ExtensionType;

// The ATL Server ISAPI extension
ExtensionType theExtension;


// Delegate ISAPI exports to theExtension
//
extern "C" DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB)
{
    return theExtension.HttpExtensionProc(lpECB);
}

extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
    return theExtension.GetExtensionVersion(pVer);
}

extern "C" BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
    return theExtension.TerminateExtension(dwFlags);
}

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


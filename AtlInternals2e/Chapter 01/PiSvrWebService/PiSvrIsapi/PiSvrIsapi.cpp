// PiSvrIsapi.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
// For custom assert and trace handling with WebDbg.exe
#ifdef _DEBUG
    CDebugReportHook g_ReportHook;
#endif
[ module(name = "MyPiSvr", type = "dll") ];
[ emitidl(restricted) ];

typedef CIsapiExtension<> ExtensionType;

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

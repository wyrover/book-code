// HelloISAPI2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <HttpExt.h>

#ifdef _MANAGED
    #pragma managed(push, off)
#endif

// Our module class definition
class CHelloISAPI2Module : public CAtlDllModuleT<CHelloISAPI2Module>
{
public:
};

// Required global instance of module.
CHelloISAPI2Module _AtlModule;

// Initialize / shutdown module on DLL load or unload
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}

#ifdef _MANAGED
    #pragma managed(pop)
#endif

BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO *pVer)
{
    pVer->dwExtensionVersion = HSE_VERSION;
    strncpy_s(pVer->lpszExtensionDesc,
              HSE_MAX_EXT_DLL_NAME_LEN,
              "Hello ISAPI Extension", _TRUNCATE);
    return TRUE;
}

DWORD WINAPI HttpExtensionProc(EXTENSION_CONTROL_BLOCK *pECB)
{
    CComObject< CServerContext > *pCtx;
    CComObject< CServerContext >::CreateInstance(&pCtx);
    pCtx->Initialize(pECB);
    // We use this smart pointer to ensure proper cleanup
    CComPtr< IUnknown > spContextUnk(pCtx);
    CHttpRequest request(pCtx);
    CHttpResponse response(pCtx);
    response << "<html>" <<
             "<head>" <<
             "<title>Hello from ISAPI</title>" <<
             "</head>" <<
             "<body>" <<
             "<h1>Hello from an ISAPI Extension</h1>";
    CStringA name;
    name = request.GetQueryParams().Lookup("name");

    if (name.IsEmpty()) {
        response << "<p>You didn't give your name";
    } else {
        response << "<p>Your name is: " << name;
    }

    response << "</body>" <<
             "</html>";
    return HSE_STATUS_SUCCESS;
}

BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
    return TRUE;
}



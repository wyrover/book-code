// HelloISAPI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <HttpExt.h>

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

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
    char *header =
        "<html>\r\n"
        "<head>\r\n"
        "<title>Hello from ISAPI</title>\r\n"
        "</head>\r\n"
        "<body>\r\n"
        "<h1>Hello from an ISAPI Extension</h1>\r\n";
    DWORD size = static_cast< DWORD >(strlen(header));
    pECB->WriteClient(pECB->ConnID, header, &size, 0);
    char *intro = "<p>Your name is: ";
    size = static_cast< DWORD >(strlen(intro));
    pECB->WriteClient(pECB->ConnID, intro, &size, 0);
    size = static_cast< DWORD >(
               strlen(pECB->lpszQueryString));
    pECB->WriteClient(pECB->ConnID, pECB->lpszQueryString,
                      &size, 0);
    char *footer =
        "\r\n</body>\r\n"
        "</html>\r\n";
    size = static_cast< DWORD >(strlen(footer));
    pECB->WriteClient(pECB->ConnID, footer, &size, 0);
    return HSE_STATUS_SUCCESS;
}

BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
    return TRUE;
}



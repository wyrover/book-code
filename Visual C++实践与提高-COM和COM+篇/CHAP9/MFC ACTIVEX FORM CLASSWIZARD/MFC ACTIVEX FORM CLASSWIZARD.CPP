// MFC ActiveX Form ClassWizard.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "MFC ActiveX Form ClassWizard.h"
#include "MFC ActiveX Form ClassWizardaw.h"

#ifdef _PSEUDO_DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE MFCActiveXFormClassWizardDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        TRACE0("MFC ACTIVEX FORM CLASSWIZARD.AWX Initializing!\n");
        // Extension DLL one-time initialization
        AfxInitExtensionModule(MFCActiveXFormClassWizardDLL, hInstance);
        // Insert this DLL into the resource chain
        new CDynLinkLibrary(MFCActiveXFormClassWizardDLL);
        // Register this custom AppWizard with MFCAPWZ.DLL
        SetCustomAppWizClass(&MFCActiveXFormClassWizardaw);
    } else if (dwReason == DLL_PROCESS_DETACH) {
        TRACE0("MFC ACTIVEX FORM CLASSWIZARD.AWX Terminating!\n");
        // Terminate the library before destructors are called
        AfxTermExtensionModule(MFCActiveXFormClassWizardDLL);
    }

    return 1;   // ok
}

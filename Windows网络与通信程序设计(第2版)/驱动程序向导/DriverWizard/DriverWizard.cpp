// DriverWizard.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DriverWizard.h"
#include "DriverWizardaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE DriverWizardDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DRIVERWIZARD.AWX Initializing!\n");
		
		// Extension DLL one-time initialization
		AfxInitExtensionModule(DriverWizardDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(DriverWizardDLL);

		// Register this custom AppWizard with MFCAPWZ.DLL
		SetCustomAppWizClass(&DriverWizardaw);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DRIVERWIZARD.AWX Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(DriverWizardDLL);
	}
	return 1;   // ok
}

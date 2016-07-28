// NetAppSrv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AppSrv.h"
#include "reflectorservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

//using namespace std;

int _cdecl _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	  int nRetCode = 0;

	  // initialize MFC and print and error on failure
	  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	  {
      // TODO: change error code to suit your needs
      wprintf(L"Fatal Error: MFC initialization failed\n");
      nRetCode = 1;
	  }
	  else
	  {
        // Create the service object
        CReflectorService reflectorService;
    
        // Parse for standard arguments (install, uninstall, version etc.)
        if (!reflectorService.ParseStandardArgs(argc, argv)) {

            // Didn't find any standard args so start the service
            // Uncomment the DebugBreak line below to enter the debugger
            // when the service is started.
		    //DebugBreak();
            reflectorService.StartService();
        }

        // When we get here, the service has been stopped
        nRetCode = reflectorService.m_Status.dwWin32ExitCode;

	}

	return nRetCode;
}



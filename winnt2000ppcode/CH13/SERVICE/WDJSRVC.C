#include <windows.h>
#include <stdio.h>
#include "srvctmpl.h"

TCHAR szSomeThreadInfo[] = TEXT("SomeThreadInfo");

//---------------------------------------------------------
#ifdef __BORLANDC__
#   pragma argsused
#endif
BOOL ServiceInitialization(DWORD dwArgc, LPTSTR *lpszArgv)
{
   OutputDebugString(TEXT("ServerInitialization\n"));
   return TRUE;
}

//---------------------------------------------------------
VOID ServiceTermination(VOID)
{
   OutputDebugString(TEXT("ServiceTermination\n"));
}

//---------------------------------------------------------
DWORD MainServiceThread(LPDWORD ThreadParam)
{
   OutputDebugString((LPTSTR)ThreadParam);

   while (TRUE) {
      Sleep(5000);
	  OutputDebugString(TEXT(".\n"));
   }
}


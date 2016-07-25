#include <windows.h>
#include <stdio.h>
#include "srvctmpl.h"

// private prototypes
BOOL NotifySCM(DWORD, DWORD, DWORD);
VOID ServiceMain(DWORD, LPTSTR *);
VOID ServiceHandler(DWORD);

HANDLE  hDoneEvent = NULL, hThread = NULL;
DWORD   dwCurrentState;
SERVICE_STATUS_HANDLE  hService;

//--------------------------------------------------------------
void main(void)
{
   SERVICE_TABLE_ENTRY ServiceTable[] = {
      {GENSRV_ServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
      {NULL, NULL}
   };

   // connect to  the service control manager
   StartServiceCtrlDispatcher(ServiceTable);
}

//--------------------------------------------------------------
VOID ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
   DWORD   ThreadId;

   if (!(hService = RegisterServiceCtrlHandler(
         GENSRV_ServiceName, 
         (LPHANDLER_FUNCTION)ServiceHandler)))
      return;

   NotifySCM(SERVICE_START_PENDING, 0, 1);

   if (!ServiceInitialization(dwArgc, lpszArgv))
      return;

   NotifySCM(SERVICE_START_PENDING, 0, 2);

   if ((hDoneEvent=CreateEvent(NULL,FALSE,FALSE,NULL)) == 0)
      return;

   NotifySCM(SERVICE_START_PENDING, 0, 3);

   if ((hThread = CreateThread(0, 0,
         (LPTHREAD_START_ROUTINE)MainServiceThread, 0, 0,
         &ThreadId)) == 0) {
      CloseHandle(hDoneEvent);
      return;
   }

   NotifySCM(SERVICE_RUNNING, 0, 0);

   WaitForSingleObject(hDoneEvent, INFINITE);

   CloseHandle(hThread);
   ExitThread(ThreadId);
   CloseHandle(hDoneEvent);
   return;
}

//--------------------------------------------------------------
VOID ServiceHandler(DWORD fdwControl)
{
   switch(fdwControl) {
      case SERVICE_CONTROL_STOP:
         OutputDebugString("Stop\n");
         NotifySCM(SERVICE_STOP_PENDING, 0, 1);
         SetEvent(hDoneEvent);
         NotifySCM(SERVICE_STOPPED, 0, 0);
         break;

      case SERVICE_CONTROL_PAUSE:
         OutputDebugString("Pause\n");
         NotifySCM(SERVICE_PAUSE_PENDING, 0, 1);
         SuspendThread(hThread);
         NotifySCM(SERVICE_PAUSED, 0, 0);
         break;

      case SERVICE_CONTROL_CONTINUE:
         OutputDebugString("Continue\n");
         NotifySCM(SERVICE_CONTINUE_PENDING, 0, 1);
         ResumeThread(hThread);
         NotifySCM(SERVICE_RUNNING, 0, 0);
         break;

      case SERVICE_CONTROL_INTERROGATE:
         OutputDebugString("Interrogate\n");
         NotifySCM(dwCurrentState, 0, 0);
         break;

      case SERVICE_CONTROL_SHUTDOWN:
         OutputDebugString("Shutdown\n");
         ServiceTermination();
         break;
   }
}

//--------------------------------------------------------------
BOOL NotifySCM(DWORD dwState, DWORD dwWin32ExitCode,
      DWORD dwProgress)
{
   SERVICE_STATUS ServiceStatus;

   // fill in the SERVICE_STATUS structure
   ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
   ServiceStatus.dwCurrentState = dwCurrentState = dwState;
   ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
      SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
   ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
   ServiceStatus.dwServiceSpecificExitCode = 0;
   ServiceStatus.dwCheckPoint = dwProgress;
   ServiceStatus.dwWaitHint = 3000;

   // send status to SCM
   return SetServiceStatus(hService, &ServiceStatus);
}

//servicehandler.cpp
#include "std.h"

void MyServiceHandler(DWORD fdwControl)
{
	// 현재 상태와 같은 제어 코드일 경우는 처리할 필요 없다.
	if (fdwControl == g_NowState)
		return;

	switch (fdwControl) {
	// 서비스를 일시 중지하라 
	case SERVICE_CONTROL_PAUSE:
		MySetStatus(SERVICE_PAUSE_PENDING,0);
		g_bPause=TRUE;
		MySetStatus(SERVICE_PAUSED);
		break;
	// 일시 중지된 서비스를 재개하라.
	case SERVICE_CONTROL_CONTINUE:
		MySetStatus(SERVICE_CONTINUE_PENDING,0);
		g_bPause=FALSE;
		MySetStatus(SERVICE_RUNNING);
		break;
	// 서비스를 중지하라 
	case SERVICE_CONTROL_STOP:
		MySetStatus(SERVICE_STOP_PENDING,0);
		SetEvent(g_ExitEvent);
		break;
	// 서비스의 현재 상태를 알려달라 
	case SERVICE_CONTROL_INTERROGATE:
	default:
		MySetStatus(g_NowState);
		break;
	}
}

void MySetStatus(DWORD dwState, DWORD dwAccept)
{
	SERVICE_STATUS ss;
	ss.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState=dwState;
	ss.dwControlsAccepted=dwAccept;
	ss.dwWin32ExitCode=0;
	ss.dwServiceSpecificExitCode=0;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=0;

	// 현재 상태를 보관해 둔다.
	g_NowState=dwState;
	SetServiceStatus(g_hSrv,&ss);
}

#include "std.h"

SERVICE_STATUS_HANDLE g_hSrv;	//서비스 핸들러
DWORD	g_NowState;				//서비스 기본 코드(자신의 상태를 저장)
BOOL	g_bPause;
HANDLE	g_ExitEvent;

void MyServiceMain(DWORD argc, LPTSTR *argv)
{
	// 서비스 핸들러를 등록한다.
	g_hSrv = RegisterServiceCtrlHandlerEx(SERVICE_NAME,(LPHANDLER_FUNCTION_EX)MyServiceHandler, 0);
	if (g_hSrv==0) 
	{
		return;
	}

	// 서비스가 시작중임을 알린다.
	MySetStatus(SERVICE_START_PENDING);
	
	// 전역 변수를 초기화한다.
	g_bPause=FALSE;

	// 이벤트를 생성한다.
	g_ExitEvent=CreateEvent(NULL, TRUE, FALSE, "MemStatExit");

	// 서비스 구동 부 초기화.
	myservice_init();
	
	// 서비스가 시작되었음을 알린다.
	MySetStatus(SERVICE_RUNNING);

	// 10초에 한번씩 메모리 통계를 작성한다.
	for (;;)
	{
		if (g_bPause == FALSE) 
		{
			// 서비스 구동 부.
			myservice_run();
		}
		if (WaitForSingleObject(g_ExitEvent, 10000) == WAIT_OBJECT_0)
			break;
	}
	MySetStatus(SERVICE_STOPPED);
}

int main()
{
	SERVICE_TABLE_ENTRY ste[]=
	{
		{SERVICE_NAME,	 (LPSERVICE_MAIN_FUNCTION)MyServiceMain},	// 서비스 명 , 서비스 메인 함수 
		{NULL,NULL}
	};
	StartServiceCtrlDispatcher(ste);
	return 0;
}







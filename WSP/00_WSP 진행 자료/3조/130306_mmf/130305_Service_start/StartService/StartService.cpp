//StartService.cpp
#include "std.h"

SERVICE_STATUS_HANDLE g_hSrv;   //...
DWORD g_NowState;				//...
BOOL g_bPause;					//...
HANDLE g_ExitEvent;				//...

void MyServiceMain(DWORD argc, LPTSTR *argv)
{
#ifndef MY_DEBUG
	AllocConsole(); // console 창 생성

	freopen( "CONOUT$",   // console 을
			 "wt",        // text write 모드로
			 stdout);	  // stdout 과 연결..
#endif

	DWORD Context = 1;
	// 서비스 핸들러를 등록한다.
	//g_hSrv=RegisterServiceCtrlHandler(SERVICE_NAME,(LPHANDLER_FUNCTION)MyServiceHandler);
	g_hSrv=RegisterServiceCtrlHandlerEx(SERVICE_NAME,(LPHANDLER_FUNCTION_EX)MyServiceHandler,&Context);
	if (g_hSrv==0) {
		return;
	}

	// 서비스가 시작중임을 알린다.
	MySetStatus(SERVICE_START_PENDING);
	
	// 전역 변수를 초기화한다.
	g_bPause=FALSE;

	// 이벤트를 생성한다.
	g_ExitEvent=CreateEvent(NULL, TRUE, FALSE, "StatExit");

	myservice_init();	
	
	// 서비스가 시작되었음을 알린다.
	MySetStatus(SERVICE_RUNNING);

	// 10초에 한번씩 메모리 통계를 작성한다.
	for (;;) 
	{
		if (g_bPause == FALSE) 
		{
			//server_run();

		}
		//=============================================================================================
		if (WaitForSingleObject(g_ExitEvent, 0) == WAIT_OBJECT_0)
			break;
	}
	myservice_exit();
	MySetStatus(SERVICE_STOPPED);
}

int main()
{
	SERVICE_TABLE_ENTRY ste[]={
		{SERVICE_NAME,	 (LPSERVICE_MAIN_FUNCTION)MyServiceMain},
		{NULL,NULL}
	};

	StartServiceCtrlDispatcher(ste);
	return 0;
}




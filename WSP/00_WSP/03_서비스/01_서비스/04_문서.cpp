/****************************************************************

  [ 제어 핸들러로 SCM에서송신하는 제어 코드에 응답한다.]

  서비스가 시작한 후에 제어가 필요하면 SCM은 제어 핸들러를 호출
  해서 서비스에 이 사실을 통지한다.
  
  사용자가 서비스 애플릿 메뉴에서 '중지', '일시정지' 등을 선택했
  을 경우가 그 예이다.

  - 제어 핸들러가 받는 주요 제어 코드 
  ------------------------------------------------------
  제어코드						설명
  -----------------------------------------------------
  SERVICE_CONTROL_PAUSE			서비스 일시 정지
  SERVICE_CONTROL_CONTINUE		일시정지중인 서비스 재시작
  SERVICE_CONTROL_STOP			서비스 정지
  SERVICE_CONTROL_INTERROGATE	서비스 최신 상태 보고
  SERVICE_CONTROL_PARAMCHANGE	서비스의 전달 인자 변경
  SERVICE_CONTROL_SHUTDOWN		시스템 셧다운 통지 
  -------------------------------------------------------

  - 핸들러 호출은 먼저 main 함수에서 StartServiceCtrlDispatcher
    API 를 호출했을 때의 스레드(메인 스레드)가 사용된다.

    즉 제어 핸들러와 ServiceMain 함수는 먼저 시작된 프로세스 내의 
	독립된 스레드이다. 

    즉 서로간 제어가 필요하다면 스레드간 통신이 필요하다. 

  - 제어 핸들러는 호출된 뒤 30초 이내에 복귀해야 한다.(중요)
    장시간 제어를 반환하지 않으면 서비스를 제어할 수 없게 된다.
	만약 그 이상의 시간이 필요한 경우에는 따로 스레드를 만들어서 
	처리하도록 하고, 핸들러는 대기하지 않고 바로 복귀시켜야 한다.

  - 핸들러 복귀시 처리해야 할 사항
    SetServiceStatus API로 서비스 상태 갱신 

    만약 처리가 시간에 맞지 않아서 따로 스레드를 만들었을 경우는
	처리중임을 나타내는 ~PENDING을 우선 넘기고 처리가 완료된 시점에서
	다시  SetServiceStatus를 호출해 상태를 갱신한다.
	이는 시작할 때의 순서와 거의 같다. 
   
  // 제어 핸들러 
  void WINAPI BeepServiceHandler(DWORD fdwControl)
  {
	char buf[256];

	SERVICE_STATUS status = {
		SERVICE_WIN32_OWN_PROCESS,			// 서비스 종류 
		SERVICE_RUNNING,					// 서비스 상태 
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE,	// 수신 가능한 제어코드 
			0, 0, 0, 0)};

	switch( fdwControl )
	{
		case SERVICE_CONTROL_PAUSE:
			g_state = SERVICE_PAUSED;
			OutputDebugString("일시정지\n");
			SetEvent(g_event);
			WaitForSingleObject(g_event2, INFINITE);
			break;
		case SERVICE_CONTROL_CONTINUE:
			g_state = SERVICE_RUNNING;
			OutputDebugString("재시작\n");
			SetEvent(g_event);
			WaitForSingleObject(g_event2, INFINITE);
			break;
		case SERVICE_CONTROL_STOP:
			g_state = SERVICE_STOPPED;
			OutputDebugString("중지\n");
			SetEvent(g_event);
			WaitForSingleObject(g_event2, INFINITE);
			break;
		case SERVICE_CONTROL_INTERROGATE:	// 서비스 최신상태 보고
			break;
		default:
			sprintf(buf, "지원하지 않는 제어코드 : 0x%081x\n", fdwControl);

			OutputDebugString(buf);

	}
	status.dwCurrentState = g_state;
	SetServiceStatus(g_ssh, &status);

	return;
}

****************************************************************/

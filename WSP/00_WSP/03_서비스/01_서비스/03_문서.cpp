/****************************************************************

  [ 실질적인 처리는 ServiceMain함수에서 한다.]

  ServiceMain함수의 예제를 볼수 있다.

  이 함수의 인자는 main함수와 마찬가지로 시작 할 때 넘겨지는
  전달인자의 개수와 전달인자의 목록의 포인터이다.
 
  서비스 애플릿 속성 대화상자의 '시작 매개변수' 항목에 지정된
  문자열이 넘어간다. 


#define BEEPSERVICE_NAME	"BoopService"
SERVICE_STATUS_HANDLE g_ssh = NULL;
HANDLE g_event = NULL;
HANDLE g_event2 = NULL;
DWORD g_state = SERVICE_STOPPED;

void WINAPI BeepServiceMain( DWORD dwArgc, LPTSTR *lpszArgv)
{

	//1. SCM에 제어 핸들러를 등록하기 위한 API---------------------------------
	//   - SCM은 서비스를 제어할 필요가 생겼을 때 이 제어 핸들러를 호출하는
	//     방법으로 서비스에 통지한다.
	//     이것이 등록되어야  SCM이 서비스 프로그램을 제어할 수 있다.
	g_ssh = RegisterServiceCtrlHandler(
		BEEPSERVICE_NAME, BeepServiceHandler);
	if(g_ssh == NULL)
	{
		MessageBox(NULL, 
			"BeepService를 시작할 수 없습니다.",
			"BeepService에러", MB_OK|MB_ICONERRRO|
			MB_SERVICE_NOTIFICATION);
			return;
	}

	//이벤트 객체 생성, 초기 상태는 리셋
	// g_state : 서비스가 실행중임을 나타냄
	// g_event : 제어 핸들러가 상태 변경이 필요함을 ServiceMain에 통지하기 위함
	// g_event2: 상태 변경이 완료된 것을 ServiceMain이 핸들러에 통지하기 위함
	// ServiceMain과 핸들러는 다른 스레드로 동작하기 때문에 동기화가 필요하다.

	g_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_event2= CreateEvent(NULL, FALSE, FALSE, NULL);
	g_state = SERVICE_RUNNING;

	// 초기화 종료 보고
	SERVICE_STATUS status = {
		SERVICE_WIN32_OWN_PROCESS,			// 서비스 종류 
		SERVICE_RUNNING,					// 서비스 상태 
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE,	// 수신 가능한 제어코드 
			0, 0, 0, 0)};


	// 2. 서비스가 시작 상태가 된것을 SCM에 통지한다.-----------------
	SetServiceStatus(g_ssh, &status);

	OutputDebugString("BeepService 서비스가 시작되었습니다.\n");



	// 3. 서비스 본연의 임무 수행 ----------------------------------- 
	//서비스 중, 2초마다 비프음
	while(1)
	{
		Beep(440, 200); // 440Hz 0.2초

		// 제어 통지를 최대 2초간 대기
		if( WaitForSingleObject(g_event, 2000) == WAIT_TIMEOUT)
			continue;

		// 일시 정지 통지가 있으면재개/정지 대기
		while( g_state == SERVICE_PAUSED)
		{
			SetEvent(g_event2);	// 제어 완료 통지
			WaitForSingleObject(g_event, INFINITE);
		}

		if( g_state == SERVICE_RUNNING)
			SetEvent(g_event2);	// 제어 완료 통지 
		else if( g_state == SERVICE_STOPPED)
			break;
	}

	OutputDebugString(
	"BeepService 서비스가 중지되었습니다.");

	SetEvent(g_event2);		// 제어 완료 통지 
}

****************************************************************/

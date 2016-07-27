/*************************************************************************************

  SERVICE 프로그램

  - 크게 3부분으로 구성
     1) MAIN FUNCTION
	 2) SERVICE MAIN
	 3) HANDLER FUNCTION

**************************************************************************************/


#include <windows.h>

#define SERVICE_CONTROL_NEWFILE 128

void MyServiceMain(DWORD argc, LPTSTR *argv);
void MyServiceHandler(DWORD opCode);

SERVICE_STATUS_HANDLE g_hSrv;
DWORD g_NowState;
BOOL g_bPause;
HANDLE g_ExitEvent;
char gbuf[65536]="메모리 통계 파일\r\n";

/*************************************************************************************

  0) 디스패처 

	 모든 서비스 프로그램의 공통된 초기화 과정 

      1) 서비스와 SCM과 통신을 하기 위한 장치 마련
	     - 실행중인 서비스들은 SCM으로 부터 끊임없는 제어 신호를 받게 되고 
		   자신의 상태를 SCM에게 보고해야 함

         - 디스패처는 SCM과 ,서비스 프로세스와의 통신을 담당하는 스레드임 

     명령 시나리오 
	 1) 새로운 서비스 시작하라
	 2) 디스패처는 새 서비스를 위한 스레드를 생성
	 3) 서비스 메인함수를 그 스레드에서 실행함
	 
	 1) 서비스를 잠시 중지하거나 재개하라
	 2) 서비스의 핸들러에게 이 신호를 전달함
	     서비스가 scm으로 부터 또는 사용자로부터의 제어 신호를 처리하도록 함

      *) 디스패처는 서비스가 실행중인 동안 무한 루프를 돌며 서비스 시작과 제어
	     신호 전달을 수행하게 됨

     위와 같은 일을 수행하기 위해 디스패처는 이 프로세스가 어떠한 서비스들을 
	 가지고 있는지에 대한 정보를 필요로 함
	 - main함수에서 디스패처를 실행할때 인자로 전달됨 
	   ex)main함수 확인 
     
**************************************************************************************/


/*************************************************************************************

  1) MAIN FUNCTION

	 디스패처 스레드를 실행 
	  - 서비스를 시작하고 핸들러에게 제어 코드를 전달하는 일을 하는 
	    독립된 스레드 
**************************************************************************************/
	/* 동작 시나리오 
	   1) SCM은 해당 서비스를 가지고 있는 프로세스의 경로를 레지스트리에서 찾게됨
	   2) 이 프로세스가 실행중이 아니라면 프로세스를 실행함
	      이미 실행중이면, 디스패처가 생성되어 있으므로 실행과 생성과정 생략함
		  
	   3) 서비스 프로세스의 main함수에서 디스패처 스레드를 생성
	      전달된 정보를 바탕으로 디스패처는 프로세스에 속한 서비스의 정보를 바탕으로
		  SCM과 통신을 시작함

       4) 해당 서비스를 시작하기 위해 디스패처는 그 서비스를 위한 별도의 스레드를 
	      생성하고 서비스 메인 함수를 호출하여 서비스를 시작함 
          - 서비스 하나당 스레드 하나
		  - 서비스 메인에서는 서비스를 위한 초기화와 핸들러 등록 

       5) 디스패처에 의해 서비스가 시작되면 서비스는 실행중 자신의 상태를 
	      수시로 SCM에게 보고해야 함
		  SCM으로 부터 제어 신호가 들어오면 디스패처가 이를 서비스의 핸들러에게 전달
		  하며 핸들러가 이 제어 신호를 처리함 

       6) 서비스가 종료되면 디스패처는 실행중인 서비스 카운드를 1감소 
	      만약 0이라면 디스패처 종료 및 main함수 리턴
	*/

int main()
{

	SERVICE_TABLE_ENTRY ste[]={
		{"MemStat",	 (LPSERVICE_MAIN_FUNCTION)MyServiceMain},	// 서비스 명 , 서비스 메인 함수 
		{NULL,NULL}
	};

	StartServiceCtrlDispatcher(ste);
	return 0;
}

// 서비스의 현재 상태를 변경하는 함수
void MySetStatus(DWORD dwState, DWORD dwAccept=SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE)
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

/*************************************************************************************

  2) SERVICE MAIN

	 실제 서비스 작업을 하는 본체 
	 - main함수에서 등록되어 서비스를 실행시킬 때 디스패처에 의해 호출되며
	   서비스 운영에 관한 여러 가지 일을 수행함 
	   - 핸들러 등록
	   - 서비스 기동
	   - 자신의 변화를 SCM에게 아림 

     - 원형은 정해져 있다. 
**************************************************************************************/

void MyServiceMain(DWORD argc, LPTSTR *argv)
{
	HANDLE hFile;
	MEMORYSTATUS ms;
	DWORD dwWritten;
	char str[256];
	SYSTEMTIME st;

	// 서비스 핸들러를 등록한다.
	g_hSrv=RegisterServiceCtrlHandler("MemStat",(LPHANDLER_FUNCTION)MyServiceHandler);
	if (g_hSrv==0) {
		return;
	}

	// 서비스가 시작중임을 알린다.
	MySetStatus(SERVICE_START_PENDING);
	
	// 전역 변수를 초기화한다.
	g_bPause=FALSE;

	// 이벤트를 생성한다.
	g_ExitEvent=CreateEvent(NULL, TRUE, FALSE, "MemStatExit");

	// 새로운 로그 파일을 작성한다.
	hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle(hFile);
	
	// 서비스가 시작되었음을 알린다.
	MySetStatus(SERVICE_RUNNING);

	// 10초에 한번씩 메모리 통계를 작성한다.
	for (;;) 
	{
		if (g_bPause == FALSE) 
		{
			// 현재 시간과 메모리 양을 조사하여 문자열로 조립한다.
			GetLocalTime(&st);
			GlobalMemoryStatus(&ms);
			wsprintf(str, "%d월 %d일 %02d시 %02d분 %02d초 => "
				"사용가능 물리 메모리=%dMbytes(%d%%), 사용가능 가상메모리=%dMbytes,"
				" 사용가능 페이징 파일=%dMbytes\r\n",
				st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
				ms.dwAvailPhys/1048576,100-ms.dwMemoryLoad,ms.dwAvailVirtual/1048576,
				ms.dwAvailPageFile/1048576);

			// 파일로 통계를 출력한다. 버퍼가 가득찬 경우 파일을 다시 만든다.
			if (strlen(gbuf) > 60000) 
			{
				hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
					CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				strcpy(gbuf,"메모리 통계 파일\r\n");
			} 
			else 
			{
				hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
					OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			}
			strcat(gbuf,str);
			WriteFile(hFile,gbuf,strlen(gbuf),&dwWritten,NULL);
			CloseHandle(hFile);
		}
		if (WaitForSingleObject(g_ExitEvent, 10000) == WAIT_OBJECT_0)
			break;
	}
	MySetStatus(SERVICE_STOPPED);
}


/*************************************************************************************

  3) HANDLER FUNCTION : 핸들러 함수 

	 서비스의 제어 신호를 처리하는 함수 
	 - 서비스의 메시지 루프의 역할 수행 
	 - 서비스 메인에서 디스패처에 등록됨
	 - 서비스 제어 프로그램이나 서비스 애플릿을 통해 명령을 받아 처리하는 부분임 
	   SCM이 이 명령을 받아 디스패처로 전달하며 디스패처는 등록된 핸들러에게 제어 신호를
	   보내게 됨 

     - 원형은 정해져 있슴 
**************************************************************************************/

void MyServiceHandler(DWORD fdwControl)
{
	HANDLE hFile;

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
	// 사용자 정의 신호 
	case SERVICE_CONTROL_NEWFILE:
		hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
			CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		strcpy(gbuf,"메모리 통계 파일\r\n");
		CloseHandle(hFile);
	// 서비스의 현재 상태를 알려달라 
	case SERVICE_CONTROL_INTERROGATE:
	default:
		MySetStatus(g_NowState);
		break;
	}
}

/****************************************************************

  [ 프로그램에서 서비스를 제어 ]

  - 서비스의 시작/정지 같은 실행제어는 일반적으로 서비스 애플릿
    에서 수행하지만, 독자적인 프로그램을 사용 제어 할 수도 있다.

  - StartService / ControlService API를 제공 

    모두 첫번째 인자에는 서비스 핸들이 들어가므로 미리 OpenService
	API를 사용해서 핸들을 얻어 두어야 한다.


// 서비스 실행을 제어하는 API

BOOL StartService(			// 반환값 : 처리 결과 
  SC_HANDLE	hService,		// 서비스 핸들
  DWORD dwServiceArgc,		// 전달인자 개수
  LPCTSTR* lpServiceArgv	// 전달인자 문자열 배열 
  );

  ==>  서비스 시작 
        2,3인자 : 서비스의 '시작 전달인자'를 지정
		이 전달인자를 ServiceMain함수의 인자로 전달한다.


BOOL ControlService(		// 반환값 : 처리 결과 
  SC_HANDLE	hService,		// 서비스 핸들
  DWORD dwControl,			// 제어 코드
  LPSERVICE_STATUS lp...	// 서비스 상태를 받을 구조체 
  );

  ==> 서비스의 중지, 일시중지, 다시 시작과 같은 실행 제어 
      2인자 : 제어 코드 
	  3인자 : 서비스 상태를 받기 위한 구조체 포인터 지정 


BOOL ChangeServiceConfig()...
  ==> 서비스 설정을 변경하거나 설정 내용을 참조 

****************************************************************/

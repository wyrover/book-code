/****************************************************************

  [ main 함수에서 서비스 진입점을 등록한다. ]

  ex1) 정기적으로 Beep음을 울리는 BeepService 구현

  - 서비스 프로그램은 백그라운드에서 동작함
    따라서 기본적으로 콘솔 프로그램임

  - 서비스로 기능하기 위해서는 SCM의 제어에 응답할 수 있는
    구조가 구현되어 있어야 함 

  - SCM에 의한 호출 시나리오

    1) 서비스 애플릿의 '시작' 메뉴 선택해서 서비스 시작 지시

    2) SCM은 등록 정보를 기반으로 서비스 프로그램의 프로세스 생성

    3) main함수를 호출 

    4) main함수는 최대한 신속하게 StartServiceCtrlDispatcher API
	   함수를 호출해야 함 
	 - 이 함수는 서비스가 정지할때까지 제어권을 돌려주지 않는다.

	 #include <windows.h>

	 int main()
	 {
		SERVICE_TABLE_ENTRY services[] = 
		{{"BeepService", BeepServiceMain }, {NULL, NULL}};

		if( !StartServiceCtrlDispatcher(services))
		{
		 MessageBox(NULL, "BeepService 서비스에서 에러 발생", 
		 "BeepService에러", MB_OK |MB_ICONERROR | MB_SERVICE_NOTIFICATION);
		}
		return 0;
	 }

	 ==> StartServiceCtrlDispatcher는 프로그램의 실행을 SCM내의 
	     코드로 옮겨서 서비스를 제어할 수 있도록 한다.

		 이때 프로그램 내부에 있는 서비스 진입점을 
		 SERVICE_TABLE_ENTRY 구조체에 설정해서 인자로 넘긴다.
		 이 구조체의 맴버에는 서비스 명과 진입점이 되는 함수 
		 포인터가 있다.
		 - 여기서 지정하는 함수는 ServiceMain이라 함

		 즉, StartServiceCtrlDispatcher API를 호출하면
		 SCM은 새롭게 스레드를 생성해서 ServiceMain함수를 호출한다.
		 이제부터가 서비스 본래의 처리를 시작하는 부분이다.


	 ==> SERVICE_TABLE_ENTRY 는 배열이다.
	     실행 파일 하나에서 여러 개의 서비스를 제공할 수 있도록하기
		 위함임

		 여러개의 서비스를 준비했어도 실제로 ServiceMain이 호출되는 
		 것은 서비스 애플릿에서 지정한 서비스뿐임

****************************************************************/

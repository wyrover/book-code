/****************************************************************

  [ 서비스 설치 /제거 ]

  생성한 서비스를 SCM을 사용해서 외부 프로그램에서 조작하는 방법

   1) 서비스의 설치와 제거

   2) 서비스의 시작/정지를 다루는 실행 제어

   3) 서비스의 설정 변경

// 서비스를 설치(인스톨)하는 처리 
void install(void)
{
	// 1) SCM 핸들 얻기 
	// 1인자 : 컴퓨터명 (네트웤에 연결된 각각의 컴퓨터를 식별하기 위한 이름)
	//         NULL 로컬로 지정 
	//		   즉, 네트웤의 다른 컴퓨터의 SCM에 접근해서 서비스 조작도
	//		   가능하다. 
	// 2인자 : SCM의 관리 데이터베이스의 이름 : 보통 NULL 사용 
	// 3인자 : 접근 권한 
	SC_HANDLER scm = OpenSCManager(NULL, NULL, 
			SC_MANAGER_ALL_ACCESS);

	if( scm == NULL )
	{
		fprintf(stderr, "SCM을 열 수 없습니다.");
		return;
	}

    //2) 서비스 설치 
	SC_HANDLER scv = CreateService(
		scm,				// SCM핸들
		BEEPSERVICE_NAME,	// 서비스 명
		"BeepService",		// 서비스 출력명
		SERVICE_ALL_ACCESS,	// 필요한 접근 권한
		SERVICE_WIN32_OWN_PROCESS,		// 서비스 종류
		SERVICE_AUTO_START,				// 시작 방법
		SERVICE_ERROR_NORMAL,			// 에러 발생시 대처 방법

		"c:\\bin\\beepService.exe",		// 서비스 경로명

		NULL,		// 로드 순서 그룹 지정
		NULL,		// 로드 순서 그룹 내의 태그를 받는다.
		NULL,		// 의존하는 서비스명 리스트
		NULL,		// 계정 ID(LocalSystem을 지정한 것임)
		NULL,		// 암호

		);

	if( scv != NULL )
	{
		CloseHandle(scv);
		printf("서비스가 설치되었습니다.\n");
	}
	else
	{
		fprintf(stderr,"서비스가 설치되지 않았습니다\n");
	}

	CloseServiceHandle(scm);
}


// 서비스를 삭제(언인스톨)하는 처리 
void uninstall(void)
{
	SC_HANDLER scm = OpenSCManager(NULL, NULL, 
			SC_MANAGER_ALL_ACCESS);

	if( scm == NULL )
	{
		fprintf(stderr, "SCM을 열 수 없습니다.");
		return;
	}
	SC_HANDLER scv = OpenService(
		scm,				// SCM핸들
		BEEPSERVICE_NAME,	// 서비스 명
		DELETE
		);

	if( scv != NULL )
	{
		if( !DeleteService(scv)
		{
			printf("서비스를 삭제할 수 없습니다.\n");
		}
		else
		{
			CloseServiceHandle(scv);	
			printf(서비스가 삭제되었습니다.\n");
		}
	}
	else
	{
		fprintf(stderr, 
				"서비스를 열 수 없습니다\n");
	}

	CloseServiceHandle(scm);	
}


****************************************************************/

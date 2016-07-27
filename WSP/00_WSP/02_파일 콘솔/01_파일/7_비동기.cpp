/****************************************************************************

  2) 이벤트 객체를 사용해서 처리 완료를 통지한다.

     typedef struct _OVERLAPPED{
		ULONG_PTR	Internal;		// [출력] 상태
		ULONG_PTR	InternalHight;	// [출력] 전송된 바이트 수
		DWORD		Offset;			// [입력] 전송 시작위치의 하위 32bit
		DWORD		OffsetHight;	// [입력] 전송 시작위치의 상위 32bit
		HANDLE		hEvent;			// [입력] 이벤트 핸들러 
	 }OVERLAPPED, *LPOVERLAPPED;

     ==> 입력은 ReadFile등에서 입력값으로 이용하며, 
	     출력은 API가 필요에 의해서 갱신한다.

         Offset과 OffsetHight는 데이터의 읽기와 쓰기를 시작할 위치
		 (파일 선두로부터의 바이트 수)를 하위 32bit, 상위 32bit로 나누어 
		 지정한다.

         - 동기 I/O 에 의한 파일 읽기 쓰기
	       Windows가 파일 핸들마다 현재 위치(파일 포인터)를 관리함
	       ReadFile / WriteFile은 항상 이 파일 포인터 위치에서 읽기 쓰기를
	       하게 되므로 위치를 지정하는 인자는 필요 없슴
	       읽기 쓰기를 완료하면 자동으로 그 데이터의 크기만큼 파일포인터를
	       뒤로 이동시킴
	       따라서 연속해서 읽기 쓰기를 하려면 ReadFile/WriteFile을 호출하면됨

         - 비동기 I/O에 의한 파일 읽기 쓰기
	      처리 완료 전에 API호출로부터 복귀하므로 같은 파일에 대해서 비동기
	      I/O를 여러 개 병행해서 실행할 수 있슴

	      이와 같은 I/O는 API를 호출한 순서에 따라 처리된다고 할 수 없슴 
	      파일포인터가 가리키는 위치는 실제로 처리가 시작될때까지 모름 
	      따라서 비동기 I/O요청 하나하나에 대해 조작 시작 위치를 명시적으로
	      지정할 수 있게 되어 있다.

         hEvnet에는 이벤트 객체 핸들을 지정한다.
		 여기에 유효한 핸들을 지정해 주면 비동기 I/O 처리가 완료되었을 때
		 윈도우가 이벤트를 시그널 상태로 설정해 준다. 
		  
		 따라서 WaitForSingleObject API와 같은 대기용 API에 이 핸들을 지정해서
		 호출하면 처리 완료를 감지하거나 완료될 때까지 스레드 실행을 정지
		 시킬 수 있다. 

     ==> ReadFile/WriteFile API에서 지정한 OVERLAPPED 구조체는 비동기 I/O가
	     완료될때까지 유효한 데이터로 존재해야 한다.

         Windows에서는 비동기 I/O 처리를 진행하면서 OVERLAPPED 구조체 맴버를
		 참조/갱신한다. 또한 완료 시에는 hEvent 맴버로 지정된 이벤트를 
		 시그널 상태로 바꿀 필요가 있다.
		 그런데 Windows에서는 비동기 I/O를 시작할 때 넘겨받은 OVERLAPPED 구조체
		 를 복사해서 가지고 있는 것이 아니라 그 포인터만 보존한다. 

		 그렇기 때문에 포인터로 참조된 곳이 무효화 되면 문제가 발생하게 된다.


		 따라서 OVERLAPPED 구조체를 지역 변수로 정의하면 문제 발생 가능성이 
		 있다.

		 BOOL foo(HANDLE hFile, char *pBuf, int bufsize, HANDLE hEvent)
		 {
			OVERLAPPED ol;		// 지역변수로 정의
			memset(&ol, 0, sizeof(ol));
			ol.Offset = 0;
			ol.OffsetHigh = 0;
			ol.hEvnet = hEvent;

			DWORD rdCnt;		// 더미 데이터

			return ReadFile(hFile, pBuf, bufsize, &rdCnt, &ol);
		 }
****************************************************************************/





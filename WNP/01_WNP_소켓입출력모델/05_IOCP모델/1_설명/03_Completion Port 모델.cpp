/*********************************************************
  [ Completion Port ]
**********************************************************/

/*********************************************************

  입출력 모델 중 가장 뛰어난 성능을 제공한다.


	1. 동작원리

		Completion Port 모델의 핵심은 입출력 완료 포트
		(I/O Completion port)라 부르는, 윈도우 운영체제가
		제공하는 구조를 이해하고 활용하는 것이다.

		입출력 완료 포트는 비동기 입출력 결과와 이 결과를 
		처리할 스레드에 대한 정보를 담고 있는 구조이다.

		Overlapped 모델(2)에서 소개한 APC 큐와 비슷한 개념임

		APC 큐와의 차이점

		- 생성과 파괴
		  APC큐 : 각 스레드마다 자동 생성 자동 파괴됨
		  I/O P : CreateIoCompletionPort() 함수를 호출하여
				  생성하고, CloseHandle()함수를 호출하여 
				  파괴한다.

		- 접근 제약
		  APC큐 : 저장된 결과는 APC큐를 소유한 스레드만
		          확인 가능
		  I/O P : 위와 같은 제약이 없다.
				  대개 입출력 완료 포트를 접근하는 스레드를
				  별도로 두는데, 이를 작업자 스레드(Worker 
				  thread)라 한다.
				  이상적인 작업자 스레드 개수는 CPU 개수와
				  같게 하는 것이지만,
				  몇 가지 이유에 인해 CPU개수  *n개를 생성
				  한다.

		- 비동기 입출력 처리 방법
		  APC 큐: 저장된 결과를 처리하려면 해당 스레드는 
		          alertable Wait상태에 진입해야 한다.
		  I/O P : 입출력완료포트에 저장된 결과를 처리하려면 
				  작업자 스레드는 GetQueuedCompletionStatus()
				  함수를 호출해야 한다.


	2. 각 단계별 동작 원리 요약 

  	 (1) 비동기 입출력 시작
		 애플리케이션을 구성하는 임의의 스레드에서 비동기 
		 입출력 함수를 호출함으로써 운영체제에 입출력 작업을
		 요청한다.

	 (2) 모든 작업자 스레드는 GetQueuedCompletionStatus()
	     함수를 호출하여 입출력 완료 포트를 감시한다.
		 완료한 비동기 입출력 작업이 아직 없다면, 모든 작업
		 자 스레드는 대기 상태가 된다.
		 이때 대기 중인 작업자 스레드 목록은 입출력 완료 포트
		 내부에 저장된다.

     (3) 비동기 입출력 작업이 완료되면, 운영체제는 입출력완료
	     포트에 결과를 저장한다. 이때 저장되는 정보를 입출력
		 완료 패킷(I/O completion packet)이라 부른다.

	 (4) 운영체제는 입출력 완료 포트에 저장된 작업자 스레드 목
	     록에서 하나를 선택하여 깨운다. 대기 상태에서 깨어난 
		 작업자 스레드는 비동기 입출력 결과를 처리한다.
		 이후 작업자 스레드는 필요에 따라 다시 비동기 입출력 함수를
		 호출할 수 있다.


    3. 소켓 입출력 절차 

	   (1) CreateIoCompletionPort() 함수를 호출하여 입출력 
	       완료 포트를 생성한다.

       (2) CPU개수에 비례하여 작업자 스레드를 생성한다.
	       모든 작업자 스레드는 GetQueuedCompletionStatus()
		   함수를 호출하여 대기 상태가 된다.

	   (3) 비동기 입출력을 지원하는 소켓을 생성한다
		   이 소켓에 대한 비동기 입출력 결과가 입출력 완료 
		   포트에 저장되려면, CreateIoCompletionPort()함수를
		   호출하여 소켓과 입출력 완료 포트를 연결해야 한다.

	  (4) 비동기 입출력 함수를 호출한다.
		  비동기 입출력 작업이 곧바로 완료되지 않으면, 소켓 
		  함수는 오류를 리턴하고 오류 코드는 WSA_IO_PENDING으로
		  설정된다.

	  (5) 비동기 입출력 작업이 완료되면, 운영체제는 입출력 
	      완료 포트에 결과를 저장하고, 대기 중인 스레드 하나
		  를 깨운다. 대기 상태에서 깨어난 작업자 스레드는 비동기
		  입출력 결과를 처리한다.

	  (6) 새로운 소켓을 생성하면 3 ~ 5를 그렇지 않으면 
		  4 ~ 5를 반복한다.


	4. 입출력 완료 포트 생성 

		 CreateIoCompletionPort() 는 두가지 역할을 한다.
		 - 입출력 완료 포트를 새로 생성
		 - 소켓과 입출력 완료 포트를 연결 
		   (소켓과 비출력 완료 포트를 연결하면, 이 소켓에 '
		    대한 비동기 입출력 결과가 입출력 완료 포트에 저장
			 된다.)

		HANDLE CreateIoCompletionPort
		{
			HANDLE		Filehandle,
			HANDLE		ExistingCopletionPort,
			ULONG		CompletonKey,
			DWORD		NumberOfConcurrentThreads
		};

		- FileHandle	
		  입출력 완료 포트와 연결할 파일 핸들
		  새로운 입출력 완료 포트를 생성할 때는 INVALID_HANDLE_
		  VALUE 값을 사용해도 됨

		- ExistingCopletionPort
		  파일 또는 소켓과 연결할 입출력 완료 포트 핸들
		  이 값이 NUL이면 새로운 입출력 완료포트를 생성한다.

		- CompletonKey
		  입출력 오나료 킷에 들어갈 부가적인 정보로 32비트 
		  값을 줄 수 있다.
		  인출력 완료 패킷은 비동기 입출력 작업이 완료될 때
		  마다 생성되어 입출력 완료 포트에 저장되는 정보이다.

		- NumberOfConcurrentThreads
		  동시에 실행할 수 있는 작업자 스레드의 개수 
		  0을 사용하면 자동으로 CPU개수와 같은 수로 설정된다.
		  운영체제는 실행 중인 작업자 스래드 개수가 여기서
		  설정한 값을 넘지 않도록 해준다.

         ex)
		   1) 입출력 완료 코드 생성 

			HANDLE hcp = CreateIoCompetionPort(INVALID_HANDLE_
					VLAUE, NULL, 0, 0);
			if( hcp == NULL) 
				return -1;

		   2) 기존의 소켓과 입출력 완료 포트를 연결하는 코드 
		   SOCKET sock;
		   HANDLE hResult = CreateIoCompetionPort
				(HANDLE)sock, hcp, (DWORD)sock, 0);

		   if( hResult == NULL)
				return -1;

     5. 비동기 입출력 결과 확인
	    작업자 스레드는 GetQueuedCompletionStatus()
		함수를 호출함으로써 입출력 완료 포트에 입출력 완료 
		패킷이 들어올 때까지 대기한다.
		입출력 완료 패킷이 입출력 완료 포트에 들어오면, 운영
		체제는 실행 중인 작업자 스레드의 개수를 채크한다.
		이값이 CreateIoCompletionPort()함수의 네번째 인자로
		설정한 값보다 작다면, 대기 상태인 작업자 스레드를
		깨워서 입출력 완료 패킷을 처리하도록 한다.

		BOOL GetQueuedCompletionStatus
		{
			HANDLE			CompletionPort,
			LPDWORD			IpNumberOfBytes,
			LPDWORD			lpCompletionKey,
			LPOWERLAPPED	*lpOverlapped,
			DWORD			dwMilliseconds
		};

		- CompletionPort
			입출력 완료 포트 핸들이다.

		- lpNumberOfBytes
			DWORD 타입 변수 주소값을 넣으면, 비동기 입출력 
			작업으로 전송된 바이트 수가 여기에 저장된다.

		- lpCompletionKey
			DWORD 타입 변수 주소값을 넣으면, CreateIoCom-
			pletionPort() 함수 호출시 전달한 세번째 인자
			(32비트)가 여기에 저장된다.

		- lpOverlapped
			OVERLAPPED 타입 포인터의 주소값을 넣으면 비동기 
			입출력 함수 호출 시 전달한 OVERLAPPED 구조체의
			주소값이 여기에 저장된다.

		- dwMilliseconds
			작업자 스레드가 대기할 시간을 밀리초단위로 
			지정한다.
			INFINITE 값을 사용하면, 입출력 완료 패킷이 
			생성되어 운영체제가 자신을 깨울 때까지 
			무한히 대기한다.
	    
	  
**********************************************************/

/*********************************************************
  [ Overlapped 모델1 ]
**********************************************************/



/*********************************************************

   지금까지 배운 다른 소켓 입출력 모델과는 근본적으로 다른
   입출력 방식 

   고성능을 제공

   동작방식에 따라 2종류로 나뉨.


  1. 동작 원리

	 Overlapped는 윈도우 운영체제에서 고성능 파일 입출력을 위해 
	 제공되는 방식 이를 소켓 입출력에서도 사용할 수 있게 만듬


	 (1) 동기 입출력 방식

		 애플리케이션코드 |==|	  |==|	  |==|	  |==| ==>

		
		  운영체제 코드		 |====|  |====|	 |====| =====>

          - 애플리케이션이 입출력 함수 호출 입출력 작업이 끝
		    날때까지 대기
			작업이 끝나면 입출력함수는 리턴하고, 애플리케이션
			은 입출력 결과를 처리하거나 다른 작업 수행 
			
		  - 소켓 입출력 모델(Select, WSAAsyncSelect, WSAEvnet-
							 Select) 들이 여기에 해당

		    입출력 함수를 안전하게 호출할 수 있는 시점을
		    운영체제가 알려주기 때문에 단순한 동기 입출력 방식
			보다 편하게 여러 소켓 처리 가능

          - 이와 같이 운영체제가 함수 호출 시점을 알려주는 
		    개념을 비동기 통지(asynchronous notification)이라 함(I/O와는 상관이 없음)

            ex) 소켓 입출력 모델 :  동기 입출력과 비동기 통지를
				결합한 형태라 할 수 있다.


	 (2) 비동기 입출력 방식 

		 애플리케이션코드 |==========|=====|===|==| =====>

		
		  운영체제 코드		 |=======|	 
								|==========|
									|=========|

         - 비동기 입출력(asynchronous I/O) or 중첩 입출력(overlapped I/o) ==> 장점
		   라 함

		 - 애플리케이션은 입출력 함수를 호출한 후 입출력 잡업
		   의 완료 여부와 무관하게 다른 작업 수행 

         - 입출력 작업이 끝나면 운영체제는 작업 완료를 애플리케이션에
		    게 알려준다.

		 - 즉, 여기서 볼 모델들은 비동기입출력, 비동기 통지(명령 받은 작업이 끝나는 시점)를
		   결합한 형태이다.


  2. Overlapped 모델을 사용하는 소켓 애플리케이션의 일반적인
     공통 단계

    (1) 비동기 입출력을 지원하는 소켓을 생성 
	    socket() 함수로 생성한 소켓은 기본적으로 비동기 입출력
		을 지원

    (2) 비동기 입출력을 지원하는 소켓 함수를 호출
	    - 13개의 함수 

          AcceptEx(),  ConnectEx(), DisconnectEx()
		  TransmitFile(), TransmitPackets(), WSAIoctl()
		  WSANSPIoctl(), WSAProviderConfigChange(), 

		  WSARecv(), WSARecvFrom(), WSARecvMsg(),
		  WSASend(), WSASendTo()

     (3) 운영체제는 소켓 입출력 작업 완료를 애플리케이션에 
	     알려주고(비동기 통지) 애플리케이션은 입출력 결과를
		 처리한다.

	 * Overlapped 모델은 운영체제의 비동기 통지 방식에 따라 
	   두 종류로 구분됨
	   어떤 것을 선택하는가에 따라 애플리케이션의 구조가 
	   달라진다.

		- 모델 1 
			소켓 입출력 작업이 완료되면, 운영체제는 애플리
			케이션이 등록한 이벤트 객체를 신호상태로 바꾼다.
			애플리케이션은 이벤트 객체를 관찰함으로써 
			작업 완료 사실을 감지할 수 있다. 

		- 모델 2
			소켓 입출력 작업이 완료되면,  운영체제는 애플리
			케이션이 등록한 함수를 자동으로 호출한다.
			일반적으로 운영체제가 호출하는 애플리케이션 함수를
			콜백함수라 한다. 
			특별히 overlapped 모델에서는 완료루틴(completion routine) ==> 함수 포인터 개념
			이라 칭한다.

	* 소켓 애플리케이션에서 Overlapped 모델을 사용하는 주된 이
	  유는 데이터를 보내고 받는 작업의 효율성 


  3. 주요 함수 설명 

	 WSASend()	&  WSARecv()

	1) int WSASend() {
		SOCKET			s;
		LPWSABUF		lpBuffers,
		DWORD			dwBufferCount,
		LPDWORD			lpNumberOfBytesSend,
		DWORD			dwFlags,
		LPWSAOVERLAPPED	lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE	lpCompletionRoutine
	  };


	2) int WSARecv() {
		SOCKET			s;
		LPWSABUF		lpBuffers,
		DWORD			dwBufferCount,
		LPDWORD			lpNumberOfBytesRecvd,
		DWORD			lpFlags,
		LPWSAOVERLAPPED	lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE	lpCompletionRoutine
	  };


	  - s
		비동기 입출력을 할 소켓

	  - lpBuffers
		WSABUF 구조체 배열의 시작 주소 

			typedef struct __WSABUF {
				u_long		len;			// 길이(byte 단위)
				char  FAR	*buf;			// 버퍼 시작 주소 
			}WSABUF, *LPWSABUF;

	  - dwBufferCount
		WSABUF 구조체 배열의 원소의 개수

	  - lpNumberOfBytesSend &lpNumberOfBytesRecvd 
		DWOR 형 변수 주소값으로, 함수 호출이 성공하면 이 변수에
		보내거나 받은 바이트 수가 저장된다.

	  - dwFlags & lpFlags
		각각 send() , recv() 함수의 마지막 인자와 동일한 역할

	  - lpOverlapped
		WSAOVERLPAPPD 구조체 변수 주소값
		비동기 입출력을 위한 정보를 운영체제에게 전달하거나,
		운영체제가 비동기 입출력 결과를 애플리케이션에 전달할
		때 사용함 

		앞의 4개의 인자는 운영체제가 내부적으로 사용
		5번째 변수  hEvent  : 이벤트 객체 핸들값 
							  Overlapped 1모델에서만 사용됨
		작업이 완료되면 hEvent가 가리키는 이벤트 객체는 신호
		상태가 된다.

		typedef struct _WSAOVERLPAPPED	{
			DWORD		Internal;
			DWORD		InternalHigh;
			DWORD		offset;
			DWORD		offsetHigh;
			WSAEVENT	hEvent;
		}WSAOVERLAPPED, *LPWSAOVERLAPPED;


	  - lpCompletionRoutine	
		입출력 작업이 완료되면 운영체제가 자동으로 호출할 
		완료 루틴(콜백함수)의 주소값 


	*) wsaSend() & wsaRecv() 함수의 특징 

	   - Scatter	/ Gather 입출력 지원
		 송신측에서 WSABUF 구조체를 사용하면,여러 개의 버퍼에
		 저장된 데이터를 모아서(gather) 보낼 수 있다.

		 ex) 송신 코드 
			buf1[128];
			buf2[256];
			WSABUF	wsabuf[2];
			wsabuf[0].buf		= buf1;
			wsabuf[0].len		= 128;
			wsabuf[1].buf		= buf2;
			wsabuf[1].len		= 256;
			WSASend(sock, wsabuf, 2, ...);


		 수신측에서도 WSABUF 구조체를 사용하면, 받은 데이터를
		 여러 개의 버퍼에 흩뜨려(scatter) 저장할 수 있다.

		 ex) 수신측 코드 
			buf1[128];
			buf2[256];
			WSABUF	wsabuf[2];
			wsabuf[0].buf		= buf1;
			wsabuf[0].len		= 128;
			wsabuf[1].buf		= buf2;
			wsabuf[1].len		= 256;
			WSARecv(sock, wsabuf, 2, ...);

		- 마지막 두 인자에 모두 NULL 을 사용하면 동기 함수로
		  동작한다.

		- Overlapped 모델(1)에서는 구조체의 hEvent 변수를,
		  Overlapped 모델(2)에서는 lpCompletionRoutine 인자를
		  사용한다.
		  단, 
		  lpCompletionRoutine 인자의 우선 순위가 높으므로 이 
		  값이 NULL 이 아니면 WSAOVERLAPPED 구조체의 hEvent변
		  수는 사용되지 않는다.


  4. Overlapped 모델(1)의 소켓 입출력 기본 절차 

	(1) 비동기 입출력을 지원하는 소켓을 생성한다.
		이때 WSACreateEvent() 함수를 호출하여 대응하는 이벤트
		객체도 같이 생성한다.

	(2) 비동기 입출력을 지원하는 소켓 함수를 호출한다.
		이때 WSAOVERLAPPED 구조체의 hEvent 변수에 이벤트 객
		체 핸들값을 넣어서 전달한다.
		비동기 입출력 작업이 곧바로 완료지 않으면, 소켓 함수
		는 오류를 리턴하고 오류 코드는 WSA_IO_PENDING으로 설정
		된다.
		나중에 비동기 입출력 작업이 완료되면, 운영체제는 이벤
		트 객체를 신호 상태로 만들어 이 사실을 애플리케이션
		에 알린다.

	(3) WSAWaitForMultipleEvents() 함수를 호출하여 이벤트 객
	    체가 신호 상태가 되기를 기다린다.

	(4) 비동기 입출력 작업이 완료하여 WSAWaitForMultipleEvents()
		함수가 리턴하면, 
		WSAGetOverlappedResult() 함수를 호출하여 비동기 입출력
		결과를 확인하고 데이터를 처리한다.

	(5) 새로운 소켓을 생성하면 1 ~ 4 ,
	    그렇지 않으면 2 ~ 4 를 반복한다.

	
	WSAGetOverlappedResult
	{
		SOCKET				s,
		LPWSAOVERLAPPED		lpOverlapped,
		LPDWORD				lpcbTransfer,
		BOOL				fWait,	
		LPDWORD				lpdwFlags
	};

	- s
	  비동기 입출력 함수 호출에 사용한 소켓을 다시 넣는다.
	  
	- lpOverlapped
	  비동기 입출력 함수 호출에 사용한 WSAOVERLAPPED 구조체
	  변수의 주소값을 다시 넣는다

	- lpcbTransfer
	  DWORD 형 변수 주소값을 전달하면 이 변수가 가리키는 영역
	  에 전송 바이트 수가 저장된다.

	- fWait
	  비동기 입출력 작업이 끝날때까지 대기하려면 TRUE, 
	  그렇지 않으면 FALSE를 사용한다.
	  WSAWaitForMultipleEvents() 함수를 이전에 호출했다면 비동기
	  입출력 작업 완료 사실을 이미 알고 있으므로 FALSE를 사용
	  한다.

	- lpdwFlags
	  DWORD 형 변수 주소값을 전달하면, 비동기 입출력 작업과 관련
	  된 부가적인 정보가 저장된다.
	  이 값은 거의 사용하지 않으므로 무시해도 좋다.


  5. Sample

	서버 동작 : 클라이언트가 보낸 데이터를 받아(WSARecv)
	            이를 문자열로 간주하여 무조건 화면에 출력
				그리고, 
				받은 문자열을 데이터 변경 없이 다시 클라이
				언트에 보낸다.(WSASend)

   클라이언트 동작 :
                사용자가 키보드로 입력한 문자열을 서버에
				보낸다.(WSASend)
				서버가 받은 데이터를 그대로 되돌려 보내면
				클라이언트는 이를 받아(WSARecv) 화면에 
				출력한다.					
		
**********************************************************/


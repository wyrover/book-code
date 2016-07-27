
// WSAAEventSelect 모델 

 /* 
	WSAAEventSelect() 함수가 핵심적인 역할을 한다는 뜻에서 붙인 이름임

	이벤트 객체를 통해 네트워크 이벤트를 감지한다.
	각 소켓에 대해 이벤트 객체를 생성하고, 이 이벤트 객체를 관찰함으로써
	멀티스레드를 사용하지 않고도 여러 개의 소켓을 처리할 수 있다. 





//-----------------------------------
// 01  동작원리 
//-----------------------------------

  각 소켓마다 이벤트 객체를 하나씩 생성하여 짝지어두면,
  네트워크 이벤트가 발생할 때마다 이벤트 객체는 신호상태가 된다.

  따라서 이벤트 객체의 신호상태를 통해 네트워크 이벤트 발생을 감지
  할 수 있다.

  하지만, 이것만으로는 어떠한 종류의 이벤트가 발생되었는지 혹은
  어떤 오류가 발생했는지 알 수 없다. 


  기능함수 

  이벤트객체 생성과 제거			: WSACreateEvent(), WSACloseEvent()
  소켓과 이벤트 객체 짝짓기			: WSAEventSelect()
  이벤트 객체의 신호 상태 감지하기	: WSAWaitForMultipleEvents()
  구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents()


  * 입출력 절차
  1) 소켓을 생성할 때마다 WSACreateEvnet()함수를 이용하여 이벤트 
     객체를 생성한다.

  2) WSAEventSelect() 함수를 이용하여 소켓과 이벤트 객체를 짝지음과 동시에
     처리할 네트워크 이벤트를 등록한다.
	 ex) 소켓을 통해 데이터를 보내거나 받을 수 있는 상황이 되면 이벤트
	     객체를 신호 상태로 변경하라는 내용을 등록한다.

  3) WSAWaitForMultipleEvents() 함수를 호출하여 이벤트 객체가 신호상태가 
     되기를 기다린다.
	 등록한 네트워크 이벤트가 발생하면 해당 소켓과 연관된 이벤트 객체가 
	 신호상태가 된다.

  4) WSAEnumNetworkEvents() 함수를 호출하여 발생한 네트워크 이벤트를 
     알아내고, 적절한 소켓 함수를 호출하여 처리한다.



  * 이벤트 객체의 생성과 제거 
  WSACreateEvents() 함수는 이벤트 객체를 생성하는 역할을 한다.
  이때 생성되는 이벤트 객체는 항상 수동 리셋 이벤트이다.
  비신호 상태로 시작한다.
  사용이 끝난 이벤트 객체는 WSACloseEvents() 함수를 호출하여 제거한다.

  WSAEVENT WSACreateEvents()
  // 성공 :이벤트 객체 핸들, 실패 : WSA_INVALID_EVENT

  BOOL WSACloseEvent(WSAEVENT hEvent)
  // 성공 : TRUE	실패 : FALSE



  * 소켓과 이벤트 객체 짝짓기 
  int WSAEventSelect( SOCKET s, WSAEVENT hEventObject, long lNetWorkEvents)

  1) 처리하고자 하는 소켓
  2) 소켓과 연관시킬 이벤트 객체의 핸들
  3) 처리할 네트워크 이벤트 종류 비트 마스크 조합처리 
     - WSAAsyncSelect와 동일 


     FD_ACCEPT		: 클라이언트가 접속하면 윈도우 메시지 발생시킴
	 FD_READ		: 데이터 수신 가능하면 윈도우 메시지 발생시킴
	 FD_WRITE		: 데이터 송신이 가능하면 윈도우 메시지 발생시킴
	 FD_CLOSE		: 상대가 접속을 종료하면 윈도우 메시지 발생시킴
	 FD_CONNECT		: 접속이 완료되면 윈도우 메시지를 발생시킴


  ex) 소켓 s와 이벤트 객체를 짝짓고, FD_READ | FD_WRITE 이벤트를 등록
      하는 예 

      WSAEVENT	hEvent	= WSACreateEvent();
	  WSAEVENTSelect( s, hEvent, FD_READ| FD_WRITE);

      위 함수 사용시 주의점

      - WSAEventSlect() 함수 호출 후 해당 소켓은 자동으로 넌블로킹 
	    모드로 전환된다.

      - accept() 함수가 리턴하는 소켓은 연결 대기 소켓과 동일한 속성
	    을 지니므로 자소켓은 다시 WSAEventSlect() 함수를 호출해서 
		속성을 변경해야 한다. 


   * 이벤트 객체의 신호 상태 감지 

   WSAWaitForMultipleEvents() 함수는 여러 이벤트 객체를 동시에 관찰
   하는 기능 제공

   DWORD WSAWaitForMultipleEvents( DWORD cEvents, const WSAEVENT *lphEvents,
			BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);

   - 1),2) 이벤트 객체의 핸들을 모두 배열에 저장해야 한다. 
     1)은 배열 윈소 개수 (최대 개수 : WSA_MAXIMUM_WAIT_EVENTS(64)
	 2) 는 배열의 시작 주소를 나타낸다. 

   - 3) TRUE  : 모든 객체가 신호 상태가 되면 리턴
        FALSE : 하나의 객체라도 신호 상태가 되면 리턴

   - 4) 대기 시간으로 밀리초 단위 사용 
        이 시간내에 조건이 만족되지 않더라도 함수 리턴
		WSA_INFINITE : 무조건 대기 

   - 5)입출력 완료 루틴과 관련된 부분, 여기는 무조건 FALSE


   * 구체적인 네트워크 이벤트 알아내기 

   int WSAEnumNetworkEvents( SOCKET s, WSAEVENT hEventObject, 
							 LPWSANETWORKEVENTS lpNetworkEvents);

   - 1) 대상 소켓
     2) 대상 소켓과 짝지어둔 이벤트 객체 핸들
	    자동으로 이벤트 객체가 비신호 상태가 된다. 
		사용하지 않으려면 NULL값을 준다.

     3) 발생한 네트워크 이벤트와 오류 정보가 저장된다. 

     typedef strut _WSANETWORKEVENTS{
		long lNetworkEvents;
		int  iErrocode[FD_MAX_EVNETS];
	 }WSANETWORKEVENTS, *LPWSANNETWORKEVENTS;

     

    1) 조합된 형태로 저장
	2) 오류 정보 
     FD_ACCEPT		: FD_ACCEPT_BIT
	 FD_READ		: FD_READ_BIT
	 FD_WRITE		: FD_WRITE_BIT
	 FD_CLOSE		: FD_CLOSE_BIT
	 FD_CONNECT		: FD_CONNECT_BIT 


    ex) 
	SOCKET				s;
	WSAEVENT			hEvent;
	WSANETWORKEVENTS	NetworkEvents;

    WSAEnumNetworkEvents(s, hEvent, &NetworkEvents);

    // FD_ACCEPT 처리
	if( NetworkEvents.lNetworkEvents & FD_ACCEPT)
	{
		if( NetworkEvents.iErrocode[FD_ACCEPT_BIT]; != 0 )
		{
			// 오류 코드 출력
		}
		else
		{
			// 함수 호출 
		}
	}


  [ 실 습 ]

*/

#pragma comment(linker, "/subsystem:console")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512

// 소켓 정보 저장을 위한 구조체
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE+1];
	int recvbytes;
	int sendbytes;
};

int nTotalSockets = 0;
SOCKETINFO *SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];

// 소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);
void err_display(int errcode);

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// 소켓 정보 추가
	if(AddSocketInfo(listen_sock) == FALSE)
		return -1;

	// WSAEventSelect()
	retval = WSAEventSelect(listen_sock, EventArray[nTotalSockets-1], FD_ACCEPT | FD_CLOSE);
	if(retval == SOCKET_ERROR) 
		err_quit("WSAEventSelect()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");
	
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) 
		err_quit("listen()");

	// 데이터 통신에 사용할 변수
	int index;
	WSANETWORKEVENTS NetworkEvents;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while(1){
		// 이벤트 객체 관찰
		index = WSAWaitForMultipleEvents(nTotalSockets, EventArray, FALSE, WSA_INFINITE, FALSE);
		if(index == WSA_WAIT_FAILED)
		{
			err_display("WSAWaitForMultipleEvents()");
			continue;
		}
		index -= WSA_WAIT_EVENT_0;

		// 구체적인 네트워크 이벤트 알아내기
		retval = WSAEnumNetworkEvents(SocketInfoArray[index]->sock, EventArray[index], &NetworkEvents);
		if(retval == SOCKET_ERROR)
		{
			err_display("WSAEnumNetworkEvents()");
			continue;
		}

		// FD_ACCEPT 이벤트 처리
		if(NetworkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if(NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				err_display(NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
				continue;
			}

			addrlen = sizeof(clientaddr);
			client_sock = accept(SocketInfoArray[index]->sock, 
				(SOCKADDR *)&clientaddr, &addrlen);
			if(client_sock == INVALID_SOCKET)
			{
				err_display("accept()");
				continue;
			}
			printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			if(nTotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)
			{
				printf("[오류] 더 이상 접속을 받아들일 수 없습니다!\n");
				closesocket(client_sock);
				continue;
			}

			if(AddSocketInfo(client_sock) == FALSE)
				continue;

			retval = WSAEventSelect(client_sock, EventArray[nTotalSockets-1], FD_READ|FD_WRITE | FD_CLOSE);
			if(retval == SOCKET_ERROR) err_quit("WSAEventSelect()");
		}

		// FD_READ, FD_WRITE 이벤트 처리
		if(NetworkEvents.lNetworkEvents & FD_READ || NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if(NetworkEvents.lNetworkEvents & FD_READ && NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				err_display(NetworkEvents.iErrorCode[FD_READ_BIT]);
				continue;
			}
			if(NetworkEvents.lNetworkEvents & FD_WRITE && NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				err_display(NetworkEvents.iErrorCode[FD_WRITE_BIT]);
				continue;
			}
			
			SOCKETINFO *ptr = SocketInfoArray[index];

			if(ptr->recvbytes == 0)
			{
				// 데이터 받기
				retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if(retval == SOCKET_ERROR)
				{
					if(WSAGetLastError() != WSAEWOULDBLOCK)
					{
						err_display("recv()");
						RemoveSocketInfo(index);
					}
					continue;
				}
				ptr->recvbytes = retval;
				// 받은 데이터 출력
				ptr->buf[retval] = '\0';
				addrlen = sizeof(clientaddr);
				getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
				printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), ptr->buf);
			}

			if(ptr->recvbytes > ptr->sendbytes)
			{
				// 데이터 보내기
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
				if(retval == SOCKET_ERROR)
				{
					if(WSAGetLastError() != WSAEWOULDBLOCK)
					{
						err_display("send()");
						RemoveSocketInfo(index);
					}
					continue;
				}
				ptr->sendbytes += retval;
				// 받은 데이터를 모두 보냈는지 체크
				if(ptr->recvbytes == ptr->sendbytes)
					ptr->recvbytes = ptr->sendbytes = 0;
			}
		}

		// FD_CLOSE 이벤트 처리
		if(NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			if(NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				err_display(NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
			RemoveSocketInfo(index);
		}
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	if(nTotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)
	{
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return FALSE;
	}

	SOCKETINFO *ptr = new SOCKETINFO;
	if(ptr == NULL)
	{
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	WSAEVENT hEvent = WSACreateEvent();
	if(hEvent == WSA_INVALID_EVENT)
	{
		err_display("WSACreateEvent()");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets] = ptr;
	EventArray[nTotalSockets] = hEvent;
	nTotalSockets++;

	return TRUE;
}

// 소켓 정보 삭제
void RemoveSocketInfo(int nIndex)
{
	SOCKETINFO *ptr = SocketInfoArray[nIndex];

	// 클라이언트 정보 얻기
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[nIndex]);

	for(int i=nIndex; i<nTotalSockets; i++)
	{
		SocketInfoArray[i] = SocketInfoArray[i+1];
		EventArray[i] = EventArray[i+1];
	}
	nTotalSockets--;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
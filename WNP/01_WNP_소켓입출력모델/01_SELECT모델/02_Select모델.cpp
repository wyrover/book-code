

// Select 모델 

 /* 
	select 함수가 핵심적인 역할을 한다는 뜻에서 붙인 이름
	소켓 모드(블로킹, 넌블로킹)에 관계없이 여러 소켓을 
	한 스레드로 처리가능하다.



//-----------------------------------
// 01  동작원리 
//-----------------------------------

  핵심 기능 : 소켓 함수를 호출해야 할 시점을 알려줌으로써
              함수 호출 시 항상 성공하도록 하는 것이다. 

    - 블로킹 소켓 :소켓 함수 호출시 조건이 만족되지 않아 블로킹
	               되는 상황을 막을 수 있다.

    - 넌블로킹 소켓 : 소켓 함수 호출 시 조건이 만족되지 않아 다시 
	               호출해야 하는 상황을 막을 수 있다. 


  동작 원리 

  1) 세개의 소켓 셋(socket set) 준비 
     - 소켓 셋은 소켓을 통해 하고자 하는 작업의 종류를 나타낸다.
	   ex) 읽기 셋 : recv() 함수를 호출해야 할 시점을 알고 싶을 경우 
	       쓰기 셋 : send() 함수를 호출할 시점을 알고 싶을 경우 

  2) 소켓 셋을 초기화(비운다.)

  3) 소켓 셋에 소켓을 넣는다.
     - 셋에 넣을 수 있는 소켓의 최대 갯수는 FD_SETSIZE(64)로 정의됨


  4) select() 를 통해 대기한다. : 블로킹 함수 

  5) select() 함수가 리턴한다는 것은 소켓 셋에 있는 소켓 중 어느 
     소켓이 사용할 준비가 되어 있다는 것을 의미 
	 ex) 연결 요청이 들어오면 읽기 셋에 있던 대기 소켓이 깨어난다.
	  
     이때 소켓셋에는 준비가 된 소켓만 남고 나머지는 모두 제거된다. 

  6) 해당 작업을 처리한다. 


  * 읽기 셋(read set)
    - 함수 호출 시점 
	  클라이언트가 접속했으므로 accept() 함수 호출 가능
	  데이터를 받았으므로 recv(), send() 등의 함수 호출 가능
	  연결이 종료되었으므로 recv(), recvfrom() 등의 함수를 호출 가능
	  이때 리턴값은 0 or SOCKET_ERROR가 된다. 

  * 쓰기 셋(write set)
    - 함수 호출 시점 
	  송신 버퍼가 충분하므로 send(), sendto()등의 함수를 호출하여 
	  데이터 전송이 가능하다.

    - 함수 호출 결과
	  넌블로킹 소켓을 사용한 connect() 함수 호출이 성공

  * 예외 셋(exception set)
	  
	     


  select 함수 원형
  int select ( int nfds, fd_set *readfds, fd_set *writefds, 
			   fd_set *excepfds, const struct timeval *timeout);

  1) 유닉스와 호환성을 위해 : 윈도우는 사용하지 않음
  2) 각각 읽기, 쓰기, 에외 셋을 나타냄
     최대 2개까지 NULL 이 될 수 있다. 

  3) 타임 아웃을 나타냄.

     struct timeval {
		long tv_sec;		// seconds
		long tv_usec;		// microseconds
	};
   
     

  [ 모델 확인]

  서버 모델 : echo 모델 적용
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
SOCKETINFO *SocketInfoArray[FD_SETSIZE];

// 소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);
// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);

int main(int argc, char* argv[])
{
	int retval;

	// 1) 윈속 초기화 ---------------------------------------------------
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

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
	if(retval == SOCKET_ERROR) err_quit("listen()");

	//------------------------------------------------------------------------

	// 2) 넌블로킹 소켓으로 전환
	u_long on = TRUE;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if(retval == SOCKET_ERROR) err_display("ioctlsocket()");

	//-----------------------------------------------------------------------

	// 데이터 통신에 사용할 변수
	FD_SET rset;
	FD_SET wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while(1)
	{

		// 3) 소켓 셋 초기화 
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(listen_sock, &rset);
		for(int i=0; i<nTotalSockets; i++)
		{
			if(SocketInfoArray[i]->recvbytes > SocketInfoArray[i]->sendbytes)
				FD_SET(SocketInfoArray[i]->sock, &wset);
			else
				FD_SET(SocketInfoArray[i]->sock, &rset);
		}
		//----------------------------------------------------------------------


		// 4) select()
		retval = select(0, &rset, &wset, NULL, NULL);
		if(retval == SOCKET_ERROR) err_quit("select()");

		// 소켓 셋 검사(1): 클라이언트 접속 수용
		if(FD_ISSET(listen_sock, &rset))
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
			if(client_sock == INVALID_SOCKET)
			{
				if(WSAGetLastError() != WSAEWOULDBLOCK)
					err_display("accept()");
			}
			else
			{
				printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				// 소켓 정보 추가
				if(AddSocketInfo(client_sock) == FALSE)
				{
					printf("[TCP 서버] 클라이언트 접속을 해제합니다!\n");
					closesocket(client_sock);
				}
			}
		}
		//-----------------------------------------------------------------------


		// 5) 소켓 셋 검사(2): 데이터 통신
		for(i=0; i<nTotalSockets; i++)
		{
			SOCKETINFO *ptr = SocketInfoArray[i];
			if(FD_ISSET(ptr->sock, &rset))
			{
				// 데이터 받기
				retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if(retval == SOCKET_ERROR){
					if(WSAGetLastError() != WSAEWOULDBLOCK){
						err_display("recv()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				else if(retval == 0){
					RemoveSocketInfo(i);
					continue;
				}
				ptr->recvbytes = retval;
				// 받은 데이터 출력
				addrlen = sizeof(clientaddr);
				getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
				ptr->buf[retval] = '\0';
				printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), ptr->buf);
			}
			if(FD_ISSET(ptr->sock, &wset))
			{
				// 데이터 보내기
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes, 
					ptr->recvbytes - ptr->sendbytes, 0);
				if(retval == SOCKET_ERROR){
					if(WSAGetLastError() != WSAEWOULDBLOCK){
						err_display("send()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->sendbytes += retval;
				if(ptr->recvbytes == ptr->sendbytes){
					ptr->recvbytes = ptr->sendbytes = 0;
				}
			}
		}
	}
	//--------------------------------------------------------------------

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	// FD_SETSIZE - 연결 대기 소켓
	if(nTotalSockets >= (FD_SETSIZE-1)){
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return FALSE;
	}

	SOCKETINFO *ptr = new SOCKETINFO;
	if(ptr == NULL){
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

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
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", 
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;

	for(int i=nIndex; i<nTotalSockets; i++){
		SocketInfoArray[i] = SocketInfoArray[i+1];
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
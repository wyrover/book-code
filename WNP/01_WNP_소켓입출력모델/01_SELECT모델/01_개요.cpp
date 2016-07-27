//----------------------------------------------------------------
//  블로킹과 넌블로킷 소켓 모드의 장단점 이해
//  Select 소켓 입출력 모델 이해 
//  WSAAsyncSelect 소켓 입출력 모델 이해 활용
//  WSAEventSelect 소켓 입출력 모델 이해 활용 
//----------------------------------------------------------------


//-----------------------------------
// 01  개요 
//-----------------------------------

/*

	지금까지 배운 소켓 함수만으로도 간단하고 실용적인 서버를 작성
	할 수 있지만 좀 더 많은 클라이언트 접속을 효율적으로 처리하려면
	새로운 입출력 모델이 필요하다.


1. 소켓 모드 
   
   소켓은 함수 호출 시 동작 방식에 따라 블로킹과 넌블로킹 소켓으로 구분
   한다.
   이를 소켓 모드(socket mode)라 한다.

   블로킹 소켓
   - 소켓 함수 호출 시 조건이 만족되지 않으면 함수는 리턴하지 않고 해당 
     스레드는 대기 상태(wait state)가 된다.

     따라서  멀티스레드를 사용하여 다른 작업을 하지 않는 한 애플리케이션이
	 더는 진행할 수 없다.

   - accept()			: 클라이언트가 접속했을 떄 
     send(), sendto()	: 송신 버퍼에 데이터를 모두 복사했을 때
	 recv(), recvfrom()	: 수신 버퍼에 도착한 데이터가 있들 떄 


   넌블로킹 소켓
   - 소켓 함수 호출 시 조건이 만족되지 않더라도 함수가 리턴하므로 해당 스레
     드는 계속 진행할 수 있다.
	 socket() 함수는 기본적으로 블로킹 소켓을 생성하므로 다음과 같이 
	 ioctlsocket() 함수를 호출하면 넌블로킹 소켓으로 바꿀 수 있다. 

     // 블로킹 소켓 생성
	 SOCKET listen_sock			= socket(AF_INET, SOCK_STREAM, 0);
	 if( listen_sock == INVALID_SOCKET) err_quit("socket()");

     // 넌블로킹 소켓으로 전환
	 u_long on	= 1;
	 retval		= ioctlsocket(listen_sock, FIONBIO, &on);
	 if( retval  == SOCKET_ERROR) err_quit("loctlsocket()");


     => 넌블로킹 소켓 함수를 호출했을 때 조건이 만족되지 않아 작업을 완료
	    할 수 없으면 소켓 함수는 오류를 리턴한다.
		이때는 WSAGetLastError() 함수를 호출하여 반드시 오류코드를 확인한다.
		넌블로킹 소켓을 사용할 경우 대개 오류 코드는 WSAWOULDBLOCK이 되며
		이는 조건이 만족되지 않음을 나타내므로 나중에 다시 소켓함수를
		호출하면 된다.


     [ 처음에 작성한 TCP 서버를 수정해보자. ]

*/

/*
#pragma comment(linker, "/subsystem:console")
#include <winsock2.h>	
#include <stdlib.h>		
#include <stdio.h>
#define BUFSIZE 512

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

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) 
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");	
	
     // 넌블로킹 소켓으로 전환 -----------------------------------

	 //------------------------------------------------------------

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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			//----------------------------------------
			//----------------------------------------
				err_display("accept()");
			continue;
		}
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while(1){
			// 데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
					break;
				//----------------------------------------
				//----------------------------------------

			}
			else if(retval == 0){
				break;
			}
			else{
				// 받은 데이터 출력
				buf[retval] = '\0';
				printf("%s", buf);
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
*/


/*

  넌블로킹 소켓의 특징

  장 점 : 소켓 함수 호출시 블록되지 않으므로 다른 작업을 진행할 수 있다.
          멀티스레드를 사용하지 않고도 여러 개의 소켓 입출력을 처리할 수 있다.

  단 점 : 소켓 함수를 호출할 때마다 WSAWOULDBLOCK등 오류 코드를 확인하고, 다시 
          해당 함수를 호출해야 하므로 프로그램 구조가 복잡해진다.

          블로킹  소켓을 사용한 경우보다 CPU 사용률이 높다. 

  => 앞으로 배울 소켓 입출력 모델을 사용하면 넌블로킹 소켓의 장점을 그대로 유지
     하면서 단점을 해결 할 수 있다. 

     새로운 소켓 입출력 모델을 확인해보기 전에 기존의 서버 작성 모델을 고찰해보자.





//-----------------------------------
// 02  서버 작성 모델 
//-----------------------------------

  지금까지 배운 TCP UDP 서버는 반복 서버와 병행 서버로 분류 가능

  1) 반복 서버(iterative server)

     접속한 여러 클라이언트를 하나씩 차례대로 처리한다.
	 TCP서버, UDP 서버가 반복 서버에 해당 

     장점 : 하나의 스레드로 클라이언트를 처리하므로 시스템 자원
	        소모가 적다.

     단점 : 서버와 클라이언트의 통신 시간이 길어지면 다른 클라이
	        언트의 대기 시간이 길어진다.

     => 반복서버는 일반적으로 UDP 서버를 작성할 때 적합하다.
	    그러나 UDP 서버가 파일 전송과 같이 시간이 오래 걸리는 작업을
		처리한다면 병행 서버로 작성하는 것이 바람직하다.

  2) 병행 서버(concurrent server)
    
	 접속한 여러 클라이언트를 병렬적으로 처리한다.
	 멀티스레드를 이용한 TCP 서버 

     장점 : 서버와 클라이언트의 통신 시간이 길어지더라도 다른 클라이
	        언트의 통신에 영향을 주지 않는다.

     단점 : 멀티프로세스 또는 멀티스레드를 이용하여 구현하므로 시스템
	        자원 소모가 크다.

     => 병행 서버는 일반적으로 TCP 서버를 작성할 때 적합하다.
	    그러나 TCP 서버가 각각의 클라이언트와 매우 짧은 시간 동안만
		 통신하고 , 접속한 클라이언트 수가 많지 않다고 가정한다면
		 반복 서버로 작성해도 된다.





//-----------------------------------
// 03  이상적인 소켓 입출력 모델 
//-----------------------------------

  반복 서버와 병행 서버의 장점을 모두 가지면서 각각의 단점을 해결한 형태

  - 모든 클라이언트 접속은 성공한다

  - 서버는 각 클라이언트의 서비스 요청에 최대한 빠르게 반응하며
    고속으로 데이터를 전송한다.

  - 위와 같은 기능을 제공하되 시스템 자원 사용량을 최소화한다.
    CPU 사용률이나 메모리 사용량 등을 최소화한다.

  => 이상적인 서버를 작성하기 위해 소켓 입출력 모델에 요구되는 사항 ?

  1) 소켓 함수 호출시 블로킹을 최소화 한다.
     - 넌블로킹 사용만으로는 충분하지 않다.
	 - CPU 사용을 최소로 하면서 넌블로킹 소켓을 사용한 모든 소켓 
	   함수 호출이 성공적으로 이루어지도록...
	   ( WSAWOULDBLOCK등의 오류코드 발생을 미연에 방지)

  2) 입출력 작업을 다른 작업과 병행
     - 동일한 시간 동안 애플리케이션이 더욱 많은 일을 처리할 수 
	   있도록 설계

  3) 스레드 개수 최소화
     - 필요한 메모리를 축소
	 - 스레드 수가 많아지면 스레드의 응답속도가 느려져 서버의 성능저하초래됨
	   이상적인 입출력 모델은 CPU 당 하나의 스레드를 실행하여 여러 클라이
	   언트의 접속을 처리 하도록 함

  4) 유저 모드와 커널모드 전환 횟수와 데이터 복사 최소화
     - 모드간 전환은 상당한 CPU 사이클을 소모함
	   모드 전환없이 구현 
	   불필요한 복사를 최소화 





//-----------------------------------
// 04  소켓 입출력 모델의 종류 
//-----------------------------------

   윈도우 CE	   : 최대 새 개의 소켓 입출력 모델 지원
   윈도우 95이상   : Completion Port 모델을 제외한 모든 소켓 입출력 지원
   윈도우 NT 계열  : 모든 소켓 입출력 모델 지원
   

   select
   WSAAsyncSelect
   WSAEventSelect
   Overlapped
   Completion Port


*/



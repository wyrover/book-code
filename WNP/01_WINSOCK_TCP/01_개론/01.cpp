/******************************************************************

  Chapter 1. 윈속에 대한 소개

	02.cpp	: 윈속 API 소개
	03.cpp  : 주소 지정
	04.cpp  : TCP/IP  서버 생성
	05.cpp  : TCP/IP  클라이언트 생성
	06.cpp  : 데이터 송수신(연결형)
	07.cpp  : 데이터 송수신(비연결형) UDP/IP
	08.cpp  : 기타 다른 API
******************************************************************/

//--------------------------------------------------
// 네트워크 프로그래밍과 소켓의 이해 
//--------------------------------------------------

/* IPC(Inter Process C)  || RPC
 네트워크 프로그래밍 : 멀리 떨어져 있는 호스트들이 서로 데이터를 주고 
 받을 수 있도록 프로그램을 구현하는 것(IPC)

  -> 파일과 달리 데이터를 주고 받을 대상이 멀리 떨어져 있기 때문에 
     소프트웨어 차원에서 호스트들간에 연결해 주는 장치가 필요할 뿐.
	 이러한 기능을 해주는 장치를 소켓이라 함 
  

     결국 소켓을 통해 멀리 떨어져 있는 두 개체를 연결시키게 됨 

     우리는 소켓과 소켓을 연결하는 가설을 해야 한다.




// Server 작성
1) 소켓 생성 함수 : 전화기 장만
int socket( int domain, int type, int protocol);

2) 소켓에 IP 주소 할당 :  전화번호를 할당 
int bind( int sockd, struct sockaddr *myaddr, int addrlen);

3) 소켓을 케이블에 연결 : 전화기를 케이블에 연결 
int listen(int sockfd, int backlog);

4) 연결 요청 : 벨이 울리고 전화를 받음 
int accept(int sockfd, struct sockaddr *addr, int *addrlen)

  ...
  대화 시작 

  windows 2000이상, 윈속2( windows socket 2.x) 를 기반으로 구현
  
  프로토콜(protocol) : 서로간의 약속 , 시스템과 시스템과 통신을 하기 위한
                       정해진 절차와 방법 

   ex) TCP/IP 프로토콜 
       UDP/IP 프로토콜 

   과제)  인터넷 프로토콜 구조 ( TCP/IP 프로토콜 구조 )
           네트워크 계층, 인터넷 계층, 전송 계층, 어플리케이션 계층 
		   패킷 

          OSI( Open Systems Interconnection) 7계층 모델 조사, 
		  각 계층의 기능을 TCP/IP 프로토콜 관점에서 확인 
    
*/



//------------------------------------------------------
// 윈도우 소켓 ( 특징, 구조, 윈속 애플리케이션 맛보기 
//------------------------------------------------------

/*
소켓 : 버클리 유닉스(Berkeley Sofrware Distibution UNIX)에서 사용한 
       네트워크 프로그래밍 인터페이스 

		=> 이를 윈도우 환경에서 사용할 수 있도록 한것을 
		윈도우 소켓, 줄여서 Winsock이라 한다.

       윈도우 95버전부터 API에 정식으로 포함하여 제공하고 있다.
*/


// 1. 특징
/*
    => 유닉스 소켓과 호환 아래의 부분이 추가되어 있다. 

	윈도우 소켓은 DLL을 통해 대부분의 기능이 제공되므로 DLL 초기화와
	종료 작업을 위한 함수가 필요하다. 

    윈도우 애플리케이션은 대개 그래픽 사용자 인터페이스(GUI)를 기반으로 
	하며,
	메시지 구동 방식으로 동작하므로 이를 위한 확장 함수가 존재한다.

    윈도우는 운영체제 차원에서 멀티스레드를 지원하므로 멀티 환경에서 
	안정적으로
	동작하기 위한 구조와 이를 위한 함수가 필요하다.

    윈속 버전
	운영체제					윈속버전
	윈도우 95					1.1(2.2)	
	윈도우 98/NT/2000/XP/2003	2.2
	윈도우 CE					1.1(2.2)

    윈속 1.1버전 : 유닉스 소켓과의 호환성 및 TCP/IP 프로토콜을 지원
	윈속 2.x버전 : 다양한 프로토콜을 지원하도록 구조 변경 및 함수 추가 
	  TCP/IP, IPv6, IrDA, Bluetooth, IPX/SPX, ATM, DECNet, TP4, 
	  DLC, NetBEUI

   
	윈속 네트웤 프로그래밍의 장점
	1) 유닉스 소켓과 소스 코드 수준에서 호환성이 높으므로 기존 프로그램 
	   포팅이 쉽다.
	2) 가장 널리 쓰이는 네트워크 프로그래밍 인터페이스이다.
	3) TCP/IP 외에도 다양한 종류의 프로토콜을 지원하므로 최소한의 코드 
	   수정으로 애플리케이션에서 사용할 프로토콜을 변경할 수 있다.
	4) 비교적 저수준의 프로그래밍 인터페이스로 , 세부적인 제어가 
	   가능하므로  고성능의 네트워크 애플리케이션 개발 가능하다.

    단점
	1) 애플리케이션 수준의 프로토콜을 프로그래머가 직접 설계하여야 
	   한다.
	   데이터 포멧, 전송 절차 등을 고려한 프로그래밍
	2) 서로 다른 바이트 정렬 방식을 사용하거나 데이터 처리 단위가 
	   서로 다른 종단 시스템간의 통신일 경우, 
	   애플리케이션 수준에서 데이터 변환처리가 필요하다.


 2. 구조 
    ws2_32.DLL 로 대부분의 기능이 제공된다. 

    윈속 1.X 버전 : WINSOCK.DLL   WSOCK32.DLL을 통해 WS2_32.DLL의 
	                기능을 궁극적으로 사용하게 된다.

    윈속 2.x 버전		윈속 1.x버전 
	애플리케이션		애플리케이션

                        WINSOCK.DLL(16bit)
						WSOCK32.DLL(32bit)

      ---------------------------------------
	  WS2_32.dll( 32bit)
	  ---------------------------------------
	   TCP/IP  IrDA   Bluetooth IPX/SPX  ... 


       [ 윈도우 소켓 구조 ]

    

  3. 윈속 애플리케이션 맛보기 

    

*/

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

	// 1. socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");	
	
	// 2. bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");
	
	// 3. listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];

	while(1)
	{
		// 4. accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
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


// 서버 실행
// telnet 127.0.0.1 900 
// Ctrl + ]  quit 명령 실행 => 클라이언트가 종료한 것을 서버가 감지 

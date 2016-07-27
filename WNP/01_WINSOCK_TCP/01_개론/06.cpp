/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	3) TCP/IP 통신(연결 지향 통신) 
		 클라이언트 : 소켓 생성 -> connect  
******************************************************************/



//---------------------------------------------------------------- 
//   3) TCP/IP 통신 : 클라이언트(연결)
//---------------------------------------------------------------- 
/*
	[클라이언트]
	socket()	==>	connect()

	클라이언트가 서버에 접속하여 TCP 프로토콜 수준의 연결이 
    이루어지도록 한다.


	int connect(
		SOCKET						s;
		const struct sockaddr FAR	*name,
		int							namelen;
	);

	1인자) 연결하기를 원하는 클라이언트의 소켓
	2인자) 연결할 서버의 주소를 담은 구조체 
	3인자) 구조체 변수의 크기 

    연결하고자 하는 프로세스가(서버) 없다면 WSAECONNREFUSED 리턴 에러 

    서버와 달리 bind() 함수 호출이 없다.
	connect함수 호출시 운영체제는 자동으로 IP, port를 설정해준다.

*/


//---------------------------------------------------------------- 
//   3) TCP/IP 통신 :  Client example
//---------------------------------------------------------------- 

#include <winsock2.h>

#define PORT	5000
void main()
{
	WSADATA			wsaData;
	SOCKET			CSocket;
	SOCKADDR_IN		SAddr;


	// 1. 윈속 라이브러리 2.2 초기화
	WSAStartup(MAKEWORD(2,2), &wsaData);


	// 2. 연결을 기다리기 위한 소켓 생성
	CSocket	= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	// 3. bind 하기 위한 주소 지정 및 bind
    SAddr.sin_family		= AF_INET;
	SAddr.sin_port			= htons(PORT);
	SAddr.sin_addr.s_addr	= inet_addr("127.0.0.1");


	// 4. 서버에 연결
	connect(CSocket, (SOCKADDR*)&SAddr, sizeof(SAddr) );



	// 5. 소켓 해제
	closesocket(CSocket);

	// 6. 윈속 사용 종료 
	WSACleanup();
}


/*
	netstat -a -n 명령 실행 : TCP/IP 네트워크 연결상태 정보 표시 기본 제공됨
				 -a : 모든 연결정보와 포트변호 표시
			 -n : IP 주소와 포트번호를 숫자로 표시 
	   1) 서버실행 
          상태 확인 : 9000  포트번호 상태가 (LISTENING연결대기중)
       2) 클라이언트 실행
	      상태 확인 : 총 3개 ESTABLISHED(연결됨) 
          => 서버측에서 만든 소켓은 모두 같은 포트 번호를 사용한다.
       3) 글자를 입력하지 않은 상태에서 Enter키를 누르면
	      클라이언트는 종료한다.
	  상태확인 : TIME_WAIT 상태인 포트(1502)가 존재한다.
       4) 약 5분 후 다시 상태 확인
	      TIME_OUT 상태인 포트는 사라지고
		  처음과 동일한 결과를 보게 된다.
       5) 클라이언트를 다시 실행하여 데이터를 몇개 보낸 후 
	      Ctrl+C를 눌러 강제 종료 해보자.
		  상태확인  : 최초 대기 서버 소켓만 존재한다.
*/

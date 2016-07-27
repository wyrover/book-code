/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	5) 프로토콜의 주소 지정 
	6) IPv4의 주소 지정
	7) 바이트 순서
	8) example

******************************************************************/


//---------------------------------------------------------------- 
//   5) 프로토콜의 주소 지정 
//---------------------------------------------------------------- 
/*
	인터넷 프로토콜(IP, internet protocol)을 이용하여 통신하기 위한
	호출 방법 

     IP : 비연결형 프로토콜(완벽한 데이터 수신 미보장)
	 TCP(Transmission Control Protocol) & UDP(User Datagram Protocol)

     => TCP와 UDP는 IP 기반의 프로토콜임.
	    TCP/IP & UDP/IP라고 부름 

     *) IPv4에서 윈속을 다루기 위해 주소를 어떻게 사용하는지의 이해
	    필요 
*/



//---------------------------------------------------------------- 
//   6) IPv4의 주소 지정 
//---------------------------------------------------------------- 
/*

  (1) 소켓 주소 구조체 정의 -------------------------------
   
   소켓 주소 구조체(socket address structures)는 네트워크 프로그램에서 
   필요로 하는 주소 정보를 담는 구조체로 , 다양한 소켓 함수의 인자로 
   사용한다.
   따라서 여러 소켓 주소 구조체가 존재하고, 가장 기본이 되는 것은 
   SOCKADDR이다.

   struct sockaddr
   {
		u_short	sa_family;			//  2byte
		char	sa_data[14];		// 14byte
   };
   typedef struct sockaddr SOCKADDR;

   1) sa_family : 
      주소 체계를 나타내며, 부호 없는 16bit 정수값을 사용한다.
	  예를 들어 TCP/IP 프로토콜을 사용한다면 이 값은 AF_INET이 된다.

   2) sa_data[14]
      해당 주소 체계에서 사용하는 주소 정보를 담고 있다. 
	  바이트 배열로 선언되어 있다.
	  예를 들어 TCP/IP 프로토콜을 사용한다면 여기에는 IP 주소와 
	  포트번호가 저장된다. 


  (2) TCP/IP & UDP/IP 소켓 주소 구조체 정의 ---------------------

   *) 실제 프로그래밍에서는 에플리케이션이 사용할 프로토콜의 종류에 
     맞는 소켓 구조체를 사용한다. 
     예를 들어 TCP/IP에서는 SOCKADDR_IN
     IrDA(적외선을 이용한 무선 통신 프로토콜) 에서는 SOCKADDR_IRDA를 
     사용한다.

	*) 32bit의 주소를 사용
	서버와 통신하기를 원할 경우 PORT 와 IP Address 필요
	서버가 클라이언트로부터 들어오는 요청을 수신하기 위해서도 
	   PORT 와 IP Address 필요
	
    => WINSOCK에서 IP주소와 PORT는 SOCKADDR_IN 구조체를 이용하여 지정

    struct sockaddr_in
	{
		short			sin_family;		// 2byte
		u_short			sin_port;		// 2byte
		struct in_addr	sin_addr;		// 4byte
		char			sin_zero[8];	// 8byte
	};
   typedef struct sockaddr_in	SOCKADDR_IN;

    1인자) IP를 사용할 경우 AF_INET으로 설정
	2인자) 통신하기를 원하는 곳의 PORT
	       well-known-port(예약 포트)
		    ex) FTP(21), HTTP(80)...
	3인자) IPv4 주소
	       32bit의 unsigned long intege 타입
				"xxx.xxx.xxx.xxx"
				"255.255.255.0"
				=> 0xff.0xff.0xff.0x00
				=> 0xffffff00(이 값이 필요함)
	4인자) 패딩 필드
	       SOCKADDR과 같은 크기로 만들기 위한 인자
   

  (3) struct in_addr 구조체 -------------------------------------

   IP 주소를 저장하기 위한 in_addr, 구조체 정의를 확인해보자. 
   동일한 메모리 영역을 각 8비트(S_un_b) 16비트(S_un_w), 
   32비트(S_addr) 단위로 접근할 수 있게 공용체(S_un)로 선언되어 있다. 
   대개 32bit 단위로 접근하므로 S_un.S_addr 필드를 사용하게 되는데, 
   매크로를 이용하여 재정의된 s_addr을 사용하면 편리하다.

   struct in_addr {
	union {
		struct { u_char s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { u_short s_w1, s_w2 ; } S_un_w;
		u_long  S_addr;
	}S_un;
	#define s_addr S_un.S_addr
   };
   typedef struct in_addr	IN_ADDR;




	 *) 점으로 구분된 주소를 32bit 정수형으로 바꾸는 함수 
		unsigned long inet_addr(
			const char FAR *cp)
		);

		 cp : NULL로 끝나는 문자열로 점으로 구분된 주소를 의미
		      그 주소에 해당하는 네트웤 바이트 순서
			  (network byte order)로 된 32bit 정수형 형식의 
			  주소를 리턴
*/



//---------------------------------------------------------------- 
//   7) 바이트 순서 
//---------------------------------------------------------------- 
/*
	각각의 컴퓨터의 프로세서에 따라 숫자를 표시하는 바이트 형식의 
	차이가 있다.

		- big_endian		
		- little_endian			: intel x86

    *) little_endian ( 하위 => 상위 )
	   256(10진수) ==> 0xff, 0x01(2진수, 2바이트)
	   컴퓨터 고유의 바이트 배열 순서임
	   이를 호스트 바이트 오더(host-byte-order)라 함 
	   호스트 바이트 오더가 반드시 little-endian은 아님 

    *) big_endian	 ( 상위 => 하위 )
	   256(10진수) ==> 0x01, 0xff(2진수, 2바이트)
	   네트웤에서 IP주소가 표시될 때 
	   네트웤 바이트 순서(network-byte-order)에 따라 표시된다.
	   인터넷 표준에서 네트웤 바이트 오더를 big_endian으로 정의함
	   

    => 따라서 둘 사이의 변환이 필요하다.

    *) host-byte-order ==> network-byte-order

       u_long  htonl ( u_long hostlong);
	   u_short htons ( u_short hostshort);

       hostlong  : 호스트 바이트 오더 형식의 4byte 숫자 
	   htonl()   : 네트웤 바이트 오더로 된 숫자값을 리턴 

       hostshort : 호스트 바이트 오더 형식의 2byte 숫자 
	   htons()   : 네트웤 바이트 오더로 된 숫자값을 리턴 



      *) network-byte-order ==> host-byte-order

       u_long  ntohl ( u_long netlong);
	   u_short ntohs ( u_short netshort);


     *) 확인
      소켓 함수에 넘겨줄 데이터 -[ 변환 : hton*() ] -> 소켓 함수 

      소켓 함수  -[변환 : ntoh*() ] -> 에플리케이션이 사용

*/



//---------------------------------------------------------------- 
//   	8) example 1
//---------------------------------------------------------------- 
/*
#include <winsock2.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return -1;

    u_short x = 0x1234;
    u_long  y = 0x12345678;

    u_short x2;
    u_long  y2;

    // 호스트 바이트 -> 네트워크 바이트
    printf("호스트 바이트 -> 네트워크 바이트\n");
    printf("0x%x -> 0x%x\n", x, x2 = htons(x));
    printf("0x%x -> 0x%x\n", y, y2 = htonl(y));

    // 네트워크 바이트 -> 호스트 바이트
    printf("네트워크 바이트 -> 호스트 바이트\n");
    printf("0x%x -> 0x%x\n", x2, ntohs(x2));
    printf("0x%x -> 0x%x\n", y2, ntohl(y2));

    // 잘못된 사용 예
    printf("잘못된 사용 예\n");
    printf("0x%x -> 0x%x\n", x, htonl(x));

    WSACleanup();
    return 0;
}
*/


//---------------------------------------------------------------- 
//   	8) example 2
//---------------------------------------------------------------- 
/*
#include <winsock2.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// 원래의 IP 주소 출력
	char *ipaddr = "147.46.114.70";
	printf("IP 주소 = %s\n", ipaddr);

	// inet_addr() 함수 연습
	// 1) .를 제거시킨다..
	// 2) 네트웤 바이트오더로 변환
	printf("IP 주소(변환 후) = 0x%x\n", inet_addr(ipaddr));

	// inet_ntoa() 함수 연습
	IN_ADDR temp;
	temp.s_addr = inet_addr(ipaddr);

	// inet_addr의 역 
	// 1) 호스트 바이트오더로 변환
	// 2) .를 추가한다...
	printf("IP 주소(변환 후) = %s\n", inet_ntoa(temp));

	WSACleanup();
	return 0;
}
*/


//---------------------------------------------------------------- 
//   	8) example 3. 주소 지정
//---------------------------------------------------------------- 

#include <winsock2.h>

#include <stdio.h>

void main()
{
	// 1. 소켓 라이브러리 초기화 

	// 2. 주소 지정 
	SOCKADDR_IN		InternetAddr;
	int				nPortId		 = 5000;	// 호스트오더 

	// IP 프로토콜 사용
	InternetAddr.sin_family		 = AF_INET;

	// 호스트 바이트 순서 => 네트웤 바이트 순서로 변환
	InternetAddr.sin_port		 = htons(nPortId);	// 네트웤..

	// 점으로 구분된 4byte의 문자열 주소를 숫자로 변환 
	InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 3. 소켓 해제 
}


/*
[ 정리 ]
1. 소켓 주소 구조체는 네트워크 프로그램에서 필요로 하는 주소 
   정보를 담고 있는 구조체로, 다양한 소켓 함수의 인자로 사용한다.

2. SOCKADDR 구조체는 여러 개의 소켓 구조체를 대표하는 타입으로,
   다양한 소켓 함수의 인자로 사용한다.

3. 소켓 주소 구조체는 크기가 크기 때문에 소켓 함수 인자로 사용
   할때는 항상 주소값을 전달하며, 이때 반드시 SOCKADDR 형으로
   변환한다.

4. 바이트 정렬이란 메모리에 데이터를 저장할 때의 바이트 순서를 
   나타내는 용어로, 빅엔디안과 리틀 엔디안 방식이 있다.

5. 네트워크 애플리케이션에서 바이트 정렬 방식을 고려해야 할 
   경우는 다음과 같다.
   1) IP 주소, 포트번호 등과 같이 프로토콜 구현을 위해 필요한
      정보 -> 빅 엔디안으로 통일
   2) 애플리케이션이 주고 받는 데이터 -> 통신 양단이 약속한
      바이트 정렬 방식 사용

6. 네트워크 용어로 빅 엔디안을 네트워크 바이트 정렬이라 부르고,
   시스템이 사용하는 고유한 바이트 정렬 방식을 호스트 바이트 
   정렬이라 부른다.

7. inet_addr() 함수는 문자열 형태의 IP 주소를 네트워크 바이트
   정렬된 32비트 숫자로 변환한다.

8. inet_ntoa() 함수는 네트워크 바이트 정렬된 32비트 IP 주소를
   문자열 형태로 변환한다.

9. TCP/IP 프로토콜은 도메인 이름을 인식하지 못하므로 사용자가
   입력한 도메인 이름을 반드시 IP 주소로 변환해야 한다.


10. HOSTENT 구조체는 도메인 이름과 IP 주소 정보를 모두 담고 있다.

11. gethostbyname() 함수는 도메인 이름을 입력으로 받아 
    HOSTENT 구조체 주소를 리턴한다.

12. gethostbyaddr 함수는 IP 주소를 입력으로 받아 
    HOSTENT 구조체 주소를 리턴한다.

*/